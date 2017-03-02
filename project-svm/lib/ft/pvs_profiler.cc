/**********************************************************
 *
 * PVS Profiler
 *
 * First author: Bagus Wibowo
 * created: 01/18/2016
 **********************************************************/

#include <queue>
#include "pvs_profiler.h"
#include "term_style.h"
#include "simlog.h"

PvsProfiler pvs_profiler;

PvsProfiler::PvsProfiler() {
    seq_no_ = 0;
    outfile_ = "pvs.out"; // default pvs out file
    mr_ = NULL;
    warned_no_md_ = false;
    last_v0_value_ = 0; // initialized to zero
}

void PvsProfiler::SetMetadata(const char *fname) {
    mr_ = new MReader(fname);;
}

void PvsProfiler::SetOutFile(const char *outfile) {
    outfile_ = outfile;
}

void PvsProfiler::AppendInst(arch_inst_t *inst) {
    // increment seq_no at the beginning
    seq_no_++;

    if (mr_ == NULL && !warned_no_md_) {
        std::cout << "Warning: No metadata provided for PVS profiling" << std::endl;
        warned_no_md_ = true;
    }

    inst_window_t iw;
    iw.important = false;
    iw.pc = inst->pc;
    struct Metadata md;
    if (mr_) {
        md = mr_->GetMetadata(inst->pc);
    }

    inst_window_.insert(inst_window_.end(),
        std::pair<uint64_t,inst_window_t>(seq_no_, iw));
    inst_profile_[inst->pc].instance_cnt++;

    for (unsigned int n=0; n<inst->num_rsrc; n++) {
        if (arch_reg_defs_.find(inst->rsrc[n].n) == arch_reg_defs_.end()) {
            arch_reg_defs_[inst->rsrc[n].n] = 0; // zero means invalid
        }
        inst_window_[seq_no_].parents[n] = arch_reg_defs_[inst->rsrc[n].n];
    }
    inst_window_[seq_no_].num_parents = inst->num_rsrc;
    for (unsigned int n=0; n<inst->num_rdst; n++) {
        int32_t arch_reg = inst->rdst[n].n;
        arch_reg_defs_[arch_reg] = seq_no_;
        if (arch_reg == R_V0)
            last_v0_value_ = inst->rdst[n].value;
    }

    // check if the current instruction is important
    bool is_important = false;

    if (inst->is_cti) {
        // always assume any control instructions as important
        is_important = true;
    }
    else if (inst->is_store) {
        if (mr_) {
            // mark current instruction as important unless it's callee saved
            // register saved to stack
            uint64_t pc = inst->pc;
            if (md.ld_sd_callee == 1) {
                sd_callee_[pc].push_back(seq_no_);
            }
            else if (md.ld_sd_callee == 0) {
                is_important = true;
            }
            else {
                std::cout << std::hex << TERM_YELLOW <<
                    "[pvs_profiler] WARNING: Unexpected metadata for pc: " << pc <<
                    " md.ld_sd_callee " << md.ld_sd_callee <<
                    TERM_RESET << std::endl;
            }
        }
        else {
            // no metadata provided, assume all stores is important
            is_important = true;
        }
    }
    else if (mr_ && inst->is_load) {
        // For callee saved register restore (load),
        // save the seq_no of the correspoing save (store) instruction
        uint64_t pc = inst->pc;
        if (md.ld_sd_callee > 1) {
            uint64_t sd_pc = md.ld_sd_callee;
            if (sd_callee_.count(sd_pc) == 1) {
                int32_t idx = inst_window_[seq_no_].num_parents;
                inst_window_[seq_no_].parents[idx] =
                    (uint64_t)( sd_callee_[sd_pc].back() );
                inst_window_[seq_no_].num_parents = idx+1;

                // clean up
                sd_callee_[sd_pc].pop_back();
                if (sd_callee_[sd_pc].empty())
                    sd_callee_.erase(sd_pc);
            }
            else {
                std::cout << std::hex << TERM_YELLOW <<
                    "[pvs_profiler] WARNING in LogRetire: Store pair pc: " <<
                    sd_pc << " absent for restore at pc: " << pc <<
                    ", seq="<< seq_no_ << TERM_RESET << std::endl;
            }
        }
    }
    else if (inst->is_trap) {
        // always assume any syscall instructions as important
        is_important = true;

        struct UsesDefs ud = SimLog::GetSyscallUsesDefs(last_v0_value_);
        std::vector<int>::iterator it;

        // handle registers used by syscall
        int32_t n=0;
        for (it=ud.uses.begin(); it!=ud.uses.end(); it++, n++) {
            inst_window_[seq_no_].parents[n] = arch_reg_defs_[*it];
        }
        inst_window_[seq_no_].num_parents = n;

        // handle registers defined by syscall
        for (it=ud.defs.begin(); it!=ud.defs.end(); it++) {
            arch_reg_defs_[*it] = seq_no_;
        }
    }

    if (!md.rw.important) {
        // the instruction is marked as statically unimportant,
        // leave it as unimportant, and remove it from instruction window
        inst_window_.erase(seq_no_);
    }
    else if (is_important) {
        std::queue<uint64_t> work_list;
        work_list.push(seq_no_);

        while (!work_list.empty()) {
            uint64_t item_no = work_list.front();
            work_list.pop();

            if (inst_window_.find(item_no) == inst_window_.end())
                continue;
            uint64_t n_parents = inst_window_[item_no].num_parents;

            for (unsigned int n=0; n<n_parents; n++) {
                uint64_t parent = inst_window_[item_no].parents[n];
                if (parent && inst_window_.find(parent) != inst_window_.end()) {
                    work_list.push(parent);
                }
            }

            // mark current instruction as important
            inst_window_[item_no].important = true;

            // increment important counter for the entry's PC and function
            inst_profile_[inst_window_[item_no].pc].important_cnt++;

            // remove the entry
            inst_window_.erase(item_no);
        }
    }

    // clean up old instructions
    uint64_t max_distance = 100 * 1000 * 1000; // 100M
    if (seq_no_ % max_distance == 0) {
        std::map<uint64_t,inst_window_t>::iterator it = inst_window_.begin();
        while (it->first + max_distance < seq_no_) it++;
        inst_window_.erase(inst_window_.begin(), it);
    }
}

