/**********************************************************
 *
 * AVF analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#include <iostream>
#include <iomanip>
#include <cassert>
#include "avf_analyzer.h"
#include "svm.h"
#include "vbit_table.h"
#include "term_style.h"

#define CYCLE_LOG_FILE FULL_PATH("results/avf_cycle.log")
#define GROUP_LOG_FILE FULL_PATH("results/avf_group.log")
#define GROUP_1M_LOG_FILE FULL_PATH("results/avf_group_1M.log")
#define GROUP_8K_LOG_FILE FULL_PATH("results/avf_group_8K.log")
#define REG_LIFE_FILE FULL_PATH("results/reg_life.log")
#define REG_USAGE_FILE FULL_PATH("results/reg_usage.log")
#define CYCLE_COMPULSORY_LOG_FILE FULL_PATH("results/avf_comp_cycle.log")

AvfAnalyzer::AvfAnalyzer(SimLog *simlog, std::string base_output_path) :
  FtAnalyzer(simlog, base_output_path),
  liveindb_(SVM->cfg().core_size_prf_int, false) {
    zero_vf_on_protection_ = true;
}

AvfAnalyzer::~AvfAnalyzer() {
}

bool AvfAnalyzer::Analyze(uint64_t max_clock) {
    uint64_t simulation_size = max_clock+1;

    std::cout << "[avf_analyzer] INFO: Starting post mortem analysis..." <<
        std::flush;

    // do the AVF post mortem analysis
    bool ret = PostMortemAnalysis(simulation_size);
    if (ret != 0) {
        std::cout << TERM_RED << " failed" << TERM_RESET << std::endl;
        return false;
    }

    LogAvfs(simulation_size, CYCLE_LOG_FILE, &total_vbits_);
    LogRegUse(simulation_size);

    // analyze grouped samples
    AnalyzeGrouped(simulation_size-SVM->cfg().end_of_warm_up,
                   CYCLE_LOG_FILE, GROUP_LOG_FILE);
    // analyze in group of 1M samples
    AnalyzeGrouped(simulation_size-SVM->cfg().end_of_warm_up, 1024*1024,
                   CYCLE_LOG_FILE, GROUP_1M_LOG_FILE);
    // analyze in group of 8K samples
    AnalyzeGrouped(simulation_size-SVM->cfg().end_of_warm_up, 8*1024,
                   CYCLE_LOG_FILE, GROUP_8K_LOG_FILE);

    // after standard AVF analysis is completed, do the AVF compulsory analysis
    PostMortemAvfCompulsoryAnalysis(simulation_size);
    LogAvfs(simulation_size, CYCLE_COMPULSORY_LOG_FILE, &compulsory_vbits_);

    if (!SVM->cfg().keep_cycle_detailed_files) {
        remove(CYCLE_LOG_FILE);
        remove(CYCLE_COMPULSORY_LOG_FILE);
    }

    std::cout << " done" << std::endl;

    LogInfo(simulation_size);

    return true;
}

/* -------------------- private function definitions ------------------------ */

