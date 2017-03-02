/**********************************************************
 *
 * Fault injection model DPIs
 *
 * First author: Bagus Wibowo
 * created: 8/2/2014
 **********************************************************/

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include "mipsarch.h"

#define FAULT_PERIOD 50000
#define FULL_PATH(path) ((output_dir + "/" + path).c_str())

uint64_t fault_cnt;
uint64_t failure_cnt;

struct BitList {
    int id;
    int entry;
    int offset;
};

std::map<int, std::string> signal_names;
std::vector<struct BitList> bit_list;
std::vector<arch_inst_t> committed_stores;
int next_seed;
int selected_bit;
bool enabled;
bool failure_was_detected;
uint64_t last_fault_cycle, current_cycle;
uint64_t warming_up_time;
std::vector<uint64_t> failure_durations;
std::string output_dir;
uint64_t start_time;
bool pc_was_incorrect;

extern Process* PROCESS;

int next_random() {
    srand(next_seed);
    next_seed = rand();
    return rand();
}

int next_random_bit_id() {
    if (!enabled || bit_list.size() == 0) {
        return 0;
    }
    else {
        return (next_random() % bit_list.size());
    }
}

static void failure_detected() {
    if (!failure_was_detected) {
        failure_cnt++;
        failure_was_detected = true;

        // record the duration from fault injected until fault detected
        uint64_t duration = current_cycle - last_fault_cycle;
        failure_durations.push_back(duration);

        std::cout << "Fault detected within " << std::dec << duration <<
            " cycle(s)." << std::endl;
    }
}

extern "C" void fi_dpi_init(const char * base_output_dir, int run_id) {
    std::cout << "[fi_dpi] Initializing" << std::endl;

    // set start time
    start_time = time(NULL);

    // set random seed
    srand(0x5ca1ab1e + run_id);
    next_seed = rand();

    // clear vectors
    bit_list.clear();
    committed_stores.clear();
    failure_durations.clear();

    // initialize global variables
    selected_bit = 0;
    enabled = true;
    fault_cnt = 0;
    failure_cnt = 0;
    failure_was_detected = true;
    last_fault_cycle = -1;
    current_cycle = 0;
    output_dir = base_output_dir;
    pc_was_incorrect = false;

    // set warming_up_time randomly
    warming_up_time = next_random() % FAULT_PERIOD;
}

extern "C" void fi_dpi_register_signal(int id, char *name, int max_entry,
  int max_offset) {
    if (signal_names.count(id) == 0) {
        signal_names[id] = name;
        for (int entry=0; entry<max_entry; entry++) {
            for (int offset=0; offset<max_offset; offset++) {
                struct BitList the_bit = {id, entry, offset};
                bit_list.push_back(the_bit);
            }
        }
    }
    else {
        std::cout << "[fi_dpi] WARNING: Signal with ID " << id <<
            " has been registered before" << std::endl;
    }
}

extern "C" int fi_dpi_clock(int cycle) {
    std::cerr << "\rclk: " << cycle << " failure/fault: " << failure_cnt <<
        "/" << fault_cnt << " -- ";
    current_cycle = cycle;
    if (!enabled || bit_list.size() == 0)
        return 0; // none could be injected

    if ((cycle) % FAULT_PERIOD == warming_up_time % FAULT_PERIOD) {
        selected_bit = next_random_bit_id();
        fault_cnt++; // increment fault count
        failure_was_detected = false; // set the flag as false
        last_fault_cycle = cycle; // record the time when the fault injected
        return 1;
    }

    // by default, return false
    return 0;
}

extern "C" int fi_dpi_get_injection_id() {
    if (!enabled || bit_list.size() == 0) return -1;
    return bit_list[selected_bit].id;
}

extern "C" int fi_dpi_get_injection_entry() {
    if (!enabled || bit_list.size() == 0) return -1;
    return bit_list[selected_bit].entry;
}

extern "C" int fi_dpi_get_injection_offset() {
    if (!enabled || bit_list.size() == 0) return -1;
    return bit_list[selected_bit].offset;
}

extern "C" int fi_dpi_get_registered_bit_cnt() {
    return bit_list.size();
}

int count_bit_diff(uint64_t value1, uint64_t value2) {
    int count = 0;
    uint64_t mask = 0x1;

    for (uint64_t n=0; n<sizeof(uint64_t)*8; n++) {
        if ((value1 & (mask<<n)) != (value2 & (mask<<n)))
            count++;
    }

    return count;
}

