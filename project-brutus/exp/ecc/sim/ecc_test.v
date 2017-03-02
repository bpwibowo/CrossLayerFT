// Uncomment the following to dump vcd file
`define VCD

module ecc_test();

reg          clock, reset;
reg  [501:0] data_in;
reg          we;
wire [0:0] data_out_3;
wire         error_3;
wire [3:0] data_out_4;
wire         error_4;
wire [10:0] data_out_5;
wire         error_5;
wire [25:0] data_out_6;
wire         error_6;
wire [56:0] data_out_7;
wire         error_7;
wire [119:0] data_out_8;
wire         error_8;
wire [246:0] data_out_9;
wire         error_9;
wire [501:0] data_out_10;
wire         error_10;
wire         parity_190_out;
wire         parity_197_out;
wire         parity_76_out;
wire         parity_64_out;
wire         parity_82_out;

mem_with_ecc_3 ecc_3(clock, reset, we, data_in[0:0], data_out_3, error_3);
mem_with_ecc_4 ecc_4(clock, reset, we, data_in[3:0], data_out_4, error_4);
mem_with_ecc_5 ecc_5(clock, reset, we, data_in[10:0], data_out_5, error_5);
mem_with_ecc_6 ecc_6(clock, reset, we, data_in[25:0], data_out_6, error_6);
mem_with_ecc_7 ecc_7(clock, reset, we, data_in[56:0], data_out_7, error_7);
mem_with_ecc_8 ecc_8(clock, reset, we, data_in[119:0], data_out_8, error_8);
mem_with_ecc_9 ecc_9(clock, reset, we, data_in[246:0], data_out_9, error_9);
mem_with_ecc_10 ecc_10(clock, reset, we, data_in[501:0], data_out_10, error_10);

parity_gen_190 p190 (data_in[190-1:0], parity_190_out);
parity_gen_197 p197 (data_in[197-1:0], parity_197_out);
parity_gen_76 p76 (data_in[76-1:0], parity_76_out);
parity_gen_64 p64 (data_in[64-1:0], parity_64_out);
parity_gen_82 p82 (data_in[82-1:0], parity_82_out);

always begin
    #5 clock = ~clock;
end

`define ITERATION 10000
integer i;
integer active_cnt;

initial begin
    clock = 1;
    reset = 1;
    active_cnt = 0;
    data_in = 0;
    we = 0;

    #100
	$display("start time = %t", $time);
    reset = 0;

    for (i=0; i<`ITERATION; i=i+1) begin
        we = $random;
        if (we) begin
            active_cnt = active_cnt+1;
            data_in[31:0] = $random;
            data_in[63:32] = $random;
            data_in[95:64] = $random;
            data_in[127:96] = $random;
            data_in[159:128] = $random;
            data_in[191:160] = $random;
            data_in[223:192] = $random;
            data_in[255:224] = $random;
            data_in[287:256] = $random;
            data_in[319:288] = $random;
            data_in[351:320] = $random;
            data_in[383:352] = $random;
            data_in[415:384] = $random;
            data_in[447:416] = $random;
            data_in[479:448] = $random;
            data_in[501:480] = $random;
        end

        #10;
    end

    $display("total_iteration = \%d", i);
    $display("active_cnt = \%d", active_cnt);
    $display("sim_time = %t", $time);

    $finish;

end

// Use `define VCD to allow dumping all value changes to waves.vcd
`ifdef VCD
initial begin
  $dumpfile("waves.vcd");
  $dumpvars;
end
`endif


endmodule
