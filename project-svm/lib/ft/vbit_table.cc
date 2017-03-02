/**********************************************************
 *
 * Table of vulnerable bits of structures implementation file
 *
 * First author: Bagus Wibowo
 * created: 06/03/2014
 **********************************************************/

#include <iostream>
#include <cstdarg>
#include "mips64r2_encodings.h"
#include "vbit_table.h"
#include "svm.h"

VbitTable vt_iq;
VbitTable vt_rob;
VbitTable vt_lq;
VbitTable vt_sq;
VbitTable vt_ibuff;
VbitTable vt_ctq;
VbitTable vt_pipe_f1_f2;
VbitTable vt_pipe_f2_dec;
VbitTable vt_pipe_ren_dis;
VbitTable vt_pipe_iss_rr;
VbitTable vt_pipe_rr_exe;
VbitTable vt_pipe_exe_lsu;
VbitTable vt_pipe_fu_wb;

void AppendVte(VbitTable *pvte, int id, ...) {
    va_list args;

    va_start(args, id);
    for (uint32_t n=0; n<pvte->num_fields; n++) {
        pvte->table[id].push_back(va_arg(args, int));
    }
    va_end(args);
}

void VtAddField(VbitTable *pvte, int field_size, int unimp_mask,
                int dispatch_mask) {
    pvte->unit_sizes.push_back(field_size);
    pvte->unimp_mask.push_back(unimp_mask);
    pvte->dispatch_mask.push_back(dispatch_mask);
    pvte->num_fields = pvte->unit_sizes.size();
}

void VtAddField(VbitTable *pvte, int field_size, int unimp_mask) {
    VtAddField(pvte, field_size, unimp_mask, 1);
}

void VbitTableInitIq() {
    // Initialize vbit table for IQ
                       // size                           unimportant mask
    VtAddField(&vt_iq, SVM->cfg().arch_size_pc,          0); // pc
    VtAddField(&vt_iq, SVM->cfg().core_width_opcode,     1); // opcode
    VtAddField(&vt_iq, SVM->cfg().core_size_prf_int_log, 1); // dst
    VtAddField(&vt_iq, 1,                                1); // dst_valid
    VtAddField(&vt_iq, SVM->cfg().core_size_prf_int_log, 0); // src1
    VtAddField(&vt_iq, SVM->cfg().core_size_prf_int_log, 0); // src2
    VtAddField(&vt_iq, SVM->cfg().core_size_lq_log,      1); // lsqID
    VtAddField(&vt_iq, SVM->cfg().core_size_al_log,      1); // alID
    VtAddField(&vt_iq, SVM->cfg().arch_size_immediate,   0); // immed
    VtAddField(&vt_iq, 1,                                1); // isSimple
    VtAddField(&vt_iq, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_iq, SVM->cfg().core_size_ctiq_log,    1); // ctiID
    VtAddField(&vt_iq, SVM->cfg().arch_size_pc,          0); // predNPC
    VtAddField(&vt_iq, 1,                                0); // predDir
    VtAddField(&vt_iq, 1,                                1); // valid
    VtAddField(&vt_iq, 1,                                1); // Desttype
    VtAddField(&vt_iq, 1,                                1); // Src1type
    VtAddField(&vt_iq, 1,                                1); // Src2type
    VtAddField(&vt_iq, 1,                                1); // isMove
    VtAddField(&vt_iq, 1,                                1); // isSplitStore_u
    VtAddField(&vt_iq, 1,                                1); // isSplitStore_l
    VtAddField(&vt_iq, 3,                                1); // cc

                                   // pc,opcode,dst,dst_valid,src1,src2,lsqID,alID,immed,isSimple,ctrlType,ctiID,predNPC,predDir,valid,Desttype,Src1type,Src2type,isMove,isSplitStore_u,isSplitStore_l,cc
    AppendVte(&vt_iq, OPCODE_LB,       0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LB,       0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LBU,      0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LH,       0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LHU,      0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LW,       0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LWU,      0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SB,       0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SH,       0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SW,       0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LD,       0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SD,       0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LWL,      0,     1,  1,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LWR,      0,     1,  1,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LDL,      0,     1,  1,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LDR,      0,     1,  1,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SWL,      0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SWR,      0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SDL,      0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SDR,      0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LL,       0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_LLD,      0,     1,  1,        1,   1,   0,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SC_0,     0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SC_1,     0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SCD_0,    0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_SCD_1,    0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ADDI,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ADDIU,    0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ANDI,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_LUI,      0,     1,  1,        1,   0,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ORI,      0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SLTI,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SLTIU,    0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_XORI,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DADDI,    0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DADDIU,   0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ADD,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ADDU,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_AND,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_NOR,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_OR,       0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SLT,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SLTU,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SUB,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SUBU,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_XOR,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DADD,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DADDU,    0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSUB,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSUBU,    0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_CLO,      0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_CLZ,      0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ROTR,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_ROTRV,    0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SLL,      0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SLLV,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SRA,      0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SRAV,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SRL,      0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SRLV,     0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSLL,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSLL32,   0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSLLV,    0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSRL,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSRL32,   0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSRLV,    0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSRA,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSRA32,   0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DSRAV,    0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DIV_LO,   0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DIV_HI,   0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DIVU_LO,  0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DIVU_HI,  0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DDIV_LO,  0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DDIV_HI,  0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DDIVU_LO, 0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DDIVU_HI, 0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MADD,     0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MADDU,    0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MFHI,     0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MFLO,     0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MSUB,     0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MSUBU,    0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MTHI,     0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MTLO,     0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MUL,      0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MULT_LO,  0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MULT_HI,  0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MULTU_LO, 0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MULTU_HI, 0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DMULT_LO, 0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DMULT_HI, 0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DMULTU_LO,0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DMULTU_HI,0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_J,        1,     1,  0,        1,   0,   0,    0,   1,    0,       0,       1,    1,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_JAL,      1,     1,  1,        1,   0,   0,    0,   1,    0,       0,       1,    1,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_JALX,     1,     1,  1,        1,   0,   0,    0,   1,    0,       0,       1,    1,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_JALR,     1,     1,  1,        1,   1,   0,    0,   1,    0,       0,       1,    1,      1,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_JALR_HB,  1,     1,  1,        1,   1,   0,    0,   1,    0,       0,       1,    1,      1,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_JR,       1,     1,  0,        1,   1,   0,    0,   1,    0,       0,       1,    1,      1,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_JR_HB,    1,     1,  0,        1,   1,   0,    0,   1,    0,       0,       1,    1,      1,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BEQ,      1,     1,  0,        1,   1,   1,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BNE,      1,     1,  0,        1,   1,   1,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BGEZ,     1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BGEZAL,   1,     1,  1,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BGTZ,     1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BLEZ,     1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BLTZ,     1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BLTZAL,   1,     1,  1,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BEQL,     1,     1,  0,        1,   1,   1,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BGEZALL,  1,     1,  1,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BGEZL,    1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BGTZL,    1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BLEZL,    1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BLTZALL,  1,     1,  1,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BLTZL,    1,     1,  0,        1,   1,   0,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BNEL,     1,     1,  0,        1,   1,   1,    0,   1,    1,       0,       1,    1,      1,      1,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SYNC,     0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SYNCI,    0,     1,  0,        1,   1,   0,    0,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_BREAK,    0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SYSCALL,  0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TEQ,      0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TGE,      0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TGEU,     0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TLT,      0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TLTU,     0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TNE,      0,     1,  0,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TEQI,     0,     1,  0,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TGEI,     0,     1,  0,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TGEIU,    0,     1,  0,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TLTI,     0,     1,  0,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TLTIU,    0,     1,  0,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_TNEI,     0,     1,  0,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_PREF,     0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_PREFX,    0,     1,  0,        1,   1,   1,    1,   1,    1,       0,       0,    0,      0,      0,    1,       1,       1,       1,     1,             1,             1, 0);
    AppendVte(&vt_iq, OPCODE_NOP,      0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SSNOP,    0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_EHB,      0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_EXT,      0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_INS,      0,     1,  1,        1,   1,   1,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DEXT,     0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DEXTM,    0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DEXTU,    0,     1,  1,        1,   1,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DINS,     0,     1,  1,        1,   1,   1,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DINSM,    0,     1,  1,        1,   1,   1,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_DINSU,    0,     1,  1,        1,   1,   1,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SEB,      0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_SEH,      0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_WSBH,     0,     1,  1,        1,   1,   0,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_RDHWR,    0,     1,  1,        1,   0,   0,    0,   1,    1,       1,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MOVN_0,   0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MOVN_1,   0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MOVZ_0,   0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_MOVZ_1,   0,     1,  1,        1,   1,   1,    0,   1,    0,       1,       0,    0,      0,      0,    1,       1,       1,       1,     0,             0,             0, 0);
    AppendVte(&vt_iq, OPCODE_UNIMPL,   0,     1,  0,        1,   0,   0,    0,   1,    0,       0,       0,    0,      0,      0,    1,       1,       0,       0,     0,             0,             0, 0);
}

