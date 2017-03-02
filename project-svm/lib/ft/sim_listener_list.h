/**********************************************************
 *
 * Lisf of simulation event listener header code
 *
 * First author: Bagus Wibowo
 * created: 05/29/2014
 **********************************************************/

#ifndef _SIM_LISTENER_LIST_H_
#define _SIM_LISTENER_LIST_H_

#include <vector>
#include "sim_listener.h"

class SimListenerList : public SimListener {
  public:
    SimListenerList();
    ~SimListenerList();

    void LogClockTick(LogClockTickArgs args);
    void LogFetch(LogFetchArgs args);
    void LogRename(LogRenameArgs args);
    void LogDispatch(LogDispatchArgs args);
    void LogDispatchLoad(LogDispatchLoadArgs args);
    void LogDispatchStore(LogDispatchStoreArgs args);
    void LogIssue(LogIssueArgs args);
    void LogRegRead(LogRegReadArgs args);
    void LogMemExecute(LogMemExecuteArgs args);
    void LogLoadExecute(LogLoadExecuteArgs args);
    void LogWriteBack(LogWriteBackArgs args);
    void LogRetire(LogRetireArgs args);
    void LogControlCommit(LogControlCommitArgs args);
    void LogStoreCommit(LogStoreCommitArgs args);
    void LogFreedReg(LogFreedRegArgs args);
    void LogFlush(LogFlushArgs args);
    void LogException(LogExceptionArgs args);
    void WarmUpEnds(WarmUpEndsArgs args);

    void AddListener(SimListener* new_listener);

  private:
    std::vector<SimListener*> listener_list_;
};

#endif /* _SIM_LISTENER_LIST_H_ */

/* end of file */
