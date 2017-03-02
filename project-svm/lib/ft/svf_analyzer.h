/**********************************************************
 *
 * SVF analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 11/13/2014
 **********************************************************/

#ifndef _SVF_ANALYZER_H_
#define _SVF_ANALYZER_H_

#include <queue>
#include <deque>
#include <stack>
#include "simlog.h"
#include "occupancy_analyzer.h"
#include "mreader.h"
#include "vbit_table.h"

#define ANALYZED_STRUCTURES_COUNT 6
#define NO_NOP_INJECTION ((int64_t)-1)

typedef enum {
    SVF_MODE_NORMAL = 0,
    SVF_MODE_NO_LIVE_VECTOR,
    SVF_MODE_NO_METADATA,
    SVF_MODE_METADATA_IN_UBITS,
    SVF_MODE_PERFECT_METADATA,
    SVF_MODE_AVFCL,
    SVF_MODE_AVFCL_NO_METADATA,
    SVF_MODE_PRF_USE_METADATA,
    SVF_MODE_PRF1_USE_METADATA,
    SVF_MODE_PRF_NOCOMP,
    SVF_MODE_PRF_WRCOMP,
    SVF_MODE_PRF_RDCOMP,
    SVF_MODE_AVFCL_NOCOMP,
    SVF_MODE_AVFCL_WRCOMP,
    SVF_MODE_AVFCL_RDCOMP,
    SVF_MODE_PRF_NOPS_METADATA,
    SVF_MODE_PRF_NOPS1_METADATA,
    SVF_MODE_PRF_NOPS2_METADATA,
} SvfMode;

struct StatsCountVbits {
    uint64_t cnt;
    uint64_t min_cnt;
    uint64_t max_cnt;
    uint64_t total_cnt;
    uint64_t min_vbits;
    uint64_t max_vbits;
    uint64_t total_vbits;
};

struct CommonDurations {
    uint64_t iq;
    uint64_t rob;
    uint64_t lq1;
    uint64_t lq2;
    uint64_t sq1;
    uint64_t sq2;
    uint64_t ib;
};

struct InstanceCounters {
    uint64_t iq;
    uint64_t rob;
    uint64_t lq;
    uint64_t sq;
    uint64_t ib;
};

struct SampleUnit {
    uint64_t counter; // instruction counter
    uint64_t period; // the period of sampling
    uint64_t seq_no; // for tracking which instruction we are sampling at
    uint64_t t_new_start; // for tracking new value
    // t_start and t_end is the established sampled value
    uint64_t t_start;
    uint64_t t_end;

    void UpdateStart(uint64_t id, uint64_t new_start) {
        if (counter++ % period == 0) {
            seq_no = id; t_new_start = new_start;
        }
    }
    void UpdateEnd(uint64_t id, uint64_t new_end) {
        if (seq_no == id) {
            t_end = new_end; t_start = t_new_start;
        }
    }
    void Flush() {}
    void Retire() {}
};

struct StartEnd {
    uint64_t start;
    uint64_t end;
};

struct SampleUnitVector {
    uint64_t counter;
    uint64_t end_counter;
    uint64_t retire_counter;
    uint64_t period; // the period of sampling
    uint64_t seq_no; // for tracking which instruction we are sampling at
    // t_start and t_end is the established sampled value
    uint64_t t_start;
    uint64_t t_end;
    std::map<uint64_t, struct StartEnd> ts;

    void UpdateStart(uint64_t id, uint64_t new_start) {
        if (counter++ % period == 0) {
            seq_no = id;
            ts[seq_no].start = new_start;
        }
    }
    void UpdateEnd(uint64_t id, uint64_t new_end) {
        if (ts.find(id) != ts.end()) {
            ts[id].end = new_end;
        }
    }
    void Flush() {
        counter = retire_counter;
        ts.clear();
    }
    void Retire() {
        if (retire_counter++ % period == 0) {
            if (!ts.empty()) {
                uint64_t key = ts.begin()->first;
                if (ts[key].end != 0) {
                    t_start = ts.begin()->second.start;
                    t_end = ts.begin()->second.end;
                }
                ts.erase(key);
            }
        }
    }
};

