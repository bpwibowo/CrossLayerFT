/**********************************************************
 *
 * FT metadata reader implementation code
 *
 * First author: Bagus Wibowo
 * created: 06/02/2014
 **********************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include "mreader.h"
#include "simlog_config.h"
#include "svm.h"

MReader::MReader(const char *path) {
    mfile_.open(path, std::ifstream::in);

    // if file can't be opened, throw an exception
    if (!mfile_.is_open()) {
        std::cout << "[MReader] Error: Could not open file " << path <<
            std::endl;
        throw -1;
    }

    ParseFile();
    ParseMetadataBits();
}

MReader::~MReader() {
    mfile_.close();
}

struct Metadata MReader::GetMetadata(uint64_t pc) {
    struct Metadata md = Metadata();

    if (metadata_.count(pc) > 0) {
        md = metadata_[pc];
    }

    return md;
}

struct Metadata2 MReader::GetMetadata2(uint64_t pc) {
    struct Metadata2 md = Metadata2();

    if (metadata2_.count(pc) > 0) {
        md = metadata2_[pc];
    }

    return md;
}

uint64_t MReader::GetMetadataBits(uint64_t pc) {
    return metadata_bits_[pc];
}

void MReader::DisplayMetadata(uint64_t pc) {
    struct Metadata md = GetMetadata(pc);
    std::cout << std::hex;
    std::cout << pc << std::endl;
    std::cout << "-- RW > important/incr_rd/incr_rrd/incr_rt/incr_rrt/" <<
        "incr_rs/incr_rrs/decr_lo/decr_hi/incr_exp/incr_rexp/incr_lo/incr_hi: "
        << std::endl;;
    std::cout << md.rw.important << "/" << md.rw.decr_rd << "/" <<
        md.rw.decr_rrd << "/" << md.rw.decr_rt << "/" << md.rw.decr_rrt <<
        "/" << md.rw.decr_rs << "/" << md.rw.decr_rrs << "/" << md.rw.decr_lo <<
        "/" << md.rw.decr_hi << "/" << md.rw.incr_exp << "/" <<
        md.rw.incr_rexp << "/" << md.rw.incr_lo << "/" << md.rw.incr_hi <<
        "/" << std::endl;
    std::cout << "-- CF > taken_kill_vector/is_branch/ntaken_kill_vector/" <<
        "is_first_instruction_in_function: " << std::endl;
    std::cout << md.cf_ntaken.kill_vector << "/" <<
        md.cf_ntaken.is_branch_type << "/" << md.cf_taken.kill_vector << "/" <<
        md.cf_taken.is_first_instruction_in_function << std::endl;
}

void MReader::GetKillVector(struct Metadata md, bool is_taken,
    bool *kill_vector) {
    uint64_t metadata_kill_vector = is_taken ? md.cf_taken.kill_vector :
                                               md.cf_ntaken.kill_vector;

    // first, clear the vector flags
    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        kill_vector[i] = false;
    }

    // set kill vector based on metadata
    kill_vector[R_R0] = false; // always zero /* 0  r0 */
    kill_vector[R_AT] = (metadata_kill_vector >> 0)&0x1; /* 1  r1 */
    kill_vector[R_V0] = (metadata_kill_vector >> 1)&0x1; /* 2  r2 */
    kill_vector[R_V1] = (metadata_kill_vector >> 2)&0x1; /* 3  r3 */
    kill_vector[R_A0] = (metadata_kill_vector >> 3)&0x1; /* 4  r4 */
    kill_vector[R_A1] = (metadata_kill_vector >> 4)&0x1; /* 5  r5 */
    kill_vector[R_A2] = (metadata_kill_vector >> 5)&0x1; /* 6  r6 */
    kill_vector[R_A3] = (metadata_kill_vector >> 6)&0x1; /* 7  r7 */
    kill_vector[R_A4] = (metadata_kill_vector >> 7)&0x1; /* 8  r8  */
    kill_vector[R_A5] = (metadata_kill_vector >> 8)&0x1; /* 9  r9  */
    kill_vector[R_A6] = (metadata_kill_vector >> 9)&0x1; /* 10 r10 */
    kill_vector[R_A7] = (metadata_kill_vector >> 10)&0x1; /* 11 r11 */
    kill_vector[R_T0] = (metadata_kill_vector >> 11)&0x1; /* 12 r12 */
    kill_vector[R_T1] = (metadata_kill_vector >> 12)&0x1; /* 13 r13 */
    kill_vector[R_T2] = (metadata_kill_vector >> 13)&0x1; /* 14 r14 */
    kill_vector[R_T3] = (metadata_kill_vector >> 14)&0x1; /* 15 r15 */
    kill_vector[R_S0] = (metadata_kill_vector >> 15)&0x1; /* 16 r16 */
    kill_vector[R_S1] = (metadata_kill_vector >> 16)&0x1; /* 17 r17 */
    kill_vector[R_S2] = (metadata_kill_vector >> 17)&0x1; /* 18 r18 */
    kill_vector[R_S3] = (metadata_kill_vector >> 18)&0x1; /* 19 r19 */
    kill_vector[R_S4] = (metadata_kill_vector >> 19)&0x1; /* 20 r20 */
    kill_vector[R_S5] = (metadata_kill_vector >> 20)&0x1; /* 21 r21 */
    kill_vector[R_S6] = (metadata_kill_vector >> 21)&0x1; /* 22 r22 */
    kill_vector[R_S7] = (metadata_kill_vector >> 22)&0x1; /* 23 r23 */
    kill_vector[R_T8] = (metadata_kill_vector >> 23)&0x1; /* 24 r24 */
    kill_vector[R_T9] = (metadata_kill_vector >> 24)&0x1; /* 25 r25 */
    kill_vector[R_K0] = (metadata_kill_vector >> 25)&0x1; /* 26 r26 */
    kill_vector[R_K1] = (metadata_kill_vector >> 26)&0x1; /* 27 r27 */
    kill_vector[R_GP] = false; /* 28 r28 */
    kill_vector[R_SP] = false; /* 29 r29 */
    kill_vector[R_S8] = (metadata_kill_vector >> 27)&0x1; /* 30 r30 */
    kill_vector[R_RA] = (metadata_kill_vector >> 28)&0x1; /* 31 r31 */
    kill_vector[R_LO] = (metadata_kill_vector >> 29)&0x1; /* 32 lo  */
    kill_vector[R_HI] = (metadata_kill_vector >> 30)&0x1; /* 33 hi  */
}

