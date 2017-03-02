/**********************************************************
 *
 * SVF analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 11/13/2014
 **********************************************************/

#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "svf_analyzer.h"
#include "term_style.h"
#include "svm.h"

#define MAX_DUR (16*1024)
#define MAX_VBITS (8*1024*1024)
#define IGNORED_DUR_LSB (4)
#define IGNORED_VBITS_LSB (7)
#define CALLEE_SAVED_STACK_SIZE (16)
#define LIVEVECTOR_FIFO_SIZE (64)
#define AL_ENTRY_TAIL_THRESHOLD (0x80)

#define SVF_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.log")
#define SVF_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.log")
#define SVF_NC_GROUP_LOG_FILE FULL_PATH("results/svf_nc_group_8K.log")
#define SVF_NC_1M_GROUP_LOG_FILE FULL_PATH("results/svf_nc_group_1M.log")
#define SVF_WC_GROUP_LOG_FILE FULL_PATH("results/svf_wc_group_8K.log")
#define SVF_WC_1M_GROUP_LOG_FILE FULL_PATH("results/svf_wc_group_1M.log")
#define SVF_RC_GROUP_LOG_FILE FULL_PATH("results/svf_rc_group_8K.log")
#define SVF_RC_1M_GROUP_LOG_FILE FULL_PATH("results/svf_rc_group_1M.log")
#define SVF_NLV_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.nlv.log")
#define SVF_NLV_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.nlv.log")
#define SVF_NM_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.nm.log")
#define SVF_NM_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.nm.log")
#define SVF_UBIT_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.ubits.log")
#define SVF_UBIT_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.ubits.log")
#define SVF_PM_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.pm.log")
#define SVF_PM_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.pm.log")
#define AVFCL_GROUP_LOG_FILE FULL_PATH("results/avfcl_group_8K.log")
#define AVFCL_1M_GROUP_LOG_FILE FULL_PATH("results/avfcl_group_1M.log")
#define AVFCL_NM_GROUP_LOG_FILE FULL_PATH("results/avfcl_nm_group_8K.log")
#define AVFCL_NM_1M_GROUP_LOG_FILE FULL_PATH("results/avfcl_nm_group_1M.log")
#define AVFCL_NC_GROUP_LOG_FILE FULL_PATH("results/avfcl_nc_group_8K.log")
#define AVFCL_NC_1M_GROUP_LOG_FILE FULL_PATH("results/avfcl_nc_group_1M.log")
#define AVFCL_WC_GROUP_LOG_FILE FULL_PATH("results/avfcl_wc_group_8K.log")
#define AVFCL_WC_1M_GROUP_LOG_FILE FULL_PATH("results/avfcl_wc_group_1M.log")
#define AVFCL_RC_GROUP_LOG_FILE FULL_PATH("results/avfcl_rc_group_8K.log")
#define AVFCL_RC_1M_GROUP_LOG_FILE FULL_PATH("results/avfcl_rc_group_1M.log")
#define SVF_PRF_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.prf.log")
#define SVF_PRF_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.prf.log")
#define SVF_PRFNOP_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.prf_nop.log")
#define SVF_PRFNOP_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.prf_nop.log")
#define SVF_PRFNOP1_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.prf_nop1.log")
#define SVF_PRFNOP1_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.prf_nop1.log")
#define SVF_PRF1_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.prf1.log")
#define SVF_PRF1_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.prf1.log")
#define SVF_PRFNOP2_GROUP_LOG_FILE FULL_PATH("results/svf_group_8K.prf_nop2.log")
#define SVF_PRFNOP2_1M_GROUP_LOG_FILE FULL_PATH("results/svf_group_1M.prf_nop2.log")
#define ECC_GROUP_LOG_FILE FULL_PATH("results/ecc_8K.log")
#define NOP_INJECT_HIST_FILE FULL_PATH("results/nop_inject_hist.log")
#define UNDETECTED_BB_HIST_FILE FULL_PATH("results/undetected_bb_hist.log")
#define PROTECTION_HIST_FILE FULL_PATH("results/prot_hist.log")

#define PHASE_PERIOD ((uint64_t)0x1 << 13)
#define PHASE_PERIOD_1M ((uint64_t)0x1 << 20)

#define INITIAL_BB_PC ((uint64_t)-1)

#define CD_UPDATE_IB 1
#define CD_UPDATE_IQ 1
#define CD_UPDATE_ROB 1
#define CD_UPDATE_LQ 1
#define CD_UPDATE_SQ 1

#define REG_LIVE_UNTRACKED     ((0x1ull<<R_R0) | \
                                (0x1ull<<R_SP) | \
                                (0x1ull<<R_GP))
#define REG_CALLEE_SAVED_MASK  ((0x1ull<<R_S0) | \
                                (0x1ull<<R_S1) | \
                                (0x1ull<<R_S2) | \
                                (0x1ull<<R_S3) | \
                                (0x1ull<<R_S4) | \
                                (0x1ull<<R_S5) | \
                                (0x1ull<<R_S6) | \
                                (0x1ull<<R_S7) | \
                                (0x1ull<<R_S8))

SvfAnalyzer::SvfAnalyzer(SimLog *simlog, std::string base_output_path) :
  FtAnalyzer(simlog, base_output_path) {
    Init(SVF_MODE_NORMAL);
}

SvfAnalyzer::SvfAnalyzer(SimLog *simlog, std::string base_output_path, SvfMode mode) :
  FtAnalyzer(simlog, base_output_path) {
    Init(mode);
}

SvfAnalyzer::~SvfAnalyzer() {
    if (outfile_group_.is_open()) outfile_group_.close();
    if (outfile_group_1M_.is_open()) outfile_group_1M_.close();
    if (mode_ == SVF_MODE_AVFCL && outfile_ecc_.is_open())
        outfile_ecc_.close();
    if (sdur_) {
        delete sdur_;
        sdur_ = NULL;
    }
    if (bbstat_) {
        delete bbstat_;
        bbstat_ = NULL;
    }
}

void SvfAnalyzer::SetMetadata(MReader* mr) {
    mr_ = mr;
    enabled_ = true; // SVF analyzer is enabled by setting its metadata reader object

    std::string group_log_file, group_1m_log_file;

    // open files for writing
    switch (mode_) {
        case SVF_MODE_NORMAL:
        default:
            group_log_file = SVF_GROUP_LOG_FILE;
            group_1m_log_file = SVF_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_NOCOMP:
            group_log_file = SVF_NC_GROUP_LOG_FILE;
            group_1m_log_file = SVF_NC_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_WRCOMP:
            group_log_file = SVF_WC_GROUP_LOG_FILE;
            group_1m_log_file = SVF_WC_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_RDCOMP:
            group_log_file = SVF_RC_GROUP_LOG_FILE;
            group_1m_log_file = SVF_RC_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_NO_LIVE_VECTOR:
            group_log_file = SVF_NLV_GROUP_LOG_FILE;
            group_1m_log_file = SVF_NLV_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_NO_METADATA:
            group_log_file = SVF_NM_GROUP_LOG_FILE;
            group_1m_log_file = SVF_NM_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_METADATA_IN_UBITS:
            group_log_file = SVF_UBIT_GROUP_LOG_FILE;
            group_1m_log_file = SVF_UBIT_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PERFECT_METADATA:
            group_log_file = SVF_PM_GROUP_LOG_FILE;
            group_1m_log_file = SVF_PM_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_AVFCL:
            group_log_file = AVFCL_GROUP_LOG_FILE;
            group_1m_log_file = AVFCL_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_AVFCL_NO_METADATA:
            group_log_file = AVFCL_NM_GROUP_LOG_FILE;
            group_1m_log_file = AVFCL_NM_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_AVFCL_NOCOMP:
            group_log_file = AVFCL_NC_GROUP_LOG_FILE;
            group_1m_log_file = AVFCL_NC_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_AVFCL_WRCOMP:
            group_log_file = AVFCL_WC_GROUP_LOG_FILE;
            group_1m_log_file = AVFCL_WC_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_AVFCL_RDCOMP:
            group_log_file = AVFCL_RC_GROUP_LOG_FILE;
            group_1m_log_file = AVFCL_RC_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_USE_METADATA:
            group_log_file = SVF_PRF_GROUP_LOG_FILE;
            group_1m_log_file = SVF_PRF_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_NOPS_METADATA:
            group_log_file = SVF_PRFNOP_GROUP_LOG_FILE;
            group_1m_log_file = SVF_PRFNOP_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_NOPS1_METADATA:
            group_log_file = SVF_PRFNOP1_GROUP_LOG_FILE;
            group_1m_log_file = SVF_PRFNOP1_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF1_USE_METADATA:
            group_log_file = SVF_PRF1_GROUP_LOG_FILE;
            group_1m_log_file = SVF_PRF1_1M_GROUP_LOG_FILE;
            break;
        case SVF_MODE_PRF_NOPS2_METADATA:
            group_log_file = SVF_PRFNOP2_GROUP_LOG_FILE;
            group_1m_log_file = SVF_PRFNOP2_1M_GROUP_LOG_FILE;
            break;
    }

    outfile_group_.open(group_log_file.c_str(),
                        std::ios::out | std::ios::trunc);
    outfile_group_1M_.open(group_1m_log_file.c_str(),
                           std::ios::out | std::ios::trunc);
    if (mode_ == SVF_MODE_AVFCL)
        outfile_ecc_.open(ECC_GROUP_LOG_FILE, std::ios::out | std::ios::trunc);
}