void VbitTableInitRob() {
    // Initialize vbit table for ROB
                        // size                           unimp_mask  dispatch_mask
    VtAddField(&vt_rob, SVM->cfg().arch_size_pc,          1,          1); // pc
    VtAddField(&vt_rob, SVM->cfg().arch_size_amt_log,     1,          1); // logDest
    VtAddField(&vt_rob, SVM->cfg().core_size_prf_int_log, 1,          1); // phyDest
    VtAddField(&vt_rob, 1,                                1,          1); // phyDestValid
    VtAddField(&vt_rob, 1,                                1,          1); // Desttype
    VtAddField(&vt_rob, 1,                                1,          1); // isLoad
    VtAddField(&vt_rob, 1,                                1,          1); // isStore
    VtAddField(&vt_rob, SVM->cfg().core_size_exe_flags,   1,          0); // exeflags
    VtAddField(&vt_rob, 1,                                1,          1); // readyflag
    VtAddField(&vt_rob, SVM->cfg().arch_size_pc,          1,          0); // nextPC
    VtAddField(&vt_rob, 1,                                1,          0); // violflag

                                     // pc,logDest,phyDest,phyDestValid,Desttype,isLoad,isStore,exeflags,readyflag,nextPC,violflag
    AppendVte(&vt_rob, OPCODE_LB,       1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LBU,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LH,       1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LHU,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LW,       1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LWU,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SB,       1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SH,       1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SW,       1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LD,       1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SD,       1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LWL,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LWR,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LDL,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LDR,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SWL,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SWR,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SDL,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SDR,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LL,       1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LLD,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SC_0,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SC_1,     1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SCD_0,    1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SCD_1,    1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ADDI,     1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ADDIU,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ANDI,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_LUI,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ORI,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SLTI,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SLTIU,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_XORI,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DADDI,    1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DADDIU,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ADD,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ADDU,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_AND,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_NOR,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_OR,       0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SLT,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SLTU,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SUB,      1,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SUBU,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_XOR,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DADD,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DADDU,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSUB,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSUBU,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_CLO,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_CLZ,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ROTR,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_ROTRV,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SLL,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SLLV,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SRA,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SRAV,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SRL,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SRLV,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSLL,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSLL32,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSLLV,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSRL,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSRL32,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSRLV,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSRA,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSRA32,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DSRAV,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DIV_LO,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DIV_HI,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DIVU_LO,  0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DIVU_HI,  0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DDIV_LO,  0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DDIV_HI,  0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DDIVU_LO, 0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DDIVU_HI, 0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MADD,     0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MADDU,    0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MFHI,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MFLO,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MSUB,     0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MSUBU,    0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MTHI,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MTLO,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MUL,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MULT_LO,  0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MULT_HI,  0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MULTU_LO, 0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MULTU_HI, 0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DMULT_LO, 0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DMULT_HI, 0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DMULTU_LO,0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DMULTU_HI,0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_J,        0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_JAL,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_JALX,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_JALR,     0,      1,      1,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_JALR_HB,  0,      1,      1,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_JR,       0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_JR_HB,    0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BEQ,      0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BNE,      0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BGEZ,     0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BGEZAL,   0,      1,      1,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BGTZ,     0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BLEZ,     0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BLTZ,     0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BLTZAL,   0,      1,      1,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BEQL,     0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BGEZALL,  0,      1,      1,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BGEZL,    0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BGTZL,    0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BLEZL,    0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BLTZALL,  0,      1,      1,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BLTZL,    0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_BNEL,     0,      0,      0,           1,       1,     1,      1,       1,        1,     1,       1);
    AppendVte(&vt_rob, OPCODE_SYNC,     0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SYNCI,    0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_BREAK,    1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SYSCALL,  1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TEQ,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TGE,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TGEU,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TLT,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TLTU,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TNE,      1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TEQI,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TGEI,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TGEIU,    1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TLTI,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TLTIU,    1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_TNEI,     1,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_PREF,     0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_PREFX,    0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_NOP,      0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SSNOP,    0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_EHB,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_EXT,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_INS,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DEXT,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DEXTM,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DEXTU,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DINS,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DINSM,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_DINSU,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SEB,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_SEH,      0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_WSBH,     0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_RDHWR,    0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MOVN_0,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MOVN_1,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MOVZ_0,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_MOVZ_1,   0,      1,      1,           1,       1,     1,      1,       1,        1,     0,       1);
    AppendVte(&vt_rob, OPCODE_UNIMPL,   0,      0,      0,           1,       1,     1,      1,       1,        1,     0,       1);
}

