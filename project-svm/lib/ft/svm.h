/**********************************************************
 *
 * System vulnerability model class header file
 *
 * First author: Bagus Wibowo
 * created: 02/06/2015
 **********************************************************/

#ifndef _SVM_H_
#define _SVM_H_

#include <string>
#include "sim_listener.h"
#include "sim_listener_list.h"
#include "simlog.h"
#include "simlog_nonspec.h"
#include "mreader.h"
#include "ft_analyzer.h"
#include "ft_analyzer_list.h"
#include "sim_logger.h"
#include "avf_analyzer.h"
#include "pvf_analyzer.h"
#include "hvf_analyzer.h"
#include "occupancy_analyzer.h"
#include "svf_analyzer.h"
#include "spec_analyzer.h"
#include "func_stats.h"
#include "vbit_analyzer.h"

struct ProtectionHistoryEntry {
    uint64_t clk;
    bool status;
};

struct SvmCfg {
    // Architecture related configurations
    uint32_t arch_size_amt;
    uint32_t arch_size_amt_log;
    uint32_t arch_size_data;
    uint32_t arch_size_data_log;
    uint32_t arch_size_pc;
    uint32_t arch_size_pc_log;
    uint32_t arch_size_instruction;
    uint32_t arch_size_instruction_log;
    uint32_t arch_size_immediate;
    uint32_t arch_size_immediate_log;
    uint32_t arch_size_addr;
    uint32_t arch_size_addr_log;

    // Core related configurations
    uint32_t core_size_al;
    uint32_t core_size_al_log;
    uint32_t core_size_fq;
    uint32_t core_size_fq_log;
    uint32_t core_size_iq_int;
    uint32_t core_size_iq_int_log;
    uint32_t core_size_iq_fp;
    uint32_t core_size_iq_fp_log;
    uint32_t core_size_prf_int;
    uint32_t core_size_prf_int_log;
    uint32_t core_size_prf_fp;
    uint32_t core_size_prf_fp_log;
    uint32_t core_size_lq;
    uint32_t core_size_lq_log;
    uint32_t core_size_sq;
    uint32_t core_size_sq_log;
    uint32_t core_size_ctiq;
    uint32_t core_size_ctiq_log;
    uint32_t core_size_freelist;
    uint32_t core_size_freelist_log;
    uint32_t core_size_exe_flags;
    uint32_t core_size_exe_flags_log;
    uint32_t core_br_types;
    uint32_t core_inst_types;
    uint32_t core_inst_types_log;
    uint32_t core_ldst_types;
    uint32_t core_ldst_types_log;
    uint32_t core_width_opcode; // micro-ops opcode width
    uint32_t core_width_opcode_log;
    uint32_t core_width_frontend;
    uint32_t core_width_frontend_log;
    uint32_t core_width_dispatch;
    uint32_t core_width_dispatch_log;
    uint32_t core_width_issue;
    uint32_t core_width_issue_log;
    uint32_t core_width_issue_mem;
    uint32_t core_width_issue_mem_log;
    uint32_t core_width_retire;
    uint32_t core_width_retire_log;
    std::string core_execute_lanes_mem;
    std::string core_execute_lanes_cti;
    std::string core_execute_lanes_int;
    std::string core_execute_lanes_fp;
    std::string core_execute_lanes_llat_int;
    std::string core_execute_lanes_llat_fp;
    std::string core_latency_ex_load;
    std::string core_latency_ex_int;
    std::string core_latency_ex_fp;
    std::string core_latency_ex_llat_int;
    std::string core_latency_ex_llat_fp;
    bool core_r0_hardwired;          // exclude R0 from being renamed
    bool nop_issued;                 // NOP is issued into functional unit

    // Miscellaneous configurations
    bool verbose;                    // show cycle count every cycle
    bool precise_freelist_tracking;  // track freelist precisely
    bool flush_reset_freelist;       // flush will reset freelist
    bool keep_cycle_detailed_files;  // keep VF cycle by cycle files
    bool adaptive_monitoring;        // enable turning SVF on and off
    uint64_t svf_active_size;        // SVF active size in cycles
    uint64_t svf_recp_active_rate;   // SVF reciprocal active rate
    bool enable_warm_up;             // enable warm-up session at the beginning
                                     // of simulation
    uint64_t end_of_warm_up;         // the cycle time when warm up ends
    bool enable_log_all_insts;       // enable logging all instructions timing
    bool enable_regs_life_dump;      // enable register life dump
    uint64_t ecc_activate_penalty;   // number of cycles in case of activating
                                     // ECC
    uint64_t sts_period;             // period between timestamp sampling
    bool enable_svf_only;            // simulate SVF only
    std::string pvs_profile_file;    // the path to PVS profile file