uint64_t MReader::GetKillBitVector(struct Metadata md, bool is_taken) {
    bool kill_vector[SVM->cfg().arch_size_amt];
    GetKillVector(md, is_taken, kill_vector);

    uint64_t kill_bit_vector = 0;
    for (uint32_t i=0; i<SVM->cfg().arch_size_amt; i++) {
        if (kill_vector[i])
            kill_bit_vector |= (0x1ull << i);
    }

    return kill_bit_vector;
}

int64_t MReader::GetNopsAdded(struct Metadata2 *pmd2) {
    int64_t nops_added = 0; // by default no NOP is added

    if (SVM->cfg().enable_nop_injection) {
        switch(SVM->cfg().nop_injection_scheme) {
            case NOP_INJECTION_SCHEME_1:
                nops_added = pmd2->unused_bit_info_scheme_1.nops_added;
                break;
            case NOP_INJECTION_SCHEME_2:
                nops_added = pmd2->unused_bit_info_scheme_2.nops_added;
                break;
            case NOP_INJECTION_SCHEME_3:
                nops_added = pmd2->unused_bit_info_scheme_3.nops_added;
                break;
            case NOP_INJECTION_SCHEME_4:
                nops_added = pmd2->unused_bit_info_scheme_4.nops_added;
                break;
            case NOP_INJECTION_SCHEME_5:
                nops_added = pmd2->unused_bit_info_scheme_5.nops_added;
                break;
            case NOP_INJECTION_BB_BOUNDARY:
                nops_added = pmd2->unused_bit_info_boundary.nops_added;
                break;
            case NOP_INJECTION_NONE:
                nops_added = 0;
                break;
            case NOP_INJECTION_SCHEME_DEFAULT:
            default:
                nops_added = pmd2->unused_bit_info.nops_added;
                break;
        }
    }

    return nops_added;
}

