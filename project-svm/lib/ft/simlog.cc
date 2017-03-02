/**********************************************************
 *
 * Simulation log module code
 *
 * First author: Bagus Wibowo
 * created: 05/14/2014
 **********************************************************/

#include <iostream>
#include <stack>
#include <queue>
#include <cassert>
#include "simlog.h"
#include <mips64.h>
#include "mips64r2_encodings.h"
#include "term_style.h"
#include "svm.h"

#define SYSCALL_INST ((uint64_t)0xc)

SimLog::SimLog(void) :
  regdb_(SVM->cfg().core_size_prf_int, REG_NO_PRODUCER),
  amtable_(SVM->cfg().arch_size_amt) {
    std::cout << "[simlog] INFO: starting simlog..." << std::endl;

    last_commit_after_flush_ = ((uint64_t)-1);

    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        amtable_[i] = i;
    }

    ResetFreeList();
    mr_ = NULL;
    mr_is_null_warning_shown_ = false;

    last_dispatched_store_seq_no_ = INVALID_SEQ_NO;
    last_retired_store_seq_no_ = INVALID_SEQ_NO;

    m_cache_hit_cnt = 0;
    m_cache_miss_cnt = 0;
}

SimLog::~SimLog(void) {
}

void SimLog::SetAVFMetadata(MReader* mr) {
    mr_ = mr;
}

void SimLog::LogClockTick(LogClockTickArgs args) {
    #ifndef NO_VERBOSE
    if (SVM->cfg().verbose)
        std::cerr << "\r" << TERM_CYAN << "clock #" << args.clk << " | " <<
            TERM_RESET << std::flush;
    #endif
    if (mr_ == NULL && !mr_is_null_warning_shown_) {
        std::cout << TERM_YELLOW << "[simlog] WARNING: Metadata file is " <<
            "not set. Marking the importance of callee saved registers " <<
            "store will be treated as always important." << TERM_RESET <<
            std::endl;
        mr_is_null_warning_shown_ = true;
    }
}

void SimLog::LogFetch(LogFetchArgs args) {
    uint64_t seq_no = args.seq_no;
    if (seq_no == 0) // invalid fetched instruction value?
        return;

    // add new entry in db
    logdb_[seq_no].pc = args.pc;
    logdb_[seq_no].save_pc = 0;
    logdb_[seq_no].store_id = INVALID_STORE_ID;
    logdb_[seq_no].load_id = INVALID_LOAD_ID;
    logdb_[seq_no].opcode = args.opcode;
    logdb_[seq_no].fu = -1;
    logdb_[seq_no].decoded_clk = args.clk;
    logdb_[seq_no].dispatched_clk = INVALID_CLK;
    logdb_[seq_no].issued_clk = INVALID_CLK;
    logdb_[seq_no].regread_clk = INVALID_CLK;
    logdb_[seq_no].memexecute_clk = INVALID_CLK;
    logdb_[seq_no].writeback_clk = INVALID_CLK;
    logdb_[seq_no].retired_clk = INVALID_CLK;
    logdb_[seq_no].storecommit_clk = INVALID_CLK;
    logdb_[seq_no].ibuff_duration = INVALID_CLK;
    logdb_[seq_no].last_store_seq_no = INVALID_SEQ_NO;
    logdb_[seq_no].important = false;
    logdb_[seq_no].sources.clear();
    logdb_[seq_no].destinations.clear();
    logdb_[seq_no].is_syscall = args.is_syscall;
    logdb_[seq_no].is_call = args.is_call;
    logdb_[seq_no].is_return = args.is_return;
    logdb_[seq_no].m_cache_hit = args.m_cache_hit;
    logdb_[seq_no].qvbit = 0;
    logdb_[seq_no].rvbit = 0;

    if (args.is_syscall && syscalldb_.count(args.pc) == 0) {
        syscalldb_[args.pc] = 1;
    }
}