    // Adaptive VF configurations
    bool enable_svf_adaptive;        // flag to indicate SVF adaptive is active
    float svf_adaptive_thd;          // the threshold of SVF when protection
                                     // should be enabled

    // SVF duration sampling period
    uint64_t dsp_ib;                 // for Instruction Buffer
    uint64_t dsp_iq;                 // for Issue Queue
    uint64_t dsp_al_dp_ret;          // for Active List dispatch-retire
    uint64_t dsp_al_wb_ret;          // for Active List writeback-retire
    uint64_t dsp_lq_dp_exec;         // for Load Queue dispatch-execute
    uint64_t dsp_lq_exec_ret;        // for Load Queue execute-retire
    uint64_t dsp_sq_dp_exec;         // for Store Queue dispatch-execute
    uint64_t dsp_sq_exec_ret;        // for Store Queue execute-retire
    uint64_t dsp_rf_rr_ret;          // for PRF regread-retire
    uint64_t dsp_rf_wb_ret;          // for PRF writeback-retire

    // NOP injection settings
    bool enable_nop_injection;       // enable NOP injection based on metadata
    uint64_t nop_injection_scheme;   // NOP injection scheme identifier

    // Submodules enable flags
    // - Non-SVF submodules
    bool enable_avf_analyzer;        // enable avf_analyzer submodule
    bool enable_pvf_analyzer;        // enable pvf_analyzer submodule
    bool enable_hvf_analyzer;        // enable hvf_analyzer submodule
    bool enable_occupancy_analyzer;  // enable occupancy_analyzer submodule
    bool enable_spec_analyzer;       // enable spec_analyzer submodule
    bool enable_func_stats;          // enable func_stats submodule
    bool enable_vbit_analyzer;       // enable vbit_analyzer submodule
    // - SVF submodules
    bool enable_svf_analyzer;         // enable svf_analyzer submodule
    bool enable_svf_nc_analyzer;      // enable svf_nc_analyzer submodule
    bool enable_svf_wc_analyzer;      // enable svf_wc_analyzer submodule
    bool enable_svf_rc_analyzer;      // enable svf_rc_analyzer submodule
    bool enable_svf_analyzer_nlv;     // enable svf_analyzer_nlv submodule
    bool enable_svf_analyzer_nm;      // enable svf_analyzer_nm submodule
    bool enable_svf_analyzer_ubits;   // enable svf_analyzer_ubits submodule
    bool enable_svf_analyzer_pm;      // enable svf_analyzer_pm submodule
    bool enable_svf_analyzer_prf;     // enable svf_analyzer_prf submodule
    bool enable_svf_analyzer_prf1;    // enable svf_analyzer_prf1 submodule
    bool enable_svf_analyzer_prfnop;  // enable svf_analyzer_prfnop submodule
    bool enable_svf_analyzer_prfnop1; // enable svf_analyzer_prfnop1 submodule
    bool enable_svf_analyzer_prfnop2; // enable svf_analyzer_prfnop2 submodule
    bool enable_avfcl_analyzer;       // enable avfcl_analyzer submodule
    bool enable_avfcl_nm_analyzer;    // enable avfcl_nm_analyzer submodule
    bool enable_avfcl_nc_analyzer;    // enable avfcl_nc_analyzer submodule
    bool enable_avfcl_wc_analyzer;    // enable avfcl_wc_analyzer submodule
    bool enable_avfcl_rc_analyzer;    // enable avfcl_rc_analyzer submodule
};

class Svm {
   public:
    Svm();
    ~Svm();

    // Initialize SVM module and set metadata file
    void Init(std::string base_output_dir);
    void SetMetadata(std::string mfile);

    // Start post mortem analysis
    void Analyze();

