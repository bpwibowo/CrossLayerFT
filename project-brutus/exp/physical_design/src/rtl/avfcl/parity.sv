module parity #(
	/* Parameters */
	parameter WIDTH = 8
	)
	(data_in, parity_out);
input      [WIDTH-1:0] data_in;
output reg             parity_out;

always @(*) begin
	int i;
	parity_out = 0;

	for (i=0; i<WIDTH; i=i+1) begin
		parity_out = parity_out ^ data_in[i];
	end
end // always(*)
endmodule

module parity_mem (clk, reset, data_in, data_out);
input      clk;
input      reset;
input      data_in;
output reg data_out;

always_ff @(posedge clk) begin : proc_data_out
	if(reset) begin
		data_out <= 0;
	end else begin
		data_out <= data_in;
	end
end
endmodule
