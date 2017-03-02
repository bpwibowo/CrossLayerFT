`define DURATION_WIDTH (14-4) // drop 4 LSBs

module lq_dur (
    input             clk,
    input             reset,
    input [7:0]       counter_in,
    input [`DURATION_WIDTH-1:0]      timestamp,
    input [5:0]       lq_idx_in,
    input [5:0]       lq_idx_exec,
    input [5:0]       lq_idx_out,
    output [`DURATION_WIDTH-1:0]     lq_duration_dp,
    output [`DURATION_WIDTH-1:0]     lq_duration_exec
);

logic [`DURATION_WIDTH-1:0] start_time;
logic [`DURATION_WIDTH-1:0] dur_dp;
logic [`DURATION_WIDTH-1:0] dur_end;
logic [5:0] mon_lq_idx;
logic       mon_active;

always_ff @(posedge clk) begin : proc_lq_dur
	if(reset) begin
		mon_active <= 0;
	end else begin
		if (counter_in == 0) begin
			start_time <= timestamp;
			mon_lq_idx <= lq_idx_in;
			mon_active <= 1;
		end
		if (mon_active && lq_idx_exec == mon_lq_idx) begin
			dur_dp <= timestamp - start_time;
		end
		if (mon_active && lq_idx_out == mon_lq_idx) begin
			dur_end <= timestamp - start_time;
			mon_active <= 0;
		end
	end
end

assign lq_duration_dp = dur_dp;
assign lq_duration_exec = dur_end;

endmodule // lq_dur
