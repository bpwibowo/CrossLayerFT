/**********************************************************
 *
 * Simulation log non-speculative header code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#ifndef _SIMLOG_NONSPEC_H_
#define _SIMLOG_NONSPEC_H_

#include "simlog.h"
#include "ft_analyzer.h"

class SimLogNonSpec : public SimLog {
  public:
    void LogClockTick(LogClockTickArgs args);
    void LogFlush(LogFlushArgs args);
    struct InstructionCount<int> GetFlushedInstructionsCount();

  private:
    struct InstructionCount<int> flushed_cnt_;
    struct InstructionCount<int> flushed_cnt_saved_;
};

#endif /* _SIMLOG_NONSPEC_H_ */

 /* end of file */
