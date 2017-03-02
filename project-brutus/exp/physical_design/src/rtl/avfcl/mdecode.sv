`define MD_SIZE_0 0
`define MD_SIZE_5 1
`define MD_SIZE_4 2
`define MD_SIZE_10 3
`define MD_SIZE_15 4
`define MD_SIZE_13 5
`define MD_SIZE_19 6

module mdecode (
    input [31:0]        inst,

    output reg [4:0]    metadata_size,
    //output reg [18:0]   metadata_vector,
    output reg [9:0]   metadata_vector,
    output reg          is_control
);

logic [5:0] opcode;
logic [4:0] rt;
logic [4:0] rd;
logic [4:0] rs;
logic [5:0] func;

assign opcode = inst[31:26];
assign rt     = inst[20:16];
assign rd     = inst[15:11];
assign rs     = inst[10:6];
assign func   = inst[5:0];

always_comb begin : proc_mdecode

    metadata_size = `MD_SIZE_0;
    metadata_vector = 0;
    is_control = 0;

    case (opcode)

        6'h0: begin // SPECIAL

            case (func)

                6'h0: begin // SLL
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h1: begin // MOVCI

                end

                6'h2: begin // SRL
                    metadata_vector = inst[25:25-4];
                    metadata_size = `MD_SIZE_4;
                end

                6'h3: begin // SRA
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h4: begin // SLLV
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h5: begin // reserved
                end

                6'h6: begin // SRLV
                    metadata_vector = inst[10:10-4];
                    metadata_size = `MD_SIZE_4;
                end

                6'h7: begin // SRAV
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h8: begin // JR
                    metadata_vector = inst[20:20-10];
                    metadata_size = `MD_SIZE_10;
                    is_control = 1;
                end

                6'h9: begin // JALR
                    metadata_vector = inst[20:20-5];
                    metadata_size = `MD_SIZE_5;
                    is_control = 1;
                end

                6'ha: begin // MOVZ
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'hb: begin // MOVN
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'hc: begin // SYSCALL
                end

                6'hd: begin // BREAK
                end

                6'he: begin // reserved
                end

                6'hf: begin // SYNC
                    metadata_vector = inst[25:25-15];
                    metadata_size = `MD_SIZE_15;
                end

                6'h10: begin // MFHI
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h11: begin // MTHI
                    metadata_vector = inst[20:20-15];
                    metadata_size = `MD_SIZE_15;
                end

                6'h12: begin // MFLO
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h13: begin // MTLO
                    metadata_vector = inst[20:20-15];
                    metadata_size = `MD_SIZE_15;
                end

                6'h14: begin // DSLLV
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h15: begin // reserved
                end

                6'h16: begin // DSRLV
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h17: begin // DSRAV
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h18: begin // MULT
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h19: begin // MULTU
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h1a: begin // DIV
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h1b: begin // DIVU
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h1c: begin // DMULT
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h1d: begin // DMULTU
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h1e: begin // DDIV
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h1f: begin // DDIVU
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h20: begin // ADD
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h21: begin // ADDU
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h22: begin // SUB
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h23: begin // SUBU
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h24: begin // AND
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h25: begin // OR
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h26: begin // XOR
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h27: begin // NOR
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h28: begin // reserved
                end

                6'h29: begin // reserved
                end

                6'h2a: begin // SLT
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h2b: begin // SLTU
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h2c: begin // DADD
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h2d: begin // DADDU
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h2e: begin // DSUB
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h2f: begin // DSUBU
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h30: begin // TGE
                end

                6'h31: begin // TGEU
                end

                6'h32: begin // TLT
                end

                6'h33: begin // TLTU
                end

                6'h34: begin // TEQ
                end

                6'h35: begin // reserved
                end

                6'h36: begin // TNE
                end

                6'h37: begin // reserved
                end

                6'h38: begin // DSLL
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h39: begin // reserved
                end

                6'h3a: begin // DSRL
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h3b: begin // DSRA
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h3c: begin // DSLL32
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h3d: begin // reserved
                end

                6'h3e: begin // DSRL32
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h3f: begin // DSRA32
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                end

            endcase // func

        end

        6'h1: begin // REGIMM
            case (rt)

                5'h0: begin // BLTZ
                    is_control = 1;
                end

                5'h1: begin // BGEZ
                    is_control = 1;
                end

                5'h2: begin // BLTZL
                    is_control = 1;
                end

                5'h3: begin // BGEZL
                    is_control = 1;
                end

                5'h4: begin // reserved
                end

                5'h5: begin // reserved
                end

                5'h6: begin // reserved
                end

                5'h7: begin // reserved
                end

                5'h8: begin // TGEI
                end

                5'h9: begin // TGEIU
                end

                5'ha: begin // TLTI
                end

                5'hb: begin // TLTIU
                end

                5'hc: begin // TEQI
                end

                5'hd: begin // reserved
                end

                5'he: begin // TNEI
                end

                5'hf: begin // reserved
                end

                5'h10: begin // BLTZAL
                    is_control = 1;
                end

                5'h11: begin // BGEZAL
                    is_control = 1;
                end

                5'h12: begin // BLTZALL
                    is_control = 1;
                end

                5'h13: begin // BGEZALL
                    is_control = 1;
                end

                5'h14: begin // reserved
                end

                5'h15: begin // reserved
                end

                5'h16: begin // reserved
                end

                5'h17: begin // reserved
                end

                5'h18: begin // reserved
                end

                5'h19: begin // reserved
                end

                5'h1a: begin // reserved
                end

                5'h1b: begin // reserved
                end

                5'h1c: begin // reserved
                end

                5'h1d: begin // reserved
                end

                5'h1e: begin // reserved
                end

                5'h1f: begin // SYNCI
                    metadata_vector = inst[25:25-15];
                    metadata_size = `MD_SIZE_15;
                end

            endcase
        end

        6'h2: begin // J
            is_control = 1;
        end

        6'h3: begin // JAL
            is_control = 1;
        end

        6'h4: begin // BEQ
            is_control = 1;
        end

        6'h5: begin // BNE
            is_control = 1;
        end

        6'h6: begin // BLEZ
            metadata_vector = inst[20:20-5];
            metadata_size = `MD_SIZE_5;
        end

        6'h7: begin // BGTZ
            metadata_vector = inst[20:20-5];
            metadata_size = `MD_SIZE_5;
        end

        6'h8: begin // ADDI
        end

        6'h9: begin // ADDIU
        end

        6'ha: begin // SLTI
        end

        6'hb: begin // SLTU
        end

        6'hc: begin // ANDI
        end

        6'hd: begin // ORI
        end

        6'he: begin // XORI
        end

        6'hf: begin // LUI
            metadata_vector = inst[25:25-5];
            metadata_size = `MD_SIZE_5;
        end

        6'h10: begin // COP0
        end

        6'h11: begin // COP1
        end

        6'h12: begin // COP2
        end

        6'h13: begin // COP1X
            case (func)
                6'hf: begin // PREFX
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end
            endcase
        end

        6'h14: begin // BEQL
            is_control = 1;
        end

        6'h15: begin // BNEL
            is_control = 1;
        end

        6'h16: begin // BLEZL
            metadata_vector = inst[20:20-5];
            metadata_size = `MD_SIZE_5;
            is_control = 1;
        end

        6'h17: begin // BGTZL
            metadata_vector = inst[20:20-5];
            metadata_size = `MD_SIZE_5;
            is_control = 1;
        end

        6'h18: begin // DADDI
        end

        6'h19: begin // DADDIU
        end

        6'h1a: begin // LDL
        end

        6'h1b: begin // LDR
        end

        6'h1c: begin // SPECIAL2
            case (func)

                6'h0: begin // MADD
                    metadata_vector = inst[15:15-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h1: begin // MADDU
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h2: begin // MUL
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h3: begin // reserved
                end

                6'h4: begin // MSUB
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h5: begin // MSUBU
                    metadata_vector = inst[15:15-10];
                    metadata_size = `MD_SIZE_10;
                end

                6'h6: begin // reserved
                end

                6'h7: begin // reserved
                end

                6'h8: begin // reserved
                end

                6'h9: begin // reserved
                end

                6'ha: begin // reserved
                end

                6'hb: begin // reserved
                end

                6'hc: begin // reserved
                end

                6'hd: begin // reserved
                end

                6'he: begin // reserved
                end

                6'hf: begin // reserved
                end

                6'h10: begin // reserved
                end

                6'h11: begin // reserved
                end

                6'h12: begin // reserved
                end

                6'h13: begin // reserved
                end

                6'h14: begin // reserved
                end

                6'h15: begin // reserved
                end

                6'h16: begin // reserved
                end

                6'h17: begin // reserved
                end

                6'h18: begin // reserved
                end

                6'h19: begin // reserved
                end

                6'h1a: begin // reserved
                end

                6'h1b: begin // reserved
                end

                6'h1c: begin // reserved
                end

                6'h1d: begin // reserved
                end

                6'h1e: begin // reserved
                end

                6'h1f: begin // reserved
                end

                6'h20: begin // CLZ
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h21: begin // CLO
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h22: begin // reserved
                end

                6'h23: begin // reserved
                end

                6'h24: begin // DCLZ
                end

                6'h25: begin // DCLO
                end

                6'h26: begin // reserved
                end

                6'h27: begin // reserved
                end

                6'h28: begin // reserved
                end

                6'h29: begin // reserved
                end

                6'h2a: begin // reserved
                end

                6'h2b: begin // reserved
                end

                6'h2c: begin // reserved
                end

                6'h2d: begin // reserved
                end

                6'h2e: begin // reserved
                end

                6'h2f: begin // reserved
                end

                6'h30: begin // reserved
                end

                6'h31: begin // reserved
                end

                6'h32: begin // reserved
                end

                6'h33: begin // reserved
                end

                6'h34: begin // reserved
                end

                6'h35: begin // reserved
                end

                6'h36: begin // reserved
                end

                6'h37: begin // reserved
                end

                6'h38: begin // reserved
                end

                6'h39: begin // reserved
                end

                6'h3a: begin // reserved
                end

                6'h3b: begin // reserved
                end

                6'h3c: begin // reserved
                end

                6'h3d: begin // reserved
                end

                6'h3e: begin // reserved
                end

                6'h3f: begin // SDBBP
                end

            endcase
        end

        6'h1d: begin // JALX
            is_control = 1;
        end

        6'h1e: begin // MDMX
        end

        6'h1f: begin // SPECIAL3
            case (func)
                6'h0: begin // EXT
                end

                6'h1: begin // DEXTM
                end

                6'h2: begin // DEXTU
                end

                6'h3: begin // DEXT
                end

                6'h4: begin // INS
                end

                6'h5: begin // DINSM
                end

                6'h6: begin // DINSU
                end

                6'h7: begin // DINS
                end

                6'h8: begin // reserved
                end

                6'h9: begin // reserved
                end

                6'ha: begin // reserved
                end

                6'hb: begin // reserved
                end

                6'hc: begin // reserved
                end

                6'hd: begin // reserved
                end

                6'he: begin // reserved
                end

                6'hf: begin // reserved
                end

                6'h10: begin // reserved
                end

                6'h11: begin // reserved
                end

                6'h12: begin // reserved
                end

                6'h13: begin // reserved
                end

                6'h14: begin // reserved
                end

                6'h15: begin // reserved
                end

                6'h16: begin // reserved
                end

                6'h17: begin // reserved
                end

                6'h18: begin // reserved
                end

                6'h19: begin // reserved
                end

                6'h1a: begin // reserved
                end

                6'h1b: begin // reserved
                end

                6'h1c: begin // reserved
                end

                6'h1d: begin // reserved
                end

                6'h1e: begin // reserved
                end

                6'h1f: begin // reserved
                end

                6'h20: begin // BSHFL
                    if (rs == 5'b10000) begin // SEB
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                    end
                    else if (rs == 5'b11000) begin // SEH
                    metadata_vector = inst[25:25-5];
                    metadata_size = `MD_SIZE_5;
                    end
                end

                6'h21: begin // reserved
                end

                6'h22: begin // reserved
                end

                6'h23: begin // reserved
                end

                6'h24: begin // DBSHFL
                end

                6'h25: begin // reserved
                end

                6'h26: begin // reserved
                end

                6'h27: begin // reserved
                end

                6'h28: begin // reserved
                end

                6'h29: begin // reserved
                end

                6'h2a: begin // reserved
                end

                6'h2b: begin // reserved
                end

                6'h2c: begin // reserved
                end

                6'h2d: begin // reserved
                end

                6'h2e: begin // reserved
                end

                6'h2f: begin // reserved
                end

                6'h30: begin // reserved
                end

                6'h31: begin // reserved
                end

                6'h32: begin // reserved
                end

                6'h33: begin // reserved
                end

                6'h34: begin // reserved
                end

                6'h35: begin // reserved
                end

                6'h36: begin // reserved
                end

                6'h37: begin // reserved
                end

                6'h38: begin // reserved
                end

                6'h39: begin // reserved
                end

                6'h3a: begin // reserved
                end

                6'h3b: begin // RDHWR
                    metadata_vector = inst[10:10-5];
                    metadata_size = `MD_SIZE_5;
                end

                6'h3c: begin // reserved
                end

                6'h3d: begin // reserved
                end

                6'h3e: begin // reserved
                end

                6'h3f: begin // reserved
                end

            endcase
        end

        6'h20: begin // LB
        end

        6'h21: begin // LH
        end

        6'h22: begin // LWL
        end

        6'h23: begin // LW
        end

        6'h24: begin // LBU
        end

        6'h25: begin // LHU
        end

        6'h26: begin // LWR
        end

        6'h27: begin // LWU
        end

        6'h28: begin // SB
        end

        6'h29: begin // SH
        end

        6'h2a: begin // SWL
        end

        6'h2b: begin // SW
        end

        6'h2c: begin // SDL
        end

        6'h2d: begin // SDR
        end

        6'h2e: begin // SWR
        end

        6'h2f: begin // CACHE
        end

        6'h30: begin // LL
        end

        6'h31: begin // LWC1
        end

        6'h32: begin // LWC2
        end

        6'h33: begin // PREF
        end

        6'h34: begin // LLD
        end

        6'h35: begin // LDC1
        end

        6'h36: begin // LDC2
        end

        6'h37: begin // LD
        end

        6'h38: begin // SC
        end

        6'h39: begin // SWC1
        end

        6'h3a: begin // SWC2
        end

        6'h3b: begin // reserved
        end

        6'h3c: begin // SCD
        end

        6'h3d: begin // SDC1
        end

        6'h3e: begin // SDC2
        end

        6'h3f: begin // SD
        end

    endcase // opcode

    // for nop
    if (inst == 32'h0) begin
        metadata_size = `MD_SIZE_13;
        metadata_vector = {inst[25:16],inst[10:8]};
    end
    // for ssnop
    if (inst == 32'h40) begin
        metadata_size = `MD_SIZE_13;
        metadata_vector = {inst[25:16],inst[10:8]};
    end

end

endmodule
