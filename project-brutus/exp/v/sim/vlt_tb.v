// Uncomment the following to dump vcd file
`define VCD

module vlt_tb ();

localparam clk_period = 10;
integer n;

reg sim_clock;
reg sim_reset;

initial begin
  #0;
  sim_clock = 1'b0;
  sim_reset = 1'b1;
  #100;
  sim_reset = 1'b0;
end

always @(sim_clock)
  #(clk_period/2) sim_clock <= ~sim_clock;

reg  important;
reg [8:0] opcode;
reg [9:0] start_ts;
reg [9:0] end_ts;
reg [11:0] prev_vbits;
reg [11:0] total_qvbits0;
reg [11:0] total_qvbits1;
reg [11:0] total_qvbits2;
reg [11:0] total_qvbits3;
reg [9:0] rf_vbits_adj;
reg rf_vbits_adj_mode;
reg [11:0] total_sq_vbits;
reg [31:0] rf_live_vector;
reg [31:0] rf_kill_vector;

wire [3:0] iq_shift1;
wire [3:0] iq_shift2;
wire iq_shift1_v;
wire iq_shift2_v;
wire [17:0] iq_vbits;
wire [3:0] rob_shift1;
wire [3:0] rob_shift2;
wire rob_shift1_v;
wire rob_shift2_v;
wire [17:0] rob_vbits;
wire [3:0] lq_shift1;
wire [3:0] lq_shift2;
wire lq_shift1_v;
wire lq_shift2_v;
wire [17:0] lq_vbits;
wire [3:0] sq_shift1;
wire [3:0] sq_shift2;
wire sq_shift1_v;
wire sq_shift2_v;
wire [17:0] sq_vbits;
wire [3:0] instbuff_shift1;
wire [3:0] instbuff_shift2;
wire instbuff_shift1_v;
wire instbuff_shift2_v;
wire [17:0] instbuff_vbits;
wire [19:0] total_svf;
wire [9:0] counter_iq;
wire [9:0] counter_rob;
wire [9:0] counter_lq;
wire [9:0] counter_sq;
wire [9:0] counter_instbuff;

integer sim_cnt;
integer vlt_iq_active_cnt;
integer vlt_rob_active_cnt;
integer vlt_lq_active_cnt;
integer vlt_sq_active_cnt;
integer vlt_instbuff_active_cnt;

vlt u_vlt (
    .clock (sim_clock),
    .reset (sim_reset),
    .important (important),
    .opcode (opcode),
    .start_ts (start_ts),
    .end_ts (end_ts),
    .prev_vbits (prev_vbits),
    .iq_shift1 (iq_shift1),
    .iq_shift2 (iq_shift2),
    .iq_shift1_v (iq_shift1_v),
    .iq_shift2_v (iq_shift2_v),
    .iq_vbits (iq_vbits),
    .rob_shift1 (rob_shift1),
    .rob_shift2 (rob_shift2),
    .rob_shift1_v (rob_shift1_v),
    .rob_shift2_v (rob_shift2_v),
    .rob_vbits (rob_vbits),
    .lq_shift1 (lq_shift1),
    .lq_shift2 (lq_shift2),
    .lq_shift1_v (lq_shift1_v),
    .lq_shift2_v (lq_shift2_v),
    .lq_vbits (lq_vbits),
    .sq_shift1 (sq_shift1),
    .sq_shift2 (sq_shift2),
    .sq_shift1_v (sq_shift1_v),
    .sq_shift2_v (sq_shift2_v),
    .sq_vbits (sq_vbits),
    .instbuff_shift1 (instbuff_shift1),
    .instbuff_shift2 (instbuff_shift2),
    .instbuff_shift1_v (instbuff_shift1_v),
    .instbuff_shift2_v (instbuff_shift2_v),
    .instbuff_vbits (instbuff_vbits),
    .total_qvbits0 (total_qvbits0),
    .total_qvbits1 (total_qvbits1),
    .total_qvbits2 (total_qvbits2),
    .total_qvbits3 (total_qvbits3),
    .rf_vbits_adj (rf_vbits_adj),
    .rf_vbits_adj_mode (rf_vbits_adj_mode),
    .total_sq_vbits (total_sq_vbits),
    .rf_live_vector (rf_live_vector),
    .rf_kill_vector (rf_kill_vector),
    .total_svf (total_svf),
    .counter_iq (counter_iq),
    .counter_rob (counter_rob),
    .counter_lq (counter_lq),
    .counter_sq (counter_sq),
    .counter_instbuff (counter_instbuff)
);

// Test-bench procedure
initial begin

  important = 1;
  opcode = 0;
  start_ts = 0;
  end_ts = 0;

  sim_cnt = 0;
  vlt_iq_active_cnt = 0;
  vlt_rob_active_cnt = 0;
  vlt_lq_active_cnt = 0;
  vlt_sq_active_cnt = 0;
  vlt_instbuff_active_cnt = 0;

  #1000;
  $display("%t: Starting random input simulation...", $time);
  for (n=0; n<100000; n=n+1) begin
    important = $random();
    opcode = $random();
    start_ts = $random();
    end_ts = $random();
    prev_vbits = $random();
    total_qvbits0 = $random();
    total_qvbits1 = $random();
    total_qvbits2 = $random();
    total_qvbits3 = $random();
    rf_vbits_adj = $random();
    rf_vbits_adj_mode = $random();
    total_sq_vbits = $random();
    rf_live_vector = $random();
    rf_kill_vector = $random();

    if (iq_shift1_v | iq_shift2_v)
        vlt_iq_active_cnt = vlt_iq_active_cnt + 1;
    if (rob_shift1_v | rob_shift2_v)
        vlt_rob_active_cnt = vlt_rob_active_cnt + 1;
    if (lq_shift1_v | lq_shift2_v)
        vlt_lq_active_cnt = vlt_lq_active_cnt + 1;
    if (sq_shift1_v | sq_shift2_v)
        vlt_sq_active_cnt = vlt_sq_active_cnt + 1;
    if (instbuff_shift1_v | instbuff_shift2_v)
        vlt_instbuff_active_cnt = vlt_instbuff_active_cnt + 1;
    sim_cnt = sim_cnt + 1;

    #clk_period;
  end
  $display("%t: Random input simulation completed.", $time);

  $display("sim_cnt = %0d", sim_cnt);
  $display("IQ active cycles = %0d", vlt_iq_active_cnt);
  $display("ROB active cycles = %0d", vlt_rob_active_cnt);
  $display("LQ active cycles = %0d", vlt_lq_active_cnt);
  $display("SQ active cycles = %0d", vlt_sq_active_cnt);
  $display("InstBuff active cycles = %0d", vlt_instbuff_active_cnt);

  $finish;
end

// Use `define VCD to allow dumping all value changes to waves.vcd
`ifdef VCD
initial begin
  $dumpfile("waves.vcd");
  $dumpvars;
end
`endif

endmodule // vlt_tb
