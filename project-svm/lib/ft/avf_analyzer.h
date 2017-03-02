/**********************************************************
 *
 * AVF analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#ifndef _AVF_ANALYZER_H_
#define _AVF_ANALYZER_H_

#include "ft_analyzer.h"
#include "simlog_nonspec.h"

class AvfAnalyzer : public FtAnalyzer {
  public:
    AvfAnalyzer(SimLog *simlog, std::string base_output_path);
    ~AvfAnalyzer();

    bool Analyze(uint64_t max_clock);

  private:
    bool PostMortemAnalysis(uint64_t simulation_size);
    void PostMortemAvfCompulsoryAnalysis(uint64_t simulation_size);
    void AddLiveRegister(int32_t opcode, uint64_t begin, uint64_t end);
    void AddLiveRegister(int32_t opcode, uint64_t begin, uint64_t begin_pc,
                         uint64_t end, uint64_t end_pc, int64_t arch_reg);
    bool IsInstructionCompleted(struct InstructionDetails inst);
    void LogAvfs(uint64_t simulation_size, const char *out_file,
                 struct TotalVulnerableBits<uint64_t> *total);
    void LogRegUse(uint64_t simulation_size);
    void LogInfo(uint64_t simulation_size);

    std::vector<struct VulnerabilityFactor> avfdb_;
    std::vector< std::vector<uint8_t> > avfdb_rf_reg_;
    std::vector<bool> liveindb_;
    std::ofstream outfile_reg_life_;
    struct TotalVulnerableBits<uint64_t> total_vbits_;
    struct TotalVulnerableBits<uint64_t> compulsory_vbits_;
};

#endif /* _AVF_ANALYZER_H_ */

 /* end of file */
