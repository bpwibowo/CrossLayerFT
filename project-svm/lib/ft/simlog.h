/**********************************************************
 *
 * Simulation log header code
 *
 * First author: Bagus Wibowo
 * created: 05/14/2014
 **********************************************************/

#ifndef _SIMLOG_H_
#define _SIMLOG_H_

#include <fstream>
#include <map>
#include <vector>
#include <deque>
#include <inttypes.h>
#include "mips64r2_encodings.h"
#include "simlog_config.h"
#include "sim_listener.h"
#include "mreader.h"

#define INVALID_STORE_ID ((int64_t)-1)
#define INVALID_LOAD_ID ((int64_t)-1)
#define INVALID_CLK ((uint64_t)-1)
#define REG_NO_PRODUCER ((uint64_t)-2)
#define INVALID_SEQ_NO ((uint64_t)-1)

struct SourceRegister {
    uint64_t producer;
    int32_t phy;
    int16_t log;
};

struct DestinationRegister {
    int32_t phy;
    int16_t log;
    bool analyzed;
    bool freed;
};

struct InstructionDetails {
    uint64_t pc;
    uint64_t save_pc;
    uint64_t decoded_clk;
    uint64_t dispatched_clk;
    uint64_t issued_clk;
    uint64_t regread_clk;
    uint64_t memexecute_clk;
    uint64_t writeback_clk;
    uint64_t retired_clk;
    uint64_t storecommit_clk;
    uint64_t ibuff_duration;
    uint64_t last_store_seq_no;
    std::vector<struct SourceRegister> sources;
    std::vector<struct DestinationRegister> destinations;
    int32_t store_id;
    int32_t load_id;
    int32_t opcode;
    uint32_t qvbit;
    uint32_t rvbit;
    int8_t fu;
    int16_t al_entry_tail_diff;
    bool src1_valid;
    bool src2_valid;
    bool important;
    bool is_syscall;
    bool is_call;
    bool is_return;
    bool m_cache_hit;
};

struct UsesDefs {
    std::vector<int> uses;
    std::vector<int> defs;
};

class SimLog : public SimListener {
  public:
    SimLog();
    virtual ~SimLog();

    void SetAVFMetadata(MReader* mr);
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
    void LogPostStoreCommit(LogStoreCommitArgs args);
    void LogFreedReg(LogFreedRegArgs args);
    virtual void LogFlush(LogFlushArgs args);
    void LogException(LogExceptionArgs args);

    bool IsInstructionAnalyzed(uint64_t seq_no);
    bool IsPcSyscall(uint64_t pc);
    static struct UsesDefs GetSyscallUsesDefs(int type);

    std::map<uint64_t, struct InstructionDetails> logdb_;
    std::deque<int64_t> freelist_;
    std::vector<uint64_t> regdb_;
    std::vector<int> amtable_;
    std::vector<int> amtable_before_exception_;
    std::map<uint64_t, uint64_t> storedb_;
    uint64_t last_dispatched_store_seq_no_;
    uint64_t last_retired_store_seq_no_;
    uint64_t m_cache_hit_cnt;
    uint64_t m_cache_miss_cnt;

  protected:
    uint64_t last_commit_after_flush_;

  private:
    void MarkImportant(uint64_t seq_no);
    void ResetFreeList();

    MReader *mr_;
    bool mr_is_null_warning_shown_;
    std::map<uint64_t, std::deque<uint64_t> > sd_callee;
    std::map<uint64_t, uint64_t> syscalldb_;
};

#endif /* _SIMLOG_H_ */

/* end of file */