void VbitTableInitLq() {
    // Initialize vbit table for LQ
                       // size                           unimp_mask  dispatch_mask
    VtAddField(&vt_lq, SVM->cfg().arch_size_addr,        1,          0); // addr
    VtAddField(&vt_lq, 1,                                1,          1); // addrValid
    VtAddField(&vt_lq, SVM->cfg().core_ldst_types_log,   0,          0); // size
    VtAddField(&vt_lq, 1,                                1,          0); // writeback
    VtAddField(&vt_lq, SVM->cfg().core_size_exe_flags,   1,          0); // lqflags
    VtAddField(&vt_lq, SVM->cfg().core_size_prf_int_log, 1,          0); // phydest
    VtAddField(&vt_lq, SVM->cfg().core_size_al_log,      1,          1); // alID
    VtAddField(&vt_lq, 1,                                0,          0); // isInsertLeft
    VtAddField(&vt_lq, 1,                                0,          0); // isInsertRight
    VtAddField(&vt_lq, SVM->cfg().arch_size_data,        0,          0); // src2Data

                                   // addr,addrValid,size,writeback,lqflags,phydest,alID,isInsertLeft,isInsertRight,src2Data
    AppendVte(&vt_lq, OPCODE_LB,         1,        1,   1,        1,      1,      1,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_LBU,        1,        1,   1,        1,      1,      1,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_LH,         1,        1,   1,        1,      1,      1,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_LHU,        1,        1,   1,        1,      1,      1,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_LW,         1,        1,   1,        1,      1,      1,   1,           1,            1,       0);
    AppendVte(&vt_lq, OPCODE_LWU,        1,        1,   1,        1,      1,      1,   1,           1,            1,       0);
    AppendVte(&vt_lq, OPCODE_LD,         1,        1,   1,        1,      1,      1,   1,           1,            1,       0);
    AppendVte(&vt_lq, OPCODE_LWL,        1,        1,   1,        1,      1,      1,   1,           1,            1,       1);
    AppendVte(&vt_lq, OPCODE_LWR,        1,        1,   1,        1,      1,      1,   1,           1,            1,       1);
    AppendVte(&vt_lq, OPCODE_LDL,        1,        1,   1,        1,      1,      1,   1,           1,            1,       1);
    AppendVte(&vt_lq, OPCODE_LDR,        1,        1,   1,        1,      1,      1,   1,           1,            1,       1);
    AppendVte(&vt_lq, OPCODE_LL,         1,        1,   1,        1,      1,      1,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_LLD,        1,        1,   1,        1,      1,      1,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_PREF,       0,        1,   0,        0,      1,      0,   1,           0,            0,       0);
    AppendVte(&vt_lq, OPCODE_PREFX,      0,        1,   0,        0,      1,      0,   1,           0,            0,       0);
}

