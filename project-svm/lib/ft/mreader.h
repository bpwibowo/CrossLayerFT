/**********************************************************
 *
 * FT metadata reader header code
 *
 * First author: Bagus Wibowo
 * created: 06/02/2014
 **********************************************************/

#ifndef _MREADER_H_
#define _MREADER_H_

#include <fstream>
#include <map>
#include <inttypes.h>
#include <mips64.h>

typedef struct {
    uint64_t kill_vector : 31;
    uint64_t is_branch_type : 1;
} metadata_cfnt_t;

typedef struct {
    uint64_t kill_vector : 31;
    uint64_t is_first_instruction_in_function : 1;
} metadata_cft_t;

typedef struct {
    uint64_t metadata_in_ubits_default : 1;
    uint64_t metadata_in_ubits_res1 : 1;
    uint64_t metadata_in_ubits_res2 : 1;
    uint64_t metadata_in_ubits_res3 : 1;
    uint64_t metadata_in_ubits_res4 : 1;
    uint64_t metadata_in_ubits_res5 : 1;
    uint64_t metadata_in_ubits_res6 : 1;
    uint64_t metadata_in_ubits_res7 : 1;
    uint64_t nops_added : 8;
    uint64_t resv_added : 8;
    uint64_t nops_id: 32;
} metadata_unused_bit_info_t;

typedef struct {
    uint64_t nops_added : 8;
    uint64_t nops_id: 32;
    uint64_t reserved: 23;
    uint64_t live_vector_info_present: 1;
} metadata_unused_bit_info_schemes_t;

typedef struct {
    uint64_t live_in_count_first : 8;
    uint64_t live_in_count_avg: 8;
    uint64_t live_out_count_first : 8;
    uint64_t live_out_count_avg: 8;
} metadata_reg_live_count_t;

typedef struct {
    uint64_t important : 1;
    uint64_t decr_rd : 1;
    uint64_t decr_rrd : 5;
    uint64_t decr_rt : 1;
    uint64_t decr_rrt : 5;
    uint64_t decr_rs : 1;
    uint64_t decr_rrs : 5;
    uint64_t decr_lo : 1;
    uint64_t decr_hi : 1;
    uint64_t incr_exp : 1;
    uint64_t incr_rexp : 5;
    uint64_t incr_lo : 1;
    uint64_t incr_hi : 1;
} metadata_rw_t;

struct Metadata {
    metadata_rw_t rw;
    metadata_cfnt_t cf_ntaken;
    metadata_cft_t cf_taken;
    uint64_t ld_sd_callee;
    uint64_t func_pc;
    std::string func_name;

    Metadata() { // setting default values
        rw.important = 0;
        rw.decr_rd = 0;
        rw.decr_rrd = 0;
        rw.decr_rt = 0;
        rw.decr_rrt = 0;
        rw.decr_rs = 0;
        rw.decr_rrs = 0;
        rw.decr_lo = 0;
        rw.decr_hi = 0;
        rw.incr_exp = 0;
        rw.incr_rexp = 0;
        rw.incr_lo = 0;
        rw.incr_hi = 0;
        cf_ntaken.kill_vector = 0;
        cf_ntaken.is_branch_type = 0;
        cf_taken.kill_vector = 0;
        cf_taken.is_first_instruction_in_function = 0;
        ld_sd_callee = 0;
        func_pc = 0;
        func_name = "function_name_not_in_metadata";
    }
};

struct Metadata2 {
    uint64_t bb_pc;
    uint64_t live_vector;
    metadata_unused_bit_info_t  unused_bit_info;
    metadata_unused_bit_info_schemes_t unused_bit_info_scheme_1, unused_bit_info_scheme_2, unused_bit_info_scheme_3, unused_bit_info_scheme_4;
    metadata_unused_bit_info_schemes_t unused_bit_info_scheme_5, unused_bit_info_boundary;
    metadata_reg_live_count_t reg_live_count;
    uint8_t ubits_count;

