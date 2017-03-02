/**********************************************************
 *
 * Speculative - Non-speculative analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 07/29/2014
 **********************************************************/

#ifndef _SPEC_ANALYZER_H_
#define _SPEC_ANALYZER_H_

#include "ft_analyzer.h"
#include "sim_listener.h"
#include "simlog_nonspec.h"

class SpecAnalyzer : public SimListener, public FtAnalyzer {
  public:
    SpecAnalyzer(SimLogNonSpec *simlog, std::string base_output_path);
    ~SpecAnalyzer();

    void LogClockTick(LogClockTickArgs args);
    void LogRetire(LogRetireArgs args);
    bool Analyze(uint64_t max_clock);

  private:
    SimLogNonSpec *simlog_;
    struct InstructionCount<int> retire_cnt_;
    std::ofstream spec_outfile_cycle_;
    std::ofstream nonspec_outfile_cycle_;
};

#endif /* _SPEC_ANALYZER_H_ */