struct DurationSampling {
    struct SampleUnitVector ib;
    struct SampleUnitVector iq_fast;
    struct SampleUnitVector iq_slow;
    struct SampleUnit al_dp_ret;
    struct SampleUnitVector al_wb_ret_fast;
    struct SampleUnitVector al_wb_ret_slow;
    struct SampleUnit lq_dp_exec;
    struct SampleUnit lq_exec_ret;
    struct SampleUnit sq_dp_exec;
    struct SampleUnit sq_exec_ret;
    struct SampleUnit rf_rr_ret;
    struct SampleUnit rf_wb_ret;
};

struct NopInjectionInfo {
    // inputs
    uint64_t pc;
    int32_t fetch_count;
    bool is_delay_slot;
    // outputs
    int64_t n_nops;
    uint64_t start_index;
};

struct BasicBlockStats {
    uint64_t boundary_after_control;
    uint64_t boundary_on_nop;
    uint64_t boundary_undetected;
    bool is_control;
    bool is_control_d;
    bool is_control_d2;
    uint64_t metadata_available_cnt;
    uint64_t metadata_not_available_cnt;
    uint64_t ubits_available;
    bool boundary_was_detected;
    std::map<uint64_t, uint64_t> undetected_bb;
};

struct RcqEntry {
    int8_t rc;
    uint64_t seq_no;
};

struct SqImp {
    bool important;
    uint64_t seq_no;
};

struct MetadataStructure {
    std::queue<struct RcqEntry> rcq;
    uint16_t curr_bb_ubits_cnt;
    uint16_t curr_bb_inst_cnt;
    std::deque<bool> impq;
    uint16_t impq_curr_bb_entry_pushed;

    uint64_t last_fetched_pc;
    uint64_t last_retired_pc;
    bool last_retired_important;
    uint64_t last_fetched_bb;
    uint8_t last_pushed_rc;
    uint8_t last_rc;
    uint8_t current_md_bb_rc;

    bool fetch_is_control;
    bool fetch_is_control_d;
    bool fetch_is_control_d2;
    bool nop_was_skipped;
    bool rc_pushed;

    std::queue<struct SqImp> sq_importance;

    // stats
    uint64_t rcq_max_size;
    uint64_t impq_max_size;
    uint64_t ubits_in_bb_max;
    uint64_t retire_with_entry_in_impq;
    uint64_t retire_no_entry_in_impq;
    uint64_t important_matches;
    uint64_t important_mismatches;
    uint64_t rc_matches;
    uint64_t rc_mismatches;

    // settings
    uint8_t ubits_for_rc;
    uint8_t ubits_on_nop;
    uint64_t rcq_size;
    uint64_t impq_size;
};

struct DynamicProtectionStructure {
    int64_t mem_st_tvbits;
    int64_t tvbits_threshold;
    bool was_enabled;
};

class SvfAnalyzer : public SimListener, public FtAnalyzer {
  public:
    SvfAnalyzer(SimLog *simlog, std::string base_output_path);
    SvfAnalyzer(SimLog *simlog, std::string base_output_path, SvfMode mode);
    ~SvfAnalyzer();

    void SetMetadata(MReader* mr);
    void LogClockTick(LogClockTickArgs args);
    void LogFetch(LogFetchArgs args);
    void LogRename(LogRenameArgs args);
    void LogDispatch(LogDispatchArgs args);
    void LogDispatchLoad(LogDispatchLoadArgs args);
    void LogDispatchStore(LogDispatchStoreArgs args);
    void LogIssue(LogIssueArgs args);
    void LogRegRead(LogRegReadArgs args);
    void LogMemExecute(LogMemExecuteArgs args);
    void LogWriteBack(LogWriteBackArgs args);
    void LogRetire(LogRetireArgs args);
    void LogControlCommit(LogControlCommitArgs args);
    void LogStoreCommit(LogStoreCommitArgs args);
    void LogFlush(LogFlushArgs args);
    void WarmUpEnds(WarmUpEndsArgs args);
    bool Analyze(uint64_t max_clock);
    bool MonitoringActive();
    void CheckNopInjection(struct NopInjectionInfo *pinfo);