void VbitTableInitSq() {
    // Initialize vbit table for SQ
                       // size                         unimp_mask  dispatch_mask
    VtAddField(&vt_sq, SVM->cfg().core_size_lq_log,    1,          1); // followingLD
    VtAddField(&vt_sq, 1,                              1,          1); // addrValid
    VtAddField(&vt_sq, SVM->cfg().arch_size_addr,      1,          0); // addr
    VtAddField(&vt_sq, SVM->cfg().core_ldst_types_log, 0,          0); // size
    VtAddField(&vt_sq, 8,                              0,          0); // data_byte
    VtAddField(&vt_sq, 8,                              0,          0); // data_half
    VtAddField(&vt_sq, 16,                             0,          0); // data_word
    VtAddField(&vt_sq, 32,                             0,          0); // data_dword
    VtAddField(&vt_sq, 1,                              1,          0); // commit
    VtAddField(&vt_sq, 2,                              1,          0); // flags

                                   // followingLD,addrValid,addr,size,data_byte,data_half,data_word,data_dword,commit,flags
    AppendVte(&vt_sq, OPCODE_SB,                1,        1,   1,   1,        1,        0,        0,         0,     1,    1);
    AppendVte(&vt_sq, OPCODE_SH,                1,        1,   1,   1,        1,        1,        0,         0,     1,    1);
    AppendVte(&vt_sq, OPCODE_SW,                1,        1,   1,   1,        1,        1,        1,         0,     1,    1);
    AppendVte(&vt_sq, OPCODE_SD,                1,        1,   1,   1,        1,        1,        1,         1,     1,    1);
    AppendVte(&vt_sq, OPCODE_SWL,               1,        1,   1,   1,        1,        1,        1,         0,     1,    1);
    AppendVte(&vt_sq, OPCODE_SWR,               1,        1,   1,   1,        1,        1,        1,         0,     1,    1);
    AppendVte(&vt_sq, OPCODE_SDL,               1,        1,   1,   1,        1,        1,        1,         1,     1,    1);
    AppendVte(&vt_sq, OPCODE_SDR,               1,        1,   1,   1,        1,        1,        1,         1,     1,    1);
    AppendVte(&vt_sq, OPCODE_SC_0,              1,        1,   1,   1,        1,        1,        1,         1,     1,    1);
    AppendVte(&vt_sq, OPCODE_SCD_0,             1,        1,   1,   1,        1,        1,        1,         1,     1,    1);
}

void VbitTableInitIBuff() {
    // Initialize vbit table for IBuff
                       // size                            unimportant mask
    VtAddField(&vt_ibuff, SVM->cfg().arch_size_pc,        0); // pc
    VtAddField(&vt_ibuff, SVM->cfg().core_width_opcode,   1); // opcode
    VtAddField(&vt_ibuff, SVM->cfg().core_inst_types_log, 1); // fu
    VtAddField(&vt_ibuff, SVM->cfg().arch_size_amt_log,   1); // dst
    VtAddField(&vt_ibuff, 1,                              1); // dst_valid
    VtAddField(&vt_ibuff, SVM->cfg().arch_size_amt_log,   0); // src1
    VtAddField(&vt_ibuff, 1,                              1); // src1_valid
    VtAddField(&vt_ibuff, SVM->cfg().arch_size_amt_log,   0); // src2
    VtAddField(&vt_ibuff, 1,                              1); // src2_valid
    VtAddField(&vt_ibuff, SVM->cfg().arch_size_immediate, 0); // immed
    VtAddField(&vt_ibuff, 1,                              1); // isLoad
    VtAddField(&vt_ibuff, 1,                              1); // isStore
    VtAddField(&vt_ibuff, SVM->cfg().core_br_types,       1); // ctrlType
    VtAddField(&vt_ibuff, SVM->cfg().core_size_ctiq_log,  1); // ctiID
    VtAddField(&vt_ibuff, SVM->cfg().arch_size_pc,        0); // predNPC
    VtAddField(&vt_ibuff, 1,                              0); // predDir
    VtAddField(&vt_ibuff, 1,                              1); // valid

                                      // pc,opcode,fu,dst,dst_valid,src1,src1_valid,src2,src2_valid,immed,isLoad,isStore,ctrlType,ctiID,predNPC,predDir,valid
    AppendVte(&vt_ibuff, OPCODE_LB,       1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LBU,      1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LH,       1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LHU,      1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LW,       1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LWU,      1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SB,       1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SH,       1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SW,       1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LD,       1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SD,       1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LWL,      1,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LWR,      1,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LDL,      1,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LDR,      1,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SWL,      1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SWR,      1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SDL,      1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SDR,      1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LL,       1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LLD,      1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SC_0,     1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SC_1,     1,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SCD_0,    1,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SCD_1,    1,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ADDI,     1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ADDIU,    0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ANDI,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_LUI,      0,     1, 1,  1,        1,   0,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ORI,      0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SLTI,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SLTIU,    0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_XORI,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DADDI,    1,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DADDIU,   0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ADD,      1,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ADDU,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_AND,      0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_NOR,      0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_OR,       0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SLT,      0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SLTU,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SUB,      1,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SUBU,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_XOR,      0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DADD,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DADDU,    0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSUB,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSUBU,    0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_CLO,      0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_CLZ,      0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ROTR,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_ROTRV,    0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SLL,      0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SLLV,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SRA,      0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SRAV,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SRL,      0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SRLV,     0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSLL,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSLL32,   0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSLLV,    0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSRL,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSRL32,   0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSRLV,    0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSRA,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSRA32,   0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DSRAV,    0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DIV_LO,   0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DIV_HI,   0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DIVU_LO,  0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DIVU_HI,  0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DDIV_LO,  0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DDIV_HI,  0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DDIVU_LO, 0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DDIVU_HI, 0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MADD,     0,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MADDU,    0,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MFHI,     0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MFLO,     0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MSUB,     0,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MSUBU,    0,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MTHI,     0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MTLO,     0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MUL,      0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MULT_LO,  0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MULT_HI,  0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MULTU_LO, 0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MULTU_HI, 0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DMULT_LO, 0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DMULT_HI, 0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DMULTU_LO,0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DMULTU_HI,0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_J,        0,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       1,    1,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_JAL,      0,     1, 1,  1,        1,   0,         1,   0,         1,    0,     1,      1,       1,    1,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_JALX,     0,     1, 1,  1,        1,   0,         1,   0,         1,    0,     1,      1,       1,    1,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_JALR,     0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       1,    1,      1,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_JALR_HB,  0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       1,    1,      1,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_JR,       0,     1, 1,  0,        1,   1,         1,   0,         1,    0,     1,      1,       1,    1,      1,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_JR_HB,    0,     1, 1,  0,        1,   1,         1,   0,         1,    0,     1,      1,       1,    1,      1,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_BEQ,      0,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BNE,      0,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BGEZ,     0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BGEZAL,   0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BGTZ,     0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BLEZ,     0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BLTZ,     0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BLTZAL,   0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BEQL,     0,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BGEZALL,  0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BGEZL,    0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BGTZL,    0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BLEZL,    0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BLTZALL,  0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BLTZL,    0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_BNEL,     0,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       1,    1,      1,      1,    1);
    AppendVte(&vt_ibuff, OPCODE_SYNC,     0,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SYNCI,    0,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_BREAK,    1,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SYSCALL,  1,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TEQ,      1,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TGE,      1,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TGEU,     1,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TLT,      1,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TLTU,     1,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TNE,      1,     1, 1,  0,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TEQI,     1,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TGEI,     1,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TGEIU,    1,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TLTI,     1,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TLTIU,    1,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_TNEI,     1,     1, 1,  0,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_PREF,     0,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_PREFX,    0,     1, 1,  0,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_NOP,      0,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SSNOP,    0,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_EHB,      0,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_EXT,      0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_INS,      0,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DEXT,     0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DEXTM,    0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DEXTU,    0,     1, 1,  1,        1,   1,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DINS,     0,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DINSM,    0,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_DINSU,    0,     1, 1,  1,        1,   1,         1,   1,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SEB,      0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_SEH,      0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_WSBH,     0,     1, 1,  1,        1,   1,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_RDHWR,    0,     1, 1,  1,        1,   0,         1,   0,         1,    1,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MOVN_0,   0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MOVN_1,   0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MOVZ_0,   0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_MOVZ_1,   0,     1, 1,  1,        1,   1,         1,   1,         1,    0,     1,      1,       0,    0,      0,      0,    1);
    AppendVte(&vt_ibuff, OPCODE_UNIMPL,   0,     1, 1,  0,        1,   0,         1,   0,         1,    0,     1,      1,       0,    0,      0,      0,    1);
}

