/**********************************************************
 *
 * System vulnerability model class implementation file
 *
 * First author: Bagus Wibowo
 * created: 02/07/2015
 **********************************************************/

#include <cassert>
#include "svm.h"
#include "term_style.h"

Svm* SVM = new Svm();

Svm::Svm() {
    clk_ = (uint64_t)-1;

    // setting default core configuration
    cfg_.core_r0_hardwired = false;
    cfg_.nop_issued = false;

    // setting default miscellaneous configuration
    cfg_.verbose = true; // by default, verbose mode is on
    cfg_.precise_freelist_tracking = true;
    cfg_.flush_reset_freelist = false;
    cfg_.keep_cycle_detailed_files = false;
    cfg_.adaptive_monitoring = false;
    cfg_.svf_active_size = 1024*1024; // by default, set to 1M cycles
    cfg_.svf_recp_active_rate = 1; // by default, set SVF to always active
    cfg_.enable_warm_up = false;
    cfg_.end_of_warm_up = 0;
    cfg_.enable_log_all_insts = true; // by default, dump all instructions
                                      // timing
    cfg_.enable_regs_life_dump = true; // enable register life dump
    cfg_.ecc_activate_penalty = 100; // by default, 100 cycles penalty upon
                                     // ECC activation
    cfg_.sts_period = 256;  // sample every 256 instructions by default
    cfg_.enable_svf_only = false;   // by default, disable SVF analysis only
    cfg_.pvs_profile_file = "";  // by default, the profile file is not provided

    // VF adaptive configurations
    cfg_.enable_svf_adaptive = false;
    cfg_.svf_adaptive_thd = 1.00;

    // SVF duration sampling
    cfg_.dsp_ib = 256;             // for Instruction Buffer
    cfg_.dsp_iq = 256;             // for Issue Queue
    cfg_.dsp_al_dp_ret = 256;      // for Active List dispatch-retire
    cfg_.dsp_al_wb_ret = 256;      // for Active List writeback-retire
    cfg_.dsp_lq_dp_exec = 256;     // for Load Queue dispatch-execute
    cfg_.dsp_lq_exec_ret = 256;    // for Load Queue execute-retire
    cfg_.dsp_sq_dp_exec = 256;     // for Store Queue dispatch-execute
    cfg_.dsp_sq_exec_ret = 256;    // for Store Queue execute-retire
    cfg_.dsp_rf_rr_ret = 256;      // for PRF regread-retire
    cfg_.dsp_rf_wb_ret = 256;      // for PRF writeback-retire

    // NOP injection settings
    cfg_.enable_nop_injection = false;  // by default, no NOP injection
    cfg_.nop_injection_scheme = 0;      // zero is the default

    // Submodule enable flags
    // - Non-SVF submodules
    cfg_.enable_avf_analyzer = true;        // enable avf_analyzer submodule by default
    cfg_.enable_pvf_analyzer = true;        // enable pvf_analyzer submodule by default
    cfg_.enable_hvf_analyzer = true;        // enable hvf_analyzer submodule by default
    cfg_.enable_occupancy_analyzer = true;  // enable occupancy_analyzer submodule by default
    cfg_.enable_spec_analyzer = true;       // enable spec_analyzer submodule by default
    cfg_.enable_func_stats = true;          // enable func_stats submodule by default
    cfg_.enable_vbit_analyzer = true;       // enable vbit_analyzer submodule by default
    // - SVF submodules
    cfg_.enable_svf_analyzer = true;        // enable svf_analyzer submodule by default
    cfg_.enable_svf_nc_analyzer = true;     // enable svf_nc_analyzer submodule by default
    cfg_.enable_svf_wc_analyzer = true;     // enable svf_wc_analyzer submodule by default
    cfg_.enable_svf_rc_analyzer = true;     // enable svf_rc_analyzer submodule by default
    cfg_.enable_svf_analyzer_nlv = true;    // enable svf_analyzer_nlv submodule by default
    cfg_.enable_svf_analyzer_nm = true;     // enable svf_analyzer_nm submodule by default
    cfg_.enable_svf_analyzer_ubits = true;  // enable svf_analyzer_ubits submodule by default
    cfg_.enable_svf_analyzer_pm = true;     // enable svf_analyzer_pm submodule by default
    cfg_.enable_svf_analyzer_prf = true;    // enable svf_analyzer_prf submodule by default
    cfg_.enable_svf_analyzer_prf1 = true;   // enable svf_analyzer_prf1 submodule by default
    cfg_.enable_svf_analyzer_prfnop = true; // enable svf_analyzer_prfnop submodule by default
    cfg_.enable_svf_analyzer_prfnop1 = true; // enable svf_analyzer_prfnop1 submodule by default
    cfg_.enable_svf_analyzer_prfnop2 = true; // enable svf_analyzer_prfnop2 submodule by default
    cfg_.enable_avfcl_analyzer = true;      // enable avfcl_analyzer submodule by default
    cfg_.enable_avfcl_nm_analyzer = true;   // enable avfcl_nm_analyzer submodule by default
    cfg_.enable_avfcl_nc_analyzer = true;   // enable avfcl_nc_analyzer submodule by default
    cfg_.enable_avfcl_wc_analyzer = true;   // enable avfcl_wc_analyzer submodule by default
    cfg_.enable_avfcl_rc_analyzer = true;   // enable avfcl_rc_analyzer submodule by default

    // clear submodule pointers
    listener_list_ = NULL;
    analyzer_list_ = NULL;
    simlog_ = NULL;
    sim_logger_ = NULL;
    avf_analyzer_ = NULL;
    pvf_analyzer_ = NULL;
    hvf_analyzer_ = NULL;
    occupancy_analyzer_ = NULL;
    svf_analyzer_ = NULL;
    svf_nc_analyzer_ = NULL;
    svf_wc_analyzer_ = NULL;
    svf_rc_analyzer_ = NULL;
    svf_analyzer_nlv_ = NULL;
    svf_analyzer_nm_ = NULL;
    svf_analyzer_ubits_ = NULL;
    svf_analyzer_pm_ = NULL;
    svf_analyzer_prf_ = NULL;
    svf_analyzer_prf1_ = NULL;
    svf_analyzer_prfnop_ = NULL;
    svf_analyzer_prfnop1_ = NULL;
    svf_analyzer_prfnop2_ = NULL;
    avfcl_analyzer_ = NULL;
    avfcl_nm_analyzer_ = NULL;
    avfcl_nc_analyzer_ = NULL;
    avfcl_wc_analyzer_ = NULL;
    avfcl_rc_analyzer_ = NULL;
    spec_analyzer_ = NULL;
    func_stats_ = NULL;
    vbit_analyzer_ = NULL;
}

