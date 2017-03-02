/**********************************************************
 *
 * Table of vulnerable bits of structures header file
 *
 * First author: Bagus Wibowo
 * created: 06/03/2014
 **********************************************************/

#ifndef _VBIT_TABLE_H_
#define _VBIT_TABLE_H_

#include <vector>
#include <map>
#include "simlog.h"

typedef enum {
    STRUCTURE_IQ = 0,
    STRUCTURE_ROB,
    STRUCTURE_LQ,
    STRUCTURE_SQ,
    STRUCTURE_RF,
    STRUCTURE_IBUFF,

    STRUCTURES_COUNT, // number of tracked structures

    // below is the list of untracked structures
    STRUCTURE_RMT,
    STRUCTURE_AMT,
    STRUCTURE_FREELIST,
    STRUCTURE_CTQ,
    STRUCTURE_PIPE_F1_F2,
    STRUCTURE_PIPE_F2_DEC,
    STRUCTURE_PIPE_REN_DIS,
    STRUCTURE_PIPE_ISS_RR,
    STRUCTURE_PIPE_RR_EXE,
    STRUCTURE_PIPE_EXE_LSU,
    STRUCTURE_PIPE_FU_WB,
} structure_t;

typedef enum {
    STRUCTURE_FU_SIMPLE = 0,
    STRUCTURE_FU_COMPLEX,
    STRUCTURE_FU_CONTROL,
    STRUCTURE_FU_MEMORY,

    STRUCTURES_FU_COUNT // number of FUs
} structure_fu_t;

typedef struct {
    std::vector<int> unit_sizes;
    std::vector<int> unimp_mask;
    std::vector<int> dispatch_mask;
    std::map< int,std::vector<int> > table;
    uint32_t num_fields;
} VbitTable;

#define INVALID_BIT_COUNT (-1)

void VbitTableInit();

int GetVulnerableBitCount(structure_t type, int opcode, bool important);
int GetVulnerableBitCount(structure_t type, int opcode, bool important, bool dispatch_mode);
int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important);
int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important, bool dispatch_mode);
int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important, int ones_cnt);
int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important, int ones_cnt, bool dispatch_mode);
int GetUnusedBitCount(structure_t type, int opcode);
int GetEntryBitsCount(structure_t type);
int GetTotalBitsCount(structure_t type);
std::string GetStructureName(structure_t type);
std::string GetFuStructureName(structure_fu_t type);

int Approximate(int value, int max_ones);
int Approximate(int value, int max_ones, int underestimate_tolerance);

#endif /* _VBIT_TABLE_H_ */