void SvfAnalyzer::LogClockTick(LogClockTickArgs args) {
    if (enabled_ == false) return; // check if enabled

    // update PRF-SVF counter based on committed register state
    uint64_t live_count = 0;
    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        if ((0x1ull << i) & (reg_live_vector_ | REG_LIVE_UNTRACKED))
            live_count++;
    }
    if (args.clk > 0 && SvfActive(args.clk-1) &&
        mode_ != SVF_MODE_PRF_WRCOMP && mode_ != SVF_MODE_PRF_RDCOMP &&
        mode_ != SVF_MODE_AVFCL_WRCOMP && mode_ != SVF_MODE_AVFCL_RDCOMP &&
        mode_ != SVF_MODE_PRF_NOPS2_METADATA && mode_ != SVF_MODE_PRF1_USE_METADATA &&
        ms_ == NULL
        ) {
        svfs_[STRUCTURE_RF][idx_] += live_count * SVM->cfg().arch_size_data;
    }
    else if ( (mode_ == SVF_MODE_PRF_NOPS2_METADATA) || (mode_ == SVF_MODE_PRF1_USE_METADATA) ) {
        svfs_[STRUCTURE_RF][idx_] += rf_live_in_first_count_verif * SVM->cfg().arch_size_data;
    }
    else if (ms_) {
        svfs_[STRUCTURE_RF][idx_] += ms_->last_rc * SVM->cfg().arch_size_data;
    }

    if (dps_ ) {
        uint64_t total_mem_structure_vbits = 0;
        for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
            total_mem_structure_vbits += svfs_[i][idx_];
        }
        int64_t tvbits_in_group_1M = (int64_t)dps_->mem_st_tvbits + (int64_t)total_mem_structure_vbits;

        if (tvbits_in_group_1M > dps_->tvbits_threshold && !dps_->was_enabled) {

            SVM->ActivateEcc(); // enable protection
            dps_->was_enabled = true;
            struct ProtectionHistoryEntry phe;
            phe.clk = args.clk;
            phe.status = true;
            SVM->prot_hist_.push_back(phe);
        }
    }

    // check for a phase crossing
    if (IsPhaseCrossing(args.clk, PHASE_PERIOD)) {
        uint64_t total_mem_structure_bits = 0;
        uint64_t total_mem_structure_vbits = 0;
        for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
            total_mem_structure_bits += GetTotalBitsCount((structure_t)i);
        }
        if (idx_ == 0) {
            for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
                if (i != 0) outfile_group_ << "\t";
                outfile_group_ << GetStructureName((structure_t)i);
            }
            outfile_group_ << "\tSys";
            for (int i=0; i<STRUCTURES_FU_COUNT; i++) {
                outfile_group_ << "\t";
                outfile_group_ << GetFuStructureName((structure_fu_t)i);
            }
            outfile_group_ << std::endl;
            outfile_ecc_ << "ecc_status" << std::endl;
        }
        total_mem_structure_vbits = 0;
        for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
            if (i != 0) outfile_group_ << "\t";
            outfile_group_ <<((float)svfs_[i][idx_]/
                (GetTotalBitsCount((structure_t)i)*PHASE_PERIOD));
            total_mem_structure_vbits += svfs_[i][idx_];
            svfs_[i].push_back(0);
        }
        outfile_group_ << "\t";
        outfile_group_ << ((float)total_mem_structure_vbits/
            (total_mem_structure_bits*PHASE_PERIOD));
        for (int i=0; i<STRUCTURES_FU_COUNT; i++) {
            outfile_group_ << "\t";
            outfile_group_ << ((float)svfs_fu_[i][idx_]/(PHASE_PERIOD));
            svfs_fu_[i].push_back(0);
        }
        outfile_group_ << std::endl;

        if (dps_) {
            dps_->mem_st_tvbits += total_mem_structure_vbits;
        }

        if (IsPhaseCrossing(args.clk, PHASE_PERIOD_1M)) {
            int coarsening = PHASE_PERIOD_1M/PHASE_PERIOD;
            if (idx_ < coarsening) {
                for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
                    if (i != 0) outfile_group_1M_ << "\t";
                    outfile_group_1M_ << GetStructureName((structure_t)i);
                }
                outfile_group_1M_ << "\tSys";
                for (int i=0; i<STRUCTURES_FU_COUNT; i++) {
                    outfile_group_1M_ << "\t";
                    outfile_group_1M_ << GetFuStructureName((structure_fu_t)i);
                }
                outfile_group_1M_ << std::endl;
            }
            total_mem_structure_vbits = 0;
            for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
                if (i != 0) outfile_group_1M_ << "\t";
                int64_t total_vbits = 0;
                for (int j=0; j<coarsening; j++) {
                    total_vbits += svfs_[i][idx_-j];
                }
                total_mem_structure_vbits += total_vbits;
                outfile_group_1M_ << ((float)total_vbits/
                    (GetTotalBitsCount((structure_t)i)*PHASE_PERIOD_1M));
            }
            outfile_group_1M_ << "\t";
            outfile_group_1M_ << ((float)total_mem_structure_vbits/
                (total_mem_structure_bits*PHASE_PERIOD_1M));
            for (int i=0; i<STRUCTURES_FU_COUNT; i++) {
                outfile_group_1M_ << "\t";
                int64_t total_vcycles = 0;
                for (int j=0; j<coarsening; j++) {
                    total_vcycles += svfs_fu_[i][idx_-j];
                }
                outfile_group_1M_ << ((float)total_vcycles/(PHASE_PERIOD_1M));
            }
            outfile_group_1M_ << std::endl;

            // handle dynamic protection
            if (dps_) {
                dps_->mem_st_tvbits = 0;
                dps_->was_enabled = false;
                SVM->DeactivateEcc(); // disable protection
                struct ProtectionHistoryEntry phe;
                phe.clk = args.clk;
                phe.status = false;
                SVM->prot_hist_.push_back(phe);
            }
        }

        // increment idx_
        idx_++;

        // dump last phase ECC protection status
        if (mode_ == SVF_MODE_AVFCL)
            outfile_ecc_ << (SVM->EccActive() ? "1" : "0") << std::endl;

        // handle adaptive monitoring
        if (SVM->cfg().adaptive_monitoring) {
            uint64_t group_id = Cycle2Group(args.clk);
            bool next_is_active = (group_id % onoff_period_) < onoff_active_size_;
            active_history_.push_back(next_is_active);
            if (SVM->cfg().enable_svf_adaptive && mode_ == SVF_MODE_NORMAL) {
                // if it toggles from active to inactive
                uint64_t sz = active_history_.size();
                if (sz >= 2 && active_history_[sz-2] && !active_history_[sz-1]) {
                    total_mem_structure_vbits = 0;
                    for (int64_t id=idx_-(onoff_active_size_); id<idx_; id++) {
                        for (int64_t st=0; st<ANALYZED_STRUCTURES_COUNT; st++) {
                            total_mem_structure_vbits += svfs_[st][id];
                        }
                    }
                    float avg_svf = float(total_mem_structure_vbits)/
                        (onoff_active_size_*total_mem_structure_bits*PHASE_PERIOD);

                    // request flush if it exceeds the threshold
                    if (avg_svf > SVM->cfg().svf_adaptive_thd) {
                        SVM->ActivateEcc();
                    }
                    else {
                        SVM->DeactivateEcc();
                    }
                }
            }
        }
        else {
            // SVF always ON
            active_history_.push_back(true);
        }
    }

    if (mode_ == SVF_MODE_METADATA_IN_UBITS) {
        vbits_rf_live_in_first_inst += rf_live_in_first_count_inst;
        vbits_rf_live_in_first += rf_live_in_first_count;
        vbits_rf_live_in_first_verif += rf_live_in_first_count_verif;
        vbits_rf_live_out_first += rf_live_out_first_count;
        vbits_rf_live_in_avg += rf_live_in_avg_count;
        vbits_rf_live_out_avg += rf_live_out_avg_count;

    }
    // clear livevectorfifo if needed
    if (livevectorfifo_.size() > 0 && !MonitoringActive()) {
        livevectorfifo_.clear();
    }
}

void SvfAnalyzer::LogFetch(LogFetchArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) sdur_->ib.UpdateStart(args.seq_no, args.clk);

    // update metadata structure
    if (ms_) {
        if (ms_->last_fetched_pc == simlog_->logdb_[args.seq_no].pc) {
            // the ubits has been included, don't reinclude for pc with multiple microops
        }
        else {
            struct Metadata2 md2 = mr_->GetMetadata2(args.pc);
            uint8_t ubits_cnt = md2.ubits_count;

            // update BB crossing information
            ms_->fetch_is_control_d2 = ms_->fetch_is_control_d;
            ms_->fetch_is_control_d = ms_->fetch_is_control;
            ms_->fetch_is_control = false;
            switch(args.opcode) {
                case OPCODE_J:
                case OPCODE_JAL:
                case OPCODE_JALX:
                case OPCODE_JALR:
                case OPCODE_JALR_HB:
                case OPCODE_JR:
                case OPCODE_JR_HB:
                case OPCODE_BEQ:
                case OPCODE_BNE:
                case OPCODE_BGEZ:
                case OPCODE_BGEZAL:
                case OPCODE_BGTZ:
                case OPCODE_BLEZ:
                case OPCODE_BLTZ:
                case OPCODE_BLTZAL:
                    ms_->fetch_is_control = true;
                    break;

                //* Deprecated Branch Likely Instructions
                case OPCODE_BEQL:
                case OPCODE_BGEZALL:
                case OPCODE_BGEZL:
                case OPCODE_BGTZL:
                case OPCODE_BLEZL:
                case OPCODE_BLTZALL:
                case OPCODE_BLTZL:
                case OPCODE_BNEL:
                    ms_->fetch_is_control_d = true;
                    break;
            }

            // handle initial BB
            if (ms_->last_fetched_bb == (uint64_t)-1) {
                ms_->last_fetched_bb = md2.bb_pc;
            }

            // handle BB crossing
            if (ms_->fetch_is_control_d2 || // the last two inst was a control
                (args.opcode == OPCODE_NOP && !ms_->fetch_is_control_d)) { // NOP found *NOT* at delay slot
                // a BB boundary detected
                if (ms_->ubits_in_bb_max < ms_->curr_bb_ubits_cnt) {
                    ms_->ubits_in_bb_max = ms_->curr_bb_ubits_cnt;
                }
                while (ms_->impq_curr_bb_entry_pushed > ms_->curr_bb_inst_cnt) {
                    assert(ms_->impq.size() > 0);
                    ms_->impq.pop_back();
                    ms_->impq_curr_bb_entry_pushed--;
                }
                while (ms_->impq_curr_bb_entry_pushed < ms_->curr_bb_inst_cnt) {
                    bool important = true;
                    if (ms_->impq.size() >= ms_->impq_size) {
                        // simulate letting tail ptr pass through head
                        important = ms_->impq[ms_->impq.size()-ms_->impq_size];
                    }
                    ms_->impq.push_back(important);
                    ms_->impq_curr_bb_entry_pushed++;
                }
                if (ms_->impq_max_size < ms_->impq.size()) {
                    ms_->impq_max_size = ms_->impq.size();
                }

                // update counters for the new basic block
                ms_->curr_bb_inst_cnt = 0;
                ms_->curr_bb_ubits_cnt = 0;
                ms_->last_fetched_bb = md2.bb_pc;
                ms_->rc_pushed = false;
                ms_->impq_curr_bb_entry_pushed = 0;
            }

            // handle added NOP
            int64_t nops_added = mr_->GetNopsAdded(&md2);
            if (nops_added) {
                // extra u-bits from NOP on the first lane that was skipped
                ubits_cnt += ms_->ubits_on_nop*nops_added;
            }

            uint64_t mbits = 0;
            uint64_t mbits_ptr = 0;
            if (ubits_cnt) {
                mbits = mr_->GetMetadataBits(args.pc);
            }

            // handle updating RCQ
            if (ms_->curr_bb_ubits_cnt < ms_->ubits_for_rc &&
                (ms_->curr_bb_ubits_cnt+ubits_cnt) >= ms_->ubits_for_rc) {
                // update bb_ubits_cnt
                ms_->curr_bb_ubits_cnt += ubits_cnt;
                // rc for current BB has been pushed
                ms_->rc_pushed = true;
                // since some bits in ubits has been used, decrement it
                ubits_cnt -= ms_->ubits_for_rc;
                mbits_ptr += ms_->ubits_for_rc;

                // now register-live count is available
                struct RcqEntry re;
                re.rc = (mbits & 0x1full);
                re.seq_no = args.seq_no;

                int8_t rc_diff = re.rc - ms_->last_pushed_rc;
                if ((rc_diff >= 2 || rc_diff <= -2) && re.rc != 0 && ms_->rcq.size() < ms_->rcq_size) {
                    if (mode_ == SVF_MODE_AVFCL_NO_METADATA) {
                        // don't push RC metadata for this mode
                    }
                    else {
                        ms_->rcq.push(re);
                    }
                    if (ms_->rcq_max_size < ms_->rcq.size()) {
                        ms_->rcq_max_size = ms_->rcq.size();
                    }
                    ms_->last_pushed_rc = re.rc;
                }
            }

            // update current BB instruction counter
            if (args.opcode != OPCODE_NOP) {
                ms_->curr_bb_inst_cnt++; // count non-NOP instructions
            }

            // handle updating ImpQ
            if (ubits_cnt && ms_->rc_pushed) { // make sure RC was pushed and ubits available
                while (ubits_cnt) {
                    bool important = ((mbits >> mbits_ptr) & 0x1) == 0;
                    if (ms_->impq.size() >= ms_->impq_size) {
                        // simulate letting tail ptr pass through head
                        important = ms_->impq[ms_->impq.size()-ms_->impq_size];
                    }
                    ms_->impq.push_back(important);
                    ms_->impq_curr_bb_entry_pushed++;
                    ubits_cnt--;
                    mbits_ptr++;
                }
                if (ms_->impq_max_size < ms_->impq.size()) {
                    ms_->impq_max_size = ms_->impq.size();
                }
            }

            ms_->last_fetched_pc = simlog_->logdb_[args.seq_no].pc;
        }
    }
}

void SvfAnalyzer::LogRename(LogRenameArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) sdur_->ib.UpdateEnd(args.seq_no, args.clk);
}

