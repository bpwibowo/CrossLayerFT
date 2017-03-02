/**********************************************************
 *
 * Function statistics header code
 *
 * First author: Abhinav Agrawal
 * created: 08/20/2014
 **********************************************************/

#ifndef _FUNC_STATS_H_
#define _FUNC_STATS_H_

#include "simlog.h"
#include "sim_listener.h"
#include "ft_analyzer.h"
#include "mreader.h"

class FuncStats : public SimListener, public FtAnalyzer {
  public:
    FuncStats(SimLog *simlog, std::string base_output_path);
    ~FuncStats();

    void SetMetadata(MReader* mr);
    void LogRetire(LogRetireArgs args);
    bool Analyze(uint64_t max_clock);

  private:
    MReader *mr_;
    bool enabled_;
    std::map<uint64_t, uint64_t> func_call_count;
    std::ofstream func_calls_file;
    uint64_t inst_count_;
};

#endif /* _FUNC_STATS_H_ */

/* end of file */
