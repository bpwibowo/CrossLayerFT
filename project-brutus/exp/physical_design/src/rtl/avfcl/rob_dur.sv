`define DURATION_WIDTH (14-4) // drop 4 LSBs

module rob_dur (
    input             clk,
    input             reset,
    input [7:0]       counter_in,
    input [7:0]       counter_wb,
    input [7:0]       al_idx_in,
    input [7:0]       al_idx_wb,
    input [7:0]       al_idx_out,
    input             fast_or_slow_in,
    input [`DURATION_WIDTH-1:0]      timestamp,
    output reg [`DURATION_WIDTH-1:0] rob_duration_dp,
    output reg [`DURATION_WIDTH-1:0] rob_duration_fast,
    output reg [`DURATION_WIDTH-1:0] rob_duration_slow
);

logic [`DURATION_WIDTH-1:0] start_time_dp;
logic [`DURATION_WIDTH-1:0] start_time_wb;
logic [5:0]  mon_rob_dp_idx;
logic [7:0]  mon_rob_wb_idx;
logic        mon_dp_active;
logic        mon_wb_active;
logic        fast_or_slow;

always_ff @(posedge clk) begin : proc_rob_dur
	if(reset) begin
		mon_dp_active <= 0;
		mon_wb_active <= 0;
	end else begin
		if (counter_in == 0) begin
			start_time_dp <= timestamp;
			mon_rob_dp_idx <= al_idx_in;
			mon_dp_active <= 1;
		end
		if (counter_wb == 0) begin
			start_time_wb <= timestamp;
			mon_rob_wb_idx <= al_idx_wb;
			fast_or_slow <= fast_or_slow_in;
			mon_wb_active <= 1;
		end
		if (mon_dp_active && al_idx_out == mon_rob_dp_idx) begin
			rob_duration_dp <= timestamp - start_time_dp;

			mon_dp_active <= 0;
		end
		if (mon_wb_active && al_idx_out == mon_rob_wb_idx) begin
			if (fast_or_slow) begin
				rob_duration_fast <= timestamp - start_time_wb;
			end
			else begin
				rob_duration_slow <= timestamp - start_time_wb;
			end

			mon_wb_active <= 0;
		end
	end
end

endmodule // rob_dur
