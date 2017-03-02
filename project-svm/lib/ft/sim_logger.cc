/**********************************************************
 *
 * SimLog logger implementation code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#include <fstream>
#include <iostream>
#include "svm.h"
#include "sim_logger.h"

SimLogger::SimLogger(SimLog *simlog, std::string base_output_path) :
  FtAnalyzer(simlog, base_output_path) {
    start_time_ = time(NULL);
}

SimLogger::~SimLogger() {
}

bool SimLogger::Analyze(uint64_t max_clock) {
    if (SVM->cfg().enable_log_all_insts)
        LogAllInstructions();

    // log m-cache hit rate
    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << "mcache: " << std::endl;
        *p_outfile_summary_ << "    hit_count: " <<
            simlog_->m_cache_hit_cnt << std::endl;
        *p_outfile_summary_ << "    access_count: " <<
            (simlog_->m_cache_hit_cnt + simlog_->m_cache_miss_cnt) << std::endl;
        *p_outfile_summary_ << "    hit_rate: " <<
            ((float)simlog_->m_cache_hit_cnt/
                (simlog_->m_cache_hit_cnt + simlog_->m_cache_miss_cnt)) <<
            std::endl;
        *p_outfile_summary_ << std::endl;
    }

    // log simulation duration
    uint64_t duration = time(NULL) - start_time_;

    std::cout << std::dec; // set interger format to decimal
    std::cout << "[sim_logger] INFO: Completed in " << duration << "s (";
    std::cout << (duration/3600)<< " hour(s) " << ((duration%3600)/60) <<
        " minute(s) " << (duration%60) << " second(s)";
    std::cout << ")" << std::endl;

    if (p_outfile_summary_->is_open()) {
        *p_outfile_summary_ << std::endl;
        *p_outfile_summary_ << "simulation_duration: " <<
            duration << " # seconds (";
        *p_outfile_summary_ << (duration/3600)<< " hour(s) " <<
            ((duration%3600)/60) << " minute(s) " <<
            (duration%60) << " second(s)";
        *p_outfile_summary_ << ")" << std::endl;
    }

    std::cout << "[sim_logger] INFO: Results are stored in " <<
        FULL_PATH("results") << std::endl;

    return true;
}

/* -------------------- private function definitions ------------------------ */

void SimLogger::LogInstruction(uint64_t seq_no) {
    logfile_ << std::hex;
    logfile_ << seq_no;
    logfile_ << " " << (simlog_->logdb_)[seq_no].pc;
    logfile_ << " B" << (simlog_->logdb_)[seq_no].ibuff_duration;
    logfile_ << " P" << (simlog_->logdb_)[seq_no].dispatched_clk;
    if ((simlog_->logdb_)[seq_no].load_id != INVALID_LOAD_ID)
        logfile_ << " L" << (simlog_->logdb_)[seq_no].dispatched_clk;
    if ((simlog_->logdb_)[seq_no].store_id != INVALID_STORE_ID)
        logfile_ << " S" << (simlog_->logdb_)[seq_no].dispatched_clk;
    logfile_ << " I" << (simlog_->logdb_)[seq_no].issued_clk;
    if ((simlog_->logdb_)[seq_no].sources.size()) {
        logfile_ << " R" << (simlog_->logdb_)[seq_no].regread_clk;
        std::vector<struct SourceRegister>::iterator srcs;
        for (srcs=(simlog_->logdb_)[seq_no].sources.begin();
            srcs!=(simlog_->logdb_)[seq_no].sources.end();
            srcs++) {

            if (srcs->producer != REG_NO_PRODUCER)
                logfile_ << ":" <<  srcs->producer;
            else
                logfile_ << ":new";
        }
    }
    if ((simlog_->logdb_)[seq_no].destinations.size() > 0)
        // if destination register exists
        logfile_ << " W" << (simlog_->logdb_)[seq_no].writeback_clk <<  ":" <<
            (simlog_->logdb_)[seq_no].destinations[0].phy;
    logfile_ << " T" << (simlog_->logdb_)[seq_no].retired_clk;
    if ((simlog_->logdb_)[seq_no].store_id != INVALID_STORE_ID)
        logfile_ << " M" << (simlog_->logdb_)[seq_no].storecommit_clk;
    logfile_ << " VQ" << (simlog_->logdb_)[seq_no].qvbit;
    logfile_ << " VR" << (simlog_->logdb_)[seq_no].rvbit;
    if (!(simlog_->logdb_)[seq_no].important)
        logfile_ << " [not important]";
    for (uint64_t i=0; i<simlog_->logdb_[seq_no].destinations.size(); i++) {
        if (!simlog_->logdb_[seq_no].destinations[i].freed) {
            logfile_ << " [not freed: R" <<
                (simlog_->logdb_)[seq_no].destinations[i].log << "]";
        }
    }
    if (!(simlog_->IsInstructionAnalyzed(seq_no)) &&
        (simlog_->logdb_)[seq_no].important) {

        logfile_ << " [important but not analyzed]";
    }
    if ((simlog_->logdb_)[seq_no].is_syscall)
        logfile_ << " [syscall]";
    logfile_ << std::endl;
}

void SimLogger::LogAllInstructions() {
    std::map<uint64_t, struct InstructionDetails>::iterator it =
        (simlog_->logdb_).begin();

    std::cout << "[simlog_logger] INFO: Logging all instructions..." <<
        std::flush;
    logfile_.open(FULL_PATH("results/simlog.out"),
                  std::ios::out | std::ios::trunc);

    while (it != (simlog_->logdb_).end()) {
        uint64_t seq_no = it->first;
        uint64_t retired_clk = it->second.retired_clk;
        if (retired_clk != INVALID_CLK) {
            LogInstruction(seq_no);
        }
        it++;
    }

    logfile_.close();
    std::cout << " done" << std::endl;
}
/* end of file */
