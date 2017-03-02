/**********************************************************
 *
 * PVF analyzer implementation code
 *
 * First author: Bagus Wibowo
 * created: 10/16/2014
 **********************************************************/

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include "svm.h"
#include "pvf_analyzer.h"
#include "term_style.h"

#define PVF_LOG_FILE FULL_PATH("results/pvf.log")
#define OCCE_LOG_FILE FULL_PATH("results/occe_group_1M.log")
#define HVFE_LOG_FILE FULL_PATH("results/hvfe_group_1M.log")
#define AVFE_LOG_FILE FULL_PATH("results/avfe_group_1M.log")
#define AVFHP_LOG_FILE FULL_PATH("results/avfhp_group_1M.log")

#define PHASE_PERIOD ((uint64_t)0x1 << 20)
#define SAMPLE_PERIOD ((uint64_t)0x1 << 9)

PvfAnalyzer::PvfAnalyzer(SimLog *simlog, std::string base_output_path,
  std::string pvs_profile_path) :
  FtAnalyzer(simlog, base_output_path) {
    enabled_ = false;

    hvf_events_.clear();
    p_hvf_event_ = new HvfEvents();
    pvs_profile_path_ = pvs_profile_path;

    LoadPvsProfile();
}

PvfAnalyzer::~PvfAnalyzer() {
}

void PvfAnalyzer::LogClockTick(LogClockTickArgs args) {
    if (IsPhaseCrossing(args.clk, PHASE_PERIOD)) {
        hvf_events_.push_back(*p_hvf_event_);
        p_hvf_event_ = new HvfEvents();
    }
}

void PvfAnalyzer::WarmUpEnds(WarmUpEndsArgs args) {
    // clear hvf_events stats
    assert(hvf_events_.size() == 0);
    assert(SVM->cfg().enable_warm_up);
    delete p_hvf_event_; // discard previous hvf events
    p_hvf_event_ = new HvfEvents(); // create a new empty one
}