  private:
    MReader *mr_;
    bool enabled_;
    SvfMode mode_;
    int idx_;
    std::ofstream outfile_group_;
    std::ofstream outfile_group_1M_;
    std::ofstream outfile_ecc_;
    std::vector<int64_t> svfs_[ANALYZED_STRUCTURES_COUNT];
    std::vector<int64_t> svfs_fu_[STRUCTURES_FU_COUNT];
    std::vector<uint64_t> savedregstack_;
    std::deque<uint64_t> livevectorfifo_;
    std::vector<bool> active_history_;
    uint64_t reg_live_vector_;
    uint64_t max_callee_saved_stack_usage_;
    uint64_t dropped_callee_saved_stack_entries_;
    uint64_t missed_callee_saved_stack_entries_;
    uint64_t max_livevectorfifo_depth_;
    uint64_t missed_livevectorfifo_entries_;
    uint64_t call_cnt_;
    uint64_t return_cnt_;
    uint64_t livevectorfifo_push_cnt_;
    uint64_t livevectorfifo_pop_cnt_;
    uint64_t last_bb_pc_;
    uint64_t last_bb_pc_dispatch_;
    uint64_t onoff_active_size_;
    uint64_t onoff_period_;
    uint64_t dispatch_inst_has_metadata_;
    uint64_t dispatch_inst_no_metadata_;
    uint64_t retire_inst_has_metadata_;
    uint64_t retire_inst_no_metadata_;
    uint64_t dispatched_nops_default_, dispatched_nops_scheme_1_, dispatched_nops_scheme_2_, dispatched_nops_scheme_3_, dispatched_nops_scheme_4_, dispatched_nops_scheme_5_;
    uint64_t dispatched_resv_default_;
    uint64_t retired_nops_default_, retired_nops_scheme_1_, retired_nops_scheme_2_, retired_nops_scheme_3_, retired_nops_scheme_4_, retired_nops_scheme_5_;
    uint64_t retired_resv_default_;
    uint64_t vbits_rf_live_in_first, vbits_rf_live_out_first, vbits_rf_live_in_avg, vbits_rf_live_out_avg, vbits_rf_live_in_first_inst, vbits_rf_live_in_first_verif;
    uint8_t  rf_live_in_first_count, rf_live_out_first_count, rf_live_in_avg_count, rf_live_out_avg_count, rf_live_in_first_count_inst, rf_live_in_first_count_verif;
    std::vector<struct StatsCountVbits> stats_cv_;
    struct CommonDurations cd_;
    struct InstanceCounters ic_;
    struct DurationSampling *sdur_;
    std::map<uint64_t, uint64_t> nops_hist_;
    struct BasicBlockStats *bbstat_;
    struct MetadataStructure *ms_;
    struct DynamicProtectionStructure *dps_;

    void Init(SvfMode mode);
    void ClearStats();
    uint64_t SingleKill(uint64_t seq_no, int16_t log, bool kill_flag);
    uint64_t SingleKill(uint64_t seq_no, int source_offset, int16_t log,
                        bool kill_flag);
    uint64_t SingleWrite(uint64_t seq_no, int16_t log, bool live_flag);
    uint64_t Cycle2Group(uint64_t cycle);
    bool SvfActive(uint64_t cycle);
    void UpdateVbitsStats(uint64_t dur, uint64_t vbits, uint64_t st);
    uint64_t CalcDuration(uint64_t start, uint64_t end);
    uint64_t CalcVbits(uint64_t dur, uint64_t vbit_per_cycle);
    bool MetadataAvailable(uint64_t seq_no);
};

#endif /* _SVF_ANALYZER_H_ */

/* end of file */
