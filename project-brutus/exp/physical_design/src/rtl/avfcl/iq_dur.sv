`define DURATION_WIDTH (14-4) // drop 4 LSBs

module iq_dur (
    input             clk,
    input             reset,
    input [7:0]       counter_in,
    input [5:0]       iq_idx,
    input [7:0]       al_idx,
    input [`DURATION_WIDTH-1:0]      timestamp,
    input [7:0]       al_tail,
    output reg [`DURATION_WIDTH-1:0] iq_duration_fast,
    output reg [`DURATION_WIDTH-1:0] iq_duration_slow
);

logic [`DURATION_WIDTH-1:0] start_time;
logic [5:0]  mon_iq_idx;
logic [7:0]  mon_al_idx;
logic        mon_active;

always_ff @(posedge clk) begin : proc_iq_dur
	if(reset) begin
		mon_active <= 0;
	end else begin
		if (counter_in == 0) begin
			start_time <= timestamp;
			mon_iq_idx <= iq_idx;
			mon_al_idx <= al_idx;
			mon_active <= 1;
		end
		if (mon_active && iq_idx == mon_iq_idx) begin
			if (al_tail - al_idx < 128) begin
				iq_duration_fast <= timestamp - start_time;
			end
			else begin
				iq_duration_slow <= timestamp - start_time;
			end

			mon_active <= 0;
		end
	end
end

endmodule // iq_dur
