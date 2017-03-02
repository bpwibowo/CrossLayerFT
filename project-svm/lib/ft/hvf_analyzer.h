#ifndef _HVF_ANALYZER_H_
#define _HVF_ANALYZER_H_

// *****************************************************************************
// AVF Statistical Aproximation
// NCSU ECE Department
// Cross Layer Reliability Project
//
// Module : HVF Analyzer Header
// This Progam was developed to examine the ability to calulate AVF using
// Vilas Sriharan and David R. Kaeli method for statistical Calulation of AVF
// from their paper titled " Using Hardware Vulnerability Factors to Enhance
// AVF analysis" dated June 2010
//
// Author Thomas P. Stanton
// Date: 17 July 2014
// Revision 1.2
// Rev Date 28 July 2014
//
// Revision History
// 1.0 Initial Header file 17 July 2014
// 1.1 cleaned up and added counters to monitor the reorder buffer
// 1.2 Cleaned up code made more compatible with hvf_analyzer.h

#include "simlog.h"
#include "sim_listener.h"
#include "ft_analyzer.h"
#include "pvf_analyzer.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define INVALID_SEQ_NO ((uint64_t)-1)
#define FULL (7)

struct Queue{
    uint64_t seq_no; //seq_no of instruction
    bool valid; //is seq_no valid flag
    bool StoreCommit; //Has the store instruction been committed to store
    bool monitored; // is this a monitored instruction
};
struct Buffer{
    uint64_t seq_no; //Sequence number of the instruction
    bool IQFlag;  //Issue Queue event monitored status
    bool LoadFlag; //Load Queue event monitored status
    bool ROBFlag;
};

class HvfAnalyzer : public SimListener, public FtAnalyzer {
  public:
    HvfAnalyzer(SimLog *simlog, std::string base_output_path,
                PvfAnalyzer *pvf_analyzer);
    ~HvfAnalyzer();

    void LogClockTick(LogClockTickArgs args);
    void LogDispatch(LogDispatchArgs args);
    void LogDispatchLoad(LogDispatchLoadArgs args);
    void LogDispatchStore(LogDispatchStoreArgs args);
    void LogIssue(LogIssueArgs args);
    void LogLoadExecute(LogLoadExecuteArgs args);
    void LogRetire(LogRetireArgs args);
    void LogFlush(LogFlushArgs args);
    void LogStoreCommit(LogStoreCommitArgs args);
    void WarmUpEnds(WarmUpEndsArgs args);
    bool Analyze(uint64_t max_clock);

private:
    int RandomNumber(int NumberOfStructures);
    void QueueClear(std::vector<struct Queue> &pqueuue, bool all,
                    uint64_t item);
    void BufferClear(std::vector<struct Buffer> &pbuffer, int start, int stop);
    void SetMonitor();
    uint32_t IQMonitorElement;
    uint32_t ROBMonitor;
    uint32_t ROBHead;
    uint32_t ROBTail;
    int IqHVFcounter;
    int ROBHVFcounter;
    int StoreHVFcounter;
    int LoadHVFcounter;
    int ROBHitCount;
    int ROBHit512;
    uint32_t ROBSize;
    uint64_t ElementClear;
    uint64_t StoreQMonitor;
    uint64_t LoadQMonitor;
    uint64_t start;
    uint64_t stop;
    double IqPvsScore;
    double ROBPvsScore;
    double StoreQPvsScore;
    double LoadQPvsScore;
    double AVFIQ;
    double AVFROB;
    double AVFStoreQueue;
    double AVFLoadQueue;
    double IqPvs_x_HvfL;
    double ROBPvs_x_HVF;
    double StorePvs_x_HVF;
    double LoadPvs_x_HVF;
    double LogDispatchCount;
    double vbit_count;
    double entry_bit_count;
    double derating;
    double ROBvbits;
    double LQvbits;
    double SQvbits;
    bool EraseALL;
    std::vector<struct Queue> StoreQ;
    std::vector<struct Queue> Issue;
    std::vector<struct Queue> Load;
    std::vector<struct Buffer> ReOrder;
    std::ofstream outfile_HvfResults;
    PvfAnalyzer *pvf_analyzer_;
};

#endif /* _HVF_ANALYZER_H_ */

/* end of file */