void SimLog::LogRename(LogRenameArgs args) {
    uint64_t seq_no = args.seq_no;
    if (seq_no == 0) // invalid seq_no
        return;

    // record the duration of staying in ibuff
    logdb_[seq_no].ibuff_duration = args.clk - logdb_[seq_no].decoded_clk;

    if (args.dest_valid) {
        if (args.dest_log == 0 && SVM->cfg().core_r0_hardwired) {
            // skip renaming architectural register 0
        }
        else {
            if (args.dest_phy < 0 || args.dest_phy >= SVM->cfg().core_size_prf_int) {
                std::cout << std::hex << TERM_YELLOW <<
                    "[simlog] WARNING: Invalid physical destination ID " <<
                    args.dest_phy << " at clock " << args.clk << TERM_RESET <<
                    std::endl;
                return;
            }
            if (args.dest_phy != freelist_.front()) {
                std::cout << std::hex << TERM_YELLOW <<
                    "[simlog] WARNING: Inconsistent renamed register at clock " <<
                    args.clk << ", ";
                std::cout << std::hex << args.dest_phy << " vs. " <<
                    freelist_.front() << TERM_RESET << std::endl;
            }
            if (logdb_[seq_no].destinations.size() > 0) {
                std::cout << TERM_YELLOW <<
                    "[simlog] WARNING: Multiple destination registers " <<
                    "of an instruction being renamed." << TERM_RESET << std::endl;
            }

            // pop freelist_
            freelist_.pop_front();
        }

        regdb_[args.dest_phy] = seq_no;
        struct DestinationRegister dest_reg;
        dest_reg.phy = args.dest_phy;
        dest_reg.log = args.dest_log;
        dest_reg.analyzed = false;
        dest_reg.freed = false;
        logdb_[seq_no].destinations.push_back(dest_reg);
    }

    struct SourceRegister src_reg;
    if (args.src1_valid) {
        src_reg.producer = regdb_[args.src1_phy];
        src_reg.log = args.src1_log;
        src_reg.phy = args.src1_phy;
        logdb_[seq_no].sources.push_back(src_reg);
    }
    if (args.src2_valid) {
        src_reg.producer = regdb_[args.src2_phy];
        src_reg.log = args.src2_log;
        src_reg.phy = args.src2_phy;
        logdb_[seq_no].sources.push_back(src_reg);
    }
}

void SimLog::LogDispatch(LogDispatchArgs args) {
    if (args.seq_no == 0)  // invalid seq_no
        return;
    logdb_[args.seq_no].dispatched_clk = args.clk;
}

void SimLog::LogDispatchLoad(LogDispatchLoadArgs args) {
    logdb_[args.seq_no].load_id = args.load_id;
    // record the seq_no of the last store which this load might depend to
    logdb_[args.seq_no].last_store_seq_no = last_dispatched_store_seq_no_;

    if (args.seq_no == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Dispatch Load with seq_no = 0 at clk " <<
            args.clk << TERM_RESET << std::endl;
    }
}

void SimLog::LogDispatchStore(LogDispatchStoreArgs args) {
    if (args.seq_no != 0) { // make sure the seq_no is valid (not zero)
        if (storedb_.count(args.store_id)) {
            std::cout << std::hex << TERM_YELLOW <<
                "[simlog] WARNING: Store ID " << args.store_id <<
                " is assigned again to seq_no " << args.seq_no <<
                " at clock " << args.clk << std::endl;
            std::cout << std::hex << "         it was assigned to seq_no " <<
                storedb_[args.store_id] << TERM_RESET << std::endl;
        }
        logdb_[args.seq_no].store_id = args.store_id;
        storedb_[args.store_id] = args.seq_no;

        last_dispatched_store_seq_no_ = args.seq_no;
    }

    if (args.seq_no == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Dispatch Store with seq_no = 0 at clk " <<
            args.clk << TERM_RESET << std::endl;
    }
}

