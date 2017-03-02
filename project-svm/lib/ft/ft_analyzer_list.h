/**********************************************************
 *
 * Lisf of FT analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 05/29/2014
 **********************************************************/

#ifndef _FT_ANALYZER_LIST_H_
#define _FT_ANALYZER_LIST_H_

#include <vector>
#include "ft_analyzer.h"

class FtAnalyzerList : public FtAnalyzer {
  public:
    FtAnalyzerList(std::string base_output_path);
    ~FtAnalyzerList();

    bool Analyze(uint64_t max_clock);
    void AddAnalyzer(FtAnalyzer* new_analyzer);

  private:
    std::vector<FtAnalyzer*> analyzer_list_;
    std::ofstream outfile_summary_;
};

#endif /* _FT_ANALYZER_LIST_H_ */

/* end of file */
