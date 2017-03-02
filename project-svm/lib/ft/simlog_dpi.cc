/**********************************************************
 *
 * Simulation log DPIs
 *
 * First author: Bagus Wibowo
 * created: 05/14/2014
 **********************************************************/

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "svm.h"

bool is_exiting = false;
uint64_t max_commit_cnt = 0;

extern "C" {

    void simlog_init(const char * base_output_dir) {
        // set SVM configurations
        struct SvmCfg cfg = SVM->cfg();

        // - architecture configurations
        cfg.arch_size_amt = SIZE_RMT;
        cfg.arch_size_amt_log = ceil(log2(cfg.arch_size_amt));
        cfg.arch_size_data = SIZE_DATA;
        cfg.arch_size_data_log = ceil(log2(cfg.arch_size_data));
        cfg.arch_size_pc = SIZE_PC;
        cfg.arch_size_pc_log = ceil(log2(cfg.arch_size_pc));
        cfg.arch_size_instruction = SIZE_INSTRUCTION;
        cfg.arch_size_instruction_log = ceil(log2(cfg.arch_size_instruction));
        cfg.arch_size_immediate = SIZE_IMMEDIATE_M;
        cfg.arch_size_immediate_log = ceil(log2(cfg.arch_size_immediate));
        cfg.arch_size_addr = SIZE_ADDR;
        cfg.arch_size_addr_log = ceil(log2(cfg.arch_size_addr));

        // - core configurations
        cfg.core_size_al = SIZE_ACTIVELIST;
        cfg.core_size_al_log = ceil(log2(cfg.core_size_al));
        cfg.core_size_fq = INST_QUEUE;
        cfg.core_size_fq_log = ceil(log2(cfg.core_size_fq));
        cfg.core_size_iq_int = SIZE_ISSUEQ;
        cfg.core_size_iq_int_log = ceil(log2(cfg.core_size_iq_int));
        cfg.core_size_prf_int = SIZE_PHYSICAL_TABLE;
        cfg.core_size_prf_int_log = ceil(log2(cfg.core_size_prf_int));
        cfg.core_size_lq = SIZE_LSQ;
        cfg.core_size_lq_log = ceil(log2(cfg.core_size_lq));
        cfg.core_size_sq = SIZE_LSQ;
        cfg.core_size_sq_log = ceil(log2(cfg.core_size_sq));
        cfg.core_size_ctiq = SIZE_CTI_QUEUE;
        cfg.core_size_ctiq_log = ceil(log2(cfg.core_size_ctiq));
        cfg.core_size_freelist = SIZE_FREE_LIST;
        cfg.core_size_freelist_log = ceil(log2(cfg.core_size_freelist));
        cfg.core_size_exe_flags = EXECUTION_FLAGS;
        cfg.core_size_exe_flags_log = ceil(log2(cfg.core_size_exe_flags));
        cfg.core_br_types = BRANCH_TYPE;
        cfg.core_inst_types = INSTRUCTION_TYPES;
        cfg.core_inst_types_log = ceil(log2(cfg.core_inst_types));
        cfg.core_ldst_types = 4;
        cfg.core_ldst_types_log = ceil(log2(cfg.core_ldst_types));
        cfg.core_width_opcode = SIZE_OPCODE; // micro-ops opcode width
        cfg.core_width_opcode_log = ceil(log2(cfg.core_width_opcode));

        cfg.core_width_frontend = FETCH_WIDTH;
        cfg.core_width_frontend_log = ceil(log2(cfg.core_width_frontend));
        cfg.core_width_dispatch = DISPATCH_WIDTH;
        cfg.core_width_dispatch_log = ceil(log2(cfg.core_width_dispatch));
        cfg.core_width_issue = ISSUE_WIDTH;
        cfg.core_width_issue_log = ceil(log2(cfg.core_width_issue));

        cfg.nop_issued = true;

        SVM->set_cfg(cfg);

        // Initialize SVM module
        SVM->Init(base_output_dir);
    }

    void simlog_close(void) {
        is_exiting = true;
    }

    void simlog_loadmetadata(const char * path) {
        SVM->SetMetadata(path);
    }

    void simlog_clocktick(uint64_t clk, uint64_t commit_cnt) {
        max_commit_cnt = commit_cnt;
        if (!is_exiting) {
            SVM->LogClockTick(clk);
        }
    }

    void simlog_fetch(uint64_t clk, uint64_t seq_no, uint64_t pc,
        int32_t opcode, int is_syscall, int is_call, int is_return,
        int m_cache_hit) {

        if (!is_exiting) {
            SVM->LogFetch(seq_no,pc,opcode,(is_syscall!=0),(is_call!=0),
                          (is_return!=0),(m_cache_hit!=0));
        }
    }

    void simlog_rename(uint64_t clk, uint64_t seq_no,
        int32_t dest_valid, int64_t dest_log, int64_t dest_phy,
        int32_t src1_valid, int64_t src1_log, int64_t src1_phy,
        int32_t src2_valid, int64_t src2_log, int64_t src2_phy) {

        if (!is_exiting) {
            SVM->LogRename(seq_no,(dest_valid!=0),dest_log,dest_phy,
                           (src1_valid!=0),src1_log,src1_phy,
                           (src2_valid!=0),src2_log,src2_phy);
        }
    }

    void simlog_dispatch(uint64_t clk, uint64_t seq_no,
        int32_t is_load, uint64_t load_id,
        int32_t is_store, uint64_t store_id) {
        if (!is_exiting) {
            SVM->LogDispatch(seq_no,(is_load!=0),load_id,
                             (is_store!=0),store_id);
        }
    }

    void simlog_issue(uint64_t clk, uint64_t seq_no, int lane) {
        if (!is_exiting) {
            SVM->LogIssue(seq_no,lane,0);
        }
    }

    void simlog_regread(uint64_t clk, uint64_t seq_no,
        uint64_t src1, uint64_t src2) {

        if (!is_exiting) {
            SVM->LogRegRead(seq_no,src1,src2);
        }
    }

    void simlog_memexecute(uint64_t clk, uint64_t seq_no) {
        if (!is_exiting) {
            SVM->LogMemExecute(seq_no);
        }
    }

    void simlog_loadexecute(uint64_t clk, uint64_t load_id) {
        if (!is_exiting) {
            SVM->LogLoadExecute(load_id);
        }
    }

    void simlog_writeback(uint64_t clk, uint64_t seq_no) {
        if (!is_exiting) {
            SVM->LogWriteBack(seq_no);
        }
    }

    void simlog_retire(uint64_t clk, uint64_t seq_no, int is_control,
        int mispred) {

        if (!is_exiting) {
            SVM->LogRetire(seq_no,(is_control!=0),(mispred!=0));
        }
    }

    void simlog_storecommit(uint64_t clk, uint64_t store_id) {
        if (!is_exiting) {
            SVM->LogStoreCommit(store_id);
        }
    }

    void simlog_freedreg(uint64_t clk, int64_t reg_id) {
        if (!is_exiting) {
            SVM->LogFreedReg(reg_id);
        }
    }

    void simlog_flush(uint64_t clk) {
        if (!is_exiting) {
            SVM->LogFlush();
        }
    }

    void simlog_exception(uint64_t clk, uint64_t seq_no, uint64_t pc,
        uint64_t v0) {

        if (!is_exiting) {
            SVM->LogException(seq_no,pc,v0);
        }
    }

    void simlog_analyze(uint64_t clk, int exit_simulation) {
        SVM->Analyze();

        if (exit_simulation) {
            if (max_commit_cnt > 10000000)
                exit(0);
            else
                exit(1);
        }
    }

} /* extern C */

/* end of file */
