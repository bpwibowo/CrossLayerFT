module counter256 (
    input             clk,
    input             reset,
    input [2:0]       increment,
    input [7:0]       reset_value,
    output reg [7:0]  count
);

always_ff @(posedge clk) begin : proc_count
	if(reset) begin
		count <= reset_value;
	end else begin
		count <= count + increment;
	end
end

endmodule // counter256

module counter64 (
    input             clk,
    input             reset,
    input [2:0]       increment,
    input [5:0]       reset_value,
    output reg [5:0]  count
);

always_ff @(posedge clk) begin : proc_count
	if(reset) begin
		count <= reset_value;
	end else begin
		count <= count + increment;
	end
end

endmodule // counter64


module counter65k (
    input             clk,
    input             reset,
    output reg [15:0] count
);

always_ff @(posedge clk) begin : proc_count
	if(reset) begin
		count <= 0;
	end else begin
		count <= count + 1;
	end
end

endmodule // counter65k
