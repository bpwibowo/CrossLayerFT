module vlt_InstBuff (
    input important_i
  , input [8:0] opcode_i
  , input [9:0] start_ts_i
  , input [9:0] end_ts_i
  , output reg [3:0] shift1_o
  , output reg [3:0] shift2_o
  , output reg shift1_v_o
  , output reg shift2_v_o
  , output reg [9:0] duration_o
  );

always @(*)
begin
  // initialize outputs to zero
  shift1_o = 0;
  shift1_v_o = 0;
  shift2_o = 0;
  shift2_v_o = 0;

  // calculate duration
  duration_o = end_ts_i - start_ts_i;

  case (opcode_i)
    // DADDU
    46: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // JR
    98: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // EXT
    137: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSRAV
    67: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DEXT
    139: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSUB
    47: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BNEL
    115: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MSUBU
    81: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // LBU
    1: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LDR
    14: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SYSCALL
    119: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DMULT_LO
    89: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DMULTU_HI
    92: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MULT_HI
    86: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DDIV_HI
    73: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TGEU
    122: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // PREFX
    133: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // TGEI
    127: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // LHU
    3: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // EHB
    136: begin
      if (important_i) begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DIV_LO
    68: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DIVU_HI
    71: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BNE
    101: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // ADDU
    36: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TEQI
    126: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DSRL
    62: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SC_1
    22: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SC_0
    21: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DSRA
    65: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SLLV
    54: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SLTI
    30: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TGEIU
    128: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SLTU
    41: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TNEI
    131: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // OR
    39: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // ADDI
    25: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BLTZ
    106: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TLTI
    129: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // MFHI
    78: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SYNC
    116: begin
      if (important_i) begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // MOVN_1
    361: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MOVN_0
    360: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BGTZL
    111: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SCD_0
    23: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SCD_1
    24: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSRL32
    63: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DDIVU_LO
    74: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BEQ
    100: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MSUB
    80: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DSUBU
    48: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TLTIU
    130: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // MADD
    76: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // MULTU_HI
    88: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SLT
    40: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LDL
    13: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // JALR_HB
    97: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // ROTRV
    52: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LLD
    20: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SUBU
    43: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BLTZL
    114: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BLTZALL
    113: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // WSBH
    147: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // AND
    37: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SLL
    53: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // JR_HB
    99: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSLLV
    61: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // INS
    138: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // RDHWR
    153: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MTLO
    83: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSLL
    59: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // NOP
    134: begin
      if (important_i) begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // NOR
    38: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SUB
    42: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BLEZ
    105: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BGEZL
    110: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MUL
    84: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSLL32
    60: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MADDU
    77: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // ANDI
    27: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DMULTU_LO
    91: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSRLV
    64: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DMULT_HI
    90: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MULT_LO
    85: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TEQ
    120: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // MOVZ_1
    368: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MOVZ_0
    367: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SEB
    145: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DIVU_LO
    70: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SRA
    55: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TLT
    123: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DADDI
    33: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SRL
    57: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // ADD
    35: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DIV_HI
    69: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // CLZ
    50: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DADDIU
    34: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // XORI
    32: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DDIV_LO
    72: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BGEZALL
    109: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // JALR
    96: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // SLTIU
    31: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // PREF
    132: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SW
    8: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // JALX
    95: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // BREAK
    118: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SH
    7: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SRLV
    58: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // ORI
    29: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SB
    6: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SEH
    146: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SD
    10: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // BEQL
    108: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SWL
    15: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // MFLO
    79: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LWR
    12: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LUI
    28: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LWU
    5: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DDIVU_HI
    75: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SRAV
    56: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // CLO
    49: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LWL
    11: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SYNCI
    117: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SWR
    16: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // LD
    9: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LB
    0: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BLEZL
    112: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LL
    19: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // ROTR
    51: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DSRA32
    66: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // LH
    2: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TLTU
    124: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // LW
    4: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DINS
    142: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // MULTU_LO
    87: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TGE
    121: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // ADDIU
    26: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BGEZAL
    103: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // J
    93: begin
      if (important_i) begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // XOR
    44: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DEXTU
    141: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SSNOP
    135: begin
      if (important_i) begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // BGEZ
    102: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // TNE
    125: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // SDL
    17: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DEXTM
    140: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DINSU
    144: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SDR
    18: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // DADD
    45: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // JAL
    94: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // BGTZ
    104: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // DINSM
    143: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // BLTZAL
    107: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
    end
    // MTHI
    82: begin
      if (important_i) begin
        shift1_o = 5;
        shift1_v_o = 1;
        shift2_o = 0;
        shift2_v_o = 0;
      end
      else begin
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
  endcase
end

endmodule
