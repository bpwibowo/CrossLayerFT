/**********************************************************
 *
 * A module of 8x10-bits adder and two shifter
 *
 * First author: Bagus Wibowo
 * created: 11/18/2014
 **********************************************************/

module addshift8x10 (
    input                                clk,
    input                                reset,
    input [11:0]                         in12_i,
    input  [9:0]                         in10_i,
    input  [3:0]                         shift1_i,
    input                                shift1_v_i,
    input  [3:0]                         shift2_i,
    input                                shift2_v_i,
    output reg [17:0]                    mul_o
    );

always @(posedge clk) begin
    if (reset)
        mul_o <= 0;
    else
        mul_o <= (shift1_v_i ? (in10_i << shift1_i) : 0) +
                 (shift2_v_i ? (in10_i << shift2_i) : 0) + in12_i;
end

endmodule