Svm::~Svm() {
    delete listener_list_;
    delete analyzer_list_;
    delete simlog_;
    delete sim_logger_;
    delete avf_analyzer_;
    delete pvf_analyzer_;
    delete hvf_analyzer_;
    delete occupancy_analyzer_;
    delete svf_analyzer_;
    delete svf_nc_analyzer_;
    delete svf_wc_analyzer_;
    delete svf_rc_analyzer_;
    delete svf_analyzer_nlv_;
    delete svf_analyzer_nm_;
    delete svf_analyzer_ubits_;
    delete svf_analyzer_pm_;
    delete svf_analyzer_prf_;
    delete svf_analyzer_prf1_;
    delete svf_analyzer_prfnop_;
    delete svf_analyzer_prfnop1_;
    delete svf_analyzer_prfnop2_;
    delete avfcl_analyzer_;
    delete avfcl_nm_analyzer_;
    delete avfcl_nc_analyzer_;
    delete avfcl_wc_analyzer_;
    delete avfcl_rc_analyzer_;
    delete spec_analyzer_;
    delete func_stats_;
    delete vbit_analyzer_;
    delete sim_logger_;
    delete mr_;
}

void Svm::Init(std::string base_output_dir) {
    VbitTableInit();

    listener_list_ = new SimListenerList();
    analyzer_list_ = new FtAnalyzerList(base_output_dir);

    simlog_ = new SimLogNonSpec();
    sim_logger_ = new SimLogger(simlog_, base_output_dir);

    if (cfg_.enable_svf_only) {
        // override all non-SVF submodule enables
        cfg_.enable_avf_analyzer = false;
        cfg_.enable_pvf_analyzer = false;
        cfg_.enable_hvf_analyzer = false;
        cfg_.enable_occupancy_analyzer = false;
        cfg_.enable_spec_analyzer = false;
        cfg_.enable_func_stats = false;
        cfg_.enable_vbit_analyzer = false;
    }

    if (cfg_.enable_avf_analyzer)
        avf_analyzer_ = new AvfAnalyzer(simlog_, base_output_dir);
    if (cfg_.enable_pvf_analyzer)
        pvf_analyzer_ = new PvfAnalyzer(simlog_, base_output_dir, cfg_.pvs_profile_file);
    if (cfg_.enable_hvf_analyzer)
        hvf_analyzer_ = new HvfAnalyzer(simlog_, base_output_dir, pvf_analyzer_);
    if (cfg_.enable_occupancy_analyzer)
        occupancy_analyzer_ = new OccupancyAnalyzer(simlog_, base_output_dir);
    if (cfg_.enable_svf_analyzer)
        svf_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir);
    if (cfg_.enable_svf_nc_analyzer)
        svf_nc_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_NOCOMP);
    if (cfg_.enable_svf_wc_analyzer)
        svf_wc_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_WRCOMP);
    if (cfg_.enable_svf_rc_analyzer)
        svf_rc_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_RDCOMP);
    if (cfg_.enable_svf_analyzer_nlv)
        svf_analyzer_nlv_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_NO_LIVE_VECTOR);
    if (cfg_.enable_svf_analyzer_nm)
        svf_analyzer_nm_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_NO_METADATA);
    if (cfg_.enable_svf_analyzer_ubits)
        svf_analyzer_ubits_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_METADATA_IN_UBITS);
    if (cfg_.enable_svf_analyzer_pm)
        svf_analyzer_pm_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PERFECT_METADATA);
    if (cfg_.enable_svf_analyzer_prf)
        svf_analyzer_prf_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_USE_METADATA);
    if (cfg_.enable_svf_analyzer_prf1)
        svf_analyzer_prf1_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF1_USE_METADATA);
    if (cfg_.enable_svf_analyzer_prfnop)
        svf_analyzer_prfnop_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_NOPS_METADATA);
    if (cfg_.enable_svf_analyzer_prfnop1)
        svf_analyzer_prfnop1_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_NOPS1_METADATA);
    if (cfg_.enable_svf_analyzer_prfnop2)
        svf_analyzer_prfnop2_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_PRF_NOPS2_METADATA);
    if (cfg_.enable_avfcl_analyzer)
        avfcl_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_AVFCL);
    if (cfg_.enable_avfcl_nm_analyzer)
        avfcl_nm_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_AVFCL_NO_METADATA);
    if (cfg_.enable_avfcl_nc_analyzer)
        avfcl_nc_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_AVFCL_NOCOMP);
    if (cfg_.enable_avfcl_wc_analyzer)
        avfcl_wc_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_AVFCL_WRCOMP);
    if (cfg_.enable_avfcl_rc_analyzer)
        avfcl_rc_analyzer_ = new SvfAnalyzer(simlog_, base_output_dir, SVF_MODE_AVFCL_RDCOMP);
    if (cfg_.enable_spec_analyzer)
        spec_analyzer_ = new SpecAnalyzer(simlog_, base_output_dir);
    if (cfg_.enable_func_stats)
        func_stats_ = new FuncStats(simlog_, base_output_dir);
    if (cfg_.enable_vbit_analyzer)
        vbit_analyzer_ = new VbitAnalyzer(simlog_, base_output_dir);

    // Adding submodules into listener_list
    listener_list_->AddListener(simlog_);
    if (cfg_.enable_hvf_analyzer) listener_list_->AddListener(hvf_analyzer_);
    // PVF analyzer must be added after HVF analyzer
    if (cfg_.enable_pvf_analyzer) listener_list_->AddListener(pvf_analyzer_);
    if (cfg_.enable_occupancy_analyzer) listener_list_->AddListener(occupancy_analyzer_);
    if (cfg_.enable_func_stats) listener_list_->AddListener(func_stats_);
    if (cfg_.enable_svf_analyzer) listener_list_->AddListener(svf_analyzer_);
    if (cfg_.enable_svf_nc_analyzer) listener_list_->AddListener(svf_nc_analyzer_);
    if (cfg_.enable_svf_wc_analyzer) listener_list_->AddListener(svf_wc_analyzer_);
    if (cfg_.enable_svf_rc_analyzer) listener_list_->AddListener(svf_rc_analyzer_);
    if (cfg_.enable_svf_analyzer_nlv) listener_list_->AddListener(svf_analyzer_nlv_);
    if (cfg_.enable_svf_analyzer_nm) listener_list_->AddListener(svf_analyzer_nm_);
    if (cfg_.enable_svf_analyzer_ubits) listener_list_->AddListener(svf_analyzer_ubits_);
    if (cfg_.enable_svf_analyzer_pm) listener_list_->AddListener(svf_analyzer_pm_);
    if (cfg_.enable_svf_analyzer_prf) listener_list_->AddListener(svf_analyzer_prf_);
    if (cfg_.enable_svf_analyzer_prf1) listener_list_->AddListener(svf_analyzer_prf1_);
    if (cfg_.enable_svf_analyzer_prfnop) listener_list_->AddListener(svf_analyzer_prfnop_);
    if (cfg_.enable_svf_analyzer_prfnop1) listener_list_->AddListener(svf_analyzer_prfnop1_);
    if (cfg_.enable_svf_analyzer_prfnop2) listener_list_->AddListener(svf_analyzer_prfnop2_);
    if (cfg_.enable_avfcl_analyzer) listener_list_->AddListener(avfcl_analyzer_);
    if (cfg_.enable_avfcl_nm_analyzer) listener_list_->AddListener(avfcl_nm_analyzer_);
    if (cfg_.enable_avfcl_nc_analyzer) listener_list_->AddListener(avfcl_nc_analyzer_);
    if (cfg_.enable_avfcl_wc_analyzer) listener_list_->AddListener(avfcl_wc_analyzer_);
    if (cfg_.enable_avfcl_rc_analyzer) listener_list_->AddListener(avfcl_rc_analyzer_);

    // Adding submodules into analyzer_list
    if (cfg_.enable_avf_analyzer) analyzer_list_->AddAnalyzer(avf_analyzer_);
    // PVF analyzer must be added before HVF analyzer
    if (cfg_.enable_pvf_analyzer) analyzer_list_->AddAnalyzer(pvf_analyzer_);
    if (cfg_.enable_hvf_analyzer) analyzer_list_->AddAnalyzer(hvf_analyzer_);
    if (cfg_.enable_occupancy_analyzer) analyzer_list_->AddAnalyzer(occupancy_analyzer_);
    if (cfg_.enable_spec_analyzer) analyzer_list_->AddAnalyzer(spec_analyzer_);
    if (cfg_.enable_func_stats) analyzer_list_->AddAnalyzer(func_stats_);
    if (cfg_.enable_vbit_analyzer) analyzer_list_->AddAnalyzer(vbit_analyzer_);
    if (cfg_.enable_svf_analyzer) analyzer_list_->AddAnalyzer(svf_analyzer_);
    if (cfg_.enable_svf_nc_analyzer) analyzer_list_->AddAnalyzer(svf_nc_analyzer_);
    if (cfg_.enable_svf_wc_analyzer) analyzer_list_->AddAnalyzer(svf_wc_analyzer_);
    if (cfg_.enable_svf_rc_analyzer) analyzer_list_->AddAnalyzer(svf_rc_analyzer_);
    if (cfg_.enable_svf_analyzer_nlv) analyzer_list_->AddAnalyzer(svf_analyzer_nlv_);
    if (cfg_.enable_svf_analyzer_nm) analyzer_list_->AddAnalyzer(svf_analyzer_nm_);
    if (cfg_.enable_svf_analyzer_ubits) analyzer_list_->AddAnalyzer(svf_analyzer_ubits_);
    if (cfg_.enable_svf_analyzer_pm) analyzer_list_->AddAnalyzer(svf_analyzer_pm_);
    if (cfg_.enable_svf_analyzer_prf) analyzer_list_->AddAnalyzer(svf_analyzer_prf_);
    if (cfg_.enable_svf_analyzer_prf1) analyzer_list_->AddAnalyzer(svf_analyzer_prf1_);
    if (cfg_.enable_svf_analyzer_prfnop) analyzer_list_->AddAnalyzer(svf_analyzer_prfnop_);
    if (cfg_.enable_svf_analyzer_prfnop1) analyzer_list_->AddAnalyzer(svf_analyzer_prfnop1_);
    if (cfg_.enable_svf_analyzer_prfnop2) analyzer_list_->AddAnalyzer(svf_analyzer_prfnop2_);
    if (cfg_.enable_avfcl_analyzer) analyzer_list_->AddAnalyzer(avfcl_analyzer_);
    if (cfg_.enable_avfcl_nm_analyzer) analyzer_list_->AddAnalyzer(avfcl_nm_analyzer_);
    if (cfg_.enable_avfcl_nc_analyzer) analyzer_list_->AddAnalyzer(avfcl_nc_analyzer_);
    if (cfg_.enable_avfcl_wc_analyzer) analyzer_list_->AddAnalyzer(avfcl_wc_analyzer_);
    if (cfg_.enable_avfcl_rc_analyzer) analyzer_list_->AddAnalyzer(avfcl_rc_analyzer_);

    analyzer_list_->AddAnalyzer(sim_logger_); // this should be the last

    flush_requested_ = false; // by default, no flush is requested
    ecc_is_active_ = false; // by default, ECC is not activated

    // by default, nop_injection_handler_ is none
    nop_injection_handler_ = NULL;
    if (cfg_.enable_nop_injection) {
        // set avfcl_analyzer_ as NOP injection query handler
        nop_injection_handler_ = avfcl_analyzer_;
    }

    DisplayStatus(cfg_.enable_avf_analyzer, "avf_analyzer");
    DisplayStatus(cfg_.enable_pvf_analyzer, "pvf_analyzer");
    DisplayStatus(cfg_.enable_hvf_analyzer, "hvf_analyzer");
    DisplayStatus(cfg_.enable_occupancy_analyzer, "occupancy_analyzer");
    DisplayStatus(cfg_.enable_spec_analyzer, "spec_analyzer");
    DisplayStatus(cfg_.enable_func_stats, "func_stats");
    DisplayStatus(cfg_.enable_vbit_analyzer, "vbit_analyzer");
    DisplayStatus(cfg_.enable_svf_analyzer, "svf_analyzer");
    DisplayStatus(cfg_.enable_svf_nc_analyzer, "svf_nc_analyzer");
    DisplayStatus(cfg_.enable_svf_wc_analyzer, "svf_wc_analyzer");
    DisplayStatus(cfg_.enable_svf_rc_analyzer, "svf_rc_analyzer");
    DisplayStatus(cfg_.enable_svf_analyzer_nlv, "svf_analyzer_nlv");
    DisplayStatus(cfg_.enable_svf_analyzer_nm, "svf_analyzer_nm");
    DisplayStatus(cfg_.enable_svf_analyzer_ubits, "svf_analyzer_ubits");
    DisplayStatus(cfg_.enable_svf_analyzer_pm, "svf_analyzer_pm");
    DisplayStatus(cfg_.enable_svf_analyzer_prf, "svf_analyzer_prf");
    DisplayStatus(cfg_.enable_svf_analyzer_prf1, "svf_analyzer_prf1");
    DisplayStatus(cfg_.enable_svf_analyzer_prfnop, "svf_analyzer_prfnop");
    DisplayStatus(cfg_.enable_svf_analyzer_prfnop1, "svf_analyzer_prfnop1");
    DisplayStatus(cfg_.enable_svf_analyzer_prfnop2, "svf_analyzer_prfnop2");
    DisplayStatus(cfg_.enable_avfcl_analyzer, "avfcl_analyzer");
    DisplayStatus(cfg_.enable_avfcl_nm_analyzer, "avfcl_nm_analyzer");
    DisplayStatus(cfg_.enable_avfcl_nc_analyzer, "avfcl_nc_analyzer");
    DisplayStatus(cfg_.enable_avfcl_wc_analyzer, "avfcl_wc_analyzer");
    DisplayStatus(cfg_.enable_avfcl_rc_analyzer, "avfcl_rc_analyzer");
}

