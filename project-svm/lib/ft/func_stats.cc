/**********************************************************
 *
 * Function statistics implementation code
 *
 * First author: Abhinav Agrawal
 * created: 08/20/2014
 **********************************************************/


#include <iostream>
#include "func_stats.h"

#define FUNC_CALLS_FILE FULL_PATH("results/rtl_func_calls.log")

FuncStats::FuncStats(SimLog *simlog, std::string base_output_path) :
  FtAnalyzer(simlog, base_output_path) {

    enabled_ = false;
    mr_ = NULL;
    inst_count_ = 0;
}

FuncStats::~FuncStats() {
    if (func_calls_file.is_open()) func_calls_file.close();
}

void FuncStats::SetMetadata(MReader* mr) {
    mr_ = mr;
    enabled_ = true; // FuncStats analyzer is enabled by setting its
                     // metadata reader object

    // open files for writing
    func_calls_file.open(FUNC_CALLS_FILE, std::ios::out | std::ios::trunc);
}

void FuncStats::LogRetire(LogRetireArgs args) {
    inst_count_++;
    if (mr_ != NULL) {
        uint64_t pc = simlog_->logdb_[args.seq_no].pc;
        struct Metadata md = mr_->GetMetadata(pc);
        if (md.cf_taken.is_first_instruction_in_function) {
            if ( (func_call_count).count(pc))
                ( (func_call_count)[pc] )++;
            else
                (func_call_count)[pc] = 1;
            func_calls_file << "clk: " << std::dec << args.clk << " F: " <<
                " " << std::hex << pc << std::dec <<
                " CC: " <<(func_call_count)[pc] <<
                " IC: "  << (inst_count_) << " " <<  md.func_name << std::endl;
       }
    }
}

bool FuncStats::Analyze(uint64_t max_clock) {
    return true;
}

/* end of file */