void VbitTableInitCtq() {
    VtAddField(&vt_ctq, 2,                        1); // counter
    VtAddField(&vt_ctq, SVM->cfg().arch_size_pc,  1); // updatePC
    VtAddField(&vt_ctq, SVM->cfg().arch_size_pc,  1); // updateNPC
    VtAddField(&vt_ctq, SVM->cfg().core_br_types, 1); // updateCtrlType
    VtAddField(&vt_ctq, 1,                        1); // updateDir
    VtAddField(&vt_ctq, 1,                        1); // commit
}

void VbitTableInitPipeF1F2() {
    VtAddField(&vt_pipe_f1_f2, 1,                                1); // ready
    VtAddField(&vt_pipe_f1_f2, 2,                                1); // predCounter
    VtAddField(&vt_pipe_f1_f2, SVM->cfg().arch_size_pc,          1); // pc
    VtAddField(&vt_pipe_f1_f2, SVM->cfg().arch_size_instruction, 1); // inst
    VtAddField(&vt_pipe_f1_f2, 1,                                1); // btbHit
    VtAddField(&vt_pipe_f1_f2, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_pipe_f1_f2, SVM->cfg().arch_size_pc,          1); // takenPC
    VtAddField(&vt_pipe_f1_f2, 1,                                1); // predDir
}

void VbitTableInitPipeF2Dec() {
    VtAddField(&vt_pipe_f2_dec, SVM->cfg().arch_size_pc,          1); // pc
    VtAddField(&vt_pipe_f2_dec, SVM->cfg().arch_size_instruction, 1); // inst
    VtAddField(&vt_pipe_f2_dec, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_pipe_f2_dec, SVM->cfg().arch_size_pc,          1); // predNPC
    VtAddField(&vt_pipe_f2_dec, 1,                                1); // predDir
    VtAddField(&vt_pipe_f2_dec, SVM->cfg().core_size_ctiq_log,    1); // ctiID
    VtAddField(&vt_pipe_f2_dec, 1,                                1); // valid
}

void VbitTableInitPipeRenDis() {
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().arch_size_pc,          1); // pc
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_width_opcode,     1); // opcode
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_inst_types_log,   1); // fu
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().arch_size_amt_log,     1); // logDest
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_size_prf_int_log, 1); // phyDest
    VtAddField(&vt_pipe_ren_dis, 1,                                1); // phyDestValid
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_size_prf_int_log, 1); // phySrc1
    VtAddField(&vt_pipe_ren_dis, 1,                                1); // phySrc1Valid
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_size_prf_int_log, 1); // phySrc2
    VtAddField(&vt_pipe_ren_dis, 1,                                1); // phySrc2Valid
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().arch_size_immediate,   1); // immed
    VtAddField(&vt_pipe_ren_dis, 1,                                1); // isLoad
    VtAddField(&vt_pipe_ren_dis, 1,                                1); // isStore
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().core_size_ctiq_log,    1); // ctiID
    VtAddField(&vt_pipe_ren_dis, SVM->cfg().arch_size_pc,          1); // predNPC
    VtAddField(&vt_pipe_ren_dis, 1,                                1); // predDir
}