void Svm::SetMetadata(std::string mfile) {
    try {
        mr_ = new MReader(mfile.c_str());
        std::cout << "[svm] Loading metadata file " << mfile <<
            std::endl;
        if (cfg_.enable_svf_analyzer) svf_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_svf_nc_analyzer) svf_nc_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_svf_wc_analyzer) svf_wc_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_svf_rc_analyzer) svf_rc_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_nlv) svf_analyzer_nlv_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_nm) svf_analyzer_nm_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_ubits) svf_analyzer_ubits_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_pm) svf_analyzer_pm_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_prf) svf_analyzer_prf_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_prf1) svf_analyzer_prf1_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_prfnop) svf_analyzer_prfnop_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_prfnop1) svf_analyzer_prfnop1_->SetMetadata(mr_);
        if (cfg_.enable_svf_analyzer_prfnop2) svf_analyzer_prfnop2_->SetMetadata(mr_);
        if (cfg_.enable_avfcl_analyzer) avfcl_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_avfcl_nm_analyzer) avfcl_nm_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_avfcl_nc_analyzer) avfcl_nc_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_avfcl_wc_analyzer) avfcl_wc_analyzer_->SetMetadata(mr_);
        if (cfg_.enable_avfcl_rc_analyzer) avfcl_rc_analyzer_->SetMetadata(mr_);
        simlog_->SetAVFMetadata(mr_);
        if (cfg_.enable_func_stats) func_stats_->SetMetadata(mr_);
        if (cfg_.enable_pvf_analyzer) pvf_analyzer_->SetMetadata(mr_);
    }
    catch (int err) {
        if (err == -1) {
            std::cout << "[svm] Loading metadata file " << mfile <<
                " could not be opened." << std::endl;
        }
    }
}