void SimLog::LogIssue(LogIssueArgs args) {
    logdb_[args.seq_no].issued_clk = args.clk;
    logdb_[args.seq_no].al_entry_tail_diff = args.al_entry_tail_diff;
    switch(args.lane) {
        case 0: logdb_[args.seq_no].fu = INSTRUCTION_TYPE_MEMORY; break;
        case 1: logdb_[args.seq_no].fu = INSTRUCTION_TYPE_CONTROL; break;
        case 2: logdb_[args.seq_no].fu = INSTRUCTION_TYPE_COMPLEX; break;
        case 3: logdb_[args.seq_no].fu = INSTRUCTION_TYPE_SIMPLE; break;
        default:
            ;//std::cout << std::hex << TERM_YELLOW <<
            // "[simlog] WARNING: Unhandled lane " << lane <<
            // TERM_RESET << std::endl;
    }
    if (args.seq_no == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Issue iwth seq_no = 0 at clk " << args.clk <<
            TERM_RESET << std::endl;
    }
}

void SimLog::LogRegRead(LogRegReadArgs args) {
    if (args.seq_no == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: RegRead with seq_no = 0 at clk " << args.clk <<
            TERM_RESET << std::endl;
    }
    logdb_[args.seq_no].regread_clk = args.clk;
}

void SimLog::LogMemExecute(LogMemExecuteArgs args) {
    if (logdb_.count(args.seq_no)) {
        logdb_[args.seq_no].memexecute_clk = args.clk;
    }
    else {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Instruction details not found at clock " <<
            args.clk << " seqId: " << args.seq_no <<
            " while recording memory execute stage" << TERM_RESET << std::endl;
    }
}

void SimLog::LogWriteBack(LogWriteBackArgs args) {
    if (args.seq_no == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Writeback with seq_no = 0 at clk " << args.clk <<
            TERM_RESET << std::endl;
    }
    logdb_[args.seq_no].writeback_clk = args.clk;
}

void SimLog::LogRetire(LogRetireArgs args) {
    if (args.seq_no == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Retire with seq_no = 0 at clk " << args.clk <<
            TERM_RESET << std::endl;
    }

    logdb_[args.seq_no].retired_clk = args.clk;
    // update local AM table if the destination register is valid
    if (logdb_[args.seq_no].destinations.size() > 0) {
        int32_t log_id = logdb_[args.seq_no].destinations[0].log;
        int32_t phy = amtable_[log_id];
        uint64_t freed_seq_no = regdb_[phy];
        if (freed_seq_no != REG_NO_PRODUCER) {
            if (logdb_.count(freed_seq_no)) {
                bool dest_found = false;
                for (uint64_t i=0;
                    i<logdb_[freed_seq_no].destinations.size();
                    i++) {

                    if (logdb_[freed_seq_no].destinations[i].log == log_id) {
                        logdb_[freed_seq_no].destinations[i].freed = true;
                        dest_found = true;
                        break;
                    }
                }
                if (!dest_found) {
                    std::cout << std::hex << TERM_YELLOW <<
                        "[simlog] WARNING: at clock " << args.clk <<
                        " destination register " << log_id <<
                        " is not found in the destinations vector" <<
                        TERM_RESET << std::endl;
                }
            }
            else
                std::cout << std::hex << TERM_YELLOW <<
                    "[simlog] WARNING: Invalid seq_no in AMT: " <<
                    freed_seq_no << " at clock " << args.clk <<
                    TERM_RESET << std::endl;
        }

        if (freed_seq_no != REG_NO_PRODUCER && args.seq_no < freed_seq_no)
            std::cout << std::hex << TERM_YELLOW <<
                "[simlog] WARNING: seq_no " << args.seq_no <<
                " is freeing a future seq_no " << freed_seq_no <<
                TERM_RESET << std::endl;

        amtable_[logdb_[args.seq_no].destinations[0].log] =
            logdb_[args.seq_no].destinations[0].phy;
    }
    else {
        // no destination register
        // do nothing
    }

    // if it is a store, update the last retired store instruction seq_no
    if (logdb_[args.seq_no].store_id != INVALID_STORE_ID)
        last_retired_store_seq_no_ = args.seq_no;

    // if it is a PREF instruction, set the writeback time to issue time + 4,
    // mem execute time as issue time + 2
    if (logdb_[args.seq_no].opcode == OPCODE_PREF) {
        assert(logdb_[args.seq_no].issued_clk < args.clk);
        logdb_[args.seq_no].writeback_clk = logdb_[args.seq_no].issued_clk + 4;
        logdb_[args.seq_no].memexecute_clk = logdb_[args.seq_no].issued_clk + 2;
    }

    // update m-cache hit/miss counter
    if (logdb_[args.seq_no].m_cache_hit) m_cache_hit_cnt++;
    else m_cache_miss_cnt++;

    if (mr_ != NULL) {
        // For callee saved register restore (load),
        // save the seq_no of the correspoing save (store) instruction
        uint64_t pc = logdb_[args.seq_no].pc;
        struct Metadata md = mr_->GetMetadata(pc);
        if (md.ld_sd_callee > 1) {
            if (logdb_[args.seq_no].load_id == INVALID_LOAD_ID) {
                std::cout << std::hex << TERM_YELLOW <<
                    "[simlog] WARNING: Unexpected metadata for " <<
                    "non-load data, pc: " << pc << ", metadata " <<
                    md.ld_sd_callee << TERM_RESET << std::endl;
            }
            else {
                uint64_t sd_pc = md.ld_sd_callee;
                if (sd_callee.count(sd_pc) == 1) {
                    logdb_[args.seq_no].save_pc =
                        (uint64_t)( sd_callee[sd_pc].back() );
                    sd_callee[sd_pc].pop_back();
                    if (sd_callee[sd_pc].empty())
                        sd_callee.erase(sd_pc);
                }
                else {
                    std::cout << std::hex << TERM_YELLOW <<
                        "[simlog] WARNING in LogRetire: Store pair pc: " <<
                        sd_pc << " absent for restore at pc: " << pc <<
                        ", seq="<< args.seq_no << TERM_RESET << std::endl;
                }
            }
        }
    }
}