void VbitTableInitPipeIssRr() {
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().arch_size_pc,          1); // pc
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_width_opcode,     1); // opcode
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_size_prf_int_log, 1); // phyDest
    VtAddField(&vt_pipe_iss_rr, 1,                                1); // phyDestValid
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_size_prf_int_log, 1); // phySrc1
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_size_prf_int_log, 1); // phySrc2
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_size_lq_log,      1); // lsqID
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_size_al_log,      1); // alID
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().arch_size_immediate,   1); // immed
    VtAddField(&vt_pipe_iss_rr, 1,                                1); // isSimple
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().core_size_ctiq_log,    1); // ctiID
    VtAddField(&vt_pipe_iss_rr, SVM->cfg().arch_size_pc,          1); // predNPC
    VtAddField(&vt_pipe_iss_rr, 1,                                1); // predDir
    VtAddField(&vt_pipe_iss_rr, 1,                                1); // valid
}

void VbitTableInitPipeRrExe() {
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().arch_size_pc,          1); // pc
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_width_opcode,     1); // opcode
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_size_prf_int_log, 1); // phyDest
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_size_prf_int_log, 1); // phySrc1
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().arch_size_data,        1); // src1Data
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_size_prf_int_log, 1); // phySrc2
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().arch_size_data,        1); // src2Data
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_size_lq_log,      1); // lsqID
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_size_al_log,      1); // alID
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().arch_size_immediate,   1); // immed
    VtAddField(&vt_pipe_rr_exe, 1,                                1); // isSimple
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().core_size_ctiq_log,    1); // ctiID
    VtAddField(&vt_pipe_rr_exe, SVM->cfg().arch_size_pc,          1); // predNPC
    VtAddField(&vt_pipe_rr_exe, 1,                                1); // predDir
    VtAddField(&vt_pipe_rr_exe, 1,                                1); // valid
}

void VbitTableInitPipeExeLsu() {
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().core_size_exe_flags,   1); // flags
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().core_ldst_types_log,   1); // ldstSize
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().core_size_prf_int_log, 1); // phyDest
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().arch_size_data,        1); // address
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().arch_size_data,        1); // src2Data
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().core_size_lq_log,      1); // lsqID
    VtAddField(&vt_pipe_exe_lsu, SVM->cfg().core_size_al_log,      1); // alID
    VtAddField(&vt_pipe_exe_lsu, 1,                                1); // valid
    VtAddField(&vt_pipe_exe_lsu, 1,                                1); // isInsertRight
    VtAddField(&vt_pipe_exe_lsu, 1,                                1); // isInsertLeft
}

void VbitTableInitPipeFuWb() {
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().arch_size_pc,          1); // pc
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().core_size_exe_flags,   1); // flags
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().core_size_prf_int_log, 1); // phyDest
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().arch_size_data,        1); // destData
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().core_size_al_log,      1); // alID
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().arch_size_pc,          1); // nextPC
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().core_br_types,         1); // ctrlType
    VtAddField(&vt_pipe_fu_wb, 1,                                1); // ctrlDir
    VtAddField(&vt_pipe_fu_wb, SVM->cfg().core_size_ctiq_log,    1); // ctiID
    VtAddField(&vt_pipe_fu_wb, 1,                                1); // predDir
    VtAddField(&vt_pipe_fu_wb, 1,                                1); // valid
}

void VbitTableInit() {
    VbitTableInitIq();
    VbitTableInitRob();
    VbitTableInitLq();
    VbitTableInitSq();
    VbitTableInitIBuff();
    VbitTableInitCtq();
    VbitTableInitPipeF1F2();
    VbitTableInitPipeF2Dec();
    VbitTableInitPipeRenDis();
    VbitTableInitPipeIssRr();
    VbitTableInitPipeRrExe();
    VbitTableInitPipeExeLsu();
    VbitTableInitPipeFuWb();
}

int CountVbit(VbitTable *pvt, int opcode, int important, bool dispatch_mode) {
    if (pvt->table.count(opcode) != 0) {
        int vbit_cnt = 0;
        for (uint32_t m=0; m<pvt->num_fields; m++) {
            int vbit_field = pvt->unit_sizes[m] * pvt->table[opcode][m];
            if (!important) vbit_field *= pvt->unimp_mask[m];
            if (dispatch_mode) vbit_field *= pvt->dispatch_mask[m];
            vbit_cnt += vbit_field;
        }
        return vbit_cnt;
    }
    else {
        std::cout << "[VbitTable] Unhandled opcode " << opcode << std::endl;
        return INVALID_BIT_COUNT;
    }
}

int CountUnusedBit(VbitTable *pvt, int opcode) {
    if (pvt->table.count(opcode) != 0) {
        int unused_bit_cnt = 0;
        for (uint32_t m=0; m<pvt->num_fields; m++) {
            if (pvt->table[opcode][m] == 0) {
                unused_bit_cnt += pvt->unit_sizes[m];
            }
        }
        return unused_bit_cnt;
    }
    else {
        std::cout << "[VbitTable] Unhandled opcode " << opcode << std::endl;
        return INVALID_BIT_COUNT;
    }
}

