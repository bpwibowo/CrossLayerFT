/**********************************************************
 *
 * Speculative - Non-speculative analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 07/29/2014
 **********************************************************/

#include <iostream>
#include "spec_analyzer.h"

#define SPEC_CYCLE_LOG_FILE FULL_PATH("results/spec_cnt_cycle.log")
#define NONSPEC_CYCLE_LOG_FILE FULL_PATH("results/nonspec_cnt_cycle.log")
#define SPEC_NONSPEC_RATIO_GROUP_LOG_FILE \
    FULL_PATH("results/spec_nonspec_ratio_cnt_group.log")

SpecAnalyzer::SpecAnalyzer(SimLogNonSpec *simlog, std::string base_output_path)
    : FtAnalyzer(simlog, base_output_path) {

    retire_cnt_.clear();

    // update the pointer to SimLogNonSpec object
    simlog_ = simlog;

    // open files for writing
    spec_outfile_cycle_.open(SPEC_CYCLE_LOG_FILE,
                             std::ios::out | std::ios::trunc);
    nonspec_outfile_cycle_.open(NONSPEC_CYCLE_LOG_FILE,
                                std::ios::out | std::ios::trunc);
}

SpecAnalyzer::~SpecAnalyzer() {
    if (spec_outfile_cycle_.is_open()) spec_outfile_cycle_.close();
    if (nonspec_outfile_cycle_.is_open()) nonspec_outfile_cycle_.close();
}

void SpecAnalyzer::LogClockTick(LogClockTickArgs args) {
    // log number of speculative instructions flushed in this clock
    struct InstructionCount<int> flushed_cnt =
        simlog_->GetFlushedInstructionsCount();
    if (spec_outfile_cycle_.is_open()) {
        spec_outfile_cycle_ << flushed_cnt.iq;
        spec_outfile_cycle_ << "\t" << flushed_cnt.rob;
        spec_outfile_cycle_ << "\t" << flushed_cnt.lq;
        spec_outfile_cycle_ << "\t" << flushed_cnt.sq;
        spec_outfile_cycle_ << std::endl;
    }

    // log number of non-speculative instructions retired in this clock
    if (nonspec_outfile_cycle_.is_open()) {
        nonspec_outfile_cycle_ << retire_cnt_.iq;
        nonspec_outfile_cycle_ << "\t" << retire_cnt_.rob;
        nonspec_outfile_cycle_ << "\t" << retire_cnt_.lq;
        nonspec_outfile_cycle_ << "\t" << retire_cnt_.sq;
        nonspec_outfile_cycle_ << std::endl;
    }

    retire_cnt_.clear(); // clear retire count
}

void SpecAnalyzer::LogRetire(LogRetireArgs args) {
    // an instruction retires, increment retire count
    retire_cnt_.iq++;
    retire_cnt_.rob++;

    // handle LQ case
    if (simlog_->logdb_[args.seq_no].load_id != INVALID_LOAD_ID) {
        // the instruction is of type load
        retire_cnt_.lq++;
    }

    // handle SQ case
    if (simlog_->logdb_[args.seq_no].store_id != INVALID_STORE_ID) {
        // the instruction is of type store
        retire_cnt_.sq++;
    }
}

