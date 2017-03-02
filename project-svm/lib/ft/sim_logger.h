/**********************************************************
 *
 * SimLog logger header code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#ifndef _SIM_LOGGER_H
#define _SIM_LOGGER_H

#include "ft_analyzer.h"

class SimLogger : public FtAnalyzer {
  public:
    SimLogger(SimLog *simlog, std::string base_output_path);
    ~SimLogger();

    bool Analyze(uint64_t max_clock);

  private:
    void LogInstruction(uint64_t seq_no);
    void LogAllInstructions();

    std::ofstream logfile_;
    uint64_t start_time_;
};

#endif /* _SIM_LOGGER_H */

/* end of file */