// Start post mortem analysis
void Svm::Analyze() {
    // call LogClockTick handler first
    LogClockTickArgs args;
    args.clk = clk_;
    listener_list_->LogClockTick(args);

    // call Analyze handler
    analyzer_list_->Analyze(clk_);
}

void Svm::LogClockTick(uint64_t clk) {
    if (clk_ != (uint64_t)-1) {
        // process previous clock
        // construct arguments
        LogClockTickArgs args;
        args.clk = clk_;

        // call LogClockTick handler
        listener_list_->LogClockTick(args);
    }

    // set clk_
    clk_ = clk;
}

void Svm::LogFetch(uint64_t seq_no, uint64_t pc,
                   uint32_t opcode, bool is_syscall, bool is_call,
                   bool is_return, bool m_cache_hit) {
    // construct arguments
    LogFetchArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;
    args.pc = pc;
    args.opcode = opcode;
    args.is_syscall = (is_syscall != 0);
    args.is_call = (is_call != 0);
    args.is_return = (is_return != 0);
    args.m_cache_hit = (m_cache_hit != 0);

    // call LogFetch handler
    listener_list_->LogFetch(args);
}

void Svm::LogRename(uint64_t seq_no,
                    bool dest_valid, int64_t dest_log, int64_t dest_phy,
                    bool src1_valid, int64_t src1_log, int64_t src1_phy,
                    bool src2_valid, int64_t src2_log, int64_t src2_phy) {
    // construct arguments
    LogRenameArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;
    args.dest_valid = dest_valid;
    args.dest_log = dest_log;
    args.dest_phy = dest_phy;
    args.src1_valid = src1_valid;
    args.src1_log = src1_log;
    args.src1_phy = src1_phy;
    args.src2_valid = src2_valid;
    args.src2_log = src2_log;
    args.src2_phy = src2_phy;

    // call LogRename handler
    listener_list_->LogRename(args);
}