    Metadata2() { // setting default values
        bb_pc = 0;
        live_vector = 0x7ffffffffull;
        unused_bit_info.metadata_in_ubits_default = 1;
        unused_bit_info.metadata_in_ubits_res1 = 1;
        unused_bit_info.metadata_in_ubits_res2 = 1;
        unused_bit_info.metadata_in_ubits_res3 = 1;
        unused_bit_info.metadata_in_ubits_res4 = 1;
        unused_bit_info.metadata_in_ubits_res5 = 1;
        unused_bit_info.metadata_in_ubits_res6 = 1;
        unused_bit_info.metadata_in_ubits_res7 = 1;
        unused_bit_info.nops_added = 0;
        unused_bit_info.resv_added = 0;
        unused_bit_info.nops_id = 0;
        unused_bit_info_scheme_1.nops_added = 0;
        unused_bit_info_scheme_1.nops_id = 0;
        unused_bit_info_scheme_1.reserved = 0;
        unused_bit_info_scheme_1.live_vector_info_present = 1;
        unused_bit_info_scheme_2.nops_added = 0;
        unused_bit_info_scheme_2.nops_id = 0;
        unused_bit_info_scheme_2.reserved = 0;
        unused_bit_info_scheme_2.live_vector_info_present = 1;
        unused_bit_info_scheme_3.nops_added = 0;
        unused_bit_info_scheme_3.nops_id = 0;
        unused_bit_info_scheme_3.reserved = 0;
        unused_bit_info_scheme_3.live_vector_info_present = 1;
        unused_bit_info_scheme_4.nops_added = 0;
        unused_bit_info_scheme_4.nops_id = 0;
        unused_bit_info_scheme_4.reserved = 0;
        unused_bit_info_scheme_4.live_vector_info_present = 1;
        unused_bit_info_scheme_5.nops_added = 0;
        unused_bit_info_scheme_5.nops_id = 0;
        unused_bit_info_scheme_5.reserved = 0;
        unused_bit_info_scheme_5.live_vector_info_present = 1;
        unused_bit_info_boundary.nops_added = 0;
        unused_bit_info_boundary.nops_id = 0;
        unused_bit_info_boundary.reserved = 0;
        unused_bit_info_boundary.live_vector_info_present = 1;
        reg_live_count.live_in_count_first = 0;
        reg_live_count.live_in_count_avg = 0;
        reg_live_count.live_out_count_first = 0;
        reg_live_count.live_out_count_avg = 0;
        ubits_count = 0;
    }
};

typedef enum {
    NOP_INJECTION_SCHEME_DEFAULT = 0,
    NOP_INJECTION_SCHEME_1,
    NOP_INJECTION_SCHEME_2,
    NOP_INJECTION_SCHEME_3,
    NOP_INJECTION_SCHEME_4,
    NOP_INJECTION_SCHEME_5,
    NOP_INJECTION_BB_BOUNDARY,
    NOP_INJECTION_NONE
} NopInjectionScheme;

class MReader {
  public:
    MReader(const char *path);
    ~MReader();

    struct Metadata GetMetadata(uint64_t pc);
    struct Metadata2 GetMetadata2(uint64_t pc);
    uint64_t GetMetadataBits(uint64_t pc);
    void DisplayMetadata(uint64_t pc);
    void GetKillVector(struct Metadata md, bool is_taken, bool *kill_vector);
    uint64_t GetKillBitVector(struct Metadata md, bool is_taken);
    int64_t GetNopsAdded(struct Metadata2 *pmd2);
    int64_t GetNopsIdx(struct Metadata2 *pmd2);

  private:
    bool ParseLine(std::string line);
    void ParseFile();
    void ParseMetadataBits();

    std::ifstream mfile_;
    std::map<uint64_t, struct Metadata> metadata_;
    std::map<uint64_t, struct Metadata2> metadata2_;
    std::map<uint64_t, uint64_t> metadata_bits_;
};

#endif /* _MREADER_H_ */

/* end of file */