void SvfAnalyzer::LogDispatch(LogDispatchArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        sdur_->al_dp_ret.UpdateStart(args.seq_no, args.clk);
        sdur_->iq_fast.UpdateStart(args.seq_no, args.clk);
        sdur_->iq_slow.UpdateStart(args.seq_no, args.clk);
    }

    if (MonitoringActive() && (livevectorfifo_.size() < LIVEVECTOR_FIFO_SIZE ||
                               mode_ == SVF_MODE_PERFECT_METADATA)) {
        // only push when monitoring is active and there still some space on the
        // live vector fifo
        struct Metadata2 md2 = mr_->GetMetadata2(simlog_->logdb_[args.seq_no].pc);
        uint64_t current_bb_pc = md2.bb_pc;
        uint8_t nops_added = md2.unused_bit_info.nops_added;
        uint8_t nops_added_scheme_1 = md2.unused_bit_info_scheme_1.nops_added;
        uint8_t nops_added_scheme_2 = md2.unused_bit_info_scheme_2.nops_added;
        uint8_t nops_added_scheme_3 = md2.unused_bit_info_scheme_3.nops_added;
        uint8_t nops_added_scheme_4 = md2.unused_bit_info_scheme_4.nops_added;
        uint8_t nops_added_scheme_5 = md2.unused_bit_info_scheme_5.nops_added;
        uint8_t resv_added = md2.unused_bit_info.resv_added;
        bool m_cache_hit = MetadataAvailable(args.seq_no);

        dispatched_nops_default_ += nops_added;
        dispatched_nops_scheme_1_ += nops_added_scheme_1;
        dispatched_nops_scheme_2_ += nops_added_scheme_2;
        dispatched_nops_scheme_3_ += nops_added_scheme_3;
        dispatched_nops_scheme_4_ += nops_added_scheme_4;
        dispatched_nops_scheme_5_ += nops_added_scheme_5;
        dispatched_resv_default_ += resv_added;

        if (mode_ == SVF_MODE_METADATA_IN_UBITS) {
            if(!m_cache_hit) {
                dispatch_inst_no_metadata_++;
            }
            else {
                dispatch_inst_has_metadata_++;
            }
        }

        if (last_bb_pc_dispatch_ != current_bb_pc) {
            if ( (mode_ == SVF_MODE_NO_LIVE_VECTOR) || (mode_ == SVF_MODE_METADATA_IN_UBITS) ) {
                // for SVF no live vector mode, don't do anything here
            }
            else if (m_cache_hit) {
                // push live vector to a specialized structure
                // (in this case, just push the seq_no of the instruction)
                livevectorfifo_.push_back(args.seq_no);
                livevectorfifo_push_cnt_++;
                if (max_livevectorfifo_depth_ < livevectorfifo_.size()) {
                    max_livevectorfifo_depth_ = livevectorfifo_.size();
                }
            }
            last_bb_pc_dispatch_ = current_bb_pc;
        }
    }
}

void SvfAnalyzer::LogDispatchLoad(LogDispatchLoadArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        sdur_->lq_dp_exec.UpdateStart(args.seq_no, args.clk);
    }
}

void SvfAnalyzer::LogDispatchStore(LogDispatchStoreArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        sdur_->sq_dp_exec.UpdateStart(args.seq_no, args.clk);
    }
}

void SvfAnalyzer::LogIssue(LogIssueArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        if (args.al_entry_tail_diff > AL_ENTRY_TAIL_THRESHOLD) {
            sdur_->iq_slow.UpdateEnd(args.seq_no, args.clk);
        }
        else {
            sdur_->iq_fast.UpdateEnd(args.seq_no, args.clk);
        }
    }
}

void SvfAnalyzer::LogRegRead(LogRegReadArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        sdur_->rf_rr_ret.UpdateStart(args.seq_no, args.clk);
    }
}

void SvfAnalyzer::LogMemExecute(LogMemExecuteArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        if (simlog_->logdb_[args.seq_no].load_id != INVALID_LOAD_ID) {
            sdur_->lq_dp_exec.UpdateEnd(args.seq_no, args.clk);
            sdur_->lq_exec_ret.UpdateStart(args.seq_no, args.clk);
        }
        if (simlog_->logdb_[args.seq_no].store_id != INVALID_STORE_ID) {
            sdur_->sq_dp_exec.UpdateEnd(args.seq_no, args.clk);
            sdur_->sq_exec_ret.UpdateStart(args.seq_no, args.clk);
        }
    }
}

void SvfAnalyzer::LogWriteBack(LogWriteBackArgs args) {
    if (enabled_ == false) return; // check if enabled

    // handle sampled duration
    if (sdur_) {
        if (simlog_->logdb_[args.seq_no].al_entry_tail_diff > AL_ENTRY_TAIL_THRESHOLD) {
            sdur_->al_wb_ret_slow.UpdateStart(args.seq_no, args.clk);
        }
        else {
            sdur_->al_wb_ret_fast.UpdateStart(args.seq_no, args.clk);
        }
        sdur_->rf_wb_ret.UpdateStart(args.seq_no, args.clk);
    }
}