void Svm::LogDispatch(uint64_t seq_no,
                      bool is_load, uint64_t load_id,
                      bool is_store, uint64_t store_id) {
    // construct arguments
    LogDispatchArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;

    // call LogDispatch handler
    listener_list_->LogDispatch(args);

    if (is_load) {
        // construct arguments
        LogDispatchLoadArgs args;
        args.clk = clk_;
        args.seq_no = seq_no;
        args.load_id = load_id;

        // call LogDispatchLoad handler
        listener_list_->LogDispatchLoad(args);
    }

    if (is_store) {
        // construct arguments
        LogDispatchStoreArgs args;
        args.clk = clk_;
        args.seq_no = seq_no;
        args.store_id = store_id;

        // call LogDispatchStore handler
        listener_list_->LogDispatchStore(args);
    }
}

void Svm::LogIssue(uint64_t seq_no, int lane, int al_entry_tail_diff) {
    // construct arguments
    LogIssueArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;
    args.lane = lane;
    args.al_entry_tail_diff = al_entry_tail_diff;

    // call LogIssue handler
    listener_list_->LogIssue(args);
}

void Svm::LogRegRead(uint64_t seq_no, uint64_t src1, uint64_t src2) {
    // construct arguments
    LogRegReadArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;
    args.src1 = src1;
    args.src2 = src2;

    // call LogRegRead handler
    listener_list_->LogRegRead(args);
}

