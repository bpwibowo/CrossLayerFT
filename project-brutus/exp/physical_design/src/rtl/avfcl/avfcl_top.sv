`timescale 1ns/100ps

`define ACC_TOTAL_WIDTH 25
`define VBITS_TOTAL_WIDTH 16
`define DURATION_WIDTH (14-4) // drop 4 LSBs

module AvfCl_top (
    input               clk,
    input               reset,
    input               stall_fetch1,
    input               stall_fetch2,
    input [31:0]        first_inst,
    output              first_inst_nop,
    output [9:0]        fetch2decode_metadata,
    output              fetch2decode_valid,

    input               stall_metadata_decode,
    input [31:0]        inst_code [0:2-1],
    input [4:0]         retire_cnt,
    input [7:0]         retire_alid,
    output [3:0]        importance_flags,
    output              lc_valid,
    output [4:0]        lc_value,

    input               stall,
    input [1:0][9:0]    opcodes,
    output [1:0][3:0]   vops,

	input               isLoad,
	input               isStore,
	input               isControl,
	input [3:0]         vop,
	input               important,
	output [7:0]        iq_vbit,
	output [7:0]        rob_wb_vbit,
	output [7:0]        rob_dp_vbit,
	output [7:0]        ib_vbit,
	input [9:0]         opcode,
	output [7:0]        lq_vbit,
	output [7:0]        lq_dp_vbit,
	output [7:0]        sq_vbit,
	output [7:0]        sq_dp_vbit,

    input [2:0]       increment,
    input [7:0]       reset_value,
    output [7:0]      count256,
    output [5:0]      count64,
    output [15:0]     count65k,

    input [7:0]       counter_in,
    input [7:0]       counter_out,
    input             pop,
    input [`DURATION_WIDTH-1:0]      timestamp,
    output [`DURATION_WIDTH-1:0]     ib_duration,

    input [5:0]       iq_idx,
    input [7:0]       al_idx,
    input [7:0]       al_tail,
    output [`DURATION_WIDTH-1:0]     iq_duration_fast,
    output [`DURATION_WIDTH-1:0]     iq_duration_slow,

    input [5:0]       sq_idx_in,
    input [5:0]       sq_idx_exec,
    input [5:0]       sq_idx_out,
    output [`DURATION_WIDTH-1:0]     sq_duration_dp,
    output [`DURATION_WIDTH-1:0]     sq_duration_exec,

    input [5:0]       lq_idx_in,
    input [5:0]       lq_idx_exec,
    input [5:0]       lq_idx_out,
    output [`DURATION_WIDTH-1:0]     lq_duration_dp,
    output [`DURATION_WIDTH-1:0]     lq_duration_exec,

    input [7:0]       counter_wb,
    input [7:0]       al_idx_in,
    input [7:0]       al_idx_wb,
    input [7:0]       al_idx_out,
    input             fast_or_slow_in,
    output [`DURATION_WIDTH-1:0]     rob_duration_dp,
    output [`DURATION_WIDTH-1:0]     rob_duration_fast,
    output [`DURATION_WIDTH-1:0]     rob_duration_slow,

    input [3:0]       valid,
    input [3:0]       fast_or_slow,
    input [`DURATION_WIDTH-1:0]      duration,
    input [`DURATION_WIDTH-1:0]      duration_dp,
    input [`DURATION_WIDTH-1:0]      duration_wb,
    input [`DURATION_WIDTH-1:0] 	  duration_fast,
    input [`DURATION_WIDTH-1:0]      duration_slow,
    input [3:0][7:0]  vlt_values,
    input [3:0][7:0]  vlt_dp_values,
    input [3:0][7:0]  vlt_wb_values,
    input [5:0]       next_rlc,
    output [`ACC_TOTAL_WIDTH-1:0]     iq_acc,
    output [`ACC_TOTAL_WIDTH-1:0]     rob_acc,
    output [`ACC_TOTAL_WIDTH-1:0]     lq_acc,
    output [`ACC_TOTAL_WIDTH-1:0]     sq_acc,
    output [`ACC_TOTAL_WIDTH-1:0]     ib_acc,
    output [`ACC_TOTAL_WIDTH-1:0]     prf_acc,

    input  [197-1:0]  data_in,
    output            parity_190_out,
    output            parity_190_mem_data_out,
    output            parity_197_out,
    output            parity_197_mem_data_out,
    output            parity_76_out,
    output            parity_76_mem_data_out,
    output            parity_64_out,
    output            parity_64_mem_data_out,
    output            parity_82_out,
    output            parity_82_mem_data_out,
    output            parity_8_out,
    output            parity_8_mem_data_out,
    output            parity_6_out,
    output            parity_6_mem_data_out
	);

