/**********************************************************
 *
 * Simulation log non-speculative implementation code
 *
 * First author: Bagus Wibowo
 * created: 05/28/2014
 **********************************************************/

#include <iostream>
#include "simlog_nonspec.h"

void SimLogNonSpec::LogClockTick(LogClockTickArgs args) {
    // call parent's LogClockTick method
    SimLog::LogClockTick(args);

    // save the flushed instructions count in flushed_cnt_ and clear it
    flushed_cnt_saved_ = flushed_cnt_;
    flushed_cnt_.clear();
}

void SimLogNonSpec::LogFlush(LogFlushArgs args) {
    // For SimLogNonSpec, LogFlush will also erase all flushed
    // instructions information from logdb_

    // get last commit seq_no
    uint64_t previous_last_commit_seq_no = last_commit_after_flush_;

    // call parent's LogFlush method
    SimLog::LogFlush(args);

    // in addition to set the flush clock, clear the instructions that
    // are flushed
    std::map<uint64_t, struct InstructionDetails>::iterator it;

    if (previous_last_commit_seq_no == ((uint64_t)-1)) {
        it = logdb_.begin();
    }
    else {
        it = logdb_.find(previous_last_commit_seq_no);
        it++; // point to the entry after the last commit after the last flush
    }

    while (it != logdb_.end()) {
        uint64_t seq_no = it->first;

        // iterator points to the next entry
        it++;

        // flush uncommitted instruction
        if (logdb_[seq_no].retired_clk == INVALID_CLK) {
            // count flushed instructions
            if (logdb_[seq_no].dispatched_clk != INVALID_CLK) {
                // the instruction was loaded into IQ, LQ, SQ, and ROB

                // handle IQ case
                flushed_cnt_.iq++;

                // handle ROB case
                flushed_cnt_.rob++;

                // handle LQ case
                if (logdb_[seq_no].load_id != INVALID_LOAD_ID) {
                    // the instruction is of type load
                    flushed_cnt_.lq++;
                }

                // handle SQ case
                if (logdb_[seq_no].store_id != INVALID_STORE_ID) {
                    // the instruction is of type store
                    flushed_cnt_.sq++;
                }
            }

            // finally, erase the entry
            logdb_.erase(seq_no);
        }
    }
}

struct InstructionCount<int> SimLogNonSpec::GetFlushedInstructionsCount() {
    return flushed_cnt_saved_;
}

 /* end of file */
