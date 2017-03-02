/**********************************************************
 *
 * Simulation event listener base header code
 *
 * First author: Bagus Wibowo
 * created: 05/29/2014
 **********************************************************/

#ifndef _SIM_LISTENER_H_
#define _SIM_LISTENER_H_

#include <inttypes.h>

typedef struct {
    uint64_t clk;
} LogClockTickArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    uint64_t pc;
    int32_t opcode;
    bool is_syscall;
    bool is_call;
    bool is_return;
    bool m_cache_hit;
} LogFetchArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    bool dest_valid;
    int64_t dest_log;
    int64_t dest_phy;
    bool src1_valid;
    int64_t src1_log;
    int64_t src1_phy;
    bool src2_valid;
    int64_t src2_log;
    int64_t src2_phy;
} LogRenameArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
} LogDispatchArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    uint64_t load_id;
} LogDispatchLoadArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    uint64_t store_id;
} LogDispatchStoreArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    int lane;
    int al_entry_tail_diff;
} LogIssueArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    uint64_t src1;
    uint64_t src2;
} LogRegReadArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
} LogMemExecuteArgs;

typedef struct {
    uint64_t clk;
    uint64_t load_id;
} LogLoadExecuteArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
} LogWriteBackArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
} LogRetireArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    bool mispred;
} LogControlCommitArgs;

typedef struct {
    uint64_t clk;
    uint64_t store_id;
} LogStoreCommitArgs;

typedef struct {
    uint64_t clk;
    int64_t reg_id;
} LogFreedRegArgs;

typedef struct {
    uint64_t clk;
} LogFlushArgs;

typedef struct {
    uint64_t clk;
    uint64_t seq_no;
    uint64_t pc;
    uint64_t v0;
} LogExceptionArgs;

typedef struct {
    uint64_t clk;
} WarmUpEndsArgs;

class SimListener {
  public:
    virtual ~SimListener() {};
    virtual void LogClockTick(LogClockTickArgs args) {}
    virtual void LogFetch(LogFetchArgs args) {}
    virtual void LogRename(LogRenameArgs args) {}
    virtual void LogDispatch(LogDispatchArgs args) {}
    virtual void LogDispatchLoad(LogDispatchLoadArgs args) {}
    virtual void LogDispatchStore(LogDispatchStoreArgs args) {}
    virtual void LogIssue(LogIssueArgs args) {}
    virtual void LogRegRead(LogRegReadArgs args) {}
    virtual void LogMemExecute(LogMemExecuteArgs args) {}
    virtual void LogLoadExecute(LogLoadExecuteArgs args) {}
    virtual void LogWriteBack(LogWriteBackArgs args) {}
    virtual void LogRetire(LogRetireArgs args) {}
    virtual void LogControlCommit(LogControlCommitArgs args) {}
    virtual void LogStoreCommit(LogStoreCommitArgs args) {}
    virtual void LogPostStoreCommit(LogStoreCommitArgs args) {}
    virtual void LogFreedReg(LogFreedRegArgs args) {}
    virtual void LogFlush(LogFlushArgs args) {}
    virtual void LogException(LogExceptionArgs args) {
        // by default, redirect to LogFlush
        LogFlushArgs log_args; log_args.clk = args.clk; LogFlush(log_args);
    }
    virtual void WarmUpEnds(WarmUpEndsArgs args) {}
};

#endif /* _SIM_LISTENER_H_ */

/* end of file */