void SvfAnalyzer::LogRetire(LogRetireArgs args) {
    if (enabled_ == false) return; // check if enabled
    uint64_t dur, vbits;
    bool svf_active;

    // handle duration
    if (sdur_) {
        sdur_->al_dp_ret.UpdateEnd(args.seq_no, args.clk);
        sdur_->al_wb_ret_fast.UpdateEnd(args.seq_no, args.clk);
        sdur_->al_wb_ret_slow.UpdateEnd(args.seq_no, args.clk);
        sdur_->lq_exec_ret.UpdateEnd(args.seq_no, args.clk);
        sdur_->rf_rr_ret.UpdateEnd(args.seq_no, args.clk);
        sdur_->rf_wb_ret.UpdateEnd(args.seq_no, args.clk);

        // notify sdur
        sdur_->ib.Retire();
        sdur_->iq_fast.Retire();
        sdur_->iq_slow.Retire();
        sdur_->al_wb_ret_slow.Retire();
        sdur_->al_wb_ret_fast.Retire();
    }

    // read metadata
    struct Metadata md = mr_->GetMetadata(simlog_->logdb_[args.seq_no].pc);
    bool important = md.rw.important;
    struct Metadata2 md2 = mr_->GetMetadata2(simlog_->logdb_[args.seq_no].pc);
    uint64_t current_bb_pc = md2.bb_pc;
    uint8_t nops_added = md2.unused_bit_info.nops_added;
    uint8_t nops_added_scheme_1 = md2.unused_bit_info_scheme_1.nops_added;
    uint8_t nops_added_scheme_2 = md2.unused_bit_info_scheme_2.nops_added;
    uint8_t nops_added_scheme_3 = md2.unused_bit_info_scheme_3.nops_added;
    uint8_t nops_added_scheme_4 = md2.unused_bit_info_scheme_4.nops_added;
    uint8_t nops_added_scheme_5 = md2.unused_bit_info_scheme_5.nops_added;
    uint8_t resv_added = md2.unused_bit_info.resv_added;
    bool m_cache_hit = MetadataAvailable(args.seq_no);

    retired_nops_default_ += nops_added;
    retired_nops_scheme_1_ += nops_added_scheme_1;
    retired_nops_scheme_2_ += nops_added_scheme_2;
    retired_nops_scheme_3_ += nops_added_scheme_3;
    retired_nops_scheme_4_ += nops_added_scheme_4;
    retired_nops_scheme_5_ += nops_added_scheme_5;
    retired_resv_default_ += resv_added;

    if (mode_ == SVF_MODE_METADATA_IN_UBITS) {
        if(!m_cache_hit) {
            retire_inst_no_metadata_++;
        }
        else {
            retire_inst_has_metadata_++;
        }
    }

    // handle basic block statistics
    if (bbstat_) {
        // this should be handled after a control instruction was executed
        bbstat_->is_control_d2 = bbstat_->is_control_d;
        bbstat_->is_control_d = bbstat_->is_control;
        bbstat_->is_control = false;

        if (bbstat_->is_control_d2) {
            // control instruction was explicitly detected
            bbstat_->boundary_after_control++;
            bbstat_->boundary_was_detected = true;
        }
    }

    // handle metadata structure
    if (ms_) {
        // update current_bb_rc if needed
        if (current_bb_pc != last_bb_pc_) {
            // BB crossing, update bb_rc
            ms_->current_md_bb_rc = md2.reg_live_count.live_in_count_first;
        }

        // handle RCQ
        if (ms_->rcq.size() > 0) {
            if (args.seq_no == ms_->rcq.front().seq_no) {
                ms_->last_rc = ms_->rcq.front().rc;
                ms_->rcq.pop(); // remove the entry at the head

                int16_t diff = ms_->last_rc - ms_->current_md_bb_rc;
                if (diff < 0) diff = -diff;
                if (diff <= 1) { // threshold of 1
                    ms_->rc_matches++;
                }
                else {
                    ms_->rc_mismatches++;
                }
            }
            else {
                // make sure we are not missing an instruction during commit
                assert(args.seq_no < ms_->rcq.front().seq_no);
            }
        }

        // handle ImpQ
        if (simlog_->logdb_[args.seq_no].opcode == OPCODE_NOP) {
            important = false; // NOP is always unimportant
        }
        else if (ms_->last_retired_pc == simlog_->logdb_[args.seq_no].pc) {
            // retiring the same PC as before
            // copy important from the previous retired instruction
            important = ms_->last_retired_important;
        }
        else {
            if (ms_->impq.size() > 0) {
                important = ms_->impq.front();
                ms_->impq.pop_front();
                ms_->retire_with_entry_in_impq++;
            }
            else {
                // handle cases at which no entry is available in ImpQ
                assert(ms_->curr_bb_inst_cnt > 0);
                ms_->curr_bb_inst_cnt--;
                ms_->retire_no_entry_in_impq++;
                important = true;
            }
            if (mode_ == SVF_MODE_AVFCL_NO_METADATA) {
                // force important to be true
                important = true;
            }
        }


        if (important == md.rw.important) {
            ms_->important_matches++;
        }
        else {
            ms_->important_mismatches++;
        }

        if (simlog_->logdb_[args.seq_no].store_id != INVALID_STORE_ID) {
            struct SqImp si;
            si.important = important;
            si.seq_no = args.seq_no;
            ms_->sq_importance.push(si);
        }

        // log the last retired PC
        ms_->last_retired_pc = simlog_->logdb_[args.seq_no].pc;
        ms_->last_retired_important = important;
    }

    ////////////////////////////////////////////////////////////////////////////

    // handle IQ
    if (!SVM->cfg().nop_issued &&
        simlog_->logdb_[args.seq_no].opcode == OPCODE_NOP) {
        // NOP doesn't occupy issue queue
    }
    else if (simlog_->logdb_[args.seq_no].issued_clk < args.clk) {
        svf_active = true; // by default, SVF is always ON
        if (SVM->cfg().adaptive_monitoring) {
            svf_active =
                SvfActive(simlog_->logdb_[args.seq_no].retired_clk) &&
                SvfActive(simlog_->logdb_[args.seq_no].issued_clk) &&
                SvfActive(simlog_->logdb_[args.seq_no].dispatched_clk);
        }
        if (svf_active) {
            dur = CalcDuration(simlog_->logdb_[args.seq_no].dispatched_clk,
                simlog_->logdb_[args.seq_no].issued_clk);
            if (sdur_) {
                if (simlog_->logdb_[args.seq_no].al_entry_tail_diff > AL_ENTRY_TAIL_THRESHOLD) {
                    dur = CalcDuration(sdur_->iq_slow.t_start, sdur_->iq_slow.t_end);
                }
                else {
                    dur = CalcDuration(sdur_->iq_fast.t_start, sdur_->iq_fast.t_end);
                }
            }
            if (ic_.iq%CD_UPDATE_IQ == 0) {
                cd_.iq = dur;
            }
            else {
                dur = cd_.iq;
            }
            vbits = CalcVbits((dur % MAX_DUR),
                GetVulnerableBitCountAppx(STRUCTURE_IQ,
                                      simlog_->logdb_[args.seq_no].opcode,
                                      m_cache_hit ? important : true));
            UpdateVbitsStats(dur, vbits, STRUCTURE_IQ);
            svfs_[STRUCTURE_IQ][idx_] += vbits % MAX_VBITS;
            ic_.iq++;
        }
    }
    else {
        std::cout << TERM_YELLOW <<
            "[svf_analyzer] WARNING: invalid issued_clk for opcode " <<
            simlog_->logdb_[args.seq_no].opcode << TERM_RESET << std::endl;
    }

    // handle ROB
    svf_active = true; // by default, SVF is always ON
    if (SVM->cfg().adaptive_monitoring) {
        if (simlog_->logdb_[args.seq_no].writeback_clk != INVALID_CLK) {
            svf_active =
                SvfActive(simlog_->logdb_[args.seq_no].retired_clk) &&
                SvfActive(simlog_->logdb_[args.seq_no].writeback_clk) &&
                SvfActive(simlog_->logdb_[args.seq_no].dispatched_clk);
        }
        else { // instruction didn't go through execution pipeline, check other timestamps
            svf_active =
                SvfActive(simlog_->logdb_[args.seq_no].retired_clk) &&
                SvfActive(simlog_->logdb_[args.seq_no].dispatched_clk);
        }
    }
    if (svf_active) {
        if (simlog_->logdb_[args.seq_no].writeback_clk != INVALID_CLK) {
            uint64_t dur_wb_ret = 1; // default to one cycle, just in case
            dur = CalcDuration(simlog_->logdb_[args.seq_no].dispatched_clk,
                simlog_->logdb_[args.seq_no].writeback_clk);
            if (sdur_) {
                dur = CalcDuration(sdur_->al_dp_ret.t_start, sdur_->al_dp_ret.t_end);
                if (simlog_->logdb_[args.seq_no].al_entry_tail_diff > AL_ENTRY_TAIL_THRESHOLD) {
                    dur_wb_ret = CalcDuration(sdur_->al_wb_ret_slow.t_start, sdur_->al_wb_ret_slow.t_end);
                }
                else {
                    dur_wb_ret = CalcDuration(sdur_->al_wb_ret_fast.t_start, sdur_->al_wb_ret_fast.t_end);
                }
                if (dur > dur_wb_ret) {
                    dur -= dur_wb_ret;
                }
                else {
                    dur = 0;
                }
            }
            if (ic_.rob%CD_UPDATE_ROB == 0) {
                cd_.rob = dur;
            }
            else {
                dur = cd_.rob;
            }
            vbits = CalcVbits((dur % MAX_DUR),
                GetVulnerableBitCountAppx(STRUCTURE_ROB,
                                      simlog_->logdb_[args.seq_no].opcode,
                                      m_cache_hit ? important : true, true));
            svfs_[STRUCTURE_ROB][idx_] += vbits % MAX_VBITS;
            UpdateVbitsStats(dur, vbits, STRUCTURE_ROB);
            ic_.rob++;

            dur = CalcDuration(simlog_->logdb_[args.seq_no].writeback_clk,
                args.clk);
            if (sdur_) {
                dur = dur_wb_ret;
            }
            vbits = CalcVbits((dur % MAX_DUR),
                GetVulnerableBitCountAppx(STRUCTURE_ROB,
                                      simlog_->logdb_[args.seq_no].opcode,
                                      m_cache_hit ? important : true, false));
            svfs_[STRUCTURE_ROB][idx_] += vbits % MAX_VBITS;
            UpdateVbitsStats(dur, vbits, STRUCTURE_ROB);
        }
        else {
            // combine dispatch-writeback and writeback-retire stage
            dur = CalcDuration(simlog_->logdb_[args.seq_no].dispatched_clk,
                args.clk);
            if (sdur_) {
                dur = CalcDuration(sdur_->al_dp_ret.t_start, sdur_->al_dp_ret.t_end);
            }
            if (ic_.rob%CD_UPDATE_ROB == 0) {
                cd_.rob = dur;
            }
            else {
                dur = cd_.rob;
            }
            vbits = CalcVbits((dur % MAX_DUR),
                GetVulnerableBitCountAppx(STRUCTURE_ROB,
                                      simlog_->logdb_[args.seq_no].opcode,
                                      m_cache_hit ? important : true, false));
            svfs_[STRUCTURE_ROB][idx_] += vbits % MAX_VBITS;
            UpdateVbitsStats(dur, vbits, STRUCTURE_ROB);
            ic_.rob++;
        }
    }

    if (simlog_->logdb_[args.seq_no].load_id != INVALID_LOAD_ID) {
        // handle LQ
        if (simlog_->logdb_[args.seq_no].memexecute_clk < args.clk &&
            simlog_->logdb_[args.seq_no].writeback_clk < args.clk) {

            svf_active = true; // by default, SVF is always ON
            if (SVM->cfg().adaptive_monitoring) {
                svf_active =
                    SvfActive(simlog_->logdb_[args.seq_no].retired_clk) &&
                    SvfActive(simlog_->logdb_[args.seq_no].memexecute_clk) &&
                    SvfActive(simlog_->logdb_[args.seq_no].dispatched_clk);
            }

            if (svf_active) {
                dur = CalcDuration(simlog_->logdb_[args.seq_no].dispatched_clk,
                    simlog_->logdb_[args.seq_no].memexecute_clk);
                if (sdur_) {
                    dur = CalcDuration(sdur_->lq_dp_exec.t_start, sdur_->lq_dp_exec.t_end);
                }
                if (ic_.lq%CD_UPDATE_LQ == 0) {
                    cd_.lq1 = dur;
                }
                else {
                    dur = cd_.lq1;
                }
                vbits = CalcVbits((dur % MAX_DUR),
                    GetVulnerableBitCountAppx(STRUCTURE_LQ,
                                          simlog_->logdb_[args.seq_no].opcode,
                                          m_cache_hit ? important : true, true));
                svfs_[STRUCTURE_LQ][idx_] += vbits % MAX_VBITS;
                UpdateVbitsStats(dur, vbits, STRUCTURE_LQ);

                dur = CalcDuration(simlog_->logdb_[args.seq_no].memexecute_clk,
                    simlog_->logdb_[args.seq_no].retired_clk);
                if (sdur_) {
                    dur = CalcDuration(sdur_->lq_exec_ret.t_start, sdur_->lq_exec_ret.t_end);
                }
                if (ic_.lq%CD_UPDATE_LQ == 0) {
                    cd_.lq2 = dur;
                }
                else {
                    dur = cd_.lq2;
                }
                vbits = CalcVbits((dur % MAX_DUR),
                    GetVulnerableBitCountAppx(STRUCTURE_LQ,
                                          simlog_->logdb_[args.seq_no].opcode,
                                          m_cache_hit ? important : true));
                svfs_[STRUCTURE_LQ][idx_] += vbits % MAX_VBITS;
                UpdateVbitsStats(dur, vbits, STRUCTURE_LQ);
                ic_.lq++;
            }
        }
        else {
            std::cout << TERM_YELLOW <<
                "[svf_analyzer] WARNING: invalid memexecute_clk for opcode " <<
                simlog_->logdb_[args.seq_no].opcode << TERM_RESET << std::endl;
        }
    }

    // handle IBUFF
    svf_active = true; // by default, SVF is always ON
    if (SVM->cfg().adaptive_monitoring) {
        svf_active =
            SvfActive(simlog_->logdb_[args.seq_no].retired_clk) &&
            SvfActive(simlog_->logdb_[args.seq_no].decoded_clk);
    }
    if (svf_active) {
        dur = CalcDuration(simlog_->logdb_[args.seq_no].decoded_clk,
            simlog_->logdb_[args.seq_no].decoded_clk+
            simlog_->logdb_[args.seq_no].ibuff_duration);
        if (sdur_) {
            dur = CalcDuration(sdur_->ib.t_start, sdur_->ib.t_end);
        }
        if (ic_.ib%CD_UPDATE_IB == 0) {
            cd_.ib = dur;
        }
        else {
            dur = cd_.ib;
        }
        vbits = CalcVbits((dur % MAX_DUR), GetVulnerableBitCountAppx(STRUCTURE_IBUFF,
                                            simlog_->logdb_[args.seq_no].opcode,
                                            m_cache_hit ? important : true));
        svfs_[STRUCTURE_IBUFF][idx_] += vbits % MAX_VBITS;
        UpdateVbitsStats(dur, vbits, STRUCTURE_IBUFF);
        ic_.ib++;
    }

    if (mode_ == SVF_MODE_METADATA_IN_UBITS) {
        uint64_t reg_live_in_first_vector = md2.live_vector;
        rf_live_in_first_count_inst = 0;
        for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
            if ((0x1ull << i) & reg_live_in_first_vector) rf_live_in_first_count_inst++;
        }
    }

    // handle PRF - part0: if it is a new basic block, mask the live vector
    //                     as way of killing registers due to control flow
    if (last_bb_pc_ != current_bb_pc) {
        uint64_t reg_live_in_first_vector = md2.live_vector;
        rf_live_in_first_count = 0;
        for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
            if ((0x1ull << i) & reg_live_in_first_vector) rf_live_in_first_count++;
        }

        if ( (mode_ != SVF_MODE_PRF_NOPS2_METADATA) || (md2.unused_bit_info_scheme_5.live_vector_info_present == 1) ) {
            rf_live_in_first_count_verif = md2.reg_live_count.live_in_count_first;
        }
        rf_live_out_first_count      = md2.reg_live_count.live_out_count_first;
        rf_live_in_avg_count         = md2.reg_live_count.live_in_count_avg;
        rf_live_out_avg_count        = md2.reg_live_count.live_out_count_avg;

        if (last_bb_pc_ == INITIAL_BB_PC) {
            reg_live_vector_ = md2.live_vector;

            // compensate due to initial SVF-PRF condition
            uint64_t init_live_amount = 0;
            for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
                if ((0x1ull << i) & reg_live_vector_) init_live_amount++;
            }
            if (mode_ != SVF_MODE_PRF_WRCOMP && mode_ != SVF_MODE_PRF_RDCOMP &&
                mode_ != SVF_MODE_AVFCL_WRCOMP && mode_ != SVF_MODE_AVFCL_RDCOMP &&
                mode_ != SVF_MODE_AVFCL && mode_ != SVF_MODE_AVFCL_NO_METADATA) {
                svfs_[STRUCTURE_RF][idx_] +=
                    args.clk * init_live_amount * SVM->cfg().arch_size_data;
            }
        }
        else {
            // handle basic block stats
            if (bbstat_) {
                if (bbstat_->is_control_d2) {
                    // the previous two committed instruction was a control inst
                    // stats has been updated earlier
                }
                else if (mr_->GetNopsAdded(&md2) > 0) {
                    // a nop indicates the boundary of basic block
                    bbstat_->boundary_on_nop++;
                    bbstat_->boundary_was_detected = true;
                }
                else {
                    // no explicit basic block boundary detected
                    bbstat_->boundary_undetected++;
                    bbstat_->boundary_was_detected = false;
                    bbstat_->undetected_bb[current_bb_pc]++;
                }
            }
        }

        // mask current reg_live_vector with live vector of current basic block
        // if metadata cache access was hit
        if ( (mode_ == SVF_MODE_NO_LIVE_VECTOR) || (mode_ == SVF_MODE_METADATA_IN_UBITS) || 
            ((mode_ == SVF_MODE_PRF_NOPS_METADATA) && (md2.unused_bit_info_scheme_4.live_vector_info_present == 0)) ||
            ((mode_ == SVF_MODE_PRF_NOPS1_METADATA) && (md2.unused_bit_info_scheme_5.live_vector_info_present == 0)) ) {
            // for SVF no live vector mode, don't do anything here
        }
        else if ( m_cache_hit ||
                 ((mode_ == SVF_MODE_PRF_NOPS_METADATA) && (md2.unused_bit_info_scheme_4.live_vector_info_present == 1)) ||
                 ((mode_ == SVF_MODE_PRF_NOPS1_METADATA) && (md2.unused_bit_info_scheme_5.live_vector_info_present == 1))) {
            while (livevectorfifo_.size() > 0) {
                uint64_t seq_no_from_fifo = livevectorfifo_.front();

                if (seq_no_from_fifo == args.seq_no) {
                    livevectorfifo_.pop_front();
                    livevectorfifo_pop_cnt_++;
                    if ( (mode_ == SVF_MODE_PRF_USE_METADATA) ||
                         (mode_ == SVF_MODE_PRF_NOPS_METADATA) ||
                         (mode_ == SVF_MODE_PRF_NOPS1_METADATA) ) {
                        // reg_live_vector is based on metadata
                        reg_live_vector_ = md2.live_vector;
                    }
                    else {
                        reg_live_vector_ &= md2.live_vector;
                    }
                    break;
                }
                else if (seq_no_from_fifo < args.seq_no) {
                    // just pop the head of fifo and try again
                    livevectorfifo_.pop_front();
                    continue;
                }
                else {
                    // well, the entry is not in fifo after all, break
                    missed_livevectorfifo_entries_++;
                    break;
                }
            }
        }

        // set current BB PC as current
        last_bb_pc_ = current_bb_pc;
    }

    // handle basic block stats
    if (bbstat_) {
        if (bbstat_->boundary_was_detected) {
            bbstat_->metadata_available_cnt++;
        }
        else {
            bbstat_->metadata_not_available_cnt++;
        }
    }

    // handle PRF - part1: compensation due to killed registers
    dur = CalcDuration(simlog_->logdb_[args.seq_no].regread_clk, args.clk);
    if (sdur_) {
        dur = CalcDuration(sdur_->rf_rr_ret.t_start, sdur_->rf_rr_ret.t_end);
    }
    uint64_t kill_vector = 0;
    uint64_t kill_amount = 0;

    if (!(simlog_->logdb_[args.seq_no].is_syscall)) {
        // for non-syscall instructions
        if (m_cache_hit && SvfActive(args.clk)) {
            // only kill if metadata cache acceses was hit
            // and SVF monitoring is active
            bool generic_ops = true;

            // check if special handling needed
            switch (simlog_->logdb_[args.seq_no].opcode) {
                // for SC and SCD instructions
                case OPCODE_SC_0:
                case OPCODE_SCD_0:
                    break; // can be handled using generic handler
                case OPCODE_SC_1:
                case OPCODE_SCD_1:
                    // it reads R0 and read R0 is ignored in SVF analyzer.
                    // thus, nothing needs to be done here.
                    generic_ops = false;
                    break;

                // for MOVN and MOVZ instructions
                case OPCODE_MOVN_0:
                case OPCODE_MOVZ_0:
                    // read RS and RT (both depend on metadata)
                    kill_vector |= SingleKill(args.seq_no, 0, md.rw.decr_rrs,
                                              md.rw.decr_rs);
                    kill_vector |= SingleKill(args.seq_no, 1, md.rw.decr_rrt,
                                              md.rw.decr_rt);
                    generic_ops = false;
                    break;
                case OPCODE_MOVN_1:
                case OPCODE_MOVZ_1:
                    // read REG_INT_TEMP_0 (always kills) and RD
                    // (depends on metadata)
                    kill_vector |= SingleKill(args.seq_no, 0, R_INT_TEMP_0,
                                              true);
                    kill_vector |= SingleKill(args.seq_no, 1, md.rw.decr_rrd,
                                              md.rw.decr_rd);
                    generic_ops = false;
                    break;

                // by default, generic operation
                default:
                    break;
            }

            if (generic_ops) {
                kill_vector |= SingleKill(args.seq_no, md.rw.decr_rrd,
                                          md.rw.decr_rd);
                kill_vector |= SingleKill(args.seq_no, md.rw.decr_rrt,
                                          md.rw.decr_rt);
                kill_vector |= SingleKill(args.seq_no, md.rw.decr_rrs,
                                          md.rw.decr_rs);
                kill_vector |= SingleKill(args.seq_no, R_LO, md.rw.decr_lo);
                kill_vector |= SingleKill(args.seq_no, R_HI, md.rw.decr_hi);
            }
        }
    }
    else {
        // for syscall
        // syscall knows what to kill, ignore the m_cache_hit status
        // and do this only when SVF monitoring is active
        if (SvfActive(args.clk)) {
            kill_vector |= SingleKill(args.seq_no, md.rw.decr_rrd, md.rw.decr_rd);
            kill_vector |= SingleKill(args.seq_no, md.rw.decr_rrt, md.rw.decr_rt);
            kill_vector |= SingleKill(args.seq_no, md.rw.decr_rrs, md.rw.decr_rs);
            kill_vector |= SingleKill(args.seq_no, R_A2, md.rw.decr_lo);
        }
    }

    if (mode_ == SVF_MODE_PRF_USE_METADATA ||
        mode_ == SVF_MODE_PRF1_USE_METADATA ||
        mode_ == SVF_MODE_PRF_NOPS_METADATA ||
        mode_ == SVF_MODE_PRF_NOPS1_METADATA ||
        mode_ == SVF_MODE_PRF_NOPS2_METADATA ||
        mode_ == SVF_MODE_PRF_WRCOMP ||
        mode_ == SVF_MODE_AVFCL_WRCOMP ||
        mode_ == SVF_MODE_AVFCL ||
        mode_ == SVF_MODE_AVFCL_NO_METADATA) {
        // always assume 0 on kill vector for this mode
        kill_vector = 0;
    }

    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        if ((0x1ull << i) & kill_vector) kill_amount++;
    }

    if (kill_amount > 0) {
        if (!(simlog_->logdb_[args.seq_no].is_syscall) &&
            SvfActive(args.clk) &&
            mode_ != SVF_MODE_PRF_NOCOMP && mode_ != SVF_MODE_AVFCL_NOCOMP) {
            vbits = CalcVbits(kill_amount * (dur % MAX_DUR),
                SVM->cfg().arch_size_data);
            UpdateVbitsStats(dur, vbits, STRUCTURE_RF);
            svfs_[STRUCTURE_RF][idx_] -= vbits % (MAX_VBITS);
        }
        else {
            // for syscall, assume read when syscall retires
            // (i.e. no compensation)
        }
        // clear reg_live_vector bits
        reg_live_vector_ &= ~kill_vector;
    }

    // handle PRF - part2: add vbits due to new live registers
    //                     if the instruction produces a register or is
    //                     a syscall
    if (simlog_->logdb_[args.seq_no].destinations.size() > 0 ||
        simlog_->logdb_[args.seq_no].is_syscall) {

        dur = CalcDuration(simlog_->logdb_[args.seq_no].writeback_clk, args.clk);
        if (sdur_) {
            dur = CalcDuration(sdur_->rf_wb_ret.t_start, sdur_->rf_wb_ret.t_end);
        }
        uint64_t live_vector = 0;
        uint64_t live_amount = 0;

        if (!(simlog_->logdb_[args.seq_no].is_syscall)) {
            int16_t log = simlog_->logdb_[args.seq_no].destinations[0].log;
            // for non-syscall instructions
            if (m_cache_hit && SvfActive(args.clk)) {
                // use information from metadata
                bool generic_ops = true;

                // check if special handling needed
                switch (simlog_->logdb_[args.seq_no].opcode) {
                    // for MOVN and MOVZ instructions
                    case OPCODE_MOVN_0:
                    case OPCODE_MOVZ_0:
                        // write to REG_INT_TEMP_0 (depends on incr_exp
                        // metadata, i.e. final output)
                        live_vector |= SingleWrite(args.seq_no, R_INT_TEMP_0,
                                                   md.rw.incr_exp);
                        generic_ops = false;
                        break;

                    case OPCODE_MOVN_1:
                    case OPCODE_MOVZ_1:
                        break; // can be handled using generic handler

                    // by default, generic operation
                    default:
                        break;
                }
                if (generic_ops) {
                    if (log == R_LO)
                        live_vector |= SingleWrite(args.seq_no, R_LO,
                                                   md.rw.incr_lo);
                    else if (log == R_HI)
                        live_vector |= SingleWrite(args.seq_no, R_HI,
                                                   md.rw.incr_hi);
                    else
                        live_vector |= SingleWrite(args.seq_no, md.rw.incr_rexp,
                                                   md.rw.incr_exp);
                }
            }
            else {
                // assume the target register is always become live
                live_vector |= (0x1ull) << log;
            }
        }
        else {
            if (SvfActive(args.clk)) {
                live_vector |= SingleWrite(args.seq_no, md.rw.incr_rexp,
                                           md.rw.incr_exp);
                live_vector |= SingleWrite(args.seq_no, R_A3, md.rw.incr_lo);
            }
        }

        if (mode_ == SVF_MODE_PRF_USE_METADATA ||
            mode_ == SVF_MODE_PRF1_USE_METADATA ||
            mode_ == SVF_MODE_PRF_NOPS_METADATA ||
            mode_ == SVF_MODE_PRF_NOPS1_METADATA ||
            mode_ == SVF_MODE_PRF_NOPS2_METADATA ||
            mode_ == SVF_MODE_PRF_RDCOMP ||
            mode_ == SVF_MODE_AVFCL_RDCOMP ||
            mode_ == SVF_MODE_AVFCL ||
            mode_ == SVF_MODE_AVFCL_NO_METADATA) {
            // always assume 0 on live vector for this mode
            live_vector = 0;
        }

        for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
            if ((0x1ull << i) & live_vector) live_amount++;
        }

        if (live_amount > 0) {
            if (!(simlog_->logdb_[args.seq_no].is_syscall) &&
                SvfActive(args.clk) && mode_ != SVF_MODE_PRF_NOCOMP && mode_ != SVF_MODE_AVFCL_NOCOMP) {
                vbits = CalcVbits(live_amount * (dur % MAX_DUR),
                    SVM->cfg().arch_size_data);
                svfs_[STRUCTURE_RF][idx_] += vbits % MAX_VBITS;
                UpdateVbitsStats(dur, vbits, STRUCTURE_RF);
            }
            else {
                // for syscall, assume write when syscall retires
                // (i.e. no additional vbits)
            }
            // set reg_live_vector bits
            reg_live_vector_ |= live_vector;
        }
    }

    bool handle_callee_saved = true; // by default, handle callee saved using stack
    if (mode_ != SVF_MODE_PRF_USE_METADATA &&
        mode_ != SVF_MODE_PRF1_USE_METADATA &&
        mode_ != SVF_MODE_PRF_NOPS_METADATA &&
        mode_ != SVF_MODE_PRF_NOPS1_METADATA &&
        mode_ != SVF_MODE_PRF_NOPS2_METADATA &&
        mode_ != SVF_MODE_PRF_WRCOMP &&
        mode_ != SVF_MODE_PRF_RDCOMP &&
        mode_ != SVF_MODE_AVFCL_WRCOMP &&
        mode_ != SVF_MODE_AVFCL_RDCOMP &&
        mode_ != SVF_MODE_AVFCL &&
        mode_ != SVF_MODE_AVFCL_NO_METADATA)
        handle_callee_saved = false; // don't include it for the listed modes

    // handle callee saved registers
    if (handle_callee_saved && simlog_->logdb_[args.seq_no].is_call) {
        call_cnt_++;
        savedregstack_.push_back(reg_live_vector_ & REG_CALLEE_SAVED_MASK);

        // update max_callee_saved_stack_usage_
        if (max_callee_saved_stack_usage_ < savedregstack_.size()) {
            max_callee_saved_stack_usage_ = savedregstack_.size();
            if (max_callee_saved_stack_usage_ > CALLEE_SAVED_STACK_SIZE)
                dropped_callee_saved_stack_entries_ =
                    max_callee_saved_stack_usage_ - CALLEE_SAVED_STACK_SIZE;
        }
    }
    if (handle_callee_saved && simlog_->logdb_[args.seq_no].is_return) {
        return_cnt_++;
        if (savedregstack_.size() == 0) {
            std::cout << TERM_YELLOW <<
                "[svf_analyzer] WARNING: call-return is not balance." <<
                TERM_RESET << std::endl;
        }
        else if (savedregstack_.size() > dropped_callee_saved_stack_entries_) {
            // only pop undropped callee saved stack entry
            uint64_t live_vector = savedregstack_.back();
            savedregstack_.pop_back();
            // kill registers that were dead
            reg_live_vector_ &= (live_vector | ~REG_CALLEE_SAVED_MASK);
        }
        else {
            missed_callee_saved_stack_entries_++;
        }
    }

    // handle functional units AVFs
    svf_active = true; // by default, SVF is always ON
    if (SVM->cfg().adaptive_monitoring) {
        svf_active = SvfActive(simlog_->logdb_[args.seq_no].retired_clk);
    }
    if (svf_active) {
        if ((m_cache_hit ? important : true) &&
            !(simlog_->logdb_[args.seq_no].is_syscall)) {

            switch(simlog_->logdb_[args.seq_no].fu) {
                case INSTRUCTION_TYPE_SIMPLE:
                    svfs_fu_[STRUCTURE_FU_SIMPLE][idx_]++;
                    break;
                case INSTRUCTION_TYPE_COMPLEX:
                    svfs_fu_[STRUCTURE_FU_COMPLEX][idx_]++;
                    break;
                case INSTRUCTION_TYPE_CONTROL:
                    svfs_fu_[STRUCTURE_FU_CONTROL][idx_]++;
                    break;
                case INSTRUCTION_TYPE_MEMORY:
                    svfs_fu_[STRUCTURE_FU_MEMORY][idx_]++;
                    break;
                case INSTRUCTION_TYPE_FP:
                default:
                    ;//std::cout << TERM_YELLOW <<
                        //"[svf_analyzer] WARNING: non-integer FU for opcode " <<
                        //simlog_->logdb_[seq_no].opcode << " PC: " <<
                        //simlog_->logdb_[seq_no].pc << TERM_RESET << std::endl;
                    break;
            }
        }
    }
}

