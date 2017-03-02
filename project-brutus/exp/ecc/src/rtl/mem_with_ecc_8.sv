module ecc_checker_8(data_in, checker_in, parity_in);

input      [ 119:0] data_in;
output reg [  6:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[119] ^ data_in[118] ^ data_in[116] ^ data_in[115] ^ data_in[113] ^ data_in[111] ^ data_in[109] ^ data_in[108] ^ data_in[106] ^ data_in[104] ^ data_in[102] ^ data_in[100] ^ data_in[98] ^ data_in[96] ^ data_in[94] ^ data_in[93] ^ data_in[91] ^ data_in[89] ^ data_in[87] ^ data_in[85] ^ data_in[83] ^ data_in[81] ^ data_in[79] ^ data_in[77] ^ data_in[75] ^ data_in[73] ^ data_in[71] ^ data_in[69] ^ data_in[67] ^ data_in[65] ^ data_in[63] ^ data_in[62] ^ data_in[60] ^ data_in[58] ^ data_in[56] ^ data_in[54] ^ data_in[52] ^ data_in[50] ^ data_in[48] ^ data_in[46] ^ data_in[44] ^ data_in[42] ^ data_in[40] ^ data_in[38] ^ data_in[36] ^ data_in[34] ^ data_in[32] ^ data_in[30] ^ data_in[28] ^ data_in[26] ^ data_in[24] ^ data_in[22] ^ data_in[20] ^ data_in[18] ^ data_in[16] ^ data_in[14] ^ data_in[12] ^ data_in[10] ^ data_in[8] ^ data_in[6] ^ data_in[4] ^ data_in[2] ^ data_in[0] ;
    checker_in[1] = data_in[119] ^ data_in[117] ^ data_in[116] ^ data_in[114] ^ data_in[113] ^ data_in[110] ^ data_in[109] ^ data_in[107] ^ data_in[106] ^ data_in[103] ^ data_in[102] ^ data_in[99] ^ data_in[98] ^ data_in[95] ^ data_in[94] ^ data_in[92] ^ data_in[91] ^ data_in[88] ^ data_in[87] ^ data_in[84] ^ data_in[83] ^ data_in[80] ^ data_in[79] ^ data_in[76] ^ data_in[75] ^ data_in[72] ^ data_in[71] ^ data_in[68] ^ data_in[67] ^ data_in[64] ^ data_in[63] ^ data_in[61] ^ data_in[60] ^ data_in[57] ^ data_in[56] ^ data_in[53] ^ data_in[52] ^ data_in[49] ^ data_in[48] ^ data_in[45] ^ data_in[44] ^ data_in[41] ^ data_in[40] ^ data_in[37] ^ data_in[36] ^ data_in[33] ^ data_in[32] ^ data_in[29] ^ data_in[28] ^ data_in[25] ^ data_in[24] ^ data_in[21] ^ data_in[20] ^ data_in[17] ^ data_in[16] ^ data_in[13] ^ data_in[12] ^ data_in[9] ^ data_in[8] ^ data_in[5] ^ data_in[4] ^ data_in[1] ^ data_in[0] ;
    checker_in[2] = data_in[118] ^ data_in[117] ^ data_in[116] ^ data_in[112] ^ data_in[111] ^ data_in[110] ^ data_in[109] ^ data_in[105] ^ data_in[104] ^ data_in[103] ^ data_in[102] ^ data_in[97] ^ data_in[96] ^ data_in[95] ^ data_in[94] ^ data_in[90] ^ data_in[89] ^ data_in[88] ^ data_in[87] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[79] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[71] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[63] ^ data_in[59] ^ data_in[58] ^ data_in[57] ^ data_in[56] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[3] = data_in[115] ^ data_in[114] ^ data_in[113] ^ data_in[112] ^ data_in[111] ^ data_in[110] ^ data_in[109] ^ data_in[101] ^ data_in[100] ^ data_in[99] ^ data_in[98] ^ data_in[97] ^ data_in[96] ^ data_in[95] ^ data_in[94] ^ data_in[86] ^ data_in[85] ^ data_in[84] ^ data_in[83] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[79] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[63] ^ data_in[55] ^ data_in[54] ^ data_in[53] ^ data_in[52] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[4] = data_in[108] ^ data_in[107] ^ data_in[106] ^ data_in[105] ^ data_in[104] ^ data_in[103] ^ data_in[102] ^ data_in[101] ^ data_in[100] ^ data_in[99] ^ data_in[98] ^ data_in[97] ^ data_in[96] ^ data_in[95] ^ data_in[94] ^ data_in[78] ^ data_in[77] ^ data_in[76] ^ data_in[75] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[71] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[63] ^ data_in[47] ^ data_in[46] ^ data_in[45] ^ data_in[44] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[5] = data_in[93] ^ data_in[92] ^ data_in[91] ^ data_in[90] ^ data_in[89] ^ data_in[88] ^ data_in[87] ^ data_in[86] ^ data_in[85] ^ data_in[84] ^ data_in[83] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[79] ^ data_in[78] ^ data_in[77] ^ data_in[76] ^ data_in[75] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[71] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[63] ^ data_in[31] ^ data_in[30] ^ data_in[29] ^ data_in[28] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[6] = data_in[62] ^ data_in[61] ^ data_in[60] ^ data_in[59] ^ data_in[58] ^ data_in[57] ^ data_in[56] ^ data_in[55] ^ data_in[54] ^ data_in[53] ^ data_in[52] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[47] ^ data_in[46] ^ data_in[45] ^ data_in[44] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[31] ^ data_in[30] ^ data_in[29] ^ data_in[28] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    parity_in = data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[10] ^ data_in[11] ^ data_in[12] ^ data_in[13] ^ data_in[14] ^ data_in[15] ^ data_in[16] ^ data_in[17] ^ data_in[18] ^ data_in[19] ^ data_in[20] ^ data_in[21] ^ data_in[22] ^ data_in[23] ^ data_in[24] ^ data_in[25] ^ data_in[26] ^ data_in[27] ^ data_in[28] ^ data_in[29] ^ data_in[30] ^ data_in[31] ^ data_in[32] ^ data_in[33] ^ data_in[34] ^ data_in[35] ^ data_in[36] ^ data_in[37] ^ data_in[38] ^ data_in[39] ^ data_in[40] ^ data_in[41] ^ data_in[42] ^ data_in[43] ^ data_in[44] ^ data_in[45] ^ data_in[46] ^ data_in[47] ^ data_in[48] ^ data_in[49] ^ data_in[50] ^ data_in[51] ^ data_in[52] ^ data_in[53] ^ data_in[54] ^ data_in[55] ^ data_in[56] ^ data_in[57] ^ data_in[58] ^ data_in[59] ^ data_in[60] ^ data_in[61] ^ data_in[62] ^ data_in[63] ^ data_in[64] ^ data_in[65] ^ data_in[66] ^ data_in[67] ^ data_in[68] ^ data_in[69] ^ data_in[70] ^ data_in[71] ^ data_in[72] ^ data_in[73] ^ data_in[74] ^ data_in[75] ^ data_in[76] ^ data_in[77] ^ data_in[78] ^ data_in[79] ^ data_in[80] ^ data_in[81] ^ data_in[82] ^ data_in[83] ^ data_in[84] ^ data_in[85] ^ data_in[86] ^ data_in[87] ^ data_in[88] ^ data_in[89] ^ data_in[90] ^ data_in[91] ^ data_in[92] ^ data_in[93] ^ data_in[94] ^ data_in[95] ^ data_in[96] ^ data_in[97] ^ data_in[98] ^ data_in[99] ^ data_in[100] ^ data_in[101] ^ data_in[102] ^ data_in[103] ^ data_in[104] ^ data_in[105] ^ data_in[106] ^ data_in[107] ^ data_in[108] ^ data_in[109] ^ data_in[110] ^ data_in[111] ^ data_in[112] ^ data_in[113] ^ data_in[114] ^ data_in[115] ^ data_in[116] ^ data_in[117] ^ data_in[118] ^ data_in[119] ^ checker_in[0] ^ checker_in[1] ^ checker_in[2] ^ checker_in[3] ^ checker_in[4] ^ checker_in[5] ^ checker_in[6] ;

end // always(*)
endmodule


module data_mem_8(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [ 119:0] data_in;
output reg [ 119:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_8(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  6:0] checker_in;
input              parity_in;
output reg [  6:0] checker_out;
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


module ecc_corrector_8(data_out, checker_out, parity_out, correct_data_out, error);

input      [ 119:0] data_out;
input      [  6:0] checker_out;
input              parity_out;
output reg [ 119:0] correct_data_out;
output reg         error;

reg        [  6:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[119] ^ data_out[118] ^ data_out[116] ^ data_out[115] ^ data_out[113] ^ data_out[111] ^ data_out[109] ^ data_out[108] ^ data_out[106] ^ data_out[104] ^ data_out[102] ^ data_out[100] ^ data_out[98] ^ data_out[96] ^ data_out[94] ^ data_out[93] ^ data_out[91] ^ data_out[89] ^ data_out[87] ^ data_out[85] ^ data_out[83] ^ data_out[81] ^ data_out[79] ^ data_out[77] ^ data_out[75] ^ data_out[73] ^ data_out[71] ^ data_out[69] ^ data_out[67] ^ data_out[65] ^ data_out[63] ^ data_out[62] ^ data_out[60] ^ data_out[58] ^ data_out[56] ^ data_out[54] ^ data_out[52] ^ data_out[50] ^ data_out[48] ^ data_out[46] ^ data_out[44] ^ data_out[42] ^ data_out[40] ^ data_out[38] ^ data_out[36] ^ data_out[34] ^ data_out[32] ^ data_out[30] ^ data_out[28] ^ data_out[26] ^ data_out[24] ^ data_out[22] ^ data_out[20] ^ data_out[18] ^ data_out[16] ^ data_out[14] ^ data_out[12] ^ data_out[10] ^ data_out[8] ^ data_out[6] ^ data_out[4] ^ data_out[2] ^ data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[119] ^ data_out[117] ^ data_out[116] ^ data_out[114] ^ data_out[113] ^ data_out[110] ^ data_out[109] ^ data_out[107] ^ data_out[106] ^ data_out[103] ^ data_out[102] ^ data_out[99] ^ data_out[98] ^ data_out[95] ^ data_out[94] ^ data_out[92] ^ data_out[91] ^ data_out[88] ^ data_out[87] ^ data_out[84] ^ data_out[83] ^ data_out[80] ^ data_out[79] ^ data_out[76] ^ data_out[75] ^ data_out[72] ^ data_out[71] ^ data_out[68] ^ data_out[67] ^ data_out[64] ^ data_out[63] ^ data_out[61] ^ data_out[60] ^ data_out[57] ^ data_out[56] ^ data_out[53] ^ data_out[52] ^ data_out[49] ^ data_out[48] ^ data_out[45] ^ data_out[44] ^ data_out[41] ^ data_out[40] ^ data_out[37] ^ data_out[36] ^ data_out[33] ^ data_out[32] ^ data_out[29] ^ data_out[28] ^ data_out[25] ^ data_out[24] ^ data_out[21] ^ data_out[20] ^ data_out[17] ^ data_out[16] ^ data_out[13] ^ data_out[12] ^ data_out[9] ^ data_out[8] ^ data_out[5] ^ data_out[4] ^ data_out[1] ^ data_out[0] ^ checker_out[1] ;
    syndrome[2] = data_out[118] ^ data_out[117] ^ data_out[116] ^ data_out[112] ^ data_out[111] ^ data_out[110] ^ data_out[109] ^ data_out[105] ^ data_out[104] ^ data_out[103] ^ data_out[102] ^ data_out[97] ^ data_out[96] ^ data_out[95] ^ data_out[94] ^ data_out[90] ^ data_out[89] ^ data_out[88] ^ data_out[87] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[79] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[71] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[63] ^ data_out[59] ^ data_out[58] ^ data_out[57] ^ data_out[56] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[2] ;
    syndrome[3] = data_out[115] ^ data_out[114] ^ data_out[113] ^ data_out[112] ^ data_out[111] ^ data_out[110] ^ data_out[109] ^ data_out[101] ^ data_out[100] ^ data_out[99] ^ data_out[98] ^ data_out[97] ^ data_out[96] ^ data_out[95] ^ data_out[94] ^ data_out[86] ^ data_out[85] ^ data_out[84] ^ data_out[83] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[79] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[63] ^ data_out[55] ^ data_out[54] ^ data_out[53] ^ data_out[52] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[3] ;
    syndrome[4] = data_out[108] ^ data_out[107] ^ data_out[106] ^ data_out[105] ^ data_out[104] ^ data_out[103] ^ data_out[102] ^ data_out[101] ^ data_out[100] ^ data_out[99] ^ data_out[98] ^ data_out[97] ^ data_out[96] ^ data_out[95] ^ data_out[94] ^ data_out[78] ^ data_out[77] ^ data_out[76] ^ data_out[75] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[71] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[63] ^ data_out[47] ^ data_out[46] ^ data_out[45] ^ data_out[44] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[4] ;
    syndrome[5] = data_out[93] ^ data_out[92] ^ data_out[91] ^ data_out[90] ^ data_out[89] ^ data_out[88] ^ data_out[87] ^ data_out[86] ^ data_out[85] ^ data_out[84] ^ data_out[83] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[79] ^ data_out[78] ^ data_out[77] ^ data_out[76] ^ data_out[75] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[71] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[63] ^ data_out[31] ^ data_out[30] ^ data_out[29] ^ data_out[28] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[5] ;
    syndrome[6] = data_out[62] ^ data_out[61] ^ data_out[60] ^ data_out[59] ^ data_out[58] ^ data_out[57] ^ data_out[56] ^ data_out[55] ^ data_out[54] ^ data_out[53] ^ data_out[52] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[47] ^ data_out[46] ^ data_out[45] ^ data_out[44] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[31] ^ data_out[30] ^ data_out[29] ^ data_out[28] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[6] ;

    parity = data_out[0] ^ data_out[1] ^ data_out[2] ^ data_out[3] ^ data_out[4] ^ data_out[5] ^ data_out[6] ^ data_out[7] ^ data_out[8] ^ data_out[9] ^ data_out[10] ^ data_out[11] ^ data_out[12] ^ data_out[13] ^ data_out[14] ^ data_out[15] ^ data_out[16] ^ data_out[17] ^ data_out[18] ^ data_out[19] ^ data_out[20] ^ data_out[21] ^ data_out[22] ^ data_out[23] ^ data_out[24] ^ data_out[25] ^ data_out[26] ^ data_out[27] ^ data_out[28] ^ data_out[29] ^ data_out[30] ^ data_out[31] ^ data_out[32] ^ data_out[33] ^ data_out[34] ^ data_out[35] ^ data_out[36] ^ data_out[37] ^ data_out[38] ^ data_out[39] ^ data_out[40] ^ data_out[41] ^ data_out[42] ^ data_out[43] ^ data_out[44] ^ data_out[45] ^ data_out[46] ^ data_out[47] ^ data_out[48] ^ data_out[49] ^ data_out[50] ^ data_out[51] ^ data_out[52] ^ data_out[53] ^ data_out[54] ^ data_out[55] ^ data_out[56] ^ data_out[57] ^ data_out[58] ^ data_out[59] ^ data_out[60] ^ data_out[61] ^ data_out[62] ^ data_out[63] ^ data_out[64] ^ data_out[65] ^ data_out[66] ^ data_out[67] ^ data_out[68] ^ data_out[69] ^ data_out[70] ^ data_out[71] ^ data_out[72] ^ data_out[73] ^ data_out[74] ^ data_out[75] ^ data_out[76] ^ data_out[77] ^ data_out[78] ^ data_out[79] ^ data_out[80] ^ data_out[81] ^ data_out[82] ^ data_out[83] ^ data_out[84] ^ data_out[85] ^ data_out[86] ^ data_out[87] ^ data_out[88] ^ data_out[89] ^ data_out[90] ^ data_out[91] ^ data_out[92] ^ data_out[93] ^ data_out[94] ^ data_out[95] ^ data_out[96] ^ data_out[97] ^ data_out[98] ^ data_out[99] ^ data_out[100] ^ data_out[101] ^ data_out[102] ^ data_out[103] ^ data_out[104] ^ data_out[105] ^ data_out[106] ^ data_out[107] ^ data_out[108] ^ data_out[109] ^ data_out[110] ^ data_out[111] ^ data_out[112] ^ data_out[113] ^ data_out[114] ^ data_out[115] ^ data_out[116] ^ data_out[117] ^ data_out[118] ^ data_out[119] ^ checker_out[0] ^ checker_out[1] ^ checker_out[2] ^ checker_out[3] ^ checker_out[4] ^ checker_out[5] ^ checker_out[6] ^ parity_out ;

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
            3: correct_data_out[119] = ~data_out[119];
            5: correct_data_out[118] = ~data_out[118];
            6: correct_data_out[117] = ~data_out[117];
            7: correct_data_out[116] = ~data_out[116];
            9: correct_data_out[115] = ~data_out[115];
            10: correct_data_out[114] = ~data_out[114];
            11: correct_data_out[113] = ~data_out[113];
            12: correct_data_out[112] = ~data_out[112];
            13: correct_data_out[111] = ~data_out[111];
            14: correct_data_out[110] = ~data_out[110];
            15: correct_data_out[109] = ~data_out[109];
            17: correct_data_out[108] = ~data_out[108];
            18: correct_data_out[107] = ~data_out[107];
            19: correct_data_out[106] = ~data_out[106];
            20: correct_data_out[105] = ~data_out[105];
            21: correct_data_out[104] = ~data_out[104];
            22: correct_data_out[103] = ~data_out[103];
            23: correct_data_out[102] = ~data_out[102];
            24: correct_data_out[101] = ~data_out[101];
            25: correct_data_out[100] = ~data_out[100];
            26: correct_data_out[99] = ~data_out[99];
            27: correct_data_out[98] = ~data_out[98];
            28: correct_data_out[97] = ~data_out[97];
            29: correct_data_out[96] = ~data_out[96];
            30: correct_data_out[95] = ~data_out[95];
            31: correct_data_out[94] = ~data_out[94];
            33: correct_data_out[93] = ~data_out[93];
            34: correct_data_out[92] = ~data_out[92];
            35: correct_data_out[91] = ~data_out[91];
            36: correct_data_out[90] = ~data_out[90];
            37: correct_data_out[89] = ~data_out[89];
            38: correct_data_out[88] = ~data_out[88];
            39: correct_data_out[87] = ~data_out[87];
            40: correct_data_out[86] = ~data_out[86];
            41: correct_data_out[85] = ~data_out[85];
            42: correct_data_out[84] = ~data_out[84];
            43: correct_data_out[83] = ~data_out[83];
            44: correct_data_out[82] = ~data_out[82];
            45: correct_data_out[81] = ~data_out[81];
            46: correct_data_out[80] = ~data_out[80];
            47: correct_data_out[79] = ~data_out[79];
            48: correct_data_out[78] = ~data_out[78];
            49: correct_data_out[77] = ~data_out[77];
            50: correct_data_out[76] = ~data_out[76];
            51: correct_data_out[75] = ~data_out[75];
            52: correct_data_out[74] = ~data_out[74];
            53: correct_data_out[73] = ~data_out[73];
            54: correct_data_out[72] = ~data_out[72];
            55: correct_data_out[71] = ~data_out[71];
            56: correct_data_out[70] = ~data_out[70];
            57: correct_data_out[69] = ~data_out[69];
            58: correct_data_out[68] = ~data_out[68];
            59: correct_data_out[67] = ~data_out[67];
            60: correct_data_out[66] = ~data_out[66];
            61: correct_data_out[65] = ~data_out[65];
            62: correct_data_out[64] = ~data_out[64];
            63: correct_data_out[63] = ~data_out[63];
            65: correct_data_out[62] = ~data_out[62];
            66: correct_data_out[61] = ~data_out[61];
            67: correct_data_out[60] = ~data_out[60];
            68: correct_data_out[59] = ~data_out[59];
            69: correct_data_out[58] = ~data_out[58];
            70: correct_data_out[57] = ~data_out[57];
            71: correct_data_out[56] = ~data_out[56];
            72: correct_data_out[55] = ~data_out[55];
            73: correct_data_out[54] = ~data_out[54];
            74: correct_data_out[53] = ~data_out[53];
            75: correct_data_out[52] = ~data_out[52];
            76: correct_data_out[51] = ~data_out[51];
            77: correct_data_out[50] = ~data_out[50];
            78: correct_data_out[49] = ~data_out[49];
            79: correct_data_out[48] = ~data_out[48];
            80: correct_data_out[47] = ~data_out[47];
            81: correct_data_out[46] = ~data_out[46];
            82: correct_data_out[45] = ~data_out[45];
            83: correct_data_out[44] = ~data_out[44];
            84: correct_data_out[43] = ~data_out[43];
            85: correct_data_out[42] = ~data_out[42];
            86: correct_data_out[41] = ~data_out[41];
            87: correct_data_out[40] = ~data_out[40];
            88: correct_data_out[39] = ~data_out[39];
            89: correct_data_out[38] = ~data_out[38];
            90: correct_data_out[37] = ~data_out[37];
            91: correct_data_out[36] = ~data_out[36];
            92: correct_data_out[35] = ~data_out[35];
            93: correct_data_out[34] = ~data_out[34];
            94: correct_data_out[33] = ~data_out[33];
            95: correct_data_out[32] = ~data_out[32];
            96: correct_data_out[31] = ~data_out[31];
            97: correct_data_out[30] = ~data_out[30];
            98: correct_data_out[29] = ~data_out[29];
            99: correct_data_out[28] = ~data_out[28];
            100: correct_data_out[27] = ~data_out[27];
            101: correct_data_out[26] = ~data_out[26];
            102: correct_data_out[25] = ~data_out[25];
            103: correct_data_out[24] = ~data_out[24];
            104: correct_data_out[23] = ~data_out[23];
            105: correct_data_out[22] = ~data_out[22];
            106: correct_data_out[21] = ~data_out[21];
            107: correct_data_out[20] = ~data_out[20];
            108: correct_data_out[19] = ~data_out[19];
            109: correct_data_out[18] = ~data_out[18];
            110: correct_data_out[17] = ~data_out[17];
            111: correct_data_out[16] = ~data_out[16];
            112: correct_data_out[15] = ~data_out[15];
            113: correct_data_out[14] = ~data_out[14];
            114: correct_data_out[13] = ~data_out[13];
            115: correct_data_out[12] = ~data_out[12];
            116: correct_data_out[11] = ~data_out[11];
            117: correct_data_out[10] = ~data_out[10];
            118: correct_data_out[9] = ~data_out[9];
            119: correct_data_out[8] = ~data_out[8];
            120: correct_data_out[7] = ~data_out[7];
            121: correct_data_out[6] = ~data_out[6];
            122: correct_data_out[5] = ~data_out[5];
            123: correct_data_out[4] = ~data_out[4];
            124: correct_data_out[3] = ~data_out[3];
            125: correct_data_out[2] = ~data_out[2];
            126: correct_data_out[1] = ~data_out[1];
            127: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_8(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [ 119:0] data_in;
output     [ 119:0] correct_data_out;
output             error;

wire       [ 119:0] data_out;
wire       [  6:0] checker_in;
wire       [  6:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_8 u_checker(data_in, checker_in, parity_in);
data_mem_8 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_8 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_8 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