int64_t MReader::GetNopsIdx(struct Metadata2 *pmd2) {
    int64_t nops_id = 0; // by default start index is zero

    if (SVM->cfg().enable_nop_injection) {
        switch(SVM->cfg().nop_injection_scheme) {
            case NOP_INJECTION_SCHEME_1:
                nops_id = pmd2->unused_bit_info_scheme_1.nops_id;
                break;
            case NOP_INJECTION_SCHEME_2:
                nops_id = pmd2->unused_bit_info_scheme_2.nops_id;
                break;
            case NOP_INJECTION_SCHEME_3:
                nops_id = pmd2->unused_bit_info_scheme_3.nops_id;
                break;
            case NOP_INJECTION_SCHEME_4:
                nops_id = pmd2->unused_bit_info_scheme_4.nops_id;
                break;
            case NOP_INJECTION_SCHEME_5:
                nops_id = pmd2->unused_bit_info_scheme_5.nops_id;
                break;
            case NOP_INJECTION_BB_BOUNDARY:
                nops_id = pmd2->unused_bit_info_boundary.nops_id;
                break;
            case NOP_INJECTION_NONE:
                nops_id = 0;
                break;
            case NOP_INJECTION_SCHEME_DEFAULT:
            default:
                nops_id = pmd2->unused_bit_info.nops_id;
                break;
        }
    }

    return nops_id;
}

/* -------------------- private function definitions ------------------------ */