bool AvfAnalyzer::PostMortemAnalysis(uint64_t simulation_size) {
    std::map<uint64_t, struct InstructionDetails>::iterator it;

    // if logdb is empty, skip it
    if (simlog_->logdb_.size() == 0)
        return -1;

    // allocate memory for AVF analysis
    avfdb_.resize(simulation_size);
    avfdb_rf_reg_.resize(simulation_size);
    for (uint64_t n=0; n<simulation_size; n++) {
        avfdb_rf_reg_[n].resize(SVM->cfg().arch_size_amt, 0);
    }

    outfile_reg_life_.open(REG_LIFE_FILE, std::ios::out | std::ios::trunc);
    // Analyze from the end to the beginning
    it = (simlog_->logdb_).end();
    do {
        it--;
        struct InstructionDetails inst = it->second;

        if (IsInstructionCompleted(inst)) { // only include completed
                                            // instructions

            // RegFile AVF post mortem analysis
            if (inst.important) { // if the instruction is marked as important
                std::vector<struct SourceRegister>::iterator srcs;
                for (srcs=inst.sources.begin();
                    srcs!=inst.sources.end();
                    srcs++) {

                    uint64_t begin_live, end_live, begin_pc, end_pc;
                    uint64_t prod = srcs->producer;
                    if (prod != REG_NO_PRODUCER) {
                        if (!simlog_->logdb_[prod].important) {
                            // producer is not important, skip it
                            continue;
                        }
                        if (!(simlog_->IsInstructionAnalyzed(prod))) {
                            // the producer has not been analyzed.
                            // Analyze now and mark register live from write to
                            // the last read
                            begin_live =
                                (simlog_->logdb_)[prod].is_syscall ?
                                (simlog_->logdb_)[prod].retired_clk :
                                (simlog_->logdb_)[prod].writeback_clk;
                            begin_pc   = (simlog_->logdb_)[prod].pc;
                            end_live =
                                inst.is_syscall ? inst.retired_clk :
                                                  inst.regread_clk;
                            end_pc = inst.pc;
                            // check if the producer has destinations
                            if ((simlog_->logdb_)[prod].destinations.size() < 1)
                                std::cout << TERM_YELLOW <<
                                    "[avf_analyzer] WARNING: producer " <<
                                    "doesn't have destinations" <<
                                    TERM_RESET << std::endl;
                            // find out which architectural register file is
                            // live
                            int64_t arch_reg =
                                (simlog_->logdb_)[prod].is_syscall ?
                                srcs->phy : // if the producer is a syscall,
                                            // then the arch_reg_id would be
                                            // the same as its physical ID
                                // otherwise, get the arch_reg_id from
                                // its producer
                                (simlog_->logdb_)[prod].destinations[0].log;
                            // mark the register in the producer as analyzed
                            bool dest_found = false;
                            bool dest_was_analyzed = false;
                            for (uint64_t i=0;
                                i<(simlog_->logdb_)[prod].destinations.size();
                                i++) {
                                if ((simlog_->logdb_)[prod].destinations[i].log
                                    == arch_reg) {
                                    dest_found = true;
                                    dest_was_analyzed =
                                        (simlog_->logdb_)[prod].destinations[i].analyzed;
                                    (simlog_->logdb_)[prod].destinations[i].analyzed = true;
                                    break;
                                }
                            }
                            if (!dest_found) {
                                std::cout << TERM_YELLOW <<
                                    "[avf_analyzer] WARNING: producer doesn't "
                                    << "produce the specified register" <<
                                    TERM_RESET << std::endl;
                            }
                            // add live register it was not analyzed before
                            if (!dest_was_analyzed) {
                                AddLiveRegister(inst.opcode, begin_live,
                                                begin_pc, end_live, end_pc,
                                                arch_reg);
                                // set the RF vbit of the producer
                                if (begin_live < end_live)
                                    (simlog_->logdb_)[prod].rvbit +=
                                        (end_live - begin_live) *
                                        GetVulnerableBitCount(STRUCTURE_RF,
                                            (simlog_->logdb_)[prod].opcode,
                                            true);
                            }
                        }
                    }
                    else { // no producer, i.e. live in since
                           // the beginning of simulation
                        // is the live-in register has been analyzed before?
                        if (liveindb_[srcs->phy]) {
                            // don't redo the live-in analysis
                            // skip it.
                            continue;
                        }
                        else {
                            begin_live = -1; // start from the very beginning
                            begin_pc = 0; // Beginning pc is assumed to be zero
                            end_live = inst.is_syscall ? inst.retired_clk :
                                                         inst.regread_clk;
                            end_pc = inst.pc;
                            // if it is live since the beginning, the
                            // arch_reg_id would be the same as its physical ID
                            int64_t arch_reg = srcs->phy;
                            // add live register
                            AddLiveRegister(inst.opcode, begin_live, begin_pc,
                                            end_live, end_pc, arch_reg);
                            // mark the physical register id as live in
                            liveindb_[srcs->phy] = true;
                        }
                    }
                }
            }

            // IssueQueue AVF post mortem analysis
            if (!SVM->cfg().nop_issued && inst.opcode == OPCODE_NOP) {
                // NOP doesn't occupy issue queue
            }
            else if (inst.issued_clk < simulation_size) {
                for (uint64_t i=inst.dispatched_clk+1;
                    i<=inst.issued_clk;
                    i++) {

                    uint32_t vbit_cnt = 0;
                    vbit_cnt += GetVulnerableBitCount(STRUCTURE_IQ, inst.opcode,
                                                      inst.important);
                    avfdb_[i].iq += vbit_cnt;
                    it->second.qvbit += vbit_cnt;
                }
            }
            else {
                std::cout << TERM_YELLOW <<
                    "[avf_analyzer] WARNING: invalid issued_clk for opcode " <<
                    inst.opcode << TERM_RESET << std::endl;
            }

            // ReorderBuffer AVF post mortem analysis
            if (inst.writeback_clk != INVALID_CLK) {
                for (uint64_t i=inst.dispatched_clk+1; i<=inst.writeback_clk; i++) {
                    uint32_t vbit_cnt = 0;
                    vbit_cnt += GetVulnerableBitCount(STRUCTURE_ROB, inst.opcode,
                                                      inst.important, true);
                    avfdb_[i].rob += vbit_cnt;
                    it->second.qvbit += vbit_cnt;
                }
                for (uint64_t i=inst.writeback_clk+1; i<=inst.retired_clk; i++) {
                    uint32_t vbit_cnt = 0;
                    vbit_cnt += GetVulnerableBitCount(STRUCTURE_ROB, inst.opcode,
                                                      inst.important, false);
                    avfdb_[i].rob += vbit_cnt;
                    it->second.qvbit += vbit_cnt;
                }
            }
            else {
                // combine dispatch-writeback and writeback-retire stage
                for (uint64_t i=inst.dispatched_clk+1; i<=inst.retired_clk; i++) {
                    uint32_t vbit_cnt = 0;
                    vbit_cnt += GetVulnerableBitCount(STRUCTURE_ROB, inst.opcode,
                                                      inst.important, false);
                    avfdb_[i].rob += vbit_cnt;
                    it->second.qvbit += vbit_cnt;
                }
            }

            // LoadQueue AVF post mortem analysis
            if (inst.load_id != INVALID_LOAD_ID) {
                if (inst.memexecute_clk < simulation_size &&
                    inst.writeback_clk < simulation_size) {

                    for (uint64_t i=inst.dispatched_clk+1;
                        i<=inst.memexecute_clk;
                        i++) {

                        uint32_t vbit_cnt = 0;
                        vbit_cnt += GetVulnerableBitCount(STRUCTURE_LQ,
                                                          inst.opcode,
                                                          inst.important, true);
                        avfdb_[i].lq += vbit_cnt;
                        it->second.qvbit += vbit_cnt;
                    }
                    // set the lifetime of LQ entry until a clock before it's
                    // written back
                    uint64_t last_lq_entry_important_clk = inst.writeback_clk-1;
                    // if there is a store which this load might depend to,
                    // extends the lifetime until the last store has committed
                    // (to mark load violation)
                    if (inst.last_store_seq_no != INVALID_SEQ_NO) {
                        if ((simlog_->logdb_).count(inst.last_store_seq_no)
                            == 0) {

                            std::cout << TERM_YELLOW <<
                                "[avf_analyzer] WARNING: the seq_no " <<
                                inst.last_store_seq_no <<
                                " of the last store before load " <<
                                "could not be found" << TERM_RESET << std::endl;
                        }
                        else {
                            if (simlog_->logdb_[inst.last_store_seq_no].storecommit_clk
                                == INVALID_CLK) {

                                std::cout << TERM_YELLOW <<
                                    "[avf_analyzer] WARNING: the seq_no of " <<
                                    "the last store before load has invalid " <<
                                    "store commit clock" <<
                                    TERM_RESET << std::endl;
                            }
                            else if (simlog_->logdb_[inst.last_store_seq_no].storecommit_clk
                                > last_lq_entry_important_clk) {
                                // extend the lifetime
                                last_lq_entry_important_clk =
                                    simlog_->logdb_[inst.last_store_seq_no].storecommit_clk;
                            }
                        }
                    }
                    for (uint64_t i=inst.memexecute_clk+1;
                        i<=last_lq_entry_important_clk;
                        i++) {

                        uint32_t vbit_cnt = 0;
                        vbit_cnt += GetVulnerableBitCount(STRUCTURE_LQ,
                                                          inst.opcode,
                                                          inst.important,
                                                          false);
                        avfdb_[i].lq += vbit_cnt;
                        it->second.qvbit += vbit_cnt;
                    }
                }
                else {
                    std::cout << TERM_YELLOW <<
                        "[avf_analyzer] WARNING: invalid memexecute_clk for " <<
                        "opcode " << inst.opcode << " PC: " << inst.pc <<
                        TERM_RESET << std::endl;
                }
            }

            // StoreQueue AVF post mortem analysis
            if (inst.store_id != INVALID_STORE_ID) {
                for (uint64_t i=inst.dispatched_clk+1;
                    i<=inst.memexecute_clk;
                    i++) {

                    uint32_t vbit_cnt = 0;
                    vbit_cnt += GetVulnerableBitCount(STRUCTURE_SQ, inst.opcode,
                                                      inst.important, true);
                    avfdb_[i].sq += vbit_cnt;
                    it->second.qvbit += vbit_cnt;
                }
                for (uint64_t i=inst.memexecute_clk+1;
                    i<=inst.storecommit_clk;
                    i++) {

                    uint32_t vbit_cnt = 0;
                    vbit_cnt += GetVulnerableBitCount(STRUCTURE_SQ, inst.opcode,
                                                      inst.important, false);
                    avfdb_[i].sq += vbit_cnt;
                    it->second.qvbit += vbit_cnt;
                }
            }

            // Instruction Buffer AVF post mortem analysis
            for (uint64_t i=inst.decoded_clk+1;
                i<=inst.decoded_clk+inst.ibuff_duration;
                i++) {

                uint32_t vbit_cnt = 0;
                vbit_cnt += GetVulnerableBitCount(STRUCTURE_IBUFF, inst.opcode,
                                                  inst.important);
                avfdb_[i].ibuff += vbit_cnt;
                it->second.qvbit += vbit_cnt;
            }

            // functional units AVFs
            if (inst.important && !inst.is_syscall) {
                if (inst.issued_clk != INVALID_CLK) {
                    switch(inst.fu) {
                        case INSTRUCTION_TYPE_SIMPLE:
                            avfdb_[inst.issued_clk+2].fu_simple++;
                            break;
                        case INSTRUCTION_TYPE_COMPLEX:
                            avfdb_[inst.issued_clk+2].fu_complex++;
                            break;
                        case INSTRUCTION_TYPE_CONTROL:
                            avfdb_[inst.issued_clk+2].fu_control++;
                            break;
                        case INSTRUCTION_TYPE_MEMORY:
                            avfdb_[inst.issued_clk+2].fu_memory++;
                            break;
                        case INSTRUCTION_TYPE_FP:
                        default:
                            ;//std::cout << TERM_YELLOW <<
                            //    "[avf_analyzer] WARNING: non-integer FU " <<
                            //    "for opcode " << inst.opcode << " PC: " <<
                            //    inst.pc << TERM_RESET << std::endl;
                            break;
                    }
                }
            }
        }
    } while(it != (simlog_->logdb_).begin());

    outfile_reg_life_.close();

    return 0;
}

