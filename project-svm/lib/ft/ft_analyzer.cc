/**********************************************************
 *
 * Fault-Tolerance base analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#include <iomanip>
#include <iostream>
#include <sstream>
#include "svm.h"
#include "ft_analyzer.h"
#include "vbit_table.h"

FtAnalyzer::FtAnalyzer(SimLog *simlog, std::string base_output_path) {
    simlog_ = simlog;
    base_output_path_ = base_output_path;
    zero_vf_on_protection_ = false;
}

FtAnalyzer::~FtAnalyzer() {
}

void FtAnalyzer::SetSummaryFileObject(std::ofstream *outfile_summary) {
    p_outfile_summary_ = outfile_summary;
}

void FtAnalyzer::AnalyzeGrouped(uint64_t simulation_size,
  const char *in_file, const char *out_file) {
    int group_size = simulation_size/POINTS_IN_GRAPH;
    AnalyzeGrouped(simulation_size, group_size, in_file, out_file, NULL);
}

void FtAnalyzer::AnalyzeGrouped(uint64_t simulation_size,
  uint64_t group_size, const char *in_file, const char *out_file) {
    AnalyzeGrouped(simulation_size, group_size, in_file, out_file, NULL);
}

void FtAnalyzer::AnalyzeGrouped(uint64_t simulation_size,
  const char *in_file, const char *out_file,
  struct TotalVulnerableBits<uint64_t> *out_total) {
    int group_size = simulation_size/POINTS_IN_GRAPH;
    AnalyzeGrouped(simulation_size, group_size, in_file, out_file, out_total);
}

void FtAnalyzer::AnalyzeGrouped(uint64_t simulation_size,
  uint64_t group_size, const char *in_file, const char *out_file,
  struct TotalVulnerableBits<uint64_t> *out_total) {
    std::ifstream infile_cycle;
    std::ofstream outfile_group;
    int64_t iq, rob, lq, sq, rf, ibuff, sys;
    int64_t fu_simple, fu_complex, fu_control, fu_memory;
    uint64_t i;
    int64_t sys_value;
    struct VulnerabilityFactor value;
    bool currently_protected = false;
    bool next_protected_status = false;
    int64_t next_switch_cycle = simulation_size;
    std::vector<struct ProtectionHistoryEntry>::iterator prot_it;

    // if out_total is provided, clear it
    if (out_total)
        out_total->clear();

    // open the files
    infile_cycle.open(in_file);
    outfile_group.open(out_file, std::ios::out | std::ios::trunc);

    outfile_group << "IQ\tROB\tLQ\tSQ\tRF\tInstBuff\tSys\tFUsimp\t" <<
        "FUcomp\tFUctrl\tFUmem" << std::endl;

    iq = rob = lq = sq = rf = ibuff = sys = 0;
    fu_simple = fu_complex = fu_control = fu_memory = 0;

    int size_iq = GetTotalBitsCount(STRUCTURE_IQ);
    int size_rob = GetTotalBitsCount(STRUCTURE_ROB);
    int size_lq = GetTotalBitsCount(STRUCTURE_LQ);
    int size_sq = GetTotalBitsCount(STRUCTURE_SQ);
    int size_rf = GetTotalBitsCount(STRUCTURE_RF);
    int size_ibuff = GetTotalBitsCount(STRUCTURE_IBUFF);
    int size_sys = size_iq + size_rob + size_lq + size_sq + size_rf +
                   size_ibuff;

    if (zero_vf_on_protection_) {
        prot_it = SVM->prot_hist_.begin();
        next_switch_cycle = 0;
        next_protected_status = false;
    }

    // analyze the input
    for (i=0; i<simulation_size; i++) {
        // handle dynamic protection
        if (zero_vf_on_protection_ && ((int64_t)i) >= next_switch_cycle) {
            next_switch_cycle = simulation_size; // which means no more switching, by default
            currently_protected = next_protected_status;
            while (prot_it != SVM->prot_hist_.end()) {
                if (prot_it->status != currently_protected) {
                    next_switch_cycle = prot_it->clk - SVM->cfg().end_of_warm_up;
                    next_protected_status = prot_it->status;
                    prot_it++;
                    break;
                }
                prot_it++;
            }
        }

        // now combine values from current clock cycles
        sys_value = 0;

        infile_cycle >> value.iq;
        infile_cycle >> value.rob;
        infile_cycle >> value.lq;
        infile_cycle >> value.sq;
        infile_cycle >> value.rf;
        infile_cycle >> value.ibuff;
        infile_cycle >> value.fu_simple;
        infile_cycle >> value.fu_complex;
        infile_cycle >> value.fu_control;
        infile_cycle >> value.fu_memory;

        if (!currently_protected) {
            // memory based structures
            iq += value.iq;
            sys_value += value.iq;
            rob += value.rob;
            sys_value += value.rob;
            lq += value.lq;
            sys_value += value.lq;
            sq += value.sq;
            sys_value += value.sq;
            rf += value.rf;
            sys_value += value.rf;
            ibuff += value.ibuff;
            sys_value += value.ibuff;
            sys += sys_value;

            // functional units
            fu_simple += value.fu_simple;
            fu_complex += value.fu_complex;
            fu_control += value.fu_control;
            fu_memory += value.fu_memory;

            if (out_total)
                out_total->add(value);
        }

        if ((i%group_size) == (group_size-1)) {
            // dumping the mean values
            outfile_group << iq / (double)group_size / size_iq;
            outfile_group << '\t' << rob / (double)group_size / size_rob;
            outfile_group << '\t' << lq / (double)group_size / size_lq;
            outfile_group << '\t' << sq / (double)group_size / size_sq;
            outfile_group << '\t' << rf / (double)group_size / size_rf;
            outfile_group << '\t' << ibuff / (double)group_size / size_ibuff;
            outfile_group << '\t' << sys / (double)group_size / size_sys;
            outfile_group << '\t' << fu_simple / (double)group_size;
            outfile_group << '\t' << fu_complex / (double)group_size;
            outfile_group << '\t' << fu_control / (double)group_size;
            outfile_group << '\t' << fu_memory / (double)group_size;

            outfile_group << std::endl;
            iq = rob = lq = sq = rf = ibuff = sys = 0;
            fu_simple = fu_complex = fu_control = fu_memory = 0;
        }
    }

    // close the files
    infile_cycle.close();
    outfile_group.close();
}

void FtAnalyzer::AnalyzeGroupError(const char *in_file1, const char *in_file2,
    const char *out_file) {

    std::ifstream infile_group_1, infile_group_2;
    std::ofstream outfile_group;

    // open files
    infile_group_1.open(in_file1);
    infile_group_2.open(in_file2);
    outfile_group.open(out_file, std::ios::out | std::ios::trunc);

    // exclude the first line
    GetLineValueFromFile(&infile_group_1);
    GetLineValueFromFile(&infile_group_2);

    // write the header
    outfile_group << "iq";
    outfile_group << '\t' << "rob";
    outfile_group << '\t' << "lq";
    outfile_group << '\t' << "sq";
    outfile_group << '\t' << "rf";
    outfile_group << '\t' << "sys";
    outfile_group << std::endl;

    // for each lines, get absolute group error
    while (!infile_group_1.eof() && !infile_group_2.eof()) {
        struct TotalVulnerableBits<double> values1, values2, diffs;

        // get values
        values1 = GetLineValueFromFile(&infile_group_1);
        values2 = GetLineValueFromFile(&infile_group_2);

        // compute absolute difference
        diffs = values1.diff(values2);
        diffs.abs();

        // log the results
        outfile_group << diffs.iq;
        outfile_group << '\t' << diffs.rob;
        outfile_group << '\t' << diffs.lq;
        outfile_group << '\t' << diffs.sq;
        outfile_group << '\t' << diffs.rf;
        outfile_group << '\t' << diffs.sys;
        outfile_group << std::endl;
    }

    // close files
    infile_group_1.close();
    infile_group_2.close();
    outfile_group.close();
}

void FtAnalyzer::DumpSummaryTotalVBit(std::string id,
    TotalVulnerableBits<uint64_t> tvb) {

    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << id << ": " << std::endl
            << "    iq: " << tvb.iq << std::endl
            << "    rob: " << tvb.rob << std::endl
            << "    lq: " << tvb.lq << std::endl
            << "    sq: " << tvb.sq << std::endl
            << "    rf: " << tvb.rf << std::endl
            << "    ibuff: " << tvb.ibuff << std::endl
            << "    sys: " << tvb.sys << std::endl
            << "    fu_simple: " << tvb.fu_simple << std::endl
            << "    fu_complex: " << tvb.fu_complex << std::endl
            << "    fu_control: " << tvb.fu_control << std::endl
            << "    fu_memory: " << tvb.fu_memory << std::endl
            << std::endl;
    }
}

void FtAnalyzer::DumpSummaryTotalVBit(std::string id,
    TotalVulnerableBits<int64_t> tvb) {

    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << id << ": " << std::endl
            << "    iq: " << tvb.iq << std::endl
            << "    rob: " << tvb.rob << std::endl
            << "    lq: " << tvb.lq << std::endl
            << "    sq: " << tvb.sq << std::endl
            << "    rf: " << tvb.rf << std::endl
            << "    ibuff: " << tvb.ibuff << std::endl
            << "    sys: " << tvb.sys << std::endl
            << "    fu_simple: " << tvb.fu_simple << std::endl
            << "    fu_complex: " << tvb.fu_complex << std::endl
            << "    fu_control: " << tvb.fu_control << std::endl
            << "    fu_memory: " << tvb.fu_memory << std::endl
            << std::endl;
    }
}

void FtAnalyzer::DumpSummaryTotalVBit(std::string id,
    TotalVulnerableBits<double> tvb) {

    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << id << ": " << std::endl
            << "    iq: " << tvb.iq << std::endl
            << "    rob: " << tvb.rob << std::endl
            << "    lq: " << tvb.lq << std::endl
            << "    sq: " << tvb.sq << std::endl
            << "    rf: " << tvb.rf << std::endl
            << "    ibuff: " << tvb.ibuff << std::endl
            << "    sys: " << tvb.sys << std::endl
            << "    fu_simple: " << tvb.fu_simple << std::endl
            << "    fu_complex: " << tvb.fu_complex << std::endl
            << "    fu_control: " << tvb.fu_control << std::endl
            << "    fu_memory: " << tvb.fu_memory << std::endl
            << std::endl;
    }
}

struct InstructionCount<double> FtAnalyzer::GetRatio(
  struct InstructionCount<int64_t> numerator,
  struct InstructionCount<int64_t> denominator) {
    InstructionCount<double> ratio;
    ratio.iq =
        (denominator.iq != 0) ? (double)numerator.iq/denominator.iq : 0.0;
    ratio.rob =
        (denominator.rob != 0) ? (double)numerator.rob/denominator.rob : 0.0;
    ratio.lq =
        (denominator.lq != 0) ? (double)numerator.lq/denominator.lq : 0.0;
    ratio.sq =
        (denominator.sq != 0) ? (double)numerator.sq/denominator.sq : 0.0;
    return ratio;
}

bool FtAnalyzer::IsPhaseCrossing(uint64_t cycle, uint64_t phase_period) {
    if (SVM->cfg().enable_warm_up) {
        if (SVM->IsWarmUpPeriod()) {
            return false; // never cross phase in warm up period
        }
        else {
            cycle -= (SVM->cfg().end_of_warm_up-1); // offset the warmup time
        }
    }
    return (cycle % phase_period) == 0;
}

/* ---------------------- private function definitions ---------------------- */

struct TotalVulnerableBits<double> FtAnalyzer::GetLineValueFromFile(
    std::ifstream *input_file) {

    char line[512];
    std::stringstream ss;
    struct TotalVulnerableBits<double> values;

    input_file->getline(line, 512);
    ss.str(line);
    ss >> values.iq >> values.rob >> values.lq >> values.sq >> values.rf >>
        values.ibuff >> values.sys;
    ss >> values.fu_simple >> values.fu_complex >> values.fu_control >>
        values.fu_memory;

    return values;
}

/* end of file */