    // Logging simulation event interface
    void LogClockTick(uint64_t clk);
    void LogFetch(uint64_t seq_no, uint64_t pc,
                  uint32_t opcode, bool is_syscall, bool is_call,
                  bool is_return, bool m_cache_hit);
    void LogRename(uint64_t seq_no,
                   bool dest_valid, int64_t dest_log, int64_t dest_phy,
                   bool src1_valid, int64_t src1_log, int64_t src1_phy,
                   bool src2_valid, int64_t src2_log, int64_t src2_phy);
    void LogDispatch(uint64_t seq_no,
                     bool is_load, uint64_t load_id,
                     bool is_store, uint64_t store_id);
    void LogIssue(uint64_t seq_no, int lane, int al_entry_tail_diff);
    void LogRegRead(uint64_t seq_no, uint64_t src1, uint64_t src2);
    void LogMemExecute(uint64_t seq_no);
    void LogLoadExecute(uint64_t load_id);
    void LogWriteBack(uint64_t seq_no);
    void LogRetire(uint64_t seq_no, bool is_control, bool mispred);
    void LogStoreCommit(uint64_t store_id);
    void LogFreedReg(int64_t reg_id);
    void LogFlush();
    void LogException(uint64_t seq_no, uint64_t pc, uint64_t v0);

    // Inform SVM that warm-up period ends
    void EndOfWarmUp();

    // accessors and mutators
    struct SvmCfg cfg() {return cfg_;}
    void set_cfg(struct SvmCfg cfg) {cfg_ = cfg;}

    // SVM and ECC status
    bool MonitoringActive();
    bool EccActive();

    // simulation warm up period
    bool IsWarmUpPeriod();

    // methods to set request flush flag and check if SVM requested a
    // pipeline flush
    void RequestFlush();
    bool IsFlushRequested();
    void ClearFlushRequestFlag();
    bool IsFlushInProgress();
    void ActivateEcc();
    void DeactivateEcc();

    // method to poll whether a NOP is needed or not
    void CheckNopInjection(struct NopInjectionInfo *pinfo);

    // protection history
    std::vector<struct ProtectionHistoryEntry> prot_hist_;

   private:
    bool initialized_;
    uint64_t clk_;
    bool flush_requested_;
    uint64_t flush_penalty_remaining_;
    bool ecc_is_active_;

    SimListenerList *listener_list_;
    FtAnalyzerList *analyzer_list_;

    SimLogNonSpec *simlog_;
    MReader *mr_;
    FtAnalyzer *sim_logger_;
    AvfAnalyzer *avf_analyzer_;
    PvfAnalyzer *pvf_analyzer_;
    HvfAnalyzer *hvf_analyzer_;
    OccupancyAnalyzer *occupancy_analyzer_;
    SvfAnalyzer *svf_analyzer_;
    SvfAnalyzer *svf_nc_analyzer_;
    SvfAnalyzer *svf_wc_analyzer_;
    SvfAnalyzer *svf_rc_analyzer_;
    SvfAnalyzer *svf_analyzer_nlv_;
    SvfAnalyzer *svf_analyzer_nm_;
    SvfAnalyzer *svf_analyzer_ubits_;
    SvfAnalyzer *svf_analyzer_pm_;
    SvfAnalyzer *svf_analyzer_prf_;
    SvfAnalyzer *svf_analyzer_prf1_;
    SvfAnalyzer *svf_analyzer_prfnop_;
    SvfAnalyzer *svf_analyzer_prfnop1_;
    SvfAnalyzer *svf_analyzer_prfnop2_;
    SvfAnalyzer *avfcl_analyzer_;
    SvfAnalyzer *avfcl_nm_analyzer_;
    SvfAnalyzer *avfcl_nc_analyzer_;
    SvfAnalyzer *avfcl_wc_analyzer_;
    SvfAnalyzer *avfcl_rc_analyzer_;
    SpecAnalyzer *spec_analyzer_;
    FuncStats *func_stats_;
    VbitAnalyzer *vbit_analyzer_;

    // NOP injection handler
    SvfAnalyzer *nop_injection_handler_;

    // SVM configuration
    struct SvmCfg cfg_;

    void DisplayStatus(bool en, std::string name);
};

extern Svm *SVM;

#endif /* _SVM_H_ */