void SvfAnalyzer::LogControlCommit(LogControlCommitArgs args) {
    if (bbstat_) {
        bbstat_->is_control = true;
    }
    return;
}

void SvfAnalyzer::LogStoreCommit(LogStoreCommitArgs args) {
    if (enabled_ == false) return; // check if enabled
    if (simlog_->storedb_.count(args.store_id) == 0) {
        std::cout << TERM_YELLOW <<
            "[svf_analyzer] WARNING: Invalid store commit with store_id " <<
            std::hex << args.store_id << " at clk " << args.clk <<
            TERM_RESET << std::endl;
        return;
    }

    uint64_t seq_no = simlog_->storedb_[args.store_id];
    if (simlog_->logdb_.count(seq_no)) {
        uint64_t dur, vbits;
        bool svf_active;

        // handle sampled duration
        if (sdur_) {
            sdur_->sq_exec_ret.UpdateEnd(seq_no, args.clk);
        }

        svf_active = true; // by default, SVF is always ON
        if (SVM->cfg().adaptive_monitoring) {
            svf_active =
                SvfActive(simlog_->logdb_[seq_no].storecommit_clk) &&
                SvfActive(simlog_->logdb_[seq_no].memexecute_clk) &&
                SvfActive(simlog_->logdb_[seq_no].dispatched_clk);
        }

        if (svf_active) {
            // read metadata
            struct Metadata md = mr_->GetMetadata(simlog_->logdb_[seq_no].pc);
            bool important = md.rw.important;
            bool m_cache_hit = MetadataAvailable(seq_no);

            if (ms_) {
                assert(ms_->sq_importance.size() > 0);
                assert(ms_->sq_importance.front().seq_no == seq_no);
                important = ms_->sq_importance.front().important;
                ms_->sq_importance.pop();
            }

            dur = CalcDuration(simlog_->logdb_[seq_no].dispatched_clk,
                simlog_->logdb_[seq_no].memexecute_clk);
            if (sdur_) {
                dur = CalcDuration(sdur_->sq_dp_exec.t_start, sdur_->sq_dp_exec.t_end);
            }
            if (ic_.sq%CD_UPDATE_SQ == 0) {
                cd_.sq1 = dur;
            }
            else {
                dur = cd_.sq1;
            }
            vbits = CalcVbits((dur % MAX_DUR), GetVulnerableBitCountAppx(STRUCTURE_SQ,
                simlog_->logdb_[seq_no].opcode,
                m_cache_hit ? important : true, 2, true));
            svfs_[STRUCTURE_SQ][idx_] += vbits % MAX_VBITS;
            UpdateVbitsStats(dur, vbits, STRUCTURE_SQ);

            dur = CalcDuration(simlog_->logdb_[seq_no].memexecute_clk,
                simlog_->logdb_[seq_no].storecommit_clk);
            if (sdur_) {
                dur = CalcDuration(sdur_->sq_exec_ret.t_start, sdur_->sq_exec_ret.t_end);
            }
            if (ic_.sq%CD_UPDATE_SQ == 0) {
                cd_.sq2 = dur;
            }
            else {
                dur = cd_.sq2;
            }
            vbits = CalcVbits((dur % MAX_DUR), GetVulnerableBitCountAppx(STRUCTURE_SQ,
                simlog_->logdb_[seq_no].opcode, m_cache_hit ? important : true, 2, false));
            svfs_[STRUCTURE_SQ][idx_] += vbits % MAX_VBITS;
            UpdateVbitsStats(dur, vbits, STRUCTURE_SQ);

            ic_.sq++;
        }

    }
    else {
        // this has been checked in SimLog but it is kept here to avoid
        // program crashes, just in case.
    }
}