// Return codes:
// 0: retired instruction is incorrect
// 1: retired instruction is correct
extern "C" int32_t fi_dpi_check_inst(uint64_t PC, int32_t tid, int32_t logDest,
  int64_t destValue, int32_t do_not_check, int32_t isLoad, int32_t isStore) {
    arch_inst_t *db_ptr;
    Thread *THREAD = NULL;

    if (!do_not_check) {
        int32_t i;
        vector<Thread*>::iterator thread_iter = PROCESS->thread_vector.begin();

        for (i=0;i<tid;i++)
            thread_iter++;
        THREAD = *thread_iter;

        db_ptr = THREAD->pop_inst_debug_queue();
        if (db_ptr->is_store) {
            committed_stores.push_back(*db_ptr);
        }

        // Here, the goal is to detect if incorrect branch path was taken in RTL
        // simulation. To detect incorrect branch path, checking PCs might not
        // be necessarily correct because the PC field of ROB might not be used
        // for RTL functionality correctness. So, to decide whether the
        // incorrect branch path was taken, either of the following must be true:
        // 1. If there is only one bit difference, check if the PCs of two
        //    consecutive retiring instructions mismatch the ones in funcsim
        // 2. If there are more than one bits difference, it is retiring an
        //    instruction from the incorrect path (it is assumed that only
        //    single bit flip is possible). This option is to enable earlier detection.
        if (PC != db_ptr->pc) { // check if PC mismatches

            if (!pc_was_incorrect && count_bit_diff(PC, db_ptr->pc) == 1) {
                pc_was_incorrect = true;
                std::cout << "[fi_dpi] Single bit difference on PCrtl and PCfuncsim detected..." << std::endl;
            }
            else {
                std::cout << "[fi_dpi] PCs mismatch:" << std::endl;
                printf("  [int] [rtl]    PC: %" PRIx64 " logDest: %d destValue: %" PRIx64 "\n", PC, logDest, destValue);
                printf("  [int] [func]   PC: %" PRIx64 " logDest: %d destValue: %" PRIx64 "\n", db_ptr->pc,
                              db_ptr->rdst[0].n,
                              db_ptr->rdst[0].value);
                MIPSARCH->print_disasm_with_values(stdout, db_ptr); printf("\n");

                // increment failure count
                failure_detected();

                // revert pc_was_incorrect flag
                pc_was_incorrect = false;

                return(1);
            }
        }
        else {
            if (pc_was_incorrect)
                std::cout << "[fi_dpi] The fault in PC was benign..." << std::endl;

            pc_was_incorrect = false;
        }
    }
    return(0);
}

extern "C" int32_t fi_dpi_check_store(uint64_t addr, uint64_t data, int32_t size) {
    arch_inst_t inst = committed_stores[0];
    uint32_t num_bytes = 0;
    uint64_t funcsim_data = inst.rsrc[1].value;
    uint64_t mask;

    // pop the head of committed_stores
    if (committed_stores.size() == 0) {
        std::cout << "[fi_dpi] WARNING: unexpected empty committed_stores.";
        std::cout << std::endl;
        return 0;
    }
    committed_stores.erase(committed_stores.begin());

    // adjust information before compare
    switch (size) {
        case 0: num_bytes = 1; mask = 0xFF; break;
        case 1: num_bytes = 2; mask = 0xFFFF; break;
        case 2: num_bytes = 4; mask = 0xFFFFFFFF; break;
        case 3:
        default: num_bytes = 8; mask = 0xFFFFFFFFFFFFFFFF; break;
    }

    funcsim_data = funcsim_data & mask;
    switch (inst.opcode) {
        case SWR:
            data = (data >> ((addr&0x3)*8));
            funcsim_data &= mask >> ((addr&0x3)*8);
            num_bytes -= addr&0x3;
        case SWL:
            data &= mask >> ((addr&0x3)*8);
            funcsim_data &= mask >> ((addr&0x3)*8);
            num_bytes = (addr&0x3)+1;
            break;
        case SDR:
            data = (data >> ((addr&0x7)*8));
            funcsim_data &= mask >> ((addr&0x7)*8);
            num_bytes -= addr&0x7;
            break;
        case SDL:
            data &= mask >> ((addr&0x7)*8);
            funcsim_data &= mask >> ((addr&0x7)*8);
            num_bytes = (addr&0x7)+1;
            break;
        default:
            data = (data >> ((addr&0x7)*8)) & mask;
            break;
    }

    // check if both funcsim and RTLsim store the same data
    if (funcsim_data != data || addr != inst.addr ||
      inst.num_bytes != num_bytes) {
        std::cout << "[fi_dpi] Stores mismatch:" << std::endl;
        std::cout << std::hex;
        // RTL store information
        std::cout << "  RTL-sim is storing data " << data;
        std::cout << " at address " << addr;
        std::cout << " with size " << num_bytes;
        std::cout << std::endl;
        // funcsim store information
        std::cout << "  Funcsim is storing data " << funcsim_data;
        std::cout << " at address " << inst.addr;
        std::cout << " with size " << inst.num_bytes;
        std::cout << std::endl;

        // increment failure count
        failure_detected();

        return(1);
    }

    return(0);
}

extern "C" void fi_dpi_handle_recovery() {
    // STQ in RTL was cleared. Do the same here.
    committed_stores.clear();
}

extern "C" void fi_dpi_handle_braindead() {
    failure_detected();
}

extern "C" void fi_dpi_handle_unexpected_syscall() {
    failure_detected();
}

extern "C" void fi_analyze() {
    std::ofstream outfile;
    outfile.open(FULL_PATH("results/summary.log"));

    outfile << "fault_cnt = " << fault_cnt << std::endl;
    outfile << "failure_cnt = " << failure_cnt << std::endl;

    uint64_t max_dur = 0;
    uint64_t min_dur = FAULT_PERIOD;
    double avg_dur = 0.0;

    if (failure_durations.size() > 0) {
        outfile << "dur_vector =";
        std::vector<uint64_t>::iterator it = failure_durations.begin();
        while (it != failure_durations.end()) {
            if (*it < min_dur)
                min_dur = *it;
            if (*it > max_dur)
                max_dur = *it;
            avg_dur += *it; // accumulate durations

            outfile << " " << *it;

            // it point to the next entry
            it++;
        }
        outfile << std::endl;

        // get the average
        avg_dur /= failure_durations.size();
    }

    outfile << "max_dur = " << max_dur << std::endl;
    outfile << "min_dur = " << min_dur << std::endl;
    outfile << "avg_dur = " << avg_dur << std::endl;

    // log simulation duration
    uint64_t duration = time(NULL) - start_time;

    outfile << std::dec; // set interger format to decimal
    outfile << std::endl;
    outfile << "simulation_duration = " << duration << " seconds (";
    outfile << (duration/3600)<< " hour(s) " << ((duration%3600)/60) <<
        " minute(s) " << (duration%60) << " second(s)";
    outfile << ")" << std::endl;

    outfile.close();
}

/* end of file */