void Svm::LogMemExecute(uint64_t seq_no) {
    // construct arguments
    LogMemExecuteArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;

    // call LogMemExecute handler
    listener_list_->LogMemExecute(args);
}

void Svm::LogLoadExecute(uint64_t load_id) {
    // construct arguments
    LogLoadExecuteArgs args;
    args.clk = clk_;
    args.load_id = load_id;

    // call LogLoadExecute handler
    listener_list_->LogLoadExecute(args);
}

void Svm::LogWriteBack(uint64_t seq_no) {
    // construct arguments
    LogWriteBackArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;

    // call LogWriteBack handler
    listener_list_->LogWriteBack(args);
}

void Svm::LogRetire(uint64_t seq_no, bool is_control, bool mispred) {
    // construct arguments
    LogRetireArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;

    // call LogRetire handler
    listener_list_->LogRetire(args);

     // if the retiring instruction is a control instruction
    if (is_control) {
        // construct arguments
        LogControlCommitArgs args;
        args.clk = clk_;
        args.seq_no = seq_no;
        args.mispred = mispred;

        // call LogControlCommit handler
        listener_list_->LogControlCommit(args);
    }
}

void Svm::LogStoreCommit(uint64_t store_id) {
    // construct arguments
    LogStoreCommitArgs args;
    args.clk = clk_;
    args.store_id = store_id;

    // call LogStoreCommit Handler
    listener_list_->LogStoreCommit(args);
}

