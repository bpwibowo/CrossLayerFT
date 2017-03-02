module counter10 (
    input clk,
    input reset,
    output reg [9:0] counter
    );

always @(posedge clk) begin
    if (reset)
        counter <= 0;
    else
        counter <= counter + 1;
end

endmodule // counter10
