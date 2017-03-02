`define DURATION_WIDTH (14-4) // drop 4 LSBs

module ib_dur (
    input             clk,
    input             reset,
    input [7:0]       counter_in,
    input [7:0]       counter_out,
    input             pop,
    input [`DURATION_WIDTH-1:0]      timestamp,
    output [`DURATION_WIDTH-1:0]     ib_duration
);

logic [`DURATION_WIDTH-1:0] start_time;
logic [1:0][`DURATION_WIDTH-1:0] durs;
logic [0:0] head;
logic [0:0] tail;

always_ff @(posedge clk) begin : proc_ib_dur
	if(reset) begin
		head <= 0;
		tail <= 0;
	end else begin
		if (counter_in == 0) begin
			start_time <= timestamp;
		end
		if (counter_out == 0) begin
			durs[tail] <= timestamp - start_time;
			tail <= tail + 1;
		end
		if (pop) begin
			head <= head + 1;
		end
	end
end

assign ib_duration = durs[head];

endmodule // ib_dur
