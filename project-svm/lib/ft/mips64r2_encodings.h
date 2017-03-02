#ifndef _MIPS64R2_ENCODINGS_H_
#define _MIPS64R2_ENCODINGS_H_

typedef enum {
    //* ---------------------------
    //*    Loads And Stores
    //* ---------------------------

    //* Aligned
    OPCODE_LB,
    OPCODE_LBU,
    OPCODE_LH,
    OPCODE_LHU,
    OPCODE_LW,
    OPCODE_LWU,
    OPCODE_SB,
    OPCODE_SH,
    OPCODE_SW,
    OPCODE_LD,
    OPCODE_SD,

    //* Unaligned
    OPCODE_LWL,
    OPCODE_LWR,
    OPCODE_LDL,
    OPCODE_LDR,
    OPCODE_SWL,
    OPCODE_SWR,
    OPCODE_SDL,
    OPCODE_SDR,

    //* Atomic Update Instructions
    OPCODE_LL,
    OPCODE_LLD,
    OPCODE_SC_0,
    OPCODE_SC_1,
    OPCODE_SCD_0,
    OPCODE_SCD_1,

    //* ---------------------------
    //*          Arithmetic
    //* ---------------------------

    //* Immediate
    OPCODE_ADDI,
    OPCODE_ADDIU,
    OPCODE_ANDI,
    OPCODE_LUI,
    OPCODE_ORI,
    OPCODE_SLTI,
    OPCODE_SLTIU,
    OPCODE_XORI,
    OPCODE_DADDI,
    OPCODE_DADDIU,

    //* Three-Operand
    OPCODE_ADD,
    OPCODE_ADDU,
    OPCODE_AND,
    OPCODE_NOR,
    OPCODE_OR,
    OPCODE_SLT,
    OPCODE_SLTU,
    OPCODE_SUB,
    OPCODE_SUBU,
    OPCODE_XOR,
    OPCODE_DADD,
    OPCODE_DADDU,
    OPCODE_DSUB,
    OPCODE_DSUBU,

    //* Two-Operand
    OPCODE_CLO,
    OPCODE_CLZ,

    //* Shift Instructions
    OPCODE_ROTR,
    OPCODE_ROTRV,
    OPCODE_SLL,
    OPCODE_SLLV,
    OPCODE_SRA,
    OPCODE_SRAV,
    OPCODE_SRL,
    OPCODE_SRLV,
    OPCODE_DSLL,
    OPCODE_DSLL32,
    OPCODE_DSLLV,
    OPCODE_DSRL,
    OPCODE_DSRL32,
    OPCODE_DSRLV,
    OPCODE_DSRA,
    OPCODE_DSRA32,
    OPCODE_DSRAV,

    //* Multiply/Divide Instructions
    OPCODE_DIV_LO,
    OPCODE_DIV_HI,
    OPCODE_DIVU_LO,
    OPCODE_DIVU_HI,
    OPCODE_DDIV_LO,
    OPCODE_DDIV_HI,
    OPCODE_DDIVU_LO,
    OPCODE_DDIVU_HI,
    OPCODE_MADD,
    OPCODE_MADDU,
    OPCODE_MFHI,
    OPCODE_MFLO,
    OPCODE_MSUB,
    OPCODE_MSUBU,
    OPCODE_MTHI,
    OPCODE_MTLO,
    OPCODE_MUL,
    OPCODE_MULT_LO,
    OPCODE_MULT_HI,
    OPCODE_MULTU_LO,
    OPCODE_MULTU_HI,
    OPCODE_DMULT_LO,
    OPCODE_DMULT_HI,
    OPCODE_DMULTU_LO,
    OPCODE_DMULTU_HI,

    //* ---------------------------
    //*     Jumps And Branches
    //* ---------------------------

    //* Unconditional Jump
    OPCODE_J,
    OPCODE_JAL,
    OPCODE_JALX,
    OPCODE_JALR,
    OPCODE_JALR_HB,
    OPCODE_JR,
    OPCODE_JR_HB,

    //* PC-Relative Conditional Branches
    OPCODE_BEQ,
    OPCODE_BNE,
    OPCODE_BGEZ,
    OPCODE_BGEZAL,
    OPCODE_BGTZ,
    OPCODE_BLEZ,
    OPCODE_BLTZ,
    OPCODE_BLTZAL,

    //* Deprecated Branch Likely Instructions
    OPCODE_BEQL,
    OPCODE_BGEZALL,
    OPCODE_BGEZL,
    OPCODE_BGTZL,
    OPCODE_BLEZL,
    OPCODE_BLTZALL,
    OPCODE_BLTZL,
    OPCODE_BNEL,

    //* ---------------------------
    //*       Miscellaneous
    //* ---------------------------

    //* Serialization
    OPCODE_SYNC,
    OPCODE_SYNCI,

    //* System Call and Breakpoint Instructions
    OPCODE_BREAK,
    OPCODE_SYSCALL,

    //* Trap-on-Condition
    OPCODE_TEQ,
    OPCODE_TGE,
    OPCODE_TGEU,
    OPCODE_TLT,
    OPCODE_TLTU,
    OPCODE_TNE,
    OPCODE_TEQI,
    OPCODE_TGEI,
    OPCODE_TGEIU,
    OPCODE_TLTI,
    OPCODE_TLTIU,
    OPCODE_TNEI,

    //* Prefetch Instructions
    OPCODE_PREF,
    OPCODE_PREFX,

    //* NOP Instructions
    OPCODE_NOP,
    OPCODE_SSNOP,

    OPCODE_EHB,

    //* Extract/Insert Bit Field
    OPCODE_EXT,
    OPCODE_INS,
    OPCODE_DEXT,
    OPCODE_DEXTM,
    OPCODE_DEXTU,
    OPCODE_DINS,
    OPCODE_DINSM,
    OPCODE_DINSU,

    //* Sign Extend Instructions
    OPCODE_SEB,
    OPCODE_SEH,

    //* Word Swap Bytes Within Halfwords
    OPCODE_WSBH,

    //* ---------------------------
    //*     System Instructions
    //* ---------------------------

    OPCODE_CACHE,

    //* (Debug) Exception Return
    OPCODE_DERET,
    OPCODE_ERET,

    //* Disable/Enable Interrupts
    OPCODE_DI,
    OPCODE_EI,

    //* Read Hardware Register
    OPCODE_RDHWR,

    //* Read/Write GPR from Previous Shadow Set
    OPCODE_RDPGPR,
    OPCODE_WRPGPR,

    //* Software Debug Breakpoint
    OPCODE_SDBBP,

    //* TLB Accesses
    OPCODE_TLBP,
    OPCODE_TLBR,
    OPCODE_TLBWI,
    OPCODE_TLBWR,

    OPCODE_WAIT,

    //* ---------------------------
    //*    FP Loads And Stores
    //* ---------------------------

    OPCODE_LWC2_HI,
    OPCODE_LWC2_LO,
    OPCODE_SWC2_HI,
    OPCODE_SWC2_LO,

    OPCODE_LWC1,
    OPCODE_LDC1,
    OPCODE_LDXC1,
    OPCODE_LUXC1,
    OPCODE_LWXC1,

    OPCODE_SWC1_ADDR,
    OPCODE_SWC1_VALUE,
    OPCODE_SDC1_ADDR,
    OPCODE_SDC1_VALUE,
    OPCODE_SDXC1_ADDR,
    OPCODE_SDXC1_VALUE,
    OPCODE_SUXC1_ADDR,
    OPCODE_SUXC1_VALUE,
    OPCODE_SWXC1_ADDR,
    OPCODE_SWXC1_VALUE,

    //* Move To/From
    OPCODE_MFC0,
    OPCODE_MTC0,

    OPCODE_CFC1,
    OPCODE_CTC1,
    OPCODE_MFC1,
    OPCODE_MFHC1,
    OPCODE_MTC1,
    OPCODE_MTC1_1,
    OPCODE_MTHC1,
    OPCODE_DMFC1,
    OPCODE_DMTC1,

    OPCODE_CFC2,
    OPCODE_CTC2,
    OPCODE_MFC2,
    OPCODE_MFHC2,
    OPCODE_MTC2,
    OPCODE_MTHC2,

    //* ---------------------------
    //*      FP Arithmetic
    //* ---------------------------

    OPCODE_ABS_S,
    OPCODE_ABS_D,
    OPCODE_ABS_PS,

    OPCODE_ADD_S,
    OPCODE_ADD_D,
    OPCODE_ADD_PS,

    //* False
    OPCODE_C_F1_S,
    OPCODE_C_F2_S,
    OPCODE_C_F1_D,
    OPCODE_C_F2_D,
    OPCODE_C_F_PS,
    //* Unordered
    OPCODE_C_UN1_S,
    OPCODE_C_UN2_S,
    OPCODE_C_UN1_D,
    OPCODE_C_UN2_D,
    OPCODE_C_UN_PS,

    //* Equal
    OPCODE_C_EQ1_S,
    OPCODE_C_EQ2_S,
    OPCODE_C_EQ1_D,
    OPCODE_C_EQ2_D,
    OPCODE_C_EQ_PS,

    //* Unordered or Equal
    OPCODE_C_UEQ1_S,
    OPCODE_C_UEQ2_S,
    OPCODE_C_UEQ1_D,
    OPCODE_C_UEQ2_D,
    OPCODE_C_UEQ_PS,

    //* Ordered or Less Than
    OPCODE_C_OLT1_S,
    OPCODE_C_OLT2_S,
    OPCODE_C_OLT1_D,
    OPCODE_C_OLT2_D,
    OPCODE_C_OLT_PS,

    //* Unordered or Less Than
    OPCODE_C_ULT1_S,
    OPCODE_C_ULT2_S,
    OPCODE_C_ULT1_D,
    OPCODE_C_ULT2_D,
    OPCODE_C_ULT_PS,

    //* Ordered or Less Than or Equal
    OPCODE_C_OLE1_S,
    OPCODE_C_OLE2_S,
    OPCODE_C_OLE1_D,
    OPCODE_C_OLE2_D,
    OPCODE_C_OLE_PS,

    //* Unordered or Less Than or Equal
    OPCODE_C_ULE1_S,
    OPCODE_C_ULE2_S,
    OPCODE_C_ULE1_D,
    OPCODE_C_ULE2_D,
    OPCODE_C_ULE_PS,

    //* Signaling False
    OPCODE_C_SF1_S,
    OPCODE_C_SF2_S,
    OPCODE_C_SF1_D,
    OPCODE_C_SF2_D,
    OPCODE_C_SF_PS,

    //* Not Greater Than or Less Than or Equal
    OPCODE_C_NGLE1_S,
    OPCODE_C_NGLE2_S,
    OPCODE_C_NGLE1_D,
    OPCODE_C_NGLE2_D,
    OPCODE_C_NGLE_PS,

    //* Signaling Equal
    OPCODE_C_SEQ1_S,
    OPCODE_C_SEQ2_S,
    OPCODE_C_SEQ1_D,
    OPCODE_C_SEQ2_D,
    OPCODE_C_SEQ_PS,

    //* Not Greater Than or Less Than
    OPCODE_C_NGL1_S,
    OPCODE_C_NGL2_S,
    OPCODE_C_NGL1_D,
    OPCODE_C_NGL2_D,
    OPCODE_C_NGL_PS,

    //* Less Than
    OPCODE_C_LT1_S,
    OPCODE_C_LT2_S,
    OPCODE_C_LT1_D,
    OPCODE_C_LT2_D,
    OPCODE_C_LT_PS,

    //* Not Greater Than or Equal
    OPCODE_C_NGE1_S,
    OPCODE_C_NGE2_S,
    OPCODE_C_NGE1_D,
    OPCODE_C_NGE2_D,
    OPCODE_C_NGE_PS,

    //* Less Than or Equal
    OPCODE_C_LE1_S,
    OPCODE_C_LE2_S,
    OPCODE_C_LE1_D,
    OPCODE_C_LE2_D,
    OPCODE_C_LE_PS,

    //* Not Greater Than
    OPCODE_C_NGT1_S,
    OPCODE_C_NGT2_S,
    OPCODE_C_NGT1_D,
    OPCODE_C_NGT2_D,
    OPCODE_C_NGT_PS,

    OPCODE_DIV_S,
    OPCODE_DIV_D,
    OPCODE_DIV_PS,

    OPCODE_MUL_S,
    OPCODE_MUL_D,
    OPCODE_MUL_PS,

    OPCODE_NEG_S,
    OPCODE_NEG_D,
    OPCODE_NEG_PS,

    OPCODE_SQRT_S,
    OPCODE_SQRT_D,

    OPCODE_SUB_S,
    OPCODE_SUB_D,
    OPCODE_SUB_PS,

    OPCODE_RECIP_S,
    OPCODE_RECIP_D,

    OPCODE_RSQRT_S,
    OPCODE_RSQRT_D,

    //* Multiply-Accumulate
    OPCODE_MADD_S,
    OPCODE_MADD_D,
    OPCODE_MADD_PS,

    OPCODE_MSUB_S,
    OPCODE_MSUB_D,
    OPCODE_MSUB_PS,

    OPCODE_NMADD_S,
    OPCODE_NMADD_D,
    OPCODE_NMADD_PS,

    OPCODE_NMSUB_S,
    OPCODE_NMSUB_D,
    OPCODE_NMSUB_PS,


    //* ---------------------------
    //*      FP Conversion
    //* ---------------------------

    OPCODE_CVT_D_S,
    OPCODE_CVT_D_W,
    OPCODE_CVT_D_L,

    OPCODE_CVT_L_S,
    OPCODE_CVT_L_D,

    OPCODE_CVT_PS_S,

    OPCODE_CVT_S_D,
    OPCODE_CVT_S_W,
    OPCODE_CVT_S_L,

    OPCODE_CVT_S_PL,
    OPCODE_CVT_S_PU,

    OPCODE_CVT_W_S,
    OPCODE_CVT_W_D,

    OPCODE_CEIL_L_S,
    OPCODE_CEIL_L_D,

    OPCODE_CEIL_W_S,
    OPCODE_CEIL_W_D,

    OPCODE_FLOOR_L_S,
    OPCODE_FLOOR_L_D,

    OPCODE_FLOOR_W_S,
    OPCODE_FLOOR_W_D,

    OPCODE_ROUND_L_S,
    OPCODE_ROUND_L_D,

    OPCODE_ROUND_W_S,
    OPCODE_ROUND_W_D,

    OPCODE_TRUNC_L_S,
    OPCODE_TRUNC_L_D,

    OPCODE_TRUNC_W_S,
    OPCODE_TRUNC_W_D,


    //* ---------------------------
    //*   FP Move Instructions
    //* ---------------------------

    //* Unconditional Move
    OPCODE_MOV_S,
    OPCODE_MOV_D,
    OPCODE_MOV_PS,

    //* Conditional Move
    OPCODE_MOVF1,
    OPCODE_MOVF1_S,
    OPCODE_MOVF2_S,
    OPCODE_MOVF2,
    OPCODE_MOVF1_D,
    OPCODE_MOVF2_D,
    OPCODE_MOVF_PS,

    OPCODE_MOVT1,
    OPCODE_MOVT1_S,
    OPCODE_MOVT2_S,
    OPCODE_MOVT2,
    OPCODE_MOVT1_D,
    OPCODE_MOVT2_D,
    OPCODE_MOVT_PS,

    OPCODE_MOVN_0,
    OPCODE_MOVN_1,
    OPCODE_MOVN_S_0,
    OPCODE_MOVN_S_1,
    OPCODE_MOVN_D_0,
    OPCODE_MOVN_D_1,
    OPCODE_MOVN_PS,

    OPCODE_MOVZ_0,
    OPCODE_MOVZ_1,

    OPCODE_MOVZ_S_0,
    OPCODE_MOVZ_S_1,

    OPCODE_MOVZ_D_0,
    OPCODE_MOVZ_D_1,
    OPCODE_MOVZ_PS,


    //* ---------------------------
    //*   FP Conditional Branches
    //* ---------------------------

    OPCODE_BC1F,
    OPCODE_BC1T,

    //* Deprecated Branch Likely
    OPCODE_BC1FL,
    OPCODE_BC1TL,

    //* ---------------------------
    //*     FP Miscellaneous
    //* ---------------------------

    OPCODE_ALNV_PS,

    OPCODE_PLL_PS,
    OPCODE_PLU_PS,
    OPCODE_PUL_PS,
    OPCODE_PUU_PS,

    OPCODE_UNIMPL
} MIPSOpcode_RTL;

#endif /* _MIPS64R2_ENCODINGS_H_ */
