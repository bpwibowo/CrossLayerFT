module ecc_checker_6(data_in, checker_in, parity_in);

input      [ 25:0] data_in;
output reg [  4:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[25] ^ data_in[24] ^ data_in[22] ^ data_in[21] ^ data_in[19] ^ data_in[17] ^ data_in[15] ^ data_in[14] ^ data_in[12] ^ data_in[10] ^ data_in[8] ^ data_in[6] ^ data_in[4] ^ data_in[2] ^ data_in[0] ;
    checker_in[1] = data_in[25] ^ data_in[23] ^ data_in[22] ^ data_in[20] ^ data_in[19] ^ data_in[16] ^ data_in[15] ^ data_in[13] ^ data_in[12] ^ data_in[9] ^ data_in[8] ^ data_in[5] ^ data_in[4] ^ data_in[1] ^ data_in[0] ;
    checker_in[2] = data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[3] = data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[4] = data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    parity_in = data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[10] ^ data_in[11] ^ data_in[12] ^ data_in[13] ^ data_in[14] ^ data_in[15] ^ data_in[16] ^ data_in[17] ^ data_in[18] ^ data_in[19] ^ data_in[20] ^ data_in[21] ^ data_in[22] ^ data_in[23] ^ data_in[24] ^ data_in[25] ^ checker_in[0] ^ checker_in[1] ^ checker_in[2] ^ checker_in[3] ^ checker_in[4] ;

end // always(*)
endmodule


module data_mem_6(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [ 25:0] data_in;
output reg [ 25:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_6(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  4:0] checker_in;
input              parity_in;
output reg [  4:0] checker_out;
output reg         parity_out;

always @(posedge clock) begin
    if (reset) begin
        checker_out <= 0;
        parity_out <= 0;
    end
    else if (we) begin
        checker_out <= checker_in;
        parity_out <= parity_in;
    end
end
endmodule


module ecc_corrector_6(data_out, checker_out, parity_out, correct_data_out, error);

input      [ 25:0] data_out;
input      [  4:0] checker_out;
input              parity_out;
output reg [ 25:0] correct_data_out;
output reg         error;

reg        [  4:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[25] ^ data_out[24] ^ data_out[22] ^ data_out[21] ^ data_out[19] ^ data_out[17] ^ data_out[15] ^ data_out[14] ^ data_out[12] ^ data_out[10] ^ data_out[8] ^ data_out[6] ^ data_out[4] ^ data_out[2] ^ data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[25] ^ data_out[23] ^ data_out[22] ^ data_out[20] ^ data_out[19] ^ data_out[16] ^ data_out[15] ^ data_out[13] ^ data_out[12] ^ data_out[9] ^ data_out[8] ^ data_out[5] ^ data_out[4] ^ data_out[1] ^ data_out[0] ^ checker_out[1] ;
    syndrome[2] = data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[2] ;
    syndrome[3] = data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[3] ;
    syndrome[4] = data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[4] ;

    parity = data_out[0] ^ data_out[1] ^ data_out[2] ^ data_out[3] ^ data_out[4] ^ data_out[5] ^ data_out[6] ^ data_out[7] ^ data_out[8] ^ data_out[9] ^ data_out[10] ^ data_out[11] ^ data_out[12] ^ data_out[13] ^ data_out[14] ^ data_out[15] ^ data_out[16] ^ data_out[17] ^ data_out[18] ^ data_out[19] ^ data_out[20] ^ data_out[21] ^ data_out[22] ^ data_out[23] ^ data_out[24] ^ data_out[25] ^ checker_out[0] ^ checker_out[1] ^ checker_out[2] ^ checker_out[3] ^ checker_out[4] ^ parity_out ;

    error = 0;
    if (syndrome == 0 && parity == 0) begin
        error = 0;
    end
    if (syndrome != 0 && parity == 0) begin
        error = 1;
    end
    if (syndrome == 0 && parity != 0) begin
        error = 0;
    end
    if (syndrome != 0 && parity != 0) begin
        error = 0;
    end

    correct_data_out = data_out;
    if (error == 0 && syndrome != 0) begin
        case(syndrome)
            3: correct_data_out[25] = ~data_out[25];
            5: correct_data_out[24] = ~data_out[24];
            6: correct_data_out[23] = ~data_out[23];
            7: correct_data_out[22] = ~data_out[22];
            9: correct_data_out[21] = ~data_out[21];
            10: correct_data_out[20] = ~data_out[20];
            11: correct_data_out[19] = ~data_out[19];
            12: correct_data_out[18] = ~data_out[18];
            13: correct_data_out[17] = ~data_out[17];
            14: correct_data_out[16] = ~data_out[16];
            15: correct_data_out[15] = ~data_out[15];
            17: correct_data_out[14] = ~data_out[14];
            18: correct_data_out[13] = ~data_out[13];
            19: correct_data_out[12] = ~data_out[12];
            20: correct_data_out[11] = ~data_out[11];
            21: correct_data_out[10] = ~data_out[10];
            22: correct_data_out[9] = ~data_out[9];
            23: correct_data_out[8] = ~data_out[8];
            24: correct_data_out[7] = ~data_out[7];
            25: correct_data_out[6] = ~data_out[6];
            26: correct_data_out[5] = ~data_out[5];
            27: correct_data_out[4] = ~data_out[4];
            28: correct_data_out[3] = ~data_out[3];
            29: correct_data_out[2] = ~data_out[2];
            30: correct_data_out[1] = ~data_out[1];
            31: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_6(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [ 25:0] data_in;
output     [ 25:0] correct_data_out;
output             error;

wire       [ 25:0] data_out;
wire       [  4:0] checker_in;
wire       [  4:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_6 u_checker(data_in, checker_in, parity_in);
data_mem_6 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_6 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_6 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