void SimLog::LogControlCommit(LogControlCommitArgs args) {
    // mark current instruction as important
    MarkImportant(args.seq_no);
}

void SimLog::LogStoreCommit(LogStoreCommitArgs args) {
    if (storedb_.count(args.store_id) == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Invalid store commit " << args.store_id <<
            " at clk " << args.clk << TERM_RESET << std::endl;
    }
    uint64_t seq_no = storedb_[args.store_id];
    if (logdb_.count(seq_no)) {
        logdb_[seq_no].storecommit_clk = args.clk;

        // mark current instruction as important
        if (mr_ != NULL) {
            // mark current instruction as important unless it is callee saved
            // register saved to stack
            uint64_t pc = logdb_[seq_no].pc;
            struct Metadata md = mr_->GetMetadata(pc);
            if (md.ld_sd_callee == 1) {
                sd_callee[pc].push_back(seq_no);
            }
            else if (md.ld_sd_callee == 0) {
                MarkImportant(seq_no);
            }
            else {
                std::cout << std::hex << TERM_YELLOW <<
                    "[simlog] WARNING: Unexpected metadata for pc: " << pc <<
                    " md.ld_sd_callee " << md.ld_sd_callee <<
                    TERM_RESET << std::endl;
            }
        }
        else {
            MarkImportant(seq_no);
        }
    }
    else {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Instruction details not found at clock " <<
            args.clk << " seqId: " << seq_no << " with store id " <<
            args.store_id << TERM_RESET << std::endl;
    }
}

void SimLog::LogPostStoreCommit(LogStoreCommitArgs args) {
    if (storedb_.count(args.store_id) == 0) {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Invalid post store commit at clk " << args.clk <<
            TERM_RESET << std::endl;
    }
    uint64_t seq_no = storedb_[args.store_id];
    if (logdb_.count(seq_no)) {
        // remove it from storedb_
        storedb_.erase(args.store_id);
    }
    else {
        std::cout << std::hex << TERM_YELLOW <<
            "[simlog] WARNING: Instruction details not found at clock " <<
            args.clk << " seqId: " << seq_no << " with store id " <<
            args.store_id << TERM_RESET << std::endl;
    }
}

void SimLog::LogFreedReg(LogFreedRegArgs args) {
    // update freelist_
    freelist_.push_back(args.reg_id);
}