void AvfAnalyzer::PostMortemAvfCompulsoryAnalysis(uint64_t simulation_size) {
    // This function does AVF compulsory analysis. AVF compulsory is part of AVF
    // that contributes to total AVF regardless of instruction's importance.
    // In other word, this is the AVF if all the instructions is not important.
    std::map<uint64_t, struct InstructionDetails>::iterator it;

    // allocate memory for AVF analysis
    avfdb_.resize(simulation_size);

    // clear out the data structure
    for (uint64_t n=0; n<simulation_size; n++) {
        avfdb_[n].iq = 0;
        avfdb_[n].rob = 0;
        avfdb_[n].lq = 0;
        avfdb_[n].sq = 0;
        avfdb_[n].rf = 0;
        avfdb_[n].ibuff = 0;
    }

    // Analyze from the end to the beginning
    it = (simlog_->logdb_).end();
    do {
        it--;
        struct InstructionDetails inst = it->second;

        if (IsInstructionCompleted(inst)) {
            // only include completed instructions

            // RegFile AVF compulsory post mortem analysis is not done as write
            // by non-important instructions will not make any bit in the
            // register vulnerable

            // IssueQueue AVF compulsory post mortem analysis
            if (!SVM->cfg().nop_issued && inst.opcode == OPCODE_NOP) {
                // NOP doesn't occupy issue queue
            }
            else if (inst.issued_clk < simulation_size) {
                for (uint64_t i=inst.dispatched_clk+1;
                    i<=inst.issued_clk;
                    i++) {

                    avfdb_[i].iq += GetVulnerableBitCount(STRUCTURE_IQ,
                                                          inst.opcode, false);
                }
            }
            else {
                std::cout << TERM_YELLOW <<
                    "[avf_analyzer] WARNING: invalid issued_clk for opcode " <<
                    inst.opcode << TERM_RESET << std::endl;
            }

            // ReorderBuffer AVF compulsory post mortem analysis
            for (uint64_t i=inst.dispatched_clk+1; i<=inst.retired_clk; i++) {
                avfdb_[i].rob += GetVulnerableBitCount(STRUCTURE_ROB,
                                                       inst.opcode, false);
            }

            // LoadQueue AVF compulsory post mortem analysis
            if (inst.load_id != INVALID_LOAD_ID) {
                for (uint64_t i=inst.dispatched_clk+1;
                    i<=inst.retired_clk;
                    i++) {

                    avfdb_[i].lq += GetVulnerableBitCount(STRUCTURE_LQ,
                                                          inst.opcode, false);
                }
            }

            // StoreQueue AVF compulsory post mortem analysis
            if (inst.store_id != INVALID_STORE_ID) {
                for (uint64_t i=inst.dispatched_clk+1;
                    i<=inst.storecommit_clk;
                    i++) {
                    avfdb_[i].sq += GetVulnerableBitCount(STRUCTURE_SQ,
                                                          inst.opcode, false);
                }
            }

            // IBuff AVF compulsory post mortem analysis
            for (uint64_t i=inst.decoded_clk+1;
                i<=inst.decoded_clk+inst.ibuff_duration;
                i++) {

                avfdb_[i].ibuff += GetVulnerableBitCount(STRUCTURE_IBUFF,
                                                         inst.opcode, false);
            }
        }
    } while(it != (simlog_->logdb_).begin());
}

