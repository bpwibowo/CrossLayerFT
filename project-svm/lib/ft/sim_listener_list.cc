/**********************************************************
 *
 * Lisf of simulation event listener implementation code
 *
 * First author: Bagus Wibowo
 * created: 05/29/2014
 **********************************************************/

#include <inttypes.h>
#include "sim_listener_list.h"

SimListenerList::SimListenerList() {
    listener_list_.clear();
}

SimListenerList::~SimListenerList() {

}

void SimListenerList::LogClockTick(LogClockTickArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogClockTick(args);
    }
}

void SimListenerList::LogFetch(LogFetchArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogFetch(args);
    }
}

void SimListenerList::LogRename(LogRenameArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogRename(args);
    }
}

void SimListenerList::LogDispatch(LogDispatchArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogDispatch(args);
    }
}

void SimListenerList::LogDispatchLoad(LogDispatchLoadArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogDispatchLoad(args);
    }
}

void SimListenerList::LogDispatchStore(LogDispatchStoreArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogDispatchStore(args);
    }
}

void SimListenerList::LogIssue(LogIssueArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogIssue(args);
    }
}

void SimListenerList::LogRegRead(LogRegReadArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogRegRead(args);
    }
}

void SimListenerList::LogMemExecute(LogMemExecuteArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogMemExecute(args);
    }
}

void SimListenerList::LogLoadExecute(LogLoadExecuteArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogLoadExecute(args);
    }
}

void SimListenerList::LogWriteBack(LogWriteBackArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogWriteBack(args);
    }
}

void SimListenerList::LogRetire(LogRetireArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogRetire(args);
    }
}

void SimListenerList::LogControlCommit(LogControlCommitArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogControlCommit(args);
    }
}

void SimListenerList::LogStoreCommit(LogStoreCommitArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogStoreCommit(args);
    }
    // resend the same arguments to LogPostStoreCommit
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogPostStoreCommit(args);
    }
}

void SimListenerList::LogFreedReg(LogFreedRegArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogFreedReg(args);
    }
}

void SimListenerList::LogFlush(LogFlushArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogFlush(args);
    }
}

void SimListenerList::LogException(LogExceptionArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->LogException(args);
    }
}

void SimListenerList::WarmUpEnds(WarmUpEndsArgs args) {
    for (std::vector<SimListener*>::iterator it = listener_list_.begin();
        it != listener_list_.end();
        it++) {
        (*it)->WarmUpEnds(args);
    }
}

void SimListenerList::AddListener(SimListener* new_listener) {
    // make sure the new_listener has not been added before
    std::vector<SimListener*>::iterator it = listener_list_.begin();

    while (it != listener_list_.end()) {
        if (*it == new_listener) {
            // the listener has been added before. Don't add again.
            return;
        }
        it++;
    }

    // add the new_listener into the list
    listener_list_.push_back(new_listener);
}

/* end of file */
