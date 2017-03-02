/**********************************************************
 *
 * PVF analyzer header code
 *
 * First author: Bagus Wibowo
 * created: 10/16/2014
 **********************************************************/

#ifndef _PVF_ANALYZER_H_
#define _PVF_ANALYZER_H_

#include "ft_analyzer.h"
#include "simlog_nonspec.h"
#include "mreader.h"

struct FunctionPvs {
    uint64_t cnt;
    uint64_t sum;

    FunctionPvs() { cnt = 0; sum = 0; }
    void Include(uint64_t val) { cnt++; sum+=val; }
    double Value() { return (double)sum/cnt; }
};

enum StructureType {
    ST_IQ,
    ST_ROB,
    ST_LQ,
    ST_SQ,
    // ST_RF,
    ST_COUNT // count how many structures
};

class HvfEventElement {
  public:
    HvfEventElement() {seq_nos_.clear(); deratings_.clear();}
    std::vector<uint64_t> seq_nos_;
    std::vector<float> deratings_;
};

class HvfEvents {
  public:
    HvfEvents() {structures_.clear(); structures_.resize(ST_COUNT);}
    std::vector<HvfEventElement> structures_;
};

class PvfAnalyzer : public SimListener, public FtAnalyzer {
  public:
    PvfAnalyzer(SimLog *simlog, std::string base_output_path,
                std::string pvs_profile_path);
    ~PvfAnalyzer();

    void LogClockTick(LogClockTickArgs args);
    void WarmUpEnds(WarmUpEndsArgs args);
    void SetMetadata(MReader* mr);
    bool Analyze(uint64_t max_clock);
    void HvfSingleEvent(uint64_t seq_no, enum StructureType structure,
                        float derating);
    void HvfOverflowEvent(uint64_t seq_no, enum StructureType structure,
                          float derating);

  private:
    void LoadPvsProfile();

    bool enabled_;
    MReader *mr_;
    std::map<uint64_t, struct FunctionPvs> func_pvs_;
    std::map<uint64_t, struct FunctionPvs> func_sif_;// static importance factor
    std::map<uint64_t, double> profile_func_pvs_;
    HvfEvents *p_hvf_event_;
    std::vector<HvfEvents> hvf_events_;
    std::string pvs_profile_path_;
};

#endif /* _PVF_ANALYZER_H_ */

 /* end of file */
