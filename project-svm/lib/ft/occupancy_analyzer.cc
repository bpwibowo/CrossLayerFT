/**********************************************************
 *
 * Occupancy analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 06/02/2014
 **********************************************************/

#include <iostream>
#include <cassert>
#include "occupancy_analyzer.h"
#include "vbit_table.h"
#include "term_style.h"
#include "svm.h"

#define CYCLE_LOG_FILE FULL_PATH("results/occ_cycle.log")
#define GROUP_LOG_FILE FULL_PATH("results/occ_group.log")
#define GROUP_1M_LOG_FILE FULL_PATH("results/occ_group_1M.log")
#define GROUP_8K_LOG_FILE FULL_PATH("results/occ_group_8K.log")

OccupancyAnalyzer::OccupancyAnalyzer(SimLog *simlog,
    std::string base_output_path) :
    FtAnalyzer(simlog, base_output_path) {

    // initialize occupancies_
    occupancies_.rf =
        (SVM->cfg().core_size_prf_int - (simlog_->freelist_.size())) *
        GetEntryBitsCount(STRUCTURE_RF);
    occupancies_.iq = 0;
    occupancies_.rob = 0;
    occupancies_.lq = 0;
    occupancies_.sq = 0;
    occupancies_.ibuff = 0;
    occupancies_.fu_simple = 0;
    occupancies_.fu_complex = 0;
    occupancies_.fu_control = 0;
    occupancies_.fu_memory = 0;
    occupancy_sq_nonspec_ = 0;

    // set delayed_occupancies_  the same as occupancies_
    occupancies_delayed_ = occupancies_;

    // set regread and execute occupancies the same as occupancies_
    occupancies_regread_ = occupancies_;
    occupancies_execute_ = occupancies_;

    // open files for writing
    outfile_cycle_.open(CYCLE_LOG_FILE, std::ios::out | std::ios::trunc);
}

OccupancyAnalyzer::~OccupancyAnalyzer() {
    if (outfile_cycle_.is_open()) outfile_cycle_.close();
}

void OccupancyAnalyzer::LogClockTick(LogClockTickArgs args) {
    if (!SVM->IsWarmUpPeriod()) {
        // dump data to file for non-warmup period
        outfile_cycle_ << occupancies_delayed_.iq;
        outfile_cycle_ << "\t" << occupancies_delayed_.rob;
        outfile_cycle_ << "\t" << occupancies_delayed_.lq;
        outfile_cycle_ << "\t" << occupancies_delayed_.sq;
        outfile_cycle_ << "\t" << occupancies_delayed_.rf;
        outfile_cycle_ << "\t" << occupancies_delayed_.ibuff;
        outfile_cycle_ << "\t" << occupancies_delayed_.fu_simple;
        outfile_cycle_ << "\t" << occupancies_delayed_.fu_complex;
        outfile_cycle_ << "\t" << occupancies_delayed_.fu_control;
        outfile_cycle_ << "\t" << occupancies_delayed_.fu_memory;
        outfile_cycle_ << std::endl;
    }

    // update RF occupancy
    occupancies_.rf =
        (SVM->cfg().core_size_prf_int - (simlog_->freelist_.size())) *
        GetEntryBitsCount(STRUCTURE_RF);

    // update FU occupancies
    occupancies_.fu_simple = occupancies_execute_.fu_simple;
    occupancies_.fu_complex = occupancies_execute_.fu_complex;
    occupancies_.fu_control = occupancies_execute_.fu_control;
    occupancies_.fu_memory = occupancies_execute_.fu_memory;

    // pipe regread and execute occupancies
    occupancies_execute_.fu_simple = occupancies_regread_.fu_simple;
    occupancies_execute_.fu_complex = occupancies_regread_.fu_complex;
    occupancies_execute_.fu_control = occupancies_regread_.fu_control;
    occupancies_execute_.fu_memory = occupancies_regread_.fu_memory;

    // clear occupancies_regread_
    occupancies_regread_.fu_simple = 0;
    occupancies_regread_.fu_complex = 0;
    occupancies_regread_.fu_control = 0;
    occupancies_regread_.fu_memory = 0;

    // update delayed occupancies
    occupancies_delayed_ = occupancies_;
}

void OccupancyAnalyzer::LogFetch(LogFetchArgs args) {
    if (args.seq_no == 0) // invalid fetched instruction value?
        return;

    assert(occupancies_.ibuff < GetTotalBitsCount(STRUCTURE_IBUFF));
    occupancies_.ibuff += GetEntryBitsCount(STRUCTURE_IBUFF);
}

void OccupancyAnalyzer::LogRename(LogRenameArgs args) {
    if (args.seq_no == 0) // invalid seq_no
        return;

    assert(occupancies_.ibuff > 0);
    occupancies_.ibuff -= GetEntryBitsCount(STRUCTURE_IBUFF);
}

void OccupancyAnalyzer::LogDispatch(LogDispatchArgs args) {
    if (SVM->cfg().nop_issued) { // if NOP is always issued to FU
        occupancies_.iq += GetEntryBitsCount(STRUCTURE_IQ);
    }
    else if (simlog_->logdb_[args.seq_no].opcode != OPCODE_NOP) {
        // only include non-NOP instructions
        occupancies_.iq += GetEntryBitsCount(STRUCTURE_IQ);
    }
    occupancies_.rob += GetEntryBitsCount(STRUCTURE_ROB);
}