void SimLog::LogFlush(LogFlushArgs args) {
    std::map<uint64_t, struct InstructionDetails>::iterator it;
    std::stack<int> phyreg_list;

    if (last_commit_after_flush_ == ((uint64_t)-1)) {
        it = logdb_.begin();
    }
    else {
        it = logdb_.find(last_commit_after_flush_);
        it++; // point to the entry after the last commit after the last flush
    }

    while (it != logdb_.end()) {
        struct InstructionDetails *payload = &(it->second);
        uint64_t seq_no = it->first;
        int64_t store_id = payload->store_id;

        // iterator points to the next entry
        it++;

        // flush uncommitted instruction
        if (logdb_[seq_no].retired_clk == INVALID_CLK) {
            // if it is a store instruction and it has not committed,
            // remove it from storedb_
            if (store_id != INVALID_STORE_ID)
                storedb_.erase(store_id);

            // record freed physical register id
            if (logdb_[seq_no].destinations.size() > 0)
                phyreg_list.push(logdb_[seq_no].destinations[0].phy);
        }
        else {
            last_commit_after_flush_ = seq_no;
        }
    }

    // store the flushed physical registers
    while(!phyreg_list.empty()) {
        freelist_.push_front(phyreg_list.top());
        phyreg_list.pop();
    }

    // reset freelist if configured to do so
    if (SVM->cfg().flush_reset_freelist) {
        uint64_t log2seqno[SVM->cfg().arch_size_amt];

        // record logical register's producers
        for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
            log2seqno[i] = regdb_[amtable_[i]];
        }

        // Reset AMT.
        for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
            amtable_[i] = i;
            regdb_[i] = log2seqno[i];
        }

        // Finally, reset freelist
        ResetFreeList();
    }

    // revert the last dispatched store to the last committed one
    last_dispatched_store_seq_no_ = last_retired_store_seq_no_;
}

void SimLog::LogException(LogExceptionArgs args) {
    uint64_t log2seqno[SVM->cfg().arch_size_amt];
    struct SourceRegister src;

    // copy current AMT
    amtable_before_exception_ = amtable_;

    // record logical register's producers
    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        log2seqno[i] = regdb_[amtable_[i]];
    }

    // is it a syscall?
    if (IsPcSyscall(args.pc)) {
        // the syscall commit was happened on the previous clock
        LogRetireArgs log_args;
        log_args.clk = args.clk-1; log_args.seq_no = args.seq_no;
        LogRetire(log_args);

        // mark some producers as important
        MarkImportant(args.seq_no); // include the syscall instruction itself

        // some registers are consumed by syscall instruction
        struct UsesDefs ud = GetSyscallUsesDefs(args.v0);
        for (std::vector<int>::iterator it = ud.uses.begin();
            it != ud.uses.end();
            it++) {

            MarkImportant(log2seqno[*it]); // mark producer of the register used
                                           // by syscall as important
            // construct new src
            src.producer = log2seqno[*it]; src.phy = amtable_[*it];
            // add the src into the source list
            logdb_[args.seq_no].sources.push_back(src);
        }
    }
    else {
        std::cout << TERM_YELLOW <<
            "[simlog] WARNING: unhandled non-syscall exception" << TERM_RESET <<
            std::endl;
    }

    // for each exception, FabScalar will reset AMT. Do the same here.
    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        amtable_[i] = i;
        regdb_[i] = log2seqno[i];
    }

    // for exception raised by syscall instruction
    if (IsPcSyscall(args.pc))  {
        // some registers are produced by syscall instruction
        struct UsesDefs ud = GetSyscallUsesDefs(args.v0);
        for (std::vector<int>::iterator it = ud.defs.begin();
            it != ud.defs.end();
            it++) {

            // mark the previous producer as freed
            bool dest_found = false;
            for (uint64_t i=0;
                i<logdb_[log2seqno[*it]].destinations.size();
                i++) {

                if (logdb_[log2seqno[*it]].destinations[i].log == *it) {
                    logdb_[log2seqno[*it]].destinations[i].freed = true;
                    dest_found = true;
                    break;
                }
            }
            if (!dest_found) {
                std::cout << TERM_YELLOW <<
                    "[simlog] WARNING: destination register is not found " <<
                    "in syscall handling." << TERM_RESET << std::endl;
            }

            // set the new producer to the syscall instruction
            regdb_[amtable_[*it]] = args.seq_no;

            // add new destination
            struct DestinationRegister dest;
            dest.phy = *it;
            dest.log = *it;
            dest.analyzed = false;
            dest.freed = false;
            logdb_[args.seq_no].destinations.push_back(dest);
        }
    }

    // next, do flush
    LogFlushArgs log_args;
    log_args.clk = args.clk;
    LogFlush(log_args);

    // reset freelist_ as well (should be after flush)
    ResetFreeList();
}

