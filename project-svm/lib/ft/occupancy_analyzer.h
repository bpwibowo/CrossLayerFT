/**********************************************************
 *
 * Occupancy analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 06/02/2014
 **********************************************************/

#ifndef _OCCUPANCY_ANALYZER_H_
#define _OCCUPANCY_ANALYZER_H_

#include "simlog.h"
#include "sim_listener.h"
#include "ft_analyzer.h"

class OccupancyAnalyzer : public SimListener, public FtAnalyzer {
  public:
    OccupancyAnalyzer(SimLog *simlog, std::string base_output_path);
    ~OccupancyAnalyzer();

    void LogClockTick(LogClockTickArgs args);
    void LogFetch(LogFetchArgs args);
    void LogRename(LogRenameArgs args);
    void LogDispatch(LogDispatchArgs args);
    void LogDispatchLoad(LogDispatchLoadArgs args);
    void LogDispatchStore(LogDispatchStoreArgs args);
    void LogIssue(LogIssueArgs args);
    void LogRetire(LogRetireArgs args);
    void LogStoreCommit(LogStoreCommitArgs args);
    void LogFlush(LogFlushArgs args);
    bool Analyze(uint64_t max_clock);

  private:
    struct VulnerabilityFactor occupancies_;
    struct VulnerabilityFactor occupancies_delayed_;
    struct VulnerabilityFactor occupancies_regread_;
    struct VulnerabilityFactor occupancies_execute_;
    std::ofstream outfile_cycle_;
    std::map<uint64_t, uint64_t> iq_map_;
    std::map<uint64_t, uint64_t> rob_map_;
    std::map<uint64_t, uint64_t> lq_map_;
    std::map<uint64_t, uint64_t> sq_map_;
    short occupancy_sq_nonspec_;
};

#endif /* _OCCUPANCY_ANALYZER_H_ */

/* end of file */