void AvfAnalyzer::AddLiveRegister(int32_t opcode, uint64_t begin,
    uint64_t end) {

    for (uint64_t i=begin+1; i<=end; i++) {
        avfdb_[i].rf += GetVulnerableBitCount(STRUCTURE_RF, opcode, true);
    }
}

void AvfAnalyzer::AddLiveRegister(int32_t opcode, uint64_t begin,
    uint64_t begin_pc, uint64_t end, uint64_t end_pc, int64_t arch_reg) {

    AddLiveRegister(opcode, begin, end);
    if (SVM->cfg().enable_regs_life_dump &&
        ((int64_t)end - (int64_t)begin) > 0) {
        outfile_reg_life_ << std::dec << arch_reg << " " << (end - begin) <<
            " " << std::hex << begin_pc << " " << end_pc << std::endl;
    }
    for (uint64_t i=begin+1; i<=end; i++) {
        avfdb_rf_reg_[i][arch_reg]++;
    }
}

bool AvfAnalyzer::IsInstructionCompleted(struct InstructionDetails inst) {
    return (inst.retired_clk != INVALID_CLK &&
        (inst.store_id == INVALID_STORE_ID || inst.storecommit_clk));
}

void AvfAnalyzer::LogAvfs(uint64_t simulation_size, const char *out_file,
    struct TotalVulnerableBits<uint64_t> *total) {

    std::ofstream outfile_cycle;
    int64_t start_cycle = 0;

    // open file for writing
    outfile_cycle.open(out_file, std::ios::out | std::ios::trunc);

    // clear the total structure
    total->clear();

    // offset the start clock in case the simulation started with warm-up
    if (SVM->cfg().enable_warm_up) {
        start_cycle = SVM->cfg().end_of_warm_up;
        assert(start_cycle > 0); // make sure start_cycle is greater than zero
    }

    for (uint64_t i=start_cycle; i<simulation_size; i++) {
        outfile_cycle << avfdb_[i].iq;
        outfile_cycle << "\t" << avfdb_[i].rob;
        outfile_cycle << "\t" << avfdb_[i].lq;
        outfile_cycle << "\t" << avfdb_[i].sq;
        outfile_cycle << "\t" << avfdb_[i].rf;
        outfile_cycle << "\t" << avfdb_[i].ibuff;
        outfile_cycle << "\t" << avfdb_[i].fu_simple;
        outfile_cycle << "\t" << avfdb_[i].fu_complex;
        outfile_cycle << "\t" << avfdb_[i].fu_control;
        outfile_cycle << "\t" << avfdb_[i].fu_memory;
        outfile_cycle << std::endl;

        total->add(avfdb_[i]);
    }

    // close the file
    outfile_cycle.close();
}