bool SimLog::IsInstructionAnalyzed(uint64_t seq_no) {
    for (uint64_t i=0; i<logdb_[seq_no].destinations.size(); i++) {
        if (logdb_[seq_no].destinations[i].analyzed == false)
            return false; // if any of its destionation has not been analyzed,
                          // return false
    }
    return true;
}

bool SimLog::IsPcSyscall(uint64_t pc) {
    return syscalldb_.count(pc);
}

/* -------------------- private function definitions ------------------------ */
void SimLog::MarkImportant(uint64_t root_seq_no) {
    std::queue<uint64_t> work_list;

    work_list.push(root_seq_no);

    while (!work_list.empty()) {
        uint64_t seq_no = work_list.front();
        work_list.pop();

        if (logdb_.count(seq_no) == 0) {
            std::cout << std::hex << TERM_YELLOW <<
                "[simlog] WARNING: invalid seq_no=" << seq_no <<
                " while marking important instruction" <<
                TERM_RESET << std::endl;
            continue; // ignore this entry and proceed
        }

        if (mr_ != NULL) { // check if metadata marks the instruction as
                           // statically unimportant
            uint64_t pc = logdb_[seq_no].pc;
            struct Metadata md = mr_->GetMetadata(pc);

            if (!md.rw.important) {
                // the instruction is marked as statically unimportant,
                // leave it as unimportant
                continue;
            }
        }

        if (logdb_[seq_no].important)  {
            // this instruction has been marked as important
            continue;
        }
        else {
            // mark this instruction as important
            logdb_[seq_no].important = true;
            // back propagate importance
            std::vector<struct SourceRegister>::iterator srcs;
            for (srcs=logdb_[seq_no].sources.begin();
                srcs!=logdb_[seq_no].sources.end();
                srcs++) {

                if (srcs->producer != REG_NO_PRODUCER)
                    work_list.push(srcs->producer);
            }
            if (mr_ != NULL) {
                uint64_t pc = logdb_[seq_no].pc;
                struct Metadata md = mr_->GetMetadata(pc);
                if (md.ld_sd_callee > 1) {
                    if (logdb_[seq_no].load_id == INVALID_LOAD_ID) {
                        std::cout << std::hex << TERM_YELLOW <<
                            "[simlog] WARNING: Unexpected metadata for " <<
                            "non-load data, pc: " << pc <<
                            ", metadata " <<md.ld_sd_callee <<
                            TERM_RESET << std::endl;
                    }
                    else {
                        uint64_t sd_pc = md.ld_sd_callee;
                        if (logdb_[seq_no].save_pc != 0) {
                            work_list.push( logdb_[seq_no].save_pc );
                        }
                        else {
                            std::cout << std::hex << TERM_YELLOW <<
                                "[simlog] WARNING in MarkImportant: " <<
                                "Store pair pc: " << sd_pc <<
                                " absent for restore at pc: " << pc <<
                                ", seq=" << seq_no << TERM_RESET << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void SimLog::ResetFreeList() {
    // clear freelist_
    freelist_.clear();

    // reinitialize freelist_
    for(uint32_t i=SVM->cfg().arch_size_amt;
        i<SVM->cfg().core_size_prf_int;
        i++) {

        freelist_.push_back(i);
    }
}

/* end of file */