void Svm::LogFreedReg(int64_t reg_id) {
    // construct arguments
    LogFreedRegArgs args;
    args.clk = clk_;
    args.reg_id = reg_id;

    // call LogFreedReg handler
    listener_list_->LogFreedReg(args);
}

void Svm::LogFlush() {
    // construct arguments
    LogFlushArgs args;
    args.clk = clk_;

    // call LogFlush handler
    listener_list_->LogFlush(args);
}

void Svm::LogException(uint64_t seq_no, uint64_t pc, uint64_t v0) {
    // contruct arguments
    LogExceptionArgs args;
    args.clk = clk_;
    args.seq_no = seq_no;
    args.pc = pc;
    args.v0 = v0;

    // call LogException handler
    listener_list_->LogException(args);
}

void Svm::EndOfWarmUp() {
    assert(clk_ > 0);
    cfg_.end_of_warm_up = clk_; // record the end of warm up time

    std::cout << "[svm] Warm-up period ends" << std::endl;

    // construct arguments
    WarmUpEndsArgs args;
    args.clk = clk_;

    // broadcast warm-up ends event
    listener_list_->WarmUpEnds(args);
}

bool Svm::MonitoringActive() {
    if (avfcl_analyzer_) {
        return !ecc_is_active_;
    }
    else if (svf_analyzer_) {
        return svf_analyzer_->MonitoringActive();
    }
    else {
        return true; // assume SVF always ON for else case
    }
}

bool Svm::EccActive() {
    return ecc_is_active_;
}

bool Svm::IsWarmUpPeriod() {
    return (cfg_.enable_warm_up && cfg_.end_of_warm_up == 0);
}

void Svm::RequestFlush() {
    // set request flush flag
    flush_requested_ = true;
    flush_penalty_remaining_ = cfg_.ecc_activate_penalty;
}

bool Svm::IsFlushRequested() {
    return flush_requested_;
}

void Svm::ClearFlushRequestFlag() {
    flush_requested_ = false;
}

bool Svm::IsFlushInProgress() {
    if (flush_penalty_remaining_ == 0)
        return false;
    flush_penalty_remaining_--;
    return true;
}

void Svm::ActivateEcc() {
    if (!ecc_is_active_) {
        RequestFlush();
        ecc_is_active_ = true;
    }
}

void Svm::DeactivateEcc() {
    ecc_is_active_ = false;
}

void Svm::CheckNopInjection(struct NopInjectionInfo *pinfo) {
    if (nop_injection_handler_) {
        nop_injection_handler_->CheckNopInjection(pinfo);
    }
    else {
        // clear n_nops
        pinfo->n_nops = NO_NOP_INJECTION;
    }
}

/* -------------------- private function definitions ------------------------ */
void Svm::DisplayStatus(bool en, std::string name) {
    std::cout << "[svm] Submodule " << name << " is " <<
        (en ? TERM_GREEN : TERM_RED) << (en ? "enabled." : "disabled.") <<
        TERM_RESET << std::endl;
}

/* end of file */
