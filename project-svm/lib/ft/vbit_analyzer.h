/**********************************************************
 *
 * Vulnerable bits analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 11/12/2014
 **********************************************************/

#ifndef _VBIT_ANALYZER_H_
#define _VBIT_ANALYZER_H_

#include <iostream>
#include "ft_analyzer.h"
#include "vbit_table.h"

class VbitAnalyzer : public FtAnalyzer {
  public:
    VbitAnalyzer(SimLog *simlog, std::string base_output_path);
    ~VbitAnalyzer();

    bool Analyze(uint64_t max_clock);

  private:
    std::ofstream summary_file_;

    void LogVbitHeader(structure_t structure);
    void LogVbit(structure_t structure, int opcode, std::string name);
};

#endif /* _VBIT_ANALYZER_H_ */

 /* end of file */
