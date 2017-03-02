module spu (
    input               clk,
    input               reset,
    input [11:0]        total_qvbits0_i,
    input [11:0]        total_qvbits1_i,
    input [11:0]        total_qvbits2_i,
    input [11:0]        total_qvbits3_i,
    input [9:0]         rf_vbits_adj_i,
    input               rf_vbits_adj_mode_i,
    input [11:0]        total_sq_vbits_i,
    input [31:0]        rf_live_vector_i,
    input [31:0]        rf_kill_vector_i,
    output reg [19:0]   total_svf_o
);

reg [31:0] reg_live_vector;
reg [12:0] reg_vbits;

always @(posedge clk) begin
    if (reset) begin
        total_svf_o <= 0;
    end
    else begin
        total_svf_o <= total_svf_o + total_qvbits0_i + total_qvbits1_i +
                       total_qvbits2_i + total_qvbits3_i + reg_vbits +
                       (rf_vbits_adj_mode_i ? rf_vbits_adj_i : (-rf_vbits_adj_i)) +
                       total_sq_vbits_i;
    end
end

always @(posedge clk) begin
    if (reset) begin
        reg_live_vector <= 31'h00000000;
    end
    else begin
        reg_live_vector <= (reg_live_vector & (~rf_kill_vector_i)) | rf_live_vector_i;
    end
end

always @(*) begin: REG_VBITS
    integer n;
    reg_vbits = 0;
    for (n=0; n<32; n=n+1) begin
        reg_vbits = reg_vbits + reg_live_vector[n];
    end
    reg_vbits = reg_vbits << 6;
end

endmodule