void SvfAnalyzer::LogFlush(LogFlushArgs args) {
    if (enabled_ == false) return; // check if enabled

    // clear livevector fifo
    livevectorfifo_.clear();

    // invalidate last_bb_pc_dispatch_
    last_bb_pc_dispatch_ = (uint64_t)-1;

    // flush duration sampling
    if (sdur_) {
        sdur_->ib.Flush();
        sdur_->iq_fast.Flush();
        sdur_->iq_slow.Flush();
        sdur_->al_wb_ret_fast.Flush();
        sdur_->al_wb_ret_slow.Flush();
    }

    // flush metadata structure
    if (ms_) {
        while(!ms_->rcq.empty()) ms_->rcq.pop();
        ms_->impq.clear();
        ms_->curr_bb_ubits_cnt = 0;
        ms_->curr_bb_inst_cnt = 0;
        ms_->last_fetched_pc = (uint64_t)-1;
        ms_->last_fetched_bb = (uint64_t)-1;
        ms_->impq_curr_bb_entry_pushed = 0;
        ms_->fetch_is_control = false;
        ms_->fetch_is_control_d = true; // to simulate known BB boundary
        ms_->fetch_is_control_d2 = false;
        ms_->rc_pushed = false; // reset
    }
}

void SvfAnalyzer::WarmUpEnds(WarmUpEndsArgs args) {
    ClearStats();
}

