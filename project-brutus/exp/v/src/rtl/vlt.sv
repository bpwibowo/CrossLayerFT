module vlt (
    input clock
  , input reset
  , input important
  , input [8:0] opcode
  , input [9:0] start_ts
  , input [9:0] end_ts
  , input [11:0] prev_vbits
  , output [3:0] iq_shift1
  , output [3:0] iq_shift2
  , output iq_shift1_v
  , output iq_shift2_v
  , output [17:0] iq_vbits
  , output [3:0] rob_shift1
  , output [3:0] rob_shift2
  , output rob_shift1_v
  , output rob_shift2_v
  , output [17:0] rob_vbits
  , output [3:0] lq_shift1
  , output [3:0] lq_shift2
  , output lq_shift1_v
  , output lq_shift2_v
  , output [17:0] lq_vbits
  , output [3:0] sq_shift1
  , output [3:0] sq_shift2
  , output sq_shift1_v
  , output sq_shift2_v
  , output [17:0] sq_vbits
  , output [3:0] instbuff_shift1
  , output [3:0] instbuff_shift2
  , output instbuff_shift1_v
  , output instbuff_shift2_v
  , output [17:0] instbuff_vbits
  , input [11:0] total_qvbits0
  , input [11:0] total_qvbits1
  , input [11:0] total_qvbits2
  , input [11:0] total_qvbits3
  , input [9:0] rf_vbits_adj
  , input rf_vbits_adj_mode
  , input [11:0] total_sq_vbits
  , input [31:0] rf_live_vector
  , input [31:0] rf_kill_vector
  , output [19:0] total_svf
  , output [9:0] counter_iq
  , output [9:0] counter_rob
  , output [9:0] counter_lq
  , output [9:0] counter_sq
  , output [9:0] counter_instbuff
  );

// Instantiate vlt_IQ

wire [9:0] iq_duration;

vlt_IQ u_vlt_iq (
  .important_i  (important),
  .opcode_i     (opcode),
  .start_ts_i   (start_ts),
  .end_ts_i     (end_ts),
  .shift1_o     (iq_shift1),
  .shift2_o     (iq_shift2),
  .shift1_v_o   (iq_shift1_v),
  .shift2_v_o   (iq_shift2_v),
  .duration_o   (iq_duration)
);

addshift8x10 u_mul_iq (
    .clk        (clock),
    .reset      (reset),
    .in12_i     (prev_vbits),
    .in10_i     (iq_duration),
    .shift1_i   (iq_shift1),
    .shift1_v_i (iq_shift1_v),
    .shift2_i   (iq_shift2),
    .shift2_v_i (iq_shift2_v),
    .mul_o      (iq_vbits)
);

counter10 u_ctr_iq (
  .clk    (clock),
  .reset  (reset),
  .counter(counter_iq)
);

// Instantiate vlt_ROB

wire [9:0] rob_duration;

vlt_ROB u_vlt_rob (
  .important_i  (important),
  .opcode_i     (opcode),
  .start_ts_i   (start_ts),
  .end_ts_i     (end_ts),
  .shift1_o     (rob_shift1),
  .shift2_o     (rob_shift2),
  .shift1_v_o   (rob_shift1_v),
  .shift2_v_o   (rob_shift2_v),
  .duration_o   (rob_duration)
);

addshift8x10 u_mul_rob (
    .clk        (clock),
    .reset      (reset),
    .in12_i     (prev_vbits),
    .in10_i     (rob_duration),
    .shift1_i   (rob_shift1),
    .shift1_v_i (rob_shift1_v),
    .shift2_i   (rob_shift2),
    .shift2_v_i (rob_shift2_v),
    .mul_o      (rob_vbits)
);

counter10 u_ctr_rob (
  .clk    (clock),
  .reset  (reset),
  .counter(counter_rob)
);

// Instantiate vlt_LQ

wire [9:0] lq_duration;

vlt_LQ u_vlt_lq (
  .important_i  (important),
  .opcode_i     (opcode),
  .start_ts_i   (start_ts),
  .end_ts_i     (end_ts),
  .shift1_o     (lq_shift1),
  .shift2_o     (lq_shift2),
  .shift1_v_o   (lq_shift1_v),
  .shift2_v_o   (lq_shift2_v),
  .duration_o   (lq_duration)
);

addshift8x10 u_mul_lq (
    .clk        (clock),
    .reset      (reset),
    .in12_i     (prev_vbits),
    .in10_i     (lq_duration),
    .shift1_i   (lq_shift1),
    .shift1_v_i (lq_shift1_v),
    .shift2_i   (lq_shift2),
    .shift2_v_i (lq_shift2_v),
    .mul_o      (lq_vbits)
);

counter10 u_ctr_lq (
  .clk    (clock),
  .reset  (reset),
  .counter(counter_lq)
);

// Instantiate vlt_SQ

wire [9:0] sq_duration;

vlt_SQ u_vlt_sq (
  .important_i  (important),
  .opcode_i     (opcode),
  .start_ts_i   (start_ts),
  .end_ts_i     (end_ts),
  .shift1_o     (sq_shift1),
  .shift2_o     (sq_shift2),
  .shift1_v_o   (sq_shift1_v),
  .shift2_v_o   (sq_shift2_v),
  .duration_o   (sq_duration)
);

addshift8x10 u_mul_sq (
    .clk        (clock),
    .reset      (reset),
    .in12_i     (prev_vbits),
    .in10_i     (sq_duration),
    .shift1_i   (sq_shift1),
    .shift1_v_i (sq_shift1_v),
    .shift2_i   (sq_shift2),
    .shift2_v_i (sq_shift2_v),
    .mul_o      (sq_vbits)
);

counter10 u_ctr_sq (
  .clk    (clock),
  .reset  (reset),
  .counter(counter_sq)
);

// Instantiate vlt_InstBuff

wire [9:0] instbuff_duration;

vlt_InstBuff u_vlt_instbuff (
  .important_i  (important),
  .opcode_i     (opcode),
  .start_ts_i   (start_ts),
  .end_ts_i     (end_ts),
  .shift1_o     (instbuff_shift1),
  .shift2_o     (instbuff_shift2),
  .shift1_v_o   (instbuff_shift1_v),
  .shift2_v_o   (instbuff_shift2_v),
  .duration_o   (instbuff_duration)
);

addshift8x10 u_mul_instbuff (
    .clk        (clock),
    .reset      (reset),
    .in12_i     (12'b0),
    .in10_i     (instbuff_duration),
    .shift1_i   (instbuff_shift1),
    .shift1_v_i (instbuff_shift1_v),
    .shift2_i   (instbuff_shift2),
    .shift2_v_i (instbuff_shift2_v),
    .mul_o      (instbuff_vbits)
);

counter10 u_ctr_instbuff (
  .clk    (clock),
  .reset  (reset),
  .counter(counter_instbuff)
);

// Instantiate SPU

spu u_spu (
    .clk                   (clock),
    .reset                 (reset),
    .total_qvbits0_i       (total_qvbits0),
    .total_qvbits1_i       (total_qvbits1),
    .total_qvbits2_i       (total_qvbits2),
    .total_qvbits3_i       (total_qvbits3),
    .rf_vbits_adj_i        (rf_vbits_adj),
    .rf_vbits_adj_mode_i   (rf_vbits_adj_mode),
    .total_sq_vbits_i      (total_sq_vbits),
    .rf_live_vector_i      (rf_live_vector),
    .rf_kill_vector_i      (rf_kill_vector),
    .total_svf_o           (total_svf)
);

endmodule
