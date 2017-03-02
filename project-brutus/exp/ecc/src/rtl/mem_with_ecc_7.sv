module ecc_checker_7(data_in, checker_in, parity_in);

input      [ 56:0] data_in;
output reg [  5:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[56] ^ data_in[55] ^ data_in[53] ^ data_in[52] ^ data_in[50] ^ data_in[48] ^ data_in[46] ^ data_in[45] ^ data_in[43] ^ data_in[41] ^ data_in[39] ^ data_in[37] ^ data_in[35] ^ data_in[33] ^ data_in[31] ^ data_in[30] ^ data_in[28] ^ data_in[26] ^ data_in[24] ^ data_in[22] ^ data_in[20] ^ data_in[18] ^ data_in[16] ^ data_in[14] ^ data_in[12] ^ data_in[10] ^ data_in[8] ^ data_in[6] ^ data_in[4] ^ data_in[2] ^ data_in[0] ;
    checker_in[1] = data_in[56] ^ data_in[54] ^ data_in[53] ^ data_in[51] ^ data_in[50] ^ data_in[47] ^ data_in[46] ^ data_in[44] ^ data_in[43] ^ data_in[40] ^ data_in[39] ^ data_in[36] ^ data_in[35] ^ data_in[32] ^ data_in[31] ^ data_in[29] ^ data_in[28] ^ data_in[25] ^ data_in[24] ^ data_in[21] ^ data_in[20] ^ data_in[17] ^ data_in[16] ^ data_in[13] ^ data_in[12] ^ data_in[9] ^ data_in[8] ^ data_in[5] ^ data_in[4] ^ data_in[1] ^ data_in[0] ;
    checker_in[2] = data_in[55] ^ data_in[54] ^ data_in[53] ^ data_in[49] ^ data_in[48] ^ data_in[47] ^ data_in[46] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[31] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[3] = data_in[52] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[47] ^ data_in[46] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[31] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[4] = data_in[45] ^ data_in[44] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[31] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[5] = data_in[30] ^ data_in[29] ^ data_in[28] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    parity_in = data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[10] ^ data_in[11] ^ data_in[12] ^ data_in[13] ^ data_in[14] ^ data_in[15] ^ data_in[16] ^ data_in[17] ^ data_in[18] ^ data_in[19] ^ data_in[20] ^ data_in[21] ^ data_in[22] ^ data_in[23] ^ data_in[24] ^ data_in[25] ^ data_in[26] ^ data_in[27] ^ data_in[28] ^ data_in[29] ^ data_in[30] ^ data_in[31] ^ data_in[32] ^ data_in[33] ^ data_in[34] ^ data_in[35] ^ data_in[36] ^ data_in[37] ^ data_in[38] ^ data_in[39] ^ data_in[40] ^ data_in[41] ^ data_in[42] ^ data_in[43] ^ data_in[44] ^ data_in[45] ^ data_in[46] ^ data_in[47] ^ data_in[48] ^ data_in[49] ^ data_in[50] ^ data_in[51] ^ data_in[52] ^ data_in[53] ^ data_in[54] ^ data_in[55] ^ data_in[56] ^ checker_in[0] ^ checker_in[1] ^ checker_in[2] ^ checker_in[3] ^ checker_in[4] ^ checker_in[5] ;

end // always(*)
endmodule


module data_mem_7(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [ 56:0] data_in;
output reg [ 56:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_7(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  5:0] checker_in;
input              parity_in;
output reg [  5:0] checker_out;
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


module ecc_corrector_7(data_out, checker_out, parity_out, correct_data_out, error);

input      [ 56:0] data_out;
input      [  5:0] checker_out;
input              parity_out;
output reg [ 56:0] correct_data_out;
output reg         error;

reg        [  5:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[56] ^ data_out[55] ^ data_out[53] ^ data_out[52] ^ data_out[50] ^ data_out[48] ^ data_out[46] ^ data_out[45] ^ data_out[43] ^ data_out[41] ^ data_out[39] ^ data_out[37] ^ data_out[35] ^ data_out[33] ^ data_out[31] ^ data_out[30] ^ data_out[28] ^ data_out[26] ^ data_out[24] ^ data_out[22] ^ data_out[20] ^ data_out[18] ^ data_out[16] ^ data_out[14] ^ data_out[12] ^ data_out[10] ^ data_out[8] ^ data_out[6] ^ data_out[4] ^ data_out[2] ^ data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[56] ^ data_out[54] ^ data_out[53] ^ data_out[51] ^ data_out[50] ^ data_out[47] ^ data_out[46] ^ data_out[44] ^ data_out[43] ^ data_out[40] ^ data_out[39] ^ data_out[36] ^ data_out[35] ^ data_out[32] ^ data_out[31] ^ data_out[29] ^ data_out[28] ^ data_out[25] ^ data_out[24] ^ data_out[21] ^ data_out[20] ^ data_out[17] ^ data_out[16] ^ data_out[13] ^ data_out[12] ^ data_out[9] ^ data_out[8] ^ data_out[5] ^ data_out[4] ^ data_out[1] ^ data_out[0] ^ checker_out[1] ;
    syndrome[2] = data_out[55] ^ data_out[54] ^ data_out[53] ^ data_out[49] ^ data_out[48] ^ data_out[47] ^ data_out[46] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[31] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[2] ;
    syndrome[3] = data_out[52] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[47] ^ data_out[46] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[31] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[3] ;
    syndrome[4] = data_out[45] ^ data_out[44] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[31] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[4] ;
    syndrome[5] = data_out[30] ^ data_out[29] ^ data_out[28] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[5] ;

    parity = data_out[0] ^ data_out[1] ^ data_out[2] ^ data_out[3] ^ data_out[4] ^ data_out[5] ^ data_out[6] ^ data_out[7] ^ data_out[8] ^ data_out[9] ^ data_out[10] ^ data_out[11] ^ data_out[12] ^ data_out[13] ^ data_out[14] ^ data_out[15] ^ data_out[16] ^ data_out[17] ^ data_out[18] ^ data_out[19] ^ data_out[20] ^ data_out[21] ^ data_out[22] ^ data_out[23] ^ data_out[24] ^ data_out[25] ^ data_out[26] ^ data_out[27] ^ data_out[28] ^ data_out[29] ^ data_out[30] ^ data_out[31] ^ data_out[32] ^ data_out[33] ^ data_out[34] ^ data_out[35] ^ data_out[36] ^ data_out[37] ^ data_out[38] ^ data_out[39] ^ data_out[40] ^ data_out[41] ^ data_out[42] ^ data_out[43] ^ data_out[44] ^ data_out[45] ^ data_out[46] ^ data_out[47] ^ data_out[48] ^ data_out[49] ^ data_out[50] ^ data_out[51] ^ data_out[52] ^ data_out[53] ^ data_out[54] ^ data_out[55] ^ data_out[56] ^ checker_out[0] ^ checker_out[1] ^ checker_out[2] ^ checker_out[3] ^ checker_out[4] ^ checker_out[5] ^ parity_out ;

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
            3: correct_data_out[56] = ~data_out[56];
            5: correct_data_out[55] = ~data_out[55];
            6: correct_data_out[54] = ~data_out[54];
            7: correct_data_out[53] = ~data_out[53];
            9: correct_data_out[52] = ~data_out[52];
            10: correct_data_out[51] = ~data_out[51];
            11: correct_data_out[50] = ~data_out[50];
            12: correct_data_out[49] = ~data_out[49];
            13: correct_data_out[48] = ~data_out[48];
            14: correct_data_out[47] = ~data_out[47];
            15: correct_data_out[46] = ~data_out[46];
            17: correct_data_out[45] = ~data_out[45];
            18: correct_data_out[44] = ~data_out[44];
            19: correct_data_out[43] = ~data_out[43];
            20: correct_data_out[42] = ~data_out[42];
            21: correct_data_out[41] = ~data_out[41];
            22: correct_data_out[40] = ~data_out[40];
            23: correct_data_out[39] = ~data_out[39];
            24: correct_data_out[38] = ~data_out[38];
            25: correct_data_out[37] = ~data_out[37];
            26: correct_data_out[36] = ~data_out[36];
            27: correct_data_out[35] = ~data_out[35];
            28: correct_data_out[34] = ~data_out[34];
            29: correct_data_out[33] = ~data_out[33];
            30: correct_data_out[32] = ~data_out[32];
            31: correct_data_out[31] = ~data_out[31];
            33: correct_data_out[30] = ~data_out[30];
            34: correct_data_out[29] = ~data_out[29];
            35: correct_data_out[28] = ~data_out[28];
            36: correct_data_out[27] = ~data_out[27];
            37: correct_data_out[26] = ~data_out[26];
            38: correct_data_out[25] = ~data_out[25];
            39: correct_data_out[24] = ~data_out[24];
            40: correct_data_out[23] = ~data_out[23];
            41: correct_data_out[22] = ~data_out[22];
            42: correct_data_out[21] = ~data_out[21];
            43: correct_data_out[20] = ~data_out[20];
            44: correct_data_out[19] = ~data_out[19];
            45: correct_data_out[18] = ~data_out[18];
            46: correct_data_out[17] = ~data_out[17];
            47: correct_data_out[16] = ~data_out[16];
            48: correct_data_out[15] = ~data_out[15];
            49: correct_data_out[14] = ~data_out[14];
            50: correct_data_out[13] = ~data_out[13];
            51: correct_data_out[12] = ~data_out[12];
            52: correct_data_out[11] = ~data_out[11];
            53: correct_data_out[10] = ~data_out[10];
            54: correct_data_out[9] = ~data_out[9];
            55: correct_data_out[8] = ~data_out[8];
            56: correct_data_out[7] = ~data_out[7];
            57: correct_data_out[6] = ~data_out[6];
            58: correct_data_out[5] = ~data_out[5];
            59: correct_data_out[4] = ~data_out[4];
            60: correct_data_out[3] = ~data_out[3];
            61: correct_data_out[2] = ~data_out[2];
            62: correct_data_out[1] = ~data_out[1];
            63: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_7(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [ 56:0] data_in;
output     [ 56:0] correct_data_out;
output             error;

wire       [ 56:0] data_out;
wire       [  5:0] checker_in;
wire       [  5:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_7 u_checker(data_in, checker_in, parity_in);
data_mem_7 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_7 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_7 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