int GetVulnerableBitCount(structure_t type, int opcode, bool important) {
    return GetVulnerableBitCount(type, opcode, important, false);
}

int GetVulnerableBitCount(structure_t type, int opcode, bool important, bool dispatch_mode) {
    int vbit_cnt = INVALID_BIT_COUNT;

    switch (type) {
        case STRUCTURE_IQ:
            return CountVbit(&vt_iq, opcode, important, dispatch_mode);
        case STRUCTURE_ROB:
            return CountVbit(&vt_rob, opcode, important, dispatch_mode);
        case STRUCTURE_LQ:
            return CountVbit(&vt_lq, opcode, important, dispatch_mode);
        case STRUCTURE_SQ:
            return CountVbit(&vt_sq, opcode, important, dispatch_mode);
        case STRUCTURE_RF:
            if (important) // every data bit in a register is vulnerable
                vbit_cnt = SVM->cfg().arch_size_data;
            else
                vbit_cnt = 0; // none of the bit is vulnerable
            break;
        case STRUCTURE_IBUFF:
            return CountVbit(&vt_ibuff, opcode, important, dispatch_mode);
        case STRUCTURE_RMT:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_RMT);
            break;
        case STRUCTURE_AMT:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_AMT);
            break;
        case STRUCTURE_FREELIST:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_FREELIST);
            break;
        case STRUCTURE_CTQ:
            // every bit in the CTQ is not vulnerable because CTQ is used
            // to update BP/BTB/RAS which are not vulnerable
            vbit_cnt = 0;
            break;
        case STRUCTURE_PIPE_F1_F2:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_F1_F2);
            break;
        case STRUCTURE_PIPE_F2_DEC:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_F2_DEC);
            break;
        case STRUCTURE_PIPE_REN_DIS:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_REN_DIS);
            break;
        case STRUCTURE_PIPE_ISS_RR:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_ISS_RR);
            break;
        case STRUCTURE_PIPE_RR_EXE:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_RR_EXE);
            break;
        case STRUCTURE_PIPE_EXE_LSU:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_EXE_LSU);
            break;
        case STRUCTURE_PIPE_FU_WB:
            // assume every bit in the entry is vulnerable
            vbit_cnt = GetEntryBitsCount(STRUCTURE_PIPE_FU_WB);
            break;
        default:
            // nothing to do
            break;
    }

    return vbit_cnt;
}

int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important) {
    return GetVulnerableBitCountAppx(type, opcode, important, 2, false);
}

int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important, bool dispatch_mode) {
    return GetVulnerableBitCountAppx(type, opcode, important, 2, dispatch_mode);
}

int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important, int ones_cnt) {
    return GetVulnerableBitCountAppx(type, opcode, important, ones_cnt, false);
}

int GetVulnerableBitCountAppx(structure_t type, int opcode, bool important, int ones_cnt, bool dispatch_mode) {
    //if (type == STRUCTURE_SQ)
    //    return Approximate(GetVulnerableBitCount(type, opcode, important, dispatch_mode), ones_cnt, 8);
    //else
        return Approximate(GetVulnerableBitCount(type, opcode, important, dispatch_mode), ones_cnt);
}

int GetUnusedBitCount(structure_t type, int opcode) {
    int unused_bit_cnt = INVALID_BIT_COUNT;

    switch (type) {
        case STRUCTURE_IQ:
            return CountUnusedBit(&vt_iq, opcode);
        case STRUCTURE_ROB:
            return CountUnusedBit(&vt_rob, opcode);
        case STRUCTURE_LQ:
            return CountUnusedBit(&vt_lq, opcode);
        case STRUCTURE_SQ:
            return CountUnusedBit(&vt_sq, opcode);
        case STRUCTURE_RF:
            return 0;
        case STRUCTURE_IBUFF:
            return CountUnusedBit(&vt_ibuff, opcode);
        case STRUCTURE_RMT:
            return 0;
        case STRUCTURE_AMT:
            return 0;
        case STRUCTURE_FREELIST:
            return 0;
        case STRUCTURE_CTQ:
            return 0;
        case STRUCTURE_PIPE_F1_F2:
            return 0;
        case STRUCTURE_PIPE_F2_DEC:
            return 0;
        case STRUCTURE_PIPE_REN_DIS:
            return 0;
        case STRUCTURE_PIPE_ISS_RR:
            return 0;
        case STRUCTURE_PIPE_RR_EXE:
            return 0;
        case STRUCTURE_PIPE_EXE_LSU:
            return 0;
        case STRUCTURE_PIPE_FU_WB:
            return 0;
        default:
            // nothing to do
            break;
    }

    return unused_bit_cnt;
}

int GetEntrySize(VbitTable *pvt) {
    int entry_size = 0;
    for (uint32_t n=0; n<pvt->num_fields; n++) {
        entry_size += pvt->unit_sizes[n];
    }
    return entry_size;
}

