// Uncomment the following to dump vcd file
`define VCD

module pmem_tb #(
  parameter SRAM_INDEX  = 10,
  parameter SRAM_WIDTH  = 197,
  parameter READ_PORTS  = 8,
  parameter WRITE_PORTS = 4) ();

localparam clk_period = 10;
integer n;

reg sim_clock;
reg sim_reset;

logic [READ_PORTS-1:0][SRAM_INDEX-1:0]  rdAddr;
logic [379:0] sram_out_InstBuffMem;
logic [1:0] sram_out_BPMem;
logic [127:0] sram_out_BTBMem;
logic [63:0] sram_out_RASMem;
logic [63:0] sram_out_CTIQMem;
logic [787:0] sram_out_PayloadRAMMem;
logic [75:0] sram_out_STQ_DATA_RAMMem;
logic [0:0] sram_out_StqImportanceBitMem;
logic [511:0] sram_out_RegFileMem;
logic [31:0] sram_out_AMTMem;
logic [31:0] sram_out_FreelistMem;
logic [31:0] sram_out_RMTMem;
logic [327:0] sram_out_ActiveListMem;
logic [303:0] sram_out_ActiveListWbMem;
logic [15:0] sram_out_ActiveListAvfClMem;
logic [3:0] sram_out_ActiveListAvfClWbMem;

logic [2-1:0][1-1:0] sram_out_InstBuffParityMem;
logic [4-1:0][1-1:0] sram_out_PayloadRAMParityMem;
logic [1-1:0][1-1:0] sram_out_STQ_DATA_RAMParityMem;
logic [8-1:0][1-1:0] sram_out_RegFileParityMem;
logic [4-1:0][1-1:0] sram_out_ActiveListParityMem;
logic [4-1:0][1-1:0] sram_out_ActiveListWbParityMem;
logic [2-1:0][9-1:0] sram_out_InstBuffEccMem;
logic [4-1:0][9-1:0] sram_out_PayloadRAMEccMem;
logic [1-1:0][8-1:0] sram_out_STQ_DATA_RAMEccMem;
logic [8-1:0][8-1:0] sram_out_RegFileEccMem;
logic [4-1:0][8-1:0] sram_out_ActiveListEccMem;
logic [4-1:0][8-1:0] sram_out_ActiveListWbEccMem;

logic [READ_PORTS-1:0]                  re;
logic [WRITE_PORTS-1:0][SRAM_INDEX-1:0] wrAddr;
logic [WRITE_PORTS-1:0][SRAM_WIDTH-1:0] wrData;
logic [WRITE_PORTS-1:0]                 we;

logic               stall_fetch1;
logic               stall_fetch2;
logic [31:0]        first_inst;
logic              first_inst_nop;
logic [12:0]       fetch2decode_metadata;
logic              fetch2decode_valid;
logic               stall_metadata_decode;
logic [1:0][31:0]        inst_code;
logic [4:0]         retire_cnt;
logic [7:0]         retire_alid;
logic [4-1:0]      importance_flags;
logic              lc_valid;
logic [4:0]        lc_value;

logic               stall;
logic [1:0][9:0]    opcodes;
logic [1:0][3:0]   vops;

logic               isLoad;
logic               isStore;
logic               isControl;
logic [3:0]         vop;
logic               important;
logic [7:0]        iq_vbit;
logic [7:0]        rob_wb_vbit;
logic [7:0]        rob_dp_vbit;
logic [7:0]        ib_vbit;
logic [9:0]         opcode;
logic [7:0]        lq_vbit;
logic [7:0]        lq_dp_vbit;
logic [7:0]        sq_vbit;
logic [7:0]        sq_dp_vbit;

logic [2:0]       increment;
logic [7:0]       reset_value;
logic [7:0]      count256;
logic [5:0]      count64;
logic [15:0]     count65k;

logic [7:0]       counter_in;
logic [7:0]       counter_out;
logic             pop;
logic [9:0]      timestamp;
logic [9:0]     ib_duration;

logic [5:0]       iq_idx;
logic [7:0]       al_idx;
logic [7:0]       al_tail;
logic [9:0]     iq_duration_fast;
logic [9:0]     iq_duration_slow;

logic [5:0]       sq_idx_in;
logic [5:0]       sq_idx_exec;
logic [5:0]       sq_idx_out;
logic [9:0]     sq_duration_dp;
logic [9:0]     sq_duration_exec;

logic [5:0]       lq_idx_in;
logic [5:0]       lq_idx_exec;
logic [5:0]       lq_idx_out;
logic [9:0]     lq_duration_dp;
logic [9:0]     lq_duration_exec;

logic [7:0]       counter_wb;
logic [7:0]       al_idx_in;
logic [7:0]       al_idx_wb;
logic [7:0]       al_idx_out;
logic             fast_or_slow_in;
logic [9:0]     rob_duration_dp;
logic [9:0]     rob_duration_fast;
logic [9:0]     rob_duration_slow;

logic [3:0]       valid;
logic [3:0]       fast_or_slow;
logic [9:0]      duration;
logic [9:0]      duration_dp;
logic [9:0]      duration_wb;
logic [9:0]      duration_fast;
logic [9:0]      duration_slow;
logic [3:0][7:0]  vlt_values;
logic [3:0][7:0]  vlt_dp_values;
logic [3:0][7:0]  vlt_wb_values;
logic [5:0]       next_rlc;
logic [24:0]     iq_acc;
logic [24:0]     rob_acc;
logic [24:0]     lq_acc;
logic [24:0]     sq_acc;
logic [24:0]     ib_acc;
logic [24:0]     prf_acc;

logic [197-1:0]  data_in;
logic            parity_190_out;
logic            parity_190_mem_data_out;
logic            parity_197_out;
logic            parity_197_mem_data_out;
logic            parity_76_out;
logic            parity_76_mem_data_out;
logic            parity_64_out;
logic            parity_64_mem_data_out;
logic            parity_82_out;
logic            parity_82_mem_data_out;
logic            parity_8_out;
logic            parity_8_mem_data_out;
logic            parity_6_out;
logic            parity_6_mem_data_out;

int write_cnt[WRITE_PORTS];
int read_cnt[READ_PORTS];

bit sim_clock_gated;

initial begin
  #0;
  sim_clock = 1'b0;
  sim_reset = 1'b1;
  sim_clock_gated = 0;
  #100;
  sim_reset = 1'b0;

  $display("%t: reset cleared", $time);
end

always begin
  #(clk_period/2);
  if (sim_clock_gated == 0)
    sim_clock <= ~sim_clock;
end

SRAM_top u_pmem (
  .clk      (sim_clock),
  .reset    (sim_reset),
  .rdAddr_i (rdAddr),
  .sram_out_InstBuffMem (sram_out_InstBuffMem),
  .sram_out_BPMem (sram_out_BPMem),
  .sram_out_BTBMem (sram_out_BTBMem),
  .sram_out_RASMem (sram_out_RASMem),
  .sram_out_CTIQMem (sram_out_CTIQMem),
  .sram_out_PayloadRAMMem (sram_out_PayloadRAMMem),
  .sram_out_STQ_DATA_RAMMem (sram_out_STQ_DATA_RAMMem),
  .sram_out_StqImportanceBitMem (sram_out_StqImportanceBitMem),
  .sram_out_RegFileMem (sram_out_RegFileMem),
  .sram_out_AMTMem (sram_out_AMTMem),
  .sram_out_FreelistMem (sram_out_FreelistMem),
  .sram_out_RMTMem (sram_out_RMTMem),
  .sram_out_ActiveListMem (sram_out_ActiveListMem),
  .sram_out_ActiveListWbMem (sram_out_ActiveListWbMem),
  .sram_out_ActiveListAvfClMem (sram_out_ActiveListAvfClMem),
  .sram_out_ActiveListAvfClWbMem (sram_out_ActiveListAvfClWbMem),
  .sram_out_InstBuffParityMem (sram_out_InstBuffParityMem),
  .sram_out_PayloadRAMParityMem (sram_out_PayloadRAMParityMem),
  .sram_out_STQ_DATA_RAMParityMem (sram_out_STQ_DATA_RAMParityMem),
  .sram_out_RegFileParityMem (sram_out_RegFileParityMem),
  .sram_out_ActiveListParityMem (sram_out_ActiveListParityMem),
  .sram_out_ActiveListWbParityMem (sram_out_ActiveListWbParityMem),
  .sram_out_InstBuffEccMem (sram_out_InstBuffEccMem),
  .sram_out_PayloadRAMEccMem (sram_out_PayloadRAMEccMem),
  .sram_out_STQ_DATA_RAMEccMem (sram_out_STQ_DATA_RAMEccMem),
  .sram_out_RegFileEccMem (sram_out_RegFileEccMem),
  .sram_out_ActiveListEccMem (sram_out_ActiveListEccMem),
  .sram_out_ActiveListWbEccMem (sram_out_ActiveListWbEccMem),
  .wrAddr_i (wrAddr),
  .wrData_i (wrData),
  .we_i     (we)
);

AvfCl_top u_avfcl (
  .clk(sim_clock),
  .reset(sim_reset),
  .stall_fetch1(stall_fetch1),
  .stall_fetch2(stall_fetch2),
  .first_inst(first_inst),
  .first_inst_nop(first_inst_nop),
  .fetch2decode_metadata(fetch2decode_metadata),
  .fetch2decode_valid(fetch2decode_valid),
  .stall_metadata_decode(stall_metadata_decode),
  .inst_code(inst_code),
  .retire_cnt(retire_cnt),
  .retire_alid(retire_alid),
  .importance_flags(importance_flags),
  .lc_valid(lc_valid),
  .lc_value(lc_value),

  .stall(stall),
  .opcodes(opcodes),
  .vops(vops),

  .isLoad(isLoad),
  .isStore(isStore),
  .isControl(isControl),
  .vop(vop),
  .important(important),
  .iq_vbit(iq_vbit),
  .rob_wb_vbit(rob_wb_vbit),
  .rob_dp_vbit(rob_dp_vbit),
  .ib_vbit(ib_vbit),
  .opcode(opcode),
  .lq_vbit(lq_vbit),
  .lq_dp_vbit(lq_dp_vbit),
  .sq_vbit(sq_vbit),
  .sq_dp_vbit(sq_dp_vbit),

  .increment(increment),
  .reset_value(reset_value),
  .count256(count256),
  .count64(count64),
  .count65k(count65k),

  .counter_in(counter_in),
  .counter_out(counter_out),
  .pop(pop),
  .timestamp(timestamp),
  .ib_duration(ib_duration),

  .iq_idx(iq_idx),
  .al_idx(al_idx),
  .al_tail(al_tail),
  .iq_duration_fast(iq_duration_fast),
  .iq_duration_slow(iq_duration_slow),

  .sq_idx_in(sq_idx_in),
  .sq_idx_exec(sq_idx_exec),
  .sq_idx_out(sq_idx_out),
  .sq_duration_dp(sq_duration_dp),
  .sq_duration_exec(sq_duration_exec),

  .lq_idx_in(lq_idx_in),
  .lq_idx_exec(lq_idx_exec),
  .lq_idx_out(lq_idx_out),
  .lq_duration_dp(lq_duration_dp),
  .lq_duration_exec(lq_duration_exec),

  .counter_wb(counter_wb),
  .al_idx_in(al_idx_in),
  .al_idx_wb(al_idx_wb),
  .al_idx_out(al_idx_out),
  .fast_or_slow_in(fast_or_slow_in),
  .rob_duration_dp(rob_duration_dp),
  .rob_duration_fast(rob_duration_fast),
  .rob_duration_slow(rob_duration_slow),

  .valid(valid),
  .fast_or_slow(fast_or_slow),
  .duration(duration),
  .duration_dp(duration_dp),
  .duration_wb(duration_wb),
  .duration_fast(duration_fast),
  .duration_slow(duration_slow),
  .vlt_values(vlt_values),
  .vlt_dp_values(vlt_dp_values),
  .vlt_wb_values(vlt_wb_values),
  .next_rlc(next_rlc),
  .iq_acc(iq_acc),
  .rob_acc(rob_acc),
  .lq_acc(lq_acc),
  .sq_acc(sq_acc),
  .ib_acc(ib_acc),
  .prf_acc(prf_acc),

  .data_in(data_in),
  .parity_190_out(parity_190_out),
  .parity_190_mem_data_out(parity_190_mem_data_out),
  .parity_197_out(parity_197_out),
  .parity_197_mem_data_out(parity_197_mem_data_out),
  .parity_76_out(parity_76_out),
  .parity_76_mem_data_out(parity_76_mem_data_out),
  .parity_64_out(parity_64_out),
  .parity_64_mem_data_out(parity_64_mem_data_out),
  .parity_82_out(parity_82_out),
  .parity_82_mem_data_out(parity_82_mem_data_out),
  .parity_8_out(parity_8_out),
  .parity_8_mem_data_out(parity_8_mem_data_out),
  .parity_6_out(parity_6_out),
  .parity_6_mem_data_out(parity_6_mem_data_out)
);

// Test-bench procedure
initial begin
  int i, j;

  re = 0;
  rdAddr = 0;
  we = 0;
  wrAddr = 0;
  wrData = 0;

  // avf-cl
  stall_fetch1 = 0;
  stall_fetch2 = 0;
  stall_metadata_decode = 0;
  first_inst = 0;
  inst_code[0] = 0;
  inst_code[1] = 0;
  retire_cnt = 0;
  retire_alid = 0;
  stall = 0;

  pop = 0;
  opcodes = 0;
  isLoad = 0;
  isStore = 0;
  isControl = 0;
  vop = 0;
  important = 0;
  opcode = 0;
  increment = 0;
  reset_value = 0;
  counter_in = 0;
  counter_out = 0;
  timestamp = 0;
  iq_idx = 0;
  al_idx = 0;
  al_tail = 0;
  sq_idx_in = 0;
  sq_idx_exec = 0;
  sq_idx_out = 0;
  lq_idx_in = 0;
  lq_idx_exec = 0;
  lq_idx_out = 0;
  counter_wb = 0;
  al_idx_in = 0;
  al_idx_wb = 0;
  al_idx_out = 0;
  fast_or_slow_in = 0;
  valid = 0;
  fast_or_slow = 0;
  duration = 0;
  duration_dp = 0;
  duration_wb = 0;
  duration_fast = 0;
  duration_slow = 0;
  vlt_values = 0;
  vlt_dp_values = 0;
  vlt_wb_values = 0;
  next_rlc = 0;

  #1000;

  for (i=0; i<WRITE_PORTS; i=i+1)
    write_cnt[i] = 0;
  for (i=0; i<READ_PORTS; i=i+1)
    read_cnt[i] = 0;
  $display("%t: Starting random input simulation...", $time);
  for (n=0; n<1000; n=n+1) begin
	if (n%1000 == 0) begin
	  $display("%t: Running iteration %0d...", $time, n);
	end
    for (i=0; i<$size(re); i=i+1) begin
      re[i] = $random();
      if (re[i]) begin
        rdAddr[i] = $random();
        read_cnt[i] += 1;
      end
    end
    for (i=0; i<$size(we); i=i+1) begin
      we[i] = $random();
      if (we[i]) begin
        wrAddr[i] = $random;
        for (j=0; j<SRAM_WIDTH; j=j+1) begin
          wrData[i][j] = $random();
        end
        write_cnt[i] += 1;
      end
    end
    for (i=0; i<$size(data_in); i=i+1) begin
      data_in[i] = $random();
    end

    // avf-cl
    stall_fetch1 = 0;
    stall_fetch2 = 0;
    stall_metadata_decode = 0;
    stall = 0;
    first_inst = $random();
    inst_code[0] = $random();
    inst_code[1] = $random();
    retire_cnt = $random();
    retire_alid = $random();
    pop = $random();
    opcodes = $random();
    isLoad = $random();
    isStore = $random();
    isControl = $random();
    vop = $random();
    important = $random();
    opcode = $random();
    increment = $random();
    reset_value = $random();
    counter_in = $random();
    counter_out = $random();
    timestamp = $random();
    iq_idx = $random();
    al_idx = $random();
    al_tail = $random();
    sq_idx_in = $random();
    sq_idx_exec = $random();
    sq_idx_out = $random();
    lq_idx_in = $random();
    lq_idx_exec = $random();
    lq_idx_out = $random();
    counter_wb = $random();
    al_idx_in = $random();
    al_idx_wb = $random();
    al_idx_out = $random();
    fast_or_slow_in = $random();
    valid = $random();
    fast_or_slow = $random();
    duration = $random();
    duration_dp = $random();
    duration_wb = $random();
    duration_fast = $random();
    duration_slow = $random();
    vlt_values = $random();
    vlt_dp_values = $random();
    vlt_wb_values = $random();
    next_rlc = $random();

    #clk_period;
  end

  stall_fetch1 = 1;
  stall_fetch2 = 1;
  stall_metadata_decode = 1;
  stall = 1;
  pop = 0;
  valid = 0;

  $display("%t: Random input simulation completed.", $time);

  for (i=0; i<WRITE_PORTS; i=i+1)
    $display("write_cnt[%0d] = %0d", i, write_cnt[i]);
  for (i=0; i<READ_PORTS; i=i+1)
    $display("read_cnt[%0d] = %0d", i, read_cnt[i]);

  #1000;

  for (i=0; i<WRITE_PORTS; i=i+1)
    write_cnt[i] = 0;
  for (i=0; i<READ_PORTS; i=i+1)
    read_cnt[i] = 0;
  $display("%t: Starting no read simulation...", $time);
  for (n=0; n<1000; n=n+1) begin
  if (n%1000 == 0) begin
    $display("%t: Running iteration %0d...", $time, n);
  end
    for (i=0; i<$size(re); i=i+1) begin
      re[i] = 0;
      if (re[i]) begin
        rdAddr[i] = $random();
        read_cnt[i] += 1;
      end
    end
    for (i=0; i<$size(we); i=i+1) begin
      we[i] = $random();
      if (we[i]) begin
        wrAddr[i] = $random;
        for (j=0; j<SRAM_WIDTH; j=j+1) begin
          wrData[i][j] = $random();
        end
        write_cnt[i] += 1;
      end
    end
    for (i=0; i<$size(data_in); i=i+1) begin
      data_in[i] = $random();
    end

    // avf-cl
    stall_fetch1 = 0;
    stall_fetch2 = 0;
    stall_metadata_decode = 0;
    first_inst = $random();
    inst_code[0] = $random();
    inst_code[1] = $random();
    pop = $random();
    opcodes = $random();
    isLoad = $random();
    isStore = $random();
    isControl = $random();
    vop = $random();
    important = $random();
    opcode = $random();
    increment = $random();
    reset_value = $random();
    counter_in = $random();
    counter_out = $random();
    timestamp = $random();
    iq_idx = $random();
    al_idx = $random();
    al_tail = $random();
    sq_idx_in = $random();
    sq_idx_exec = $random();
    sq_idx_out = $random();
    lq_idx_in = $random();
    lq_idx_exec = $random();
    lq_idx_out = $random();
    counter_wb = $random();
    al_idx_in = $random();
    al_idx_wb = $random();
    al_idx_out = $random();
    fast_or_slow_in = $random();
    valid = $random();
    fast_or_slow = $random();
    duration = $random();
    duration_dp = $random();
    duration_wb = $random();
    duration_fast = $random();
    duration_slow = $random();
    vlt_values = $random();
    vlt_dp_values = $random();
    vlt_wb_values = $random();
    next_rlc = $random();

    #clk_period;
  end

  stall_fetch1 = 1;
  stall_fetch2 = 1;
  stall_metadata_decode = 1;
  stall = 1;
  pop = 0;
  valid = 0;

  $display("%t: no read simulation completed.", $time);

  for (i=0; i<WRITE_PORTS; i=i+1)
    $display("write_cnt[%0d] = %0d", i, write_cnt[i]);
  for (i=0; i<READ_PORTS; i=i+1)
    $display("read_cnt[%0d] = %0d", i, read_cnt[i]);

  #1000;

  for (i=0; i<WRITE_PORTS; i=i+1)
    write_cnt[i] = 0;
  for (i=0; i<READ_PORTS; i=i+1)
    read_cnt[i] = 0;
  $display("%t: Starting no write simulation...", $time);
  for (n=0; n<1000; n=n+1) begin
  if (n%1000 == 0) begin
    $display("%t: Running iteration %0d...", $time, n);
  end
    for (i=0; i<$size(re); i=i+1) begin
      re[i] = $random();
      if (re[i]) begin
        rdAddr[i] = $random();
        read_cnt[i] += 1;
      end
    end
    for (i=0; i<$size(we); i=i+1) begin
      we[i] = 0;
      if (we[i]) begin
        wrAddr[i] = $random;
        for (j=0; j<SRAM_WIDTH; j=j+1) begin
          wrData[i][j] = $random();
        end
        write_cnt[i] += 1;
      end
    end
    for (i=0; i<$size(data_in); i=i+1) begin
      data_in[i] = $random();
    end

    // avf-cl
    stall_fetch1 = 0;
    stall_fetch2 = 0;
    stall_metadata_decode = 0;
    retire_cnt = $random();
    retire_alid = $random();
    pop = $random();
    opcodes = $random();
    isLoad = $random();
    isStore = $random();
    isControl = $random();
    vop = $random();
    important = $random();
    opcode = $random();
    increment = $random();
    reset_value = $random();
    counter_in = $random();
    counter_out = $random();
    timestamp = $random();
    iq_idx = $random();
    al_idx = $random();
    al_tail = $random();
    sq_idx_in = $random();
    sq_idx_exec = $random();
    sq_idx_out = $random();
    lq_idx_in = $random();
    lq_idx_exec = $random();
    lq_idx_out = $random();
    counter_wb = $random();
    al_idx_in = $random();
    al_idx_wb = $random();
    al_idx_out = $random();
    fast_or_slow_in = $random();
    valid = $random();
    fast_or_slow = $random();
    duration = $random();
    duration_dp = $random();
    duration_wb = $random();
    duration_fast = $random();
    duration_slow = $random();
    vlt_values = $random();
    vlt_dp_values = $random();
    vlt_wb_values = $random();
    next_rlc = $random();

    #clk_period;
  end

  stall_fetch1 = 1;
  stall_fetch2 = 1;
  stall_metadata_decode = 1;
  stall = 1;
  pop = 0;
  valid = 0;

  for (i=0; i<WRITE_PORTS; i=i+1)
    $display("write_cnt[%0d] = %0d", i, write_cnt[i]);
  for (i=0; i<READ_PORTS; i=i+1)
    $display("read_cnt[%0d] = %0d", i, read_cnt[i]);

  $display("%t: No write simulation completed.", $time);

  sim_clock_gated = 1;
  $display("%t: Starting clock gated simulation...", $time);
  #475;
  sim_clock_gated = 0;
  #50;
  sim_clock_gated = 1;
  #475;
  $display("%t: Clock gated simulation completed.", $time);
  sim_clock_gated = 0;

  $finish;
end

// Use `define VCD to allow dumping all value changes to waves.vcd
`ifdef VCD
initial begin
  $dumpfile("waves.vcd");
  $dumpvars;
end
`endif

endmodule // pmem_tb