bool SvfAnalyzer::Analyze(uint64_t max_clock) {
    if (enabled_ == false) return true; // check if enabled

    std::string mode_name;
    std::string mode_suffix;

    switch (mode_) {
        case SVF_MODE_PRF_NOCOMP:
            mode_name = " PRF without compensation";
            mode_suffix = "_nm";
            break;
        case SVF_MODE_PRF_WRCOMP:
            mode_name = " PRF write compensation";
            mode_suffix = "_nm";
            break;
        case SVF_MODE_PRF_RDCOMP:
            mode_name = " PRF read compensation";
            mode_suffix = "_nm";
            break;
        case SVF_MODE_NO_METADATA:
            mode_name = " no metadata";
            mode_suffix = "_nm";
            break;
        case SVF_MODE_NO_LIVE_VECTOR:
            mode_name = " no live vector";
            mode_suffix = "_nlv";
            break;
        case SVF_MODE_PERFECT_METADATA:
            mode_name = " perfect metadata";
            mode_suffix = "_pm";
            break;
        case SVF_MODE_METADATA_IN_UBITS:
            mode_name = " only use metadata fittting in unused bits";
            mode_suffix = "_ubits";
            break;
        case SVF_MODE_PRF_USE_METADATA:
            mode_name = " rf using prf";
            mode_suffix = "_prf_nops";
            break;
        case SVF_MODE_PRF1_USE_METADATA:
            mode_name = " rf using prf1";
            mode_suffix = "_prf1_nops";
            break;
        case SVF_MODE_PRF_NOPS1_METADATA:
            mode_name = " rf using prf1 that fits in nops";
            mode_suffix = "_prf_nops1";
            break;
        case SVF_MODE_PRF_NOPS2_METADATA:
            mode_name = " rf using prf2 that fits in nops";
            mode_suffix = "_prf_nops2";
            break;
        case SVF_MODE_PRF_NOPS_METADATA:
            mode_name = " rf using prf that fits in nops";
            mode_suffix = "_ubits";
            break;
        case SVF_MODE_AVFCL:
            mode_name = " avf-cl";
            mode_suffix = "_avfcl";
            break;
        case SVF_MODE_AVFCL_NO_METADATA:
            mode_name = " avf-cl_nm";
            mode_suffix = "_avfcl_nm";
            break;
        case SVF_MODE_AVFCL_NOCOMP:
            mode_name = " avf-cl_nc";
            mode_suffix = "_avfcl_nc";
            break;
        case SVF_MODE_AVFCL_WRCOMP:
            mode_name = " avf-cl_wc";
            mode_suffix = "_avfcl_wc";
            break;
        case SVF_MODE_AVFCL_RDCOMP:
            mode_name = " avf-cl_rc";
            mode_suffix = "_avfcl_rc";
            break;
        default:
            mode_name = "";
            mode_suffix = "";
    }

    std::cout << "[svf_analyzer" << mode_suffix << "]" <<
        " INFO: Starting post simulation vulnerability factor analysis..." <<
        std::flush;

    struct TotalVulnerableBits<uint64_t> vbits_total;
    vbits_total.clear();

    for (unsigned int i=0; i<svfs_[STRUCTURE_IQ].size(); i++) {
        struct TotalVulnerableBits<uint64_t> vbits_sample;
        vbits_sample.iq = svfs_[STRUCTURE_IQ][i];
        vbits_sample.rob = svfs_[STRUCTURE_ROB][i];
        vbits_sample.lq = svfs_[STRUCTURE_LQ][i];
        vbits_sample.sq = svfs_[STRUCTURE_SQ][i];
        vbits_sample.rf = svfs_[STRUCTURE_RF][i];
        vbits_sample.ibuff = svfs_[STRUCTURE_IBUFF][i];
        vbits_sample.fu_simple = svfs_fu_[STRUCTURE_FU_SIMPLE][i];
        vbits_sample.fu_complex = svfs_fu_[STRUCTURE_FU_COMPLEX][i];
        vbits_sample.fu_control = svfs_fu_[STRUCTURE_FU_CONTROL][i];
        vbits_sample.fu_memory = svfs_fu_[STRUCTURE_FU_MEMORY][i];

        vbits_total.add(vbits_sample);
    }

    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << std::endl <<
            "# Total vulnerable bits (IQ/ROB/LQ/SQ/RF/IBuffSys/FUs) of SVF" <<
            mode_name << std::endl;
        DumpSummaryTotalVBit("svf_total_vbits" + mode_suffix, vbits_total);

        if (mode_ == SVF_MODE_NORMAL) {
            // dump this in normal mode only
            *p_outfile_summary_ << std::endl;
            *p_outfile_summary_ << "max_callee_saved_stack_usage: " <<
                max_callee_saved_stack_usage_ << std::endl;
            *p_outfile_summary_ << "dropped_callee_saved_stack_entries: " <<
                dropped_callee_saved_stack_entries_ << std::endl;
            *p_outfile_summary_ << "missed_callee_saved_stack_entries: " <<
                missed_callee_saved_stack_entries_ << std::endl;
            *p_outfile_summary_ << "call_cnt: " << call_cnt_ << std::endl;
            *p_outfile_summary_ << "return_cnt: " << return_cnt_ << std::endl;
            *p_outfile_summary_ << "max_livevectorfifo_depth: " <<
                max_livevectorfifo_depth_ << std::endl;
            *p_outfile_summary_ << "livevectorfifo_push_cnt: " <<
                livevectorfifo_push_cnt_ << std::endl;
            *p_outfile_summary_ << "livevectorfifo_pop_cnt: " <<
                livevectorfifo_pop_cnt_ << std::endl;
            *p_outfile_summary_ << "missed_livevectorfifo_entries: " <<
                missed_livevectorfifo_entries_ << std::endl;
            *p_outfile_summary_ << std::endl;
        }
        if (mode_ == SVF_MODE_METADATA_IN_UBITS) {
            *p_outfile_summary_ << "dispatch_inst_has_metadata_: " <<
                dispatch_inst_has_metadata_ << std::endl;
            *p_outfile_summary_ << "dispatch_inst_no_metadata_: " <<
                dispatch_inst_no_metadata_ << std::endl;
            *p_outfile_summary_ << "dispatch_inst_no_metadata_perc_: " <<
                (dispatch_inst_no_metadata_*100.00/(float)(dispatch_inst_no_metadata_+dispatch_inst_has_metadata_)) << std::endl;
            *p_outfile_summary_ << "retire_inst_has_metadata_: " <<
                retire_inst_has_metadata_ << std::endl;
            *p_outfile_summary_ << "retire_inst_no_metadata_: " <<
                retire_inst_no_metadata_ << std::endl;
            *p_outfile_summary_ << "retire_inst_no_metadata_perc_: " <<
                (retire_inst_no_metadata_*100.00/(float)(retire_inst_no_metadata_+retire_inst_has_metadata_)) << std::endl;
            *p_outfile_summary_ << "dispatched_nops_default_: " <<
                dispatched_nops_default_ << std::endl;
            *p_outfile_summary_ << "dispatched_nops_scheme_1_: " <<
                dispatched_nops_scheme_1_ << std::endl;
            *p_outfile_summary_ << "dispatched_nops_scheme_2_: " <<
                dispatched_nops_scheme_2_ << std::endl;
            *p_outfile_summary_ << "dispatched_nops_scheme_3_: " <<
                dispatched_nops_scheme_3_ << std::endl;
            *p_outfile_summary_ << "dispatched_nops_scheme_4_: " <<
                dispatched_nops_scheme_4_ << std::endl;
            *p_outfile_summary_ << "dispatched_nops_scheme_5_: " <<
                dispatched_nops_scheme_5_ << std::endl;
            *p_outfile_summary_ << "dispatched_resv_default_: " <<
                dispatched_resv_default_ << std::endl;
            *p_outfile_summary_ << "retired_nops_default_: " <<
                retired_nops_default_ << std::endl;
            *p_outfile_summary_ << "retired_nops_scheme_1_: " <<
                retired_nops_scheme_1_ << std::endl;
            *p_outfile_summary_ << "retired_nops_scheme_2_: " <<
                retired_nops_scheme_2_ << std::endl;
            *p_outfile_summary_ << "retired_nops_scheme_3_: " <<
                retired_nops_scheme_3_ << std::endl;
            *p_outfile_summary_ << "retired_nops_scheme_4_: " <<
                retired_nops_scheme_4_ << std::endl;
            *p_outfile_summary_ << "retired_nops_scheme_5_: " <<
                retired_nops_scheme_5_ << std::endl;
            *p_outfile_summary_ << "retired_resv_default_: " <<
                retired_resv_default_ << std::endl;

            *p_outfile_summary_ << "vbits_rf_live_in_first: " <<
                vbits_rf_live_in_first << std::endl;
            *p_outfile_summary_ << "vbits_rf_live_in_first_verif: " <<
                vbits_rf_live_in_first_verif << std::endl;
            *p_outfile_summary_ << "vbits_rf_live_out_first: " <<
                vbits_rf_live_out_first << std::endl;
            *p_outfile_summary_ << "vbits_rf_live_in_avg: " <<
                vbits_rf_live_in_avg << std::endl;
            *p_outfile_summary_ << "vbits_rf_live_out_avg: " <<
                vbits_rf_live_out_avg << std::endl;
            *p_outfile_summary_ << "vbits_rf_live_in_first_inst: " <<
                vbits_rf_live_in_first_inst << std::endl;
        }
    }

    // dump counter and vbits stats
    if (mode_ == SVF_MODE_NORMAL) {
        *p_outfile_summary_ << "stats_cv:" << std::endl;

        for (int n=0; n<ANALYZED_STRUCTURES_COUNT; n++) {
            std::string name;

            switch(n) {
                case STRUCTURE_IQ: name="IQ"; break;
                case STRUCTURE_ROB: name="ROB"; break;
                case STRUCTURE_LQ: name="LQ"; break;
                case STRUCTURE_SQ: name="SQ"; break;
                case STRUCTURE_RF: name="RF"; break;
                case STRUCTURE_IBUFF: name="IBUFF"; break;
                default: name="Others"; break;
            }
            *p_outfile_summary_ << std::dec;
            *p_outfile_summary_ << "    " << name << ":" << std::endl;
            *p_outfile_summary_ << "        min_cnt:" << stats_cv_[n].min_cnt << std::endl;
            *p_outfile_summary_ << "        max_cnt:" << stats_cv_[n].max_cnt << std::endl;
            *p_outfile_summary_ << "        avg_cnt:" << (((float)stats_cv_[n].total_cnt)/stats_cv_[n].cnt) << std::endl;
            *p_outfile_summary_ << "        min_vbits:" << stats_cv_[n].min_vbits << std::endl;
            *p_outfile_summary_ << "        max_vbits:" << stats_cv_[n].max_vbits << std::endl;
            *p_outfile_summary_ << "        avg_vbits:" << (((float)stats_cv_[n].total_vbits)/stats_cv_[n].cnt) << std::endl;
        }

        *p_outfile_summary_ << std::endl;
    }

    // dump nops histogram
    if (mode_ == SVF_MODE_AVFCL) {
        std::ofstream fhist;
        fhist.open(NOP_INJECT_HIST_FILE, std::ios::out | std::ios::trunc);

        if (fhist.is_open()) {
            std::map<uint64_t, uint64_t>::iterator it;

            for (it=nops_hist_.begin(); it!=nops_hist_.end(); it++) {
                fhist << "\"" << std::hex << (it->first) << "\": ";
                fhist << std::dec << (it->second) << std::endl;
            }
        }
    }

    // dump undetected basic block histogram
    if (bbstat_) {
        std::ofstream fhist;
        fhist.open(UNDETECTED_BB_HIST_FILE, std::ios::out | std::ios::trunc);

        if (fhist.is_open()) {
            std::map<uint64_t, uint64_t>::iterator it;

            for (it=bbstat_->undetected_bb.begin(); it!=bbstat_->undetected_bb.end(); it++) {
                fhist << "\"" << std::hex << (it->first) << "\": ";
                fhist << std::dec << (it->second) << std::endl;
            }
        }
    }

    if (bbstat_) {
        // dump basic block stats
        *p_outfile_summary_ << "boundary_after_control: " <<
            bbstat_->boundary_after_control << std::endl;
        *p_outfile_summary_ << "boundary_on_nop: " <<
            bbstat_->boundary_on_nop << std::endl;
        *p_outfile_summary_ << "boundary_undetected: " <<
            bbstat_->boundary_undetected << std::endl;
        float ratio = (float)bbstat_->boundary_after_control + (float)bbstat_->boundary_on_nop;
        ratio = ratio/(ratio+bbstat_->boundary_undetected);
        *p_outfile_summary_ << "boundary_detected_ratio: " << ratio << std::endl;

        *p_outfile_summary_ << "metadata_available_cnt: " <<
            bbstat_->metadata_available_cnt << std::endl;
        *p_outfile_summary_ << "metadata_not_available_cnt: " <<
            bbstat_->metadata_not_available_cnt << std::endl;
        ratio = (float)bbstat_->metadata_available_cnt;
        ratio = ratio/(ratio+bbstat_->metadata_not_available_cnt);
        *p_outfile_summary_ << "metadata_available_ratio: " << ratio << std::endl;

        *p_outfile_summary_ << std::endl;
    }

    if (ms_) {
        // dump metadata structure stats
        *p_outfile_summary_ << "rcq_max_size: " << ms_->rcq_max_size << std::endl;
        *p_outfile_summary_ << "impq_max_size: " << ms_->impq_max_size << std::endl;
        *p_outfile_summary_ << "ubits_in_bb_max: " << ms_->ubits_in_bb_max << std::endl;
        *p_outfile_summary_ << "retire_with_entry_in_impq: " << ms_->retire_with_entry_in_impq << std::endl;
        *p_outfile_summary_ << "retire_no_entry_in_impq: " << ms_->retire_no_entry_in_impq << std::endl;
        *p_outfile_summary_ << "important_matches: " << ms_->important_matches << std::endl;
        *p_outfile_summary_ << "important_mismatches: " << ms_->important_mismatches << std::endl;
        *p_outfile_summary_ << "rc_matches: " << ms_->rc_matches << std::endl;
        *p_outfile_summary_ << "rc_mismatches: " << ms_->rc_mismatches << std::endl;
    }

    if (dps_) {
        std::ofstream fhist;
        fhist.open(PROTECTION_HIST_FILE, std::ios::out | std::ios::trunc);

        if (fhist.is_open()) {
            std::vector<struct ProtectionHistoryEntry>::iterator it;

            for (it=SVM->prot_hist_.begin(); it!=SVM->prot_hist_.end(); it++) {
                fhist << (it->clk) << ": " << (it->status) << std::endl;
            }

            fhist.close();
        }
    }

    std::cout << " done" << std::endl;

    return true;
}

bool SvfAnalyzer::MonitoringActive() {
    // return the latest SVF state
    return active_history_.back();
}


void SvfAnalyzer::CheckNopInjection(struct NopInjectionInfo *pinfo) {
    if (mode_ == SVF_MODE_AVFCL) {
        struct Metadata2 md2 = mr_->GetMetadata2(pinfo->pc);
        int64_t n_nops = mr_->GetNopsAdded(&md2);
        if (n_nops > 0) {
            pinfo->start_index = mr_->GetNopsIdx(&md2) - 1;
            pinfo->n_nops = n_nops;

            if (pinfo->fetch_count > 0) {
                // the nop is not fetched to the first lane
                nops_hist_[pinfo->pc]++;
            }
        }
        else {
            pinfo->n_nops = NO_NOP_INJECTION;
        }
    }
    else {
        // by default, always return no NOP injection needed
        pinfo->n_nops = NO_NOP_INJECTION;
    }
}


/* -------------------- private function definitions ------------------------ */

void SvfAnalyzer::Init(SvfMode mode) {
    // clear enabled_ flag
    enabled_ = false;

    // initialize metadata reader pointer
    mr_ = NULL;

    // clear SVF stats
    ClearStats();

    // initialize reg_live_vector
    reg_live_vector_ = 0;

    // initialize max_callee_saved_stack_usage and dropped entry count
    max_callee_saved_stack_usage_ = 0;
    dropped_callee_saved_stack_entries_ = 0;
    call_cnt_ = 0;
    return_cnt_ = 0;

    // initialize last_bb_pc_
    last_bb_pc_ = INITIAL_BB_PC;
    last_bb_pc_dispatch_ = INITIAL_BB_PC;

    // initialize livevector fifo
    livevectorfifo_.clear();
    max_livevectorfifo_depth_ = 0;

    // set mode
    mode_ = mode;

    // initialize stats
    stats_cv_.resize(ANALYZED_STRUCTURES_COUNT);
    for (int n=0; n<ANALYZED_STRUCTURES_COUNT; n++) {
        stats_cv_[n].cnt = 0;
    }

    // initialize SVF on-off setting
    uint64_t raw_active_size = SVM->cfg().svf_active_size;
    assert(raw_active_size%PHASE_PERIOD == 0); // make sure it is multiple of PHASE_PERIOD
    onoff_active_size_ = (raw_active_size/PHASE_PERIOD); // convert to group
    onoff_period_ = onoff_active_size_ * SVM->cfg().svf_recp_active_rate;

    sdur_ = NULL; // by default, no sampled duration is used
    if (mode == SVF_MODE_AVFCL ||
        mode == SVF_MODE_AVFCL_NOCOMP ||
        mode == SVF_MODE_AVFCL_RDCOMP ||
        mode == SVF_MODE_AVFCL_WRCOMP ||
        mode == SVF_MODE_AVFCL_NO_METADATA) {
        sdur_ = new DurationSampling();

        sdur_->ib.counter = 0;
        sdur_->ib.period = SVM->cfg().dsp_ib;
        sdur_->iq_fast.counter = 0;
        sdur_->iq_fast.period = SVM->cfg().dsp_iq;
        sdur_->iq_slow.counter = 0;
        sdur_->iq_slow.period = SVM->cfg().dsp_iq;
        sdur_->al_dp_ret.counter = 0;
        sdur_->al_dp_ret.period = SVM->cfg().dsp_al_dp_ret;
        sdur_->al_wb_ret_fast.counter = 0;
        sdur_->al_wb_ret_fast.period = SVM->cfg().dsp_al_wb_ret;
        sdur_->al_wb_ret_slow.counter = 0;
        sdur_->al_wb_ret_slow.period = SVM->cfg().dsp_al_wb_ret;
        sdur_->lq_dp_exec.counter = 0;
        sdur_->lq_dp_exec.period = SVM->cfg().dsp_lq_dp_exec;
        sdur_->lq_exec_ret.counter = 0;
        sdur_->lq_exec_ret.period = SVM->cfg().dsp_lq_exec_ret;
        sdur_->sq_dp_exec.counter = 0;
        sdur_->sq_dp_exec.period = SVM->cfg().dsp_sq_dp_exec;
        sdur_->sq_exec_ret.counter = 0;
        sdur_->sq_exec_ret.period = SVM->cfg().dsp_sq_exec_ret;
        sdur_->rf_rr_ret.counter = 0;
        sdur_->rf_rr_ret.period = SVM->cfg().dsp_rf_rr_ret;
        sdur_->rf_wb_ret.counter = 0;
        sdur_->rf_wb_ret.period = SVM->cfg().dsp_rf_wb_ret;
    }

    bbstat_ = NULL; // by default, no basic block stat collection
    if (mode == SVF_MODE_AVFCL) {
        bbstat_ = new BasicBlockStats();
        bbstat_->is_control_d = false;
        bbstat_->is_control_d2 = false;
        bbstat_->boundary_was_detected = true;
    }

    ms_ = NULL; // by default, no metadata queue instance
    if (mode == SVF_MODE_AVFCL || mode == SVF_MODE_AVFCL_NO_METADATA) {
        ms_ = new MetadataStructure();
        while(!ms_->rcq.empty()) ms_->rcq.pop();
        ms_->impq.clear();
        ms_->curr_bb_ubits_cnt = 0;
        ms_->curr_bb_inst_cnt = 0;
        ms_->last_fetched_pc = (uint64_t)-1;
        ms_->last_fetched_bb = (uint64_t)-1;
        ms_->fetch_is_control = false;
        ms_->fetch_is_control_d = false;
        ms_->fetch_is_control_d2 = false;

        ms_->last_pushed_rc = SVM->cfg().arch_size_amt; // conservative value at start of simulation
        ms_->last_rc = SVM->cfg().arch_size_amt; // conservative value at start of simulation

        // update setting variables
        ms_->ubits_for_rc = 5; // first 5-bits for register-live count
        ms_->ubits_on_nop = 13; // 13 ubits on NOP
        ms_->rcq_size = 64;
        ms_->impq_size = 256;
    }

    if (mode == SVF_MODE_AVFCL && SVM->cfg().enable_svf_adaptive) {
        dps_ = new DynamicProtectionStructure();
        uint64_t mem_st_tsize = 0;
        for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
            mem_st_tsize += GetTotalBitsCount((structure_t)i);
        }
        dps_->tvbits_threshold = SVM->cfg().svf_adaptive_thd * mem_st_tsize * PHASE_PERIOD_1M;
        std::cout << "[avfcl_analyzer] Running dynamic protection with threshold = " << dps_->tvbits_threshold << " out of " << (mem_st_tsize * PHASE_PERIOD_1M) << " vbits/1M_cycles" << std::endl;
    }
}