int GetEntryBitsCount(structure_t type) {
    switch (type) {
        case STRUCTURE_IQ: return GetEntrySize(&vt_iq);
        case STRUCTURE_ROB: return GetEntrySize(&vt_rob);
        case STRUCTURE_LQ: return GetEntrySize(&vt_lq);
        case STRUCTURE_SQ: return GetEntrySize(&vt_sq);
        case STRUCTURE_RF:
            // Register has SVM->cfg().arch_size_data+1 bits
            return SVM->cfg().arch_size_data+1;
        case STRUCTURE_IBUFF: return GetEntrySize(&vt_ibuff);

        case STRUCTURE_RMT: return SVM->cfg().core_size_prf_int_log;
        case STRUCTURE_AMT: return SVM->cfg().core_size_prf_int_log;
        case STRUCTURE_FREELIST: return SVM->cfg().core_size_prf_int_log;

        case STRUCTURE_CTQ: return GetEntrySize(&vt_ctq);
        case STRUCTURE_PIPE_F1_F2: return GetEntrySize(&vt_pipe_f1_f2);
        case STRUCTURE_PIPE_F2_DEC: return GetEntrySize(&vt_pipe_f2_dec);
        case STRUCTURE_PIPE_REN_DIS: return GetEntrySize(&vt_pipe_ren_dis);
        case STRUCTURE_PIPE_ISS_RR: return GetEntrySize(&vt_pipe_iss_rr);
        case STRUCTURE_PIPE_RR_EXE: return GetEntrySize(&vt_pipe_rr_exe);
        case STRUCTURE_PIPE_EXE_LSU: return GetEntrySize(&vt_pipe_exe_lsu);
        case STRUCTURE_PIPE_FU_WB: return GetEntrySize(&vt_pipe_fu_wb);
        default:
            return INVALID_BIT_COUNT;
    }
}

int GetTotalBitsCount(structure_t type) {
    switch (type) {
        case STRUCTURE_IQ:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_iq_int);
        case STRUCTURE_ROB:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_al);
        case STRUCTURE_LQ:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_lq);
        case STRUCTURE_SQ:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_sq);
        case STRUCTURE_RF:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_prf_int);
        case STRUCTURE_IBUFF:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_fq);
        case STRUCTURE_RMT:
            return (GetEntryBitsCount(type) * SVM->cfg().arch_size_amt);
        case STRUCTURE_AMT:
            return (GetEntryBitsCount(type) * SVM->cfg().arch_size_amt);
        case STRUCTURE_FREELIST:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_freelist);
        case STRUCTURE_CTQ:
            return (GetEntryBitsCount(type) * SVM->cfg().core_size_ctiq);
        case STRUCTURE_PIPE_F1_F2:
            return (GetEntryBitsCount(type) * SVM->cfg().core_width_frontend);
        case STRUCTURE_PIPE_F2_DEC:
            return (GetEntryBitsCount(type) * SVM->cfg().core_width_frontend);
        case STRUCTURE_PIPE_REN_DIS:
            return (GetEntryBitsCount(type) * SVM->cfg().core_width_dispatch);
        case STRUCTURE_PIPE_ISS_RR:
            return (GetEntryBitsCount(type) * SVM->cfg().core_width_issue);
        case STRUCTURE_PIPE_RR_EXE:
            return (GetEntryBitsCount(type) * SVM->cfg().core_width_issue);
        case STRUCTURE_PIPE_EXE_LSU:
            return (GetEntryBitsCount(type) * 1);
        case STRUCTURE_PIPE_FU_WB:
            return (GetEntryBitsCount(type) * SVM->cfg().core_width_issue);
        default:
            return INVALID_BIT_COUNT;
    }
}

std::string GetStructureName(structure_t type) {
    switch (type) {
        case STRUCTURE_IQ: return "IQ";
        case STRUCTURE_ROB: return "ROB";
        case STRUCTURE_LQ: return "LQ";
        case STRUCTURE_SQ: return "SQ";
        case STRUCTURE_RF: return "RF";
        case STRUCTURE_IBUFF: return "InstBuff";
        case STRUCTURE_RMT: return "RMT";
        case STRUCTURE_AMT: return "AMT";
        case STRUCTURE_FREELIST: return "FreeList";
        case STRUCTURE_CTQ: return "CTQ";
        case STRUCTURE_PIPE_F1_F2: return "Pipe_F1_F2";
        case STRUCTURE_PIPE_F2_DEC: return "Pipe_F2_DEC";
        case STRUCTURE_PIPE_REN_DIS: return "Pipe_REN_DIS";
        case STRUCTURE_PIPE_ISS_RR: return "Pipe_ISS_RR";
        case STRUCTURE_PIPE_RR_EXE: return "Pipe_RR_EXE";
        case STRUCTURE_PIPE_EXE_LSU: return "Pipe_EXE_LSU";
        case STRUCTURE_PIPE_FU_WB: return "Pipe_FU_WB";
        default: return "invalid structure";
    }
}

std::string GetFuStructureName(structure_fu_t type) {
    switch (type) {
        case STRUCTURE_FU_SIMPLE: return "FUsimp";
        case STRUCTURE_FU_COMPLEX: return "FUcomp";
        case STRUCTURE_FU_CONTROL: return "FUctrl";
        case STRUCTURE_FU_MEMORY: return "FUmem";
        default: return "invalid structure";
    }
}

int Approximate(int value, int max_ones, int underestimate_tolerance) {
    int ones_cnt = 0;
    int value_appx = 0;

    for (int i=30; i>=0 && ones_cnt < max_ones; i--) {
        if ((0x1 << i) & value) {
            ones_cnt++;
            value = value & ~((0x1 << i));
            if (ones_cnt < max_ones || i == 0) {
                value_appx |= (0x1 << i);
            }
            else {
                value_appx |= (0x1 << i);
                if (underestimate_tolerance < 0) { // no tolerance set
                    if (value >= (0x1 << (i-1))) {
                        value_appx += (0x1 << i);
                    }
                }
                else {
                    if (value > underestimate_tolerance) {
                        value_appx += (0x1 << i);
                    }
                }
            }
        }
    }

    return value_appx;
}

int Approximate(int value, int max_ones) {
    return Approximate(value, max_ones, -1);
}

/* end of file */