void OccupancyAnalyzer::LogDispatchLoad(LogDispatchLoadArgs args) {
    occupancies_.lq += GetEntryBitsCount(STRUCTURE_LQ);
}

void OccupancyAnalyzer::LogDispatchStore(LogDispatchStoreArgs args) {
    sq_map_[args.store_id] = 0;
    occupancies_.sq += GetEntryBitsCount(STRUCTURE_SQ);
}

void OccupancyAnalyzer::LogIssue(LogIssueArgs args) {
    occupancies_.iq -= GetEntryBitsCount(STRUCTURE_IQ);

    if (!(simlog_->logdb_[args.seq_no].is_syscall)) {
        switch(simlog_->logdb_[args.seq_no].fu) {
            case INSTRUCTION_TYPE_SIMPLE:
                assert(occupancies_regread_.fu_simple == 0);
                occupancies_regread_.fu_simple++;
                break;
            case INSTRUCTION_TYPE_COMPLEX:
                assert(occupancies_regread_.fu_complex == 0);
                occupancies_regread_.fu_complex++;
                break;
            case INSTRUCTION_TYPE_CONTROL:
                assert(occupancies_regread_.fu_control == 0);
                occupancies_regread_.fu_control++;
                break;
            case INSTRUCTION_TYPE_MEMORY:
                assert(occupancies_regread_.fu_memory == 0);
                occupancies_regread_.fu_memory++;
                break;
            case INSTRUCTION_TYPE_FP:
            default:
                break;
        }
    }
}

void OccupancyAnalyzer::LogRetire(LogRetireArgs args) {
    // handle ROB
    occupancies_.rob -= GetEntryBitsCount(STRUCTURE_ROB);

    // handle LQ
    if (simlog_->logdb_[args.seq_no].load_id != INVALID_LOAD_ID)
        occupancies_.lq -= GetEntryBitsCount(STRUCTURE_LQ);

    // handle store retire
    if (simlog_->logdb_[args.seq_no].store_id != INVALID_STORE_ID)
        occupancy_sq_nonspec_ += GetEntryBitsCount(STRUCTURE_SQ);
}

void OccupancyAnalyzer::LogStoreCommit(LogStoreCommitArgs args) {
    if (simlog_->storedb_.count(args.store_id) == 0) {
        std::cout << TERM_YELLOW << "[occupancy_analyzer] WARNING: " <<
            "Invalid store commit with store_id " << std::hex <<
            args.store_id << " at clk " << args.clk << TERM_RESET << std::endl;
        return;
    }

    uint64_t seq_no = simlog_->storedb_[args.store_id];
    if (simlog_->logdb_.count(seq_no)) {
        int vbit_count = GetEntryBitsCount(STRUCTURE_SQ);
        occupancies_.sq -=  vbit_count;
        occupancy_sq_nonspec_ -= vbit_count;
    }
    else {
        // this has been checked in SimLog but it is kept here to avoid
        // program crashes, just in case.
    }
}

void OccupancyAnalyzer::LogFlush(LogFlushArgs args) {
    // handle IQ, ROB, LQ, and IBUFF
    occupancies_.iq = 0;
    occupancies_.rob = 0;
    occupancies_.lq = 0;
    occupancies_.ibuff = 0;

    // handle SQ
    occupancies_.sq = occupancy_sq_nonspec_; // restore Occupancy of SQ from its
                                             // non-speculative counter

    // handle FUs
    occupancies_.fu_simple = 0;
    occupancies_.fu_complex = 0;
    occupancies_.fu_control = 0;
    occupancies_.fu_memory = 0;
    occupancies_execute_ = occupancies_; // clear occ at execute
    occupancies_regread_ = occupancies_execute_; // clear occ at regread
}

bool OccupancyAnalyzer::Analyze(uint64_t max_clock) {
    uint64_t simulation_size = max_clock+1;
    struct TotalVulnerableBits<uint64_t> total_bits;

    // close the outfile_cycle_
    if (outfile_cycle_.is_open()) outfile_cycle_.close();

    std::cout << "[occ_analyzer] INFO: Starting occupancy analysis..." <<
        std::flush;

    // reanalyze the file to create grouped version
    AnalyzeGrouped(simulation_size-SVM->cfg().end_of_warm_up, CYCLE_LOG_FILE,
                   GROUP_LOG_FILE, &total_bits);

    // analyze in group of 1M samples
    AnalyzeGrouped(simulation_size-SVM->cfg().end_of_warm_up, 1024*1024,
                   CYCLE_LOG_FILE, GROUP_1M_LOG_FILE);

    // analyze in group of 8K samples
    AnalyzeGrouped(simulation_size-SVM->cfg().end_of_warm_up, 8*1024,
                   CYCLE_LOG_FILE, GROUP_8K_LOG_FILE);

    if (p_outfile_summary_->is_open()) {
        // dump total_bits information
        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ << "# Total vulnerable bits " <<
            "(IQ/ROB/LQ/SQ/RF/IBuff/Sys/FUs) of Occupancy" << std::endl;
        DumpSummaryTotalVBit("occupancy_total_vbits", total_bits);
    }

    if (!SVM->cfg().keep_cycle_detailed_files) {
        remove(CYCLE_LOG_FILE);
    }

    std::cout << " done" << std::endl;

    return true;
}

/* end of file */
