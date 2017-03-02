/**********************************************************
 *
 * PVS Profiler header file
 *
 * First author: Bagus Wibowo
 * created: 01/18/2016
 **********************************************************/

#ifndef _PVS_PROFILER_H_
#define _PVS_PROFILER_H_

#include "mipsarch.h"
#include "mreader.h"

typedef struct {
    uint64_t pc;
    unsigned int num_parents;
    uint64_t parents[MAX_RSRC+1]; // +1 for corresponding store instruction
                                  // in case of load.
    bool important;
} inst_window_t;

typedef struct {
    uint64_t important_cnt;
    uint64_t instance_cnt;
} profile_t;

class PvsProfiler {
  public:
    // methods
    PvsProfiler();
    void SetMetadata(const char *fname);
    void SetOutFile(const char *outfile);
    void AppendInst(arch_inst_t *inst);
    void Finalize();

    // properties
  private:
    uint64_t seq_no_;
    std::string outfile_;
    bool warned_no_md_;
    MReader *mr_;
    std::map<uint64_t, inst_window_t> inst_window_;
    std::map<uint64_t, profile_t> inst_profile_;
    std::map<uint64_t, profile_t> fn_profile_;
    std::map<int32_t,uint64_t> arch_reg_defs_;
    uint64_t last_v0_value_;
    std::map<uint64_t, std::deque<uint64_t> > sd_callee_;
};

extern PvsProfiler pvs_profiler;

#endif /* _PVS_PROFILER_H_ */