nop_lane u_nop_lane(
	.clk(clk),
    .reset(reset),
    .stall_fetch1(stall_fetch1),
    .stall_fetch2(stall_fetch2),
    .first_inst(first_inst),
    .first_inst_nop(first_inst_nop),
    .fetch2decode_metadata(fetch2decode_metadata),
    .fetch2decode_valid(fetch2decode_valid)
    );

metadata_decode u_md(
    .clk(clk),
    .reset(reset),
    .stall(stall_metadata_decode),
    .fetch2decode_metadata(fetch2decode_metadata),
    .fetch2decode_valid(fetch2decode_valid),
    .inst_code(inst_code),
    .retire_cnt(retire_cnt),
    .retire_alid(retire_alid),
    .importance_flags(importance_flags),
    .lc_valid(lc_valid),
    .lc_value(lc_value)
	);

vop_decoder u_vop_decoder (
    .clk(clk),
    .reset(reset),
    .stall(stall),
    .opcodes(opcodes),
    .vops(vops)
	);

vlt u_vlt (
    .isLoad(isLoad),
    .isStore(isStore),
    .isControl(isControl),
    .vop(vop),
    .important(important),
    .iq_vbit(iq_vbit),
    .rob_wb_vbit(rob_wb_vbit),
    .rob_dp_vbit(rob_dp_vbit),
    .ib_vbit(ib_vbit)
);

vlt_lq u_vlt_lq (
    .opcode(opcode[3:0]),
    .important(important),
    .lq_vbit(lq_vbit)
);

vlt_lq_dp u_vlt_lq_dp (
    .opcode(opcode[3:0]),
    .important(important),
    .lq_dp_vbit(lq_dp_vbit)
);

vlt_sq u_vlt_sq (
    .opcode(opcode[3:0]),
    .important(important),
    .sq_vbit(sq_vbit)
);

vlt_sq_dp u_vlt_sq_dp (
    .opcode(opcode[3:0]),
    .important(important),
    .sq_dp_vbit(sq_dp_vbit)
);

counter256 u_counter256(
    .clk(clk),
    .reset(reset),
    .increment(increment),
    .reset_value(reset_value),
    .count(count256)
);

counter64 u_counter64(
    .clk(clk),
    .reset(reset),
    .increment(increment),
    .reset_value(reset_value[5:0]),
    .count(count64)
);

counter65k u_counter65k(
	.clk(clk),
	.reset(reset),
	.count(count65k)
);

ib_dur u_ib_dur (
    .clk(clk),
    .reset(reset),
    .counter_in(counter_in),
    .counter_out(counter_out),
    .pop(pop),
    .timestamp(timestamp),
    .ib_duration(ib_duration)
);

iq_dur u_iq_dur (
    .clk(clk),
    .reset(reset),
    .counter_in(counter_in),
    .iq_idx(iq_idx),
    .al_idx(al_idx),
    .timestamp(timestamp),
    .al_tail(al_tail),
    .iq_duration_fast(iq_duration_fast),
    .iq_duration_slow(iq_duration_slow)
);

sq_dur u_sq_dur (
    .clk(clk),
    .reset(reset),
    .counter_in(counter_in),
    .timestamp(timestamp),
    .sq_idx_in(sq_idx_in),
    .sq_idx_exec(sq_idx_exec),
    .sq_idx_out(sq_idx_out),
    .sq_duration_dp(sq_duration_dp),
    .sq_duration_exec(sq_duration_exec)
);

lq_dur u_lq_dur (
    .clk(clk),
    .reset(reset),
    .counter_in(counter_in),
    .timestamp(timestamp),
    .lq_idx_in(lq_idx_in),
    .lq_idx_exec(lq_idx_exec),
    .lq_idx_out(lq_idx_out),
    .lq_duration_dp(lq_duration_dp),
    .lq_duration_exec(lq_duration_exec)
);