bool SpecAnalyzer::Analyze(uint64_t max_clock) {
    std::ifstream spec_infile_cycle, nonspec_infile_cycle;
    std::ofstream outfile_group;
    uint64_t simulation_size = max_clock+1;
    uint64_t i;
    uint64_t group_size = simulation_size/POINTS_IN_GRAPH;
    struct InstructionCount<int64_t> total_s_cnt, total_ns_cnt;
    struct InstructionCount<int64_t> group_s_cnt, group_ns_cnt;

    std::cout <<
        "[spec_analyzer] INFO: Starting speculative-nonspeculative analysis..."
        << std::flush;

    // clear total and group counters
    total_ns_cnt.clear();
    total_s_cnt.clear();
    group_ns_cnt.clear();
    group_s_cnt.clear();

    // close the outfiles
    if (spec_outfile_cycle_.is_open()) spec_outfile_cycle_.close();
    if (nonspec_outfile_cycle_.is_open()) nonspec_outfile_cycle_.close();

    // reopen them in reading mode
    spec_infile_cycle.open(SPEC_CYCLE_LOG_FILE);
    nonspec_infile_cycle.open(NONSPEC_CYCLE_LOG_FILE);
    outfile_group.open(SPEC_NONSPEC_RATIO_GROUP_LOG_FILE,
                       std::ios::out | std::ios::trunc);

    // log header
    outfile_group << "iq";
    outfile_group << '\t' << "rob";
    outfile_group << '\t' << "lq";
    outfile_group << '\t' << "sq";
    outfile_group << std::endl;

    // analyze the inputs
    for (i=0; i<simulation_size; i++) {
        struct InstructionCount<int64_t> s_value, ns_value;

        // get the values from files
        spec_infile_cycle >> s_value.iq >> s_value.rob >>
            s_value.lq >> s_value.sq;
        nonspec_infile_cycle >> ns_value.iq >> ns_value.rob >>
            ns_value.lq >> ns_value.sq;

        // accumulate total non-speculative and speculative instructions counter
        total_ns_cnt.add(ns_value);
        total_s_cnt.add(s_value);

        // accumulate grouped total non-speculative and speculative
        // instructions counter
        group_ns_cnt.add(ns_value);
        group_s_cnt.add(s_value);

        // if the samples for one group is complete
        if ((i%group_size) == (group_size-1)) {
            // get the ratio
            InstructionCount<double> ratio =
                GetRatio(group_s_cnt, group_ns_cnt);

            // log the ratio
            outfile_group << ratio.iq;
            outfile_group << '\t' << ratio.rob;
            outfile_group << '\t' << ratio.lq;
            outfile_group << '\t' << ratio.sq;
            outfile_group << std::endl;

            // clear group counters
            group_ns_cnt.clear();
            group_s_cnt.clear();
        }
    }

    uint64_t samples_left = i%group_size;
    if (samples_left) {
        // get the ratio
        InstructionCount<double> ratio = GetRatio(group_s_cnt, group_ns_cnt);

        // log the ratio
        outfile_group << ratio.iq;
        outfile_group << '\t' << ratio.rob;
        outfile_group << '\t' << ratio.lq;
        outfile_group << '\t' << ratio.sq;
        outfile_group << std::endl;
    }

    // close files
    spec_infile_cycle.close();
    nonspec_infile_cycle.close();
    outfile_group.close();

    // log summary information
    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ << "Speculative-Nonspeculative ratio:" << std::endl;

        InstructionCount<double> ratio = GetRatio(total_s_cnt, total_ns_cnt);
        *p_outfile_summary_ <<
            "  total_spec_iq = " << total_s_cnt.iq << std::endl <<
            "  total_nonspec_iq = " << total_ns_cnt.iq << std::endl <<
            "  spec_nonspec_ratio_iq = " << ratio.iq << std::endl <<
            "  total_spec_rob = " << total_s_cnt.rob << std::endl <<
            "  total_nonspec_rob = " << total_ns_cnt.rob << std::endl <<
            "  spec_nonspec_ratio_rob = " << ratio.rob << std::endl <<
            "  total_spec_lq = " << total_s_cnt.lq << std::endl <<
            "  total_nonspec_lq = " << total_ns_cnt.lq << std::endl <<
            "  spec_nonspec_ratio_lq = " << ratio.lq << std::endl <<
            "  total_spec_sq = " << total_s_cnt.sq << std::endl <<
            "  total_nonspec_sq = " << total_ns_cnt.sq << std::endl <<
            "  spec_nonspec_ratio_sq = " << ratio.sq << std::endl;
    }

    std::cout << " done" << std::endl;

    return true;
}

/* end of file */