bool PvfAnalyzer::Analyze(uint64_t max_clock) {
    if (!enabled_) return true;

    std::cout << "[pvf_analyzer] INFO: Starting program analysis..." <<
        std::flush;

    // iterate across all instructions in logdb
    std::map<uint64_t, struct InstructionDetails>::iterator it =
        simlog_->logdb_.begin();
    while (it != simlog_->logdb_.end()) {
        uint64_t pc = it->second.pc;
        struct Metadata md = mr_->GetMetadata(pc);

        // get the function ID from metadata from the seq_no
        uint64_t func_id = md.func_pc;

        // include current importance into func_pvs_
        func_pvs_[func_id].Include(it->second.important);

        // include current static importance into func_sif_
        func_sif_[func_id].Include(md.rw.important);

        // it points to the next instruction
        it++;
    }

    // dump PVS function data into file
    std::ofstream outfile;
    std::ofstream groupfile;
    outfile.open(PVF_LOG_FILE, std::ios::out | std::ios::trunc);

    if (outfile.is_open()) {
        // show PVS function data
        outfile << "=== Dumping PVF function data ===" << std::endl;

        std::map<uint64_t, struct FunctionPvs>::iterator func_pvs_it =
            func_pvs_.begin();
        while (func_pvs_it != func_pvs_.end()) {
            outfile << std::hex << "  " <<
                mr_->GetMetadata(func_pvs_it->first).func_name << ":\t" <<
                func_pvs_it->second.Value();
            if (profile_func_pvs_.size() > 0) {
                std::map<uint64_t, double>::iterator it =
                    profile_func_pvs_.find(func_pvs_it->first);
                outfile << "\t";
                if (it != profile_func_pvs_.end()) {
                    outfile << it->second;
                }
                else {
                    outfile << "-";
                }
            }
            outfile << std::endl;
            func_pvs_it++;
        }
        outfile << std::endl;

        // show static importance factor of functions
        outfile << "=== Dumping SIF function data ===" << std::endl;

        std::map<uint64_t, struct FunctionPvs>::iterator func_sif_it =
            func_sif_.begin();
        while (func_sif_it != func_sif_.end()) {
            outfile << std::hex << "  " <<
                mr_->GetMetadata(func_sif_it->first).func_name << ":\t" <<
                func_sif_it->second.Value() << std::endl;
            func_sif_it++;
        }
        outfile << std::endl;

        // show retiring instructions occupancy estimation
        outfile << "=== Dumping OCC estimate data ===" << std::endl;
        groupfile.open(OCCE_LOG_FILE, std::ios::out | std::ios::trunc);
        if (groupfile.is_open()) {
            groupfile << "IQ\tROB\tLQ\tSQ\tRF" << std::endl;
        }
        else {
            std::cout << TERM_YELLOW << "[pvf_analyzer] Could not open " <<
                OCCE_LOG_FILE << " file." << TERM_RESET << std::endl;
        }
        for (uint64_t n=0; n<hvf_events_.size(); n++) {
            outfile << "region #" << std::dec << n;
            for (uint64_t s=0; s<hvf_events_[n].structures_.size(); s++) {
                uint64_t occ_counter = 0;
                for (uint64_t m=0;
                    m<hvf_events_[n].structures_[s].seq_nos_.size();
                    m++) {

                    occ_counter += 1;
                }
                outfile << "\t" << std::fixed <<
                    std::setw(8) << std::setprecision(5) <<
                    ((float)occ_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                if(groupfile.is_open()) {
                    groupfile << "\t" << std::fixed <<
                    std::setw(8) << std::setprecision(5) <<
                    ((float)occ_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                }
            }
            outfile << std::endl;
            if(groupfile.is_open()) {
                groupfile << std::endl;
            }
        }
        groupfile.close();
        outfile << std::endl;

        // show HVF estimation
        outfile << "=== Dumping HVF estimate data ===" << std::endl;
        groupfile.open(HVFE_LOG_FILE, std::ios::out | std::ios::trunc);
        if (groupfile.is_open()) {
            groupfile << "IQ\tROB\tLQ\tSQ\tRF" << std::endl;
        }
        else {
            std::cout << TERM_YELLOW << "[pvf_analyzer] Could not open " <<
                HVFE_LOG_FILE << " file." << TERM_RESET << std::endl;
        }
        for (uint64_t n=0; n<hvf_events_.size(); n++) {
            outfile << "region #" << std::dec << n;
            for (uint64_t s=0; s<hvf_events_[n].structures_.size(); s++) {
                float hvf_counter = 0.0;
                for (uint64_t m=0;
                    m<hvf_events_[n].structures_[s].seq_nos_.size();
                    m++) {

                    hvf_counter += hvf_events_[n].structures_[s].deratings_[m];
                }
                outfile << "\t" << std::fixed <<
                    std::setw(8) << std::setprecision(5) <<
                    ((float)hvf_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                if(groupfile.is_open()) {
                    groupfile << "\t" << std::fixed <<
                    std::setw(8) << std::setprecision(5) <<
                    ((float)hvf_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                }
            }
            outfile << std::endl;
            if(groupfile.is_open()) {
                groupfile << std::endl;
            }
        }
        groupfile.close();
        outfile << std::endl;

        // compute AVF estimation using HVF and PVF
        outfile << "=== Dumping AVF estimate data ===" << std::endl;
        groupfile.open(AVFE_LOG_FILE, std::ios::out | std::ios::trunc);
        if (groupfile.is_open()) {
            groupfile << "IQ\tROB\tLQ\tSQ\tRF" << std::endl;
        }
        else {
            std::cout << TERM_YELLOW << "[pvf_analyzer] Could not open " <<
                AVFE_LOG_FILE << " file." << TERM_RESET << std::endl;
        }
        for (uint64_t n=0; n<hvf_events_.size(); n++) {
            outfile << "region #" << std::dec << n;
            for (uint64_t s=0; s<hvf_events_[n].structures_.size(); s++) {
                float avf_counter = 0;
                for (uint64_t m=0;
                    m<hvf_events_[n].structures_[s].seq_nos_.size();
                    m++) {

                    uint64_t seq_no = hvf_events_[n].structures_[s].seq_nos_[m];
                    float derating =
                        hvf_events_[n].structures_[s].deratings_[m];
                    it = simlog_->logdb_.find(seq_no);
                    if (it != simlog_->logdb_.end()) {
                        uint64_t pc = it->second.pc;
                        struct Metadata md = mr_->GetMetadata(pc);

                        std::map<uint64_t, struct FunctionPvs>::iterator func_pvs_it =
                            func_pvs_.find(md.func_pc);
                        if (func_pvs_it != func_pvs_.end()) {
                            avf_counter += func_pvs_it->second.Value() *
                                derating;
                        }
                        else {
                            std::cout << TERM_YELLOW <<
                                "[pvf_analyzer] Could not find function ID " <<
                                md.func_pc << " in func_pvs" << TERM_RESET <<
                                std::endl;
                        }
                    }
                    else {
                        std::cout << TERM_YELLOW <<
                            "[pvf_analyzer] WARNING: Invalid seq_no " <<
                            std::hex << seq_no << TERM_RESET << std::endl;
                    }
                }
                outfile << "\t" << std::fixed <<
                    std::setw(8) << std::setprecision(5) <<
                    ((float)avf_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                if(groupfile.is_open()) {
                    groupfile << "\t" << std::fixed <<
                    std::setw(8) << std::setprecision(5) <<
                    ((float)avf_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                }
            }
            outfile << std::endl;
            if(groupfile.is_open()) {
                groupfile << std::endl;
            }
        }
        groupfile.close();
        outfile << std::endl;

        // compute AVF-HP estimation using HVF from simulatoin and
        // PVS from profile file
        if (profile_func_pvs_.size() > 0) { // only if PVS profile is available
            outfile << "=== Dumping AVF-HP estimate data ===" << std::endl;
            groupfile.open(AVFHP_LOG_FILE, std::ios::out | std::ios::trunc);
            if (groupfile.is_open()) {
                groupfile << "IQ\tROB\tLQ\tSQ\tRF" << std::endl;
            }
            else {
                std::cout << TERM_YELLOW << "[pvf_analyzer] Could not open " <<
                    AVFHP_LOG_FILE << " file." << TERM_RESET << std::endl;
            }
            for (uint64_t n=0; n<hvf_events_.size(); n++) {
                outfile << "region #" << std::dec << n;
                for (uint64_t s=0; s<hvf_events_[n].structures_.size(); s++) {
                    float avf_counter = 0;
                    for (uint64_t m=0;
                        m<hvf_events_[n].structures_[s].seq_nos_.size();
                        m++) {

                        uint64_t seq_no = hvf_events_[n].structures_[s].seq_nos_[m];
                        float derating =
                            hvf_events_[n].structures_[s].deratings_[m];
                        it = simlog_->logdb_.find(seq_no);
                        if (it != simlog_->logdb_.end()) {
                            uint64_t pc = it->second.pc;
                            struct Metadata md = mr_->GetMetadata(pc);

                            std::map<uint64_t, double>::iterator func_pvs_it =
                                profile_func_pvs_.find(md.func_pc);
                            if (func_pvs_it != profile_func_pvs_.end()) {
                                avf_counter += func_pvs_it->second *
                                    derating;
                            }
                            else {
                                // if not found, assume conservative PVS = 1
                                avf_counter += 1 * derating;
                                std::cout << TERM_YELLOW <<
                                    "[pvf_analyzer] Could not find function ID " <<
                                    md.func_pc << " in profile_func_pvs_" <<
                                    TERM_RESET << std::endl;
                            }
                        }
                        else {
                            std::cout << TERM_YELLOW <<
                                "[pvf_analyzer] WARNING: Invalid seq_no " <<
                                std::hex << seq_no << TERM_RESET << std::endl;
                        }
                    }
                    outfile << "\t" << std::fixed <<
                        std::setw(8) << std::setprecision(5) <<
                        ((float)avf_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                    if(groupfile.is_open()) {
                        groupfile << "\t" << std::fixed <<
                        std::setw(8) << std::setprecision(5) <<
                        ((float)avf_counter/(PHASE_PERIOD/SAMPLE_PERIOD));
                    }
                }
                outfile << std::endl;
                if(groupfile.is_open()) {
                    groupfile << std::endl;
                }
            }
            groupfile.close();
            outfile << std::endl;
        }

        outfile.close();
    }

    std::cout << " done" << std::endl;

    return true;
}

void PvfAnalyzer::SetMetadata(MReader* mr) {
    mr_ = mr;
    enabled_ = true; // PVF analyzer is enabled by setting its
                     // metadata reader object
}

void PvfAnalyzer::HvfSingleEvent(uint64_t seq_no, enum StructureType structure,
    float derating) {

    std::map<uint64_t, struct InstructionDetails>::iterator it =
        simlog_->logdb_.find(seq_no);
    if (it != simlog_->logdb_.end()) {
        switch(structure) {
            case ST_IQ:
            case ST_ROB:
            case ST_LQ:
            case ST_SQ:
            {
                p_hvf_event_->structures_[structure].seq_nos_.push_back(seq_no);
                p_hvf_event_->structures_[structure].deratings_.push_back(derating);
                break;
            }
            default:
                // do nothing
                break;
        }
    }
    else {
        std::cout << TERM_YELLOW << "[pvf_analyzer] Could not find seq_no " <<
            seq_no << " in logdb" << TERM_RESET << std::endl;
    }
}

void PvfAnalyzer::HvfOverflowEvent(uint64_t seq_no, enum StructureType structure,
    float derating) {

    std::map<uint64_t, struct InstructionDetails>::iterator it =
        simlog_->logdb_.find(seq_no);
    if (it != simlog_->logdb_.end()) {
        switch(structure) {
            case ST_IQ:
            case ST_ROB:
            case ST_LQ:
            case ST_SQ:
            {
                // upon 3-bit counter overflow, record 8 single-events
                for (int i=0; i<8; i++)
                    HvfSingleEvent(seq_no, structure, derating);
            }
            default:
                // do nothing
                break;
        }
    }
    else {
        std::cout << TERM_YELLOW << "[pvf_analyzer] Could not find seq_no " <<
            seq_no << " in logdb" << TERM_RESET << std::endl;
    }
}

/* -------------------- private function definitions ------------------------ */

void PvfAnalyzer::LoadPvsProfile() {
    std::cout << "[pvf_analyzer] Loading PVS profile file " << pvs_profile_path_
        << std::endl;
    if (pvs_profile_path_ == "") {
        std::cout << TERM_YELLOW << "[pvf_analyzer] PVS profile is not provided."
            << " AVF-HP will NOT be calculated." << TERM_RESET << std::endl;
        return;
    }

    std::ifstream fin(pvs_profile_path_.c_str());
    if (!fin.is_open()) {
        std::cout << TERM_YELLOW << "[pvf_analyzer] Couldn't open file " <<
            pvs_profile_path_ << ", make sure the file exists." <<
            TERM_RESET << std::endl;
        return;
    }

    std::string line;
    bool func_found = false;
    while (std::getline(fin, line)) {
        if (func_found) {
            uint64_t key;
            double value;

            if (line.find(":") == std::string::npos) continue;
            std::string ltoken = line.substr(0, line.find(":")-1);
            std::string rtoken = line.substr(line.find(":")+1, std::string::npos);

            ltoken.erase(0, ltoken.find("\"")+1);
            ltoken.erase(ltoken.find("\""), std::string::npos);
            std::stringstream ssl;
            ssl << std::hex << ltoken;
            ssl >> key;
            std::stringstream ssr;
            ssr << rtoken;
            ssr >> value;

            profile_func_pvs_[key] = value;
        }
        else if (line.find("func:") != std::string::npos) {
            func_found = true;
        }
    }

    fin.close();
}

/* end of file */