void PvsProfiler::Finalize() {
    std::ofstream fout;
    std::map<uint64_t,profile_t>::iterator it;

    fout.open(outfile_.c_str(), std::ios::out | std::ios::trunc);

    fout << "inst:" << std::endl;

    for (it=inst_profile_.begin(); it!=inst_profile_.end(); ++it) {
        fout << "    ";
        fout << std::hex << "\"" << it->first << "\" : ";
        fout << std::dec << it->second.important_cnt << "/";
        fout << std::dec << it->second.instance_cnt << std::endl;

        if (mr_) {
            uint64_t fn_pc = mr_->GetMetadata(it->first).func_pc;
            if (fn_pc == 0) {
                std::cout << "PC: " << std::hex << it->first <<
                    " has invalid function PC from metadata" << std::endl;
                continue; // skip it
            }
            fn_profile_[fn_pc].important_cnt += it->second.important_cnt;
            fn_profile_[fn_pc].instance_cnt += it->second.instance_cnt;
        }
    }

    if (mr_) {
        fout << std::endl;
        fout << "func_raw:" << std::endl;

        for (it=fn_profile_.begin(); it!=fn_profile_.end(); ++it) {
            fout << "    ";
            fout << std::hex << "\"" << it->first << "\" : ";
            fout << std::dec << it->second.important_cnt << "/";
            fout << std::dec << it->second.instance_cnt << std::endl;
        }

        fout << std::endl;
        fout << "func:" << std::endl;

        for (it=fn_profile_.begin(); it!=fn_profile_.end(); ++it) {
            fout << "    ";
            fout << std::hex << "\"" << it->first << "\" : ";
            fout << std::dec <<
                ((double)(it->second.important_cnt)/it->second.instance_cnt);
            fout << std::endl;
        }
    }

    std::cout << "PVS profile data is stored in " << outfile_ << std::endl;

    fout.close();
}

/* end of file */