rob_dur u_rob_dur (
    .clk(clk),
    .reset(reset),
    .counter_in(counter_in),
    .counter_wb(counter_wb),
    .al_idx_in(al_idx_in),
    .al_idx_wb(al_idx_wb),
    .al_idx_out(al_idx_out),
    .fast_or_slow_in(fast_or_slow_in),
    .timestamp(timestamp),
    .rob_duration_dp(rob_duration_dp),
    .rob_duration_fast(rob_duration_fast),
    .rob_duration_slow(rob_duration_slow)
);

avfcl_acc_iq u_avfcl_acc_iq (
    .clk(clk),
    .reset(reset),
    .valid(valid),
    .fast_or_slow(fast_or_slow),
    .duration_fast(duration_fast),
    .duration_slow(duration_slow),
    .vlt_values(vlt_values),
    .counter256(count256),
    .iq_acc(iq_acc)
);

avfcl_acc_rob u_avfcl_acc_rob (
    .clk(clk),
    .reset(reset),
    .valid(valid),
    .fast_or_slow(fast_or_slow),
    .duration_dp(duration_dp),
    .duration_fast(duration_fast),
    .duration_slow(duration_slow),
    .vlt_dp_values(vlt_dp_values),
    .vlt_wb_values(vlt_wb_values),
    .counter256(count256),
    .rob_acc(rob_acc)
);

avfcl_acc_lq u_avfcl_acc_lq (
    .clk(clk),
    .reset(reset),
    .valid(valid),
    .duration_dp(duration_dp),
    .duration_wb(duration_wb),
    .vlt_dp_values(vlt_dp_values[0]),
    .vlt_wb_values(vlt_wb_values[0]),
    .lq_acc(lq_acc)
);

avfcl_acc_sq u_avfcl_acc_sq (
    .clk(clk),
    .reset(reset),
    .valid(valid),
    .duration_dp(duration_dp),
    .duration_wb(duration_wb),
    .vlt_dp_values(vlt_dp_values[0]),
    .vlt_wb_values(vlt_wb_values[0]),
    .sq_acc(sq_acc)
);

avfcl_acc_ib u_avfcl_acc_ib (
    .clk(clk),
    .reset(reset),
    .valid(valid),
    .duration(duration),
    .vlt_values(vlt_values),
    .counter256(count256),
    .ib_acc(ib_acc)
);

avfcl_acc_prf u_avfcl_acc_prf (
    .clk(clk),
    .reset(reset),
    .valid(valid[0]),
    .next_rlc(next_rlc),
    .prf_acc(prf_acc)
);

parity #(
    .WIDTH (190)
    )
    u_parity_190 (
    .data_in    (data_in[190-1:0]),
    .parity_out (parity_190_out)
);
parity_mem u_parity_190_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_190_out),
    .data_out   (parity_190_mem_data_out)
);

parity #(
    .WIDTH (197)
    )
    u_parity_197 (
    .data_in    (data_in[197-1:0]),
    .parity_out (parity_197_out)
);
parity_mem u_parity_197_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_197_out),
    .data_out   (parity_197_mem_data_out)
);

parity #(
    .WIDTH (76)
    )
    u_parity_76 (
    .data_in    (data_in[76-1:0]),
    .parity_out (parity_76_out)
);
parity_mem u_parity_76_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_76_out),
    .data_out   (parity_76_mem_data_out)
);

parity #(
    .WIDTH (64)
    )
    u_parity_64 (
    .data_in    (data_in[64-1:0]),
    .parity_out (parity_64_out)
);
parity_mem u_parity_64_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_64_out),
    .data_out   (parity_64_mem_data_out)
);

parity #(
    .WIDTH (82)
    )
    u_parity_82 (
    .data_in    (data_in[82-1:0]),
    .parity_out (parity_82_out)
);
parity_mem u_parity_82_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_82_out),
    .data_out   (parity_82_mem_data_out)
);

parity #(
    .WIDTH (8)
    )
    u_parity_8 (
    .data_in    (data_in[8-1:0]),
    .parity_out (parity_8_out)
);
parity_mem u_parity_8_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_8_out),
    .data_out   (parity_8_mem_data_out)
);


parity #(
    .WIDTH (6)
    )
    u_parity_6 (
    .data_in    (data_in[6-1:0]),
    .parity_out (parity_6_out)
);
parity_mem u_parity_6_mem (
    .clk        (clk),
    .reset      (reset),
    .data_in    (parity_6_out),
    .data_out   (parity_6_mem_data_out)
);


endmodule // AvfCl_top