void AvfAnalyzer::LogRegUse(uint64_t simulation_size) {
    std::ofstream outfile_reg_use;
    int64_t dead_cycles[SVM->cfg().arch_size_amt];
    int64_t live_count[SVM->cfg().arch_size_amt];

    // open file for writing
    outfile_reg_use.open(REG_USAGE_FILE, std::ios::out | std::ios::trunc);

    for (uint32_t j=0; j<SVM->cfg().arch_size_amt; j++) {
        dead_cycles[j] = 0;
        live_count[j] = 0;
    }

    for (uint64_t i=0; i<simulation_size; i++) {
        for (uint32_t j=0; j<SVM->cfg().arch_size_amt; j++) {
            live_count[j] += avfdb_rf_reg_[i][j];
            if (!avfdb_rf_reg_[i][j])
                dead_cycles[j]++;
        }
    }

    for (uint32_t j=0; j<SVM->cfg().arch_size_amt; j++) {
        outfile_reg_use << "r" << j <<" "<< dead_cycles[j] << " " <<
            live_count[j] << std::endl;
    }

    // close the file
    outfile_reg_use.close();
}

void AvfAnalyzer::LogInfo(uint64_t simulation_size) {
    int size_system = 0;

    // get system size
    for (int i=0; i<STRUCTURES_COUNT; i++) {
        structure_t type = (structure_t)i;
        size_system += GetTotalBitsCount(type);
    }

    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << "structures_list:" << std::endl;
        *p_outfile_summary_ << std::fixed << std::setprecision(2);

        for (int i=0; i<STRUCTURES_COUNT; i++) {
            structure_t type = (structure_t)i;
            *p_outfile_summary_ << "    " << GetStructureName(type) << ": " <<
                GetTotalBitsCount(type) << " # bits " <<
                "(" << std::setw(5) <<
                (double)GetTotalBitsCount(type)/size_system*100 << "%)" <<
                std::endl;
        }
        *p_outfile_summary_ << std::endl;

        *p_outfile_summary_ << "structures_total_size: " <<
            size_system << " # bits" << std::endl;

        // if simulation started with warm-up, adjust simulation size
        if (SVM->cfg().enable_warm_up) {
            assert(SVM->cfg().end_of_warm_up > 0);
            simulation_size -= (SVM->cfg().end_of_warm_up - 1);
        }

        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ << "clocks_simulated: " << simulation_size <<
            " # clocks" << std::endl;

        // dump total_vbits_ information
        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ <<
            "# Total vulnerable bits (IQ/ROB/LQ/SQ/RF/IBuff/Sys/FUs) of AVF" <<
            std::endl;
        DumpSummaryTotalVBit("avf_total_vbits", total_vbits_);

        // dump compulsory_vbits_ information
        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ <<
            "# Total vulnerable bits (IQ/ROB/LQ/SQ/RF/IBuff/Sys/FUs) of " <<
            "AVF compulsory" << std::endl;
        DumpSummaryTotalVBit("avf_compulsory_total_vbits",
                             compulsory_vbits_);

        // dump AVF compulsory : AVF ratio
        struct TotalVulnerableBits<double> ratio_vbits;
        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ << "# Ratio of AVF compulsory to total AVF " <<
            "(IQ/ROB/LQ/SQ/RF/IBuff/Sys)" << std::endl;
        ratio_vbits.iq = (double)compulsory_vbits_.iq/total_vbits_.iq;
        ratio_vbits.rob = (double)compulsory_vbits_.rob/total_vbits_.rob;
        ratio_vbits.lq = (double)compulsory_vbits_.lq/total_vbits_.lq;
        ratio_vbits.sq = (double)compulsory_vbits_.sq/total_vbits_.sq;
        ratio_vbits.rf = (double)compulsory_vbits_.rf/total_vbits_.rf;
        ratio_vbits.ibuff = (double)compulsory_vbits_.ibuff/total_vbits_.ibuff;
        ratio_vbits.sys = (double)compulsory_vbits_.sys/total_vbits_.sys;
        DumpSummaryTotalVBit("avf_to_avf_compulsory_ratio",
                             ratio_vbits);
    }
}

 /* end of file */