void SvfAnalyzer::ClearStats() {
    // clear svfs_ counters
    idx_ = 0;
    for (int i=0; i<ANALYZED_STRUCTURES_COUNT; i++) {
        svfs_[i].clear();
        svfs_[i].push_back(0);
    }
    for (int i=0; i<STRUCTURES_FU_COUNT; i++) {
        svfs_fu_[i].clear();
        svfs_fu_[i].push_back(0);
    }

    // clear active_history_
    active_history_.clear();
    active_history_.push_back(true); // the first period is always active

    // clear livevectorfifo
    missed_livevectorfifo_entries_ = 0;

    // clear callee saved stack stats
    call_cnt_ = 0;
    return_cnt_ = 0;
    missed_callee_saved_stack_entries_ = 0;

    // clear metadata fitting in unused bits stats
    dispatch_inst_has_metadata_ = 0;
    dispatch_inst_no_metadata_ = 0;
    retire_inst_has_metadata_ = 0;
    retire_inst_no_metadata_ = 0;
    dispatched_nops_default_ = 0;
    dispatched_nops_scheme_1_ = 0;
    dispatched_nops_scheme_2_ = 0;
    dispatched_nops_scheme_3_ = 0;
    dispatched_nops_scheme_4_ = 0;
    dispatched_nops_scheme_5_ = 0;
    dispatched_resv_default_ = 0;
    retired_nops_default_ = 0;
    retired_nops_scheme_1_ = 0;
    retired_nops_scheme_2_ = 0;
    retired_nops_scheme_3_ = 0;
    retired_nops_scheme_4_ = 0;
    retired_nops_scheme_5_ = 0;
    retired_resv_default_ = 0;

    vbits_rf_live_in_first = 0;
    vbits_rf_live_in_first_inst = 0;
    vbits_rf_live_in_first_verif = 0;
    vbits_rf_live_out_first = 0;
    vbits_rf_live_in_avg = 0;
    vbits_rf_live_out_avg = 0;

    // clear bbstat_
    if (bbstat_) {
        bbstat_->boundary_after_control = 0;
        bbstat_->boundary_on_nop = 0;
        bbstat_->boundary_undetected = 0;
        bbstat_->metadata_available_cnt = 0;
        bbstat_->metadata_not_available_cnt = 0;
        bbstat_->ubits_available = 0;
        bbstat_->undetected_bb.clear();
    }

    if (ms_) {
        ms_->rcq_max_size = 0;
        ms_->impq_max_size = 0;
        ms_->retire_with_entry_in_impq = 0;
        ms_->retire_no_entry_in_impq = 0;
        ms_->important_matches = 0;
        ms_->important_mismatches = 0;
        ms_->rc_matches = 0;
        ms_->rc_mismatches = 0;
    }

    if (dps_) {
        dps_->mem_st_tvbits = 0;
        dps_->was_enabled = false;
        SVM->prot_hist_.clear();
    }

    if (mode_ == SVF_MODE_AVFCL) {
        // clear nops_hists_
        nops_hist_.clear();
    }
}

uint64_t SvfAnalyzer::SingleKill(uint64_t seq_no, int16_t log, bool kill_flag) {
    if (kill_flag) { // search if kill_flag is true
        // syscall doesn't have source operands
        if (simlog_->logdb_[seq_no].is_syscall) {
            return (0x1ull << log);
        }

        for (uint32_t i=0; i<simlog_->logdb_[seq_no].sources.size(); i++) {
            if (simlog_->logdb_[seq_no].sources[i].log == log) {
                return SingleKill(seq_no, i, log, kill_flag);
            }
        }

        // source not found
        std::cout << std::hex << TERM_YELLOW <<
            "[svf_analyzer] WARNING: reg source could not be found at PC " <<
            simlog_->logdb_[seq_no].pc << std::endl;
        std::cout << "  looking for " << log << std::endl;
        std::cout << "  available sources count: " <<
            simlog_->logdb_[seq_no].sources.size() << " list:";
        for (uint32_t i=0; i<simlog_->logdb_[seq_no].sources.size(); i++) {
            std::cout << " " << simlog_->logdb_[seq_no].sources[i].log;
        }
        std::cout << TERM_RESET << std::endl;
    }
    return 0ull;
}

uint64_t SvfAnalyzer::SingleKill(uint64_t seq_no, int source_offset,
    int16_t log, bool kill_flag) {

    if (kill_flag) {
        if (simlog_->logdb_[seq_no].sources.size() > (uint32_t)source_offset) {
            if (simlog_->logdb_[seq_no].sources[source_offset].log == log) {
                return (0x1ull << log);
            }
            else {
                std::cout << std::hex << TERM_YELLOW <<
                    "[svf_analyzer] WARNING: Instruction at PC " <<
                    simlog_->logdb_[seq_no].pc << " expects source " <<
                    (source_offset+1) << ": " << log <<
                    TERM_RESET << std::endl;
            }
        }
        else {
            std::cout << std::hex << TERM_YELLOW <<
                "[svf_analyzer] WARNING: Instruction at PC " <<
                simlog_->logdb_[seq_no].pc << " expects at least " <<
                (source_offset+1) << " sources." <<
                TERM_RESET << std::endl;
        }
    }
    return 0ull;
}

uint64_t SvfAnalyzer::SingleWrite(uint64_t seq_no, int16_t log,
    bool live_flag) {

    if (live_flag) {
        // syscall doesn't have target register destination
        if (simlog_->logdb_[seq_no].is_syscall) {
            return (0x1ull << log);
        }
        // make sure the dest log matches
        else if (simlog_->logdb_[seq_no].destinations[0].log == log) {
            return (0x1ull << log);
        }
        else {
            std::cout << std::hex << TERM_YELLOW <<
                "[svf_analyzer] WARNING: PC " << simlog_->logdb_[seq_no].pc <<
                " destination registers mismatch." << TERM_RESET << std::endl;
            if (simlog_->logdb_[seq_no].destinations[0].log != log)
                std::cout << std::hex << TERM_YELLOW <<
                    "    Expecting log_id " << log <<
                    " from metadata but found " <<
                    simlog_->logdb_[seq_no].destinations[0].log << " instead" <<
                    TERM_RESET << std::endl;
        }
    }
    return (0ull);
}

uint64_t SvfAnalyzer::Cycle2Group(uint64_t cycle) {
    if (SVM->cfg().enable_warm_up) {
        if (SVM->IsWarmUpPeriod() || cycle < SVM->cfg().end_of_warm_up) {
            // always return 0 during warm up period
            return 0;
        }
        else {
            cycle -= (SVM->cfg().end_of_warm_up-1); // offset the warmup time
        }
    }
    return (cycle/PHASE_PERIOD);
}

bool SvfAnalyzer::SvfActive(uint64_t cycle) {
    // return SVF is active at given cycle
    return active_history_[(Cycle2Group(cycle))];
}

void SvfAnalyzer::UpdateVbitsStats(uint64_t dur, uint64_t vbits, uint64_t st) {
    if (mode_ == SVF_MODE_NORMAL) {
        if(stats_cv_[st].cnt) {
            if (stats_cv_[st].min_cnt > dur)
                stats_cv_[st].min_cnt = dur;
            if (stats_cv_[st].max_cnt < dur)
                stats_cv_[st].max_cnt = dur;
            stats_cv_[st].total_cnt += dur;
            if (stats_cv_[st].min_vbits > vbits)
                stats_cv_[st].min_vbits = vbits;
            if (stats_cv_[st].max_vbits < vbits)
                stats_cv_[st].max_vbits = vbits;
            stats_cv_[st].total_vbits += vbits;
        }
        else {
            stats_cv_[st].min_cnt = dur;
            stats_cv_[st].max_cnt = dur;
            stats_cv_[st].total_cnt = dur;
            stats_cv_[st].min_vbits = vbits;
            stats_cv_[st].max_vbits = vbits;
            stats_cv_[st].total_vbits = vbits;
        }
        stats_cv_[st].cnt++;
    }
}

uint64_t SvfAnalyzer::CalcDuration(uint64_t start, uint64_t end) {
    uint64_t mask = ~((((uint64_t)1)<<IGNORED_DUR_LSB)-1);
    start = start & mask;
    end = end & mask;
    return (end-start);
}

uint64_t SvfAnalyzer::CalcVbits(uint64_t dur, uint64_t vbit_per_cycle) {
    uint64_t res = dur * vbit_per_cycle;
    uint64_t mask = ~((((uint64_t)1)<<IGNORED_VBITS_LSB)-1);
    res = res & mask;
    return (res);
}

bool SvfAnalyzer::MetadataAvailable(uint64_t seq_no) {
    bool md_available = true; // default to true

    if (mode_ == SVF_MODE_NO_METADATA || mode_ == SVF_MODE_AVFCL_NO_METADATA) {
        // force md_available to false to simulate no metadata mode
        md_available = false;
    }
    else if (mode_ == SVF_MODE_PERFECT_METADATA) {
        // force md_available to true to simulate perfect metadata mode
        md_available = true;
    }
    else if (mode_ == SVF_MODE_METADATA_IN_UBITS) {
        struct Metadata2 md2 = mr_->GetMetadata2(simlog_->logdb_[seq_no].pc);
        bool metadata_in_ubits = md2.unused_bit_info.metadata_in_ubits_default;
        if(!metadata_in_ubits) {
            // force md_available to false to simulate metadata not fitting in unused bits
            md_available = false;
        }
        else {
            md_available = true;
        }
    }
    else if (mode_ == SVF_MODE_AVFCL ||
        mode_ == SVF_MODE_AVFCL_NOCOMP ||
        mode_ == SVF_MODE_AVFCL_RDCOMP ||
        mode_ == SVF_MODE_AVFCL_WRCOMP) {
        // assume metadata is always there
        md_available = true;
    }
    else if ( (mode_ == SVF_MODE_PRF_USE_METADATA) || (mode_ == SVF_MODE_PRF_NOPS_METADATA) ) {
        md_available = true;
    }
    else {
        // else, check md_available from simlog's m_cache_hit
        md_available = simlog_->logdb_[seq_no].m_cache_hit;
    }

    return md_available;
}

/* end of file */

