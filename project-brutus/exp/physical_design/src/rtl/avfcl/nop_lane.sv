module nop_lane (
    input               clk,
    input               reset,
    input               stall_fetch1,
    input               stall_fetch2,
    input [31:0]        first_inst,

    output              first_inst_nop,
    output reg [9:0]   fetch2decode_metadata,
    output reg          fetch2decode_valid
);

logic [13:0] fetch1fetch2_metadata;
logic        fetch1fetch2_valid;

assign first_inst_nop = (first_inst == 32'b0);

always_ff @(posedge clk) begin : proc_fetch1fetch2
    if(reset) begin
        fetch1fetch2_valid <= 0;
        fetch1fetch2_metadata <= 0;
    end else begin
        if (first_inst_nop) begin
            fetch1fetch2_valid <= 1;
            fetch1fetch2_metadata <= {first_inst[25:16],first_inst[10:8]};
        end
        else if (stall_fetch1) begin
            // keep the values
        end
        else begin
            fetch1fetch2_valid <= 0;
        end
    end
end

always_ff @(posedge clk) begin : proc_fetch2decode
    if(reset) begin
        fetch2decode_valid <= 0;
        fetch2decode_metadata <= 0;
    end
    else if (stall_fetch2) begin
        // keep the values
    end
    else if (fetch2decode_valid) begin
        fetch2decode_valid <= 1;
        fetch2decode_metadata <= fetch1fetch2_metadata[9:0];
    end
    else begin
        fetch2decode_valid <= 0;
    end
end

endmodule