static std::vector<std::string> &split(const std::string &s, char delim,
    std::vector<std::string> &elems) {

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(),
                std::find_if(s.begin(),
                             s.end(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

static uint64_t String2Int(std::string str) {
    uint64_t value;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> value;
    return value;
}

bool MReader::ParseLine(std::string line) {
    union {
        uint64_t uint64;
        uint8_t uint8;
        metadata_rw_t rw;
        metadata_cfnt_t cfnt;
        metadata_cft_t cft;
        metadata_unused_bit_info_t unused_bit_info;
        metadata_unused_bit_info_schemes_t unused_bit_info_scheme;
        metadata_reg_live_count_t reg_live_count;
    } temp;

    std::vector<std::string> tokens = split(line, ':');
    if (tokens.size() < 2)
        return false;
    uint64_t id = String2Int(trim(split(line, ':')[0]));

    std::string value = trim(split(line, ':')[1]);
    tokens = split(value, ' ');
    if (tokens.size() < 8)
        return false;
    temp.uint64 = String2Int( trim(tokens[0]) );
    metadata_[id].rw = temp.rw;
    temp.uint64 = String2Int( trim(tokens[1]) );
    metadata_[id].cf_ntaken = temp.cfnt;
    temp.uint64 = String2Int( trim(tokens[2]) );
    metadata_[id].cf_taken = temp.cft;
    metadata_[id].ld_sd_callee = String2Int( trim(tokens[3]) );
    metadata_[id].func_pc = String2Int( trim(tokens[4]) );
    metadata_[id].func_name = trim(tokens[5]);
    metadata2_[id].bb_pc = String2Int( trim(tokens[6]) );
    metadata2_[id].live_vector = String2Int( trim(tokens[7]) );
    if (tokens.size() > 8) {
        temp.uint64 = String2Int( trim(tokens[8]) );
        metadata2_[id].unused_bit_info = temp.unused_bit_info;
    }
    if (tokens.size() > 9) {
        temp.uint64 = String2Int( trim(tokens[9]) );
        metadata2_[id].unused_bit_info_scheme_1 = temp.unused_bit_info_scheme;
    }
    if (tokens.size() > 10) {
        temp.uint64 = String2Int( trim(tokens[10]) );
        metadata2_[id].reg_live_count = temp.reg_live_count;
    }
    if (tokens.size() > 11) {
        temp.uint64 = String2Int( trim(tokens[11]) );
        metadata2_[id].unused_bit_info_scheme_2 = temp.unused_bit_info_scheme;
    }
    if (tokens.size() > 12) {
        temp.uint64 = String2Int( trim(tokens[12]) );
        metadata2_[id].unused_bit_info_scheme_3 = temp.unused_bit_info_scheme;
    }
    if (tokens.size() > 13) {
        temp.uint64 = String2Int( trim(tokens[13]) );
        metadata2_[id].unused_bit_info_scheme_4 = temp.unused_bit_info_scheme;
    }
    if (tokens.size() > 14) {
        temp.uint64 = String2Int( trim(tokens[14]) );
        metadata2_[id].unused_bit_info_scheme_5 = temp.unused_bit_info_scheme;
    }
    if (tokens.size() > 15) {
        temp.uint64 = String2Int( trim(tokens[15]) );
        metadata2_[id].unused_bit_info_boundary = temp.unused_bit_info_scheme;
    }
    if (tokens.size() > 16) {
        temp.uint64 = String2Int( trim(tokens[16]) );
        metadata2_[id].ubits_count = temp.uint8;
    }
    return true;
}

void MReader::ParseFile() {
    std::string line;
    while (getline(mfile_, line) ) { ParseLine(line); }
}

void MReader::ParseMetadataBits() {
    std::map<uint64_t, struct Metadata2>::iterator it, it_next, it_begin;

    std::cout << "Parsing Metadata Bits ..." << std::endl;
    it = metadata2_.begin();

    it_begin = it;
    uint64_t curr_bb_pc = (uint64_t)-1;
    uint64_t curr_bb_ubits_cnt = 0;
    std::deque<bool> importance;
    uint64_t rc = (uint64_t)-1;
    bool rc_posted = false;

    while (it != metadata2_.end()) {
        uint64_t pc = it->first;
        struct Metadata md = metadata_[pc];
        struct Metadata2 md2 = it->second;

        if (curr_bb_pc == (uint64_t)-1) {
            curr_bb_pc = md2.bb_pc;
            rc = md2.reg_live_count.live_in_count_first;
        }

        // update info based on current instruction's metadata
        curr_bb_ubits_cnt += md2.ubits_count;
        int64_t nops_added = GetNopsAdded(&md2);
        if (nops_added) {
            curr_bb_ubits_cnt += nops_added*13;
        }
        importance.push_back(md.rw.important);

        // point to the next pc
        it++;

        if (it != metadata2_.end()) {
            pc = it->first;
            md = metadata_[pc];
            md2 = it->second;
        }
        if (md2.bb_pc != curr_bb_pc || it == metadata2_.end()) {
            // BB was ended
            while (it_begin != it) {
                uint64_t pc_b = it_begin->first;
                struct Metadata md_b = metadata_[pc_b];
                struct Metadata2 md2_b = it_begin->second;

                int64_t ubits_b = (int64_t) md2_b.ubits_count;
                ubits_b += GetNopsAdded(&md2_b)*13;
                uint64_t md_bits = 0;
                uint64_t md_bits_pointer = 0;

                if (ubits_b) {
                    if (!rc_posted) {
                        // assume RC fits in the ubits
                        md_bits = rc & 0x1F; // 5-bits
                        rc_posted = true;
                        md_bits_pointer += 5;
                    }

                    while (ubits_b - md_bits_pointer > 0 && importance.size() > 0) {
                        bool important_bit = importance.front();
                        importance.pop_front();
                        if (!important_bit) {
                            md_bits |= 0x1ull << md_bits_pointer;
                        }
                        md_bits_pointer++;
                    }

                    metadata_bits_[pc_b] = md_bits;
                }

                it_begin++;
            }

            // prepare for new basic block
            if (it != metadata2_.end()) { // do this unless the last instruction
                curr_bb_pc = md2.bb_pc;
                importance.clear();
                rc = (uint64_t)md2.reg_live_count.live_in_count_first;
                rc_posted = false;
                curr_bb_ubits_cnt = 0;
            }
        }
    }
}

/* end of file */
