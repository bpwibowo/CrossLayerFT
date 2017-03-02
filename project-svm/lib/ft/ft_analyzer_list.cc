/**********************************************************
 *
 * Lisf of FT analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 05/29/2014
 **********************************************************/

#include <inttypes.h>
#include <iostream>
#include <fstream>
#include "ft_analyzer_list.h"

#define SUMMARY_LOG_FILE FULL_PATH("results/svm_results.yaml")

FtAnalyzerList::FtAnalyzerList(std::string base_output_path) :
    FtAnalyzer(NULL, "") {

    base_output_path_ = base_output_path;
    analyzer_list_.clear();

    // open outfile_summary_, base_output_path_ must be set before
    outfile_summary_.open(SUMMARY_LOG_FILE, std::ios::out | std::ios::trunc);
}

FtAnalyzerList::~FtAnalyzerList() {

}

bool FtAnalyzerList::Analyze(uint64_t max_clock) {
    for (std::vector<FtAnalyzer*>::iterator it = analyzer_list_.begin();
        it != analyzer_list_.end();
        it++) {
        if ((*it)->Analyze(max_clock) == false)
            return false;
    }

    // finally, if the summary file was open, close it
    if (outfile_summary_.is_open())
        outfile_summary_.close();

    return true;
}

void FtAnalyzerList::AddAnalyzer(FtAnalyzer* new_analyzer) {
    // make sure the new_analyzer has not been added before
    std::vector<FtAnalyzer*>::iterator it = analyzer_list_.begin();

    while (it != analyzer_list_.end()) {
        if (*it == new_analyzer) {
            // the analyzer has been added before. Don't add again.
            return;
        }
        it++;
    }

    // add the new_analyzer into the list
    analyzer_list_.push_back(new_analyzer);

    // set the summary file object for that analyzer
    new_analyzer->SetSummaryFileObject(&outfile_summary_);
}

/* end of file */
