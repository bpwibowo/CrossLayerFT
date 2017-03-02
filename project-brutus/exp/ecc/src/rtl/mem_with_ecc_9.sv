module ecc_checker_9(data_in, checker_in, parity_in);

input      [ 246:0] data_in;
output reg [  7:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[246] ^ data_in[245] ^ data_in[243] ^ data_in[242] ^ data_in[240] ^ data_in[238] ^ data_in[236] ^ data_in[235] ^ data_in[233] ^ data_in[231] ^ data_in[229] ^ data_in[227] ^ data_in[225] ^ data_in[223] ^ data_in[221] ^ data_in[220] ^ data_in[218] ^ data_in[216] ^ data_in[214] ^ data_in[212] ^ data_in[210] ^ data_in[208] ^ data_in[206] ^ data_in[204] ^ data_in[202] ^ data_in[200] ^ data_in[198] ^ data_in[196] ^ data_in[194] ^ data_in[192] ^ data_in[190] ^ data_in[189] ^ data_in[187] ^ data_in[185] ^ data_in[183] ^ data_in[181] ^ data_in[179] ^ data_in[177] ^ data_in[175] ^ data_in[173] ^ data_in[171] ^ data_in[169] ^ data_in[167] ^ data_in[165] ^ data_in[163] ^ data_in[161] ^ data_in[159] ^ data_in[157] ^ data_in[155] ^ data_in[153] ^ data_in[151] ^ data_in[149] ^ data_in[147] ^ data_in[145] ^ data_in[143] ^ data_in[141] ^ data_in[139] ^ data_in[137] ^ data_in[135] ^ data_in[133] ^ data_in[131] ^ data_in[129] ^ data_in[127] ^ data_in[126] ^ data_in[124] ^ data_in[122] ^ data_in[120] ^ data_in[118] ^ data_in[116] ^ data_in[114] ^ data_in[112] ^ data_in[110] ^ data_in[108] ^ data_in[106] ^ data_in[104] ^ data_in[102] ^ data_in[100] ^ data_in[98] ^ data_in[96] ^ data_in[94] ^ data_in[92] ^ data_in[90] ^ data_in[88] ^ data_in[86] ^ data_in[84] ^ data_in[82] ^ data_in[80] ^ data_in[78] ^ data_in[76] ^ data_in[74] ^ data_in[72] ^ data_in[70] ^ data_in[68] ^ data_in[66] ^ data_in[64] ^ data_in[62] ^ data_in[60] ^ data_in[58] ^ data_in[56] ^ data_in[54] ^ data_in[52] ^ data_in[50] ^ data_in[48] ^ data_in[46] ^ data_in[44] ^ data_in[42] ^ data_in[40] ^ data_in[38] ^ data_in[36] ^ data_in[34] ^ data_in[32] ^ data_in[30] ^ data_in[28] ^ data_in[26] ^ data_in[24] ^ data_in[22] ^ data_in[20] ^ data_in[18] ^ data_in[16] ^ data_in[14] ^ data_in[12] ^ data_in[10] ^ data_in[8] ^ data_in[6] ^ data_in[4] ^ data_in[2] ^ data_in[0] ;
    checker_in[1] = data_in[246] ^ data_in[244] ^ data_in[243] ^ data_in[241] ^ data_in[240] ^ data_in[237] ^ data_in[236] ^ data_in[234] ^ data_in[233] ^ data_in[230] ^ data_in[229] ^ data_in[226] ^ data_in[225] ^ data_in[222] ^ data_in[221] ^ data_in[219] ^ data_in[218] ^ data_in[215] ^ data_in[214] ^ data_in[211] ^ data_in[210] ^ data_in[207] ^ data_in[206] ^ data_in[203] ^ data_in[202] ^ data_in[199] ^ data_in[198] ^ data_in[195] ^ data_in[194] ^ data_in[191] ^ data_in[190] ^ data_in[188] ^ data_in[187] ^ data_in[184] ^ data_in[183] ^ data_in[180] ^ data_in[179] ^ data_in[176] ^ data_in[175] ^ data_in[172] ^ data_in[171] ^ data_in[168] ^ data_in[167] ^ data_in[164] ^ data_in[163] ^ data_in[160] ^ data_in[159] ^ data_in[156] ^ data_in[155] ^ data_in[152] ^ data_in[151] ^ data_in[148] ^ data_in[147] ^ data_in[144] ^ data_in[143] ^ data_in[140] ^ data_in[139] ^ data_in[136] ^ data_in[135] ^ data_in[132] ^ data_in[131] ^ data_in[128] ^ data_in[127] ^ data_in[125] ^ data_in[124] ^ data_in[121] ^ data_in[120] ^ data_in[117] ^ data_in[116] ^ data_in[113] ^ data_in[112] ^ data_in[109] ^ data_in[108] ^ data_in[105] ^ data_in[104] ^ data_in[101] ^ data_in[100] ^ data_in[97] ^ data_in[96] ^ data_in[93] ^ data_in[92] ^ data_in[89] ^ data_in[88] ^ data_in[85] ^ data_in[84] ^ data_in[81] ^ data_in[80] ^ data_in[77] ^ data_in[76] ^ data_in[73] ^ data_in[72] ^ data_in[69] ^ data_in[68] ^ data_in[65] ^ data_in[64] ^ data_in[61] ^ data_in[60] ^ data_in[57] ^ data_in[56] ^ data_in[53] ^ data_in[52] ^ data_in[49] ^ data_in[48] ^ data_in[45] ^ data_in[44] ^ data_in[41] ^ data_in[40] ^ data_in[37] ^ data_in[36] ^ data_in[33] ^ data_in[32] ^ data_in[29] ^ data_in[28] ^ data_in[25] ^ data_in[24] ^ data_in[21] ^ data_in[20] ^ data_in[17] ^ data_in[16] ^ data_in[13] ^ data_in[12] ^ data_in[9] ^ data_in[8] ^ data_in[5] ^ data_in[4] ^ data_in[1] ^ data_in[0] ;
    checker_in[2] = data_in[245] ^ data_in[244] ^ data_in[243] ^ data_in[239] ^ data_in[238] ^ data_in[237] ^ data_in[236] ^ data_in[232] ^ data_in[231] ^ data_in[230] ^ data_in[229] ^ data_in[224] ^ data_in[223] ^ data_in[222] ^ data_in[221] ^ data_in[217] ^ data_in[216] ^ data_in[215] ^ data_in[214] ^ data_in[209] ^ data_in[208] ^ data_in[207] ^ data_in[206] ^ data_in[201] ^ data_in[200] ^ data_in[199] ^ data_in[198] ^ data_in[193] ^ data_in[192] ^ data_in[191] ^ data_in[190] ^ data_in[186] ^ data_in[185] ^ data_in[184] ^ data_in[183] ^ data_in[178] ^ data_in[177] ^ data_in[176] ^ data_in[175] ^ data_in[170] ^ data_in[169] ^ data_in[168] ^ data_in[167] ^ data_in[162] ^ data_in[161] ^ data_in[160] ^ data_in[159] ^ data_in[154] ^ data_in[153] ^ data_in[152] ^ data_in[151] ^ data_in[146] ^ data_in[145] ^ data_in[144] ^ data_in[143] ^ data_in[138] ^ data_in[137] ^ data_in[136] ^ data_in[135] ^ data_in[130] ^ data_in[129] ^ data_in[128] ^ data_in[127] ^ data_in[123] ^ data_in[122] ^ data_in[121] ^ data_in[120] ^ data_in[115] ^ data_in[114] ^ data_in[113] ^ data_in[112] ^ data_in[107] ^ data_in[106] ^ data_in[105] ^ data_in[104] ^ data_in[99] ^ data_in[98] ^ data_in[97] ^ data_in[96] ^ data_in[91] ^ data_in[90] ^ data_in[89] ^ data_in[88] ^ data_in[83] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[75] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[59] ^ data_in[58] ^ data_in[57] ^ data_in[56] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[3] = data_in[242] ^ data_in[241] ^ data_in[240] ^ data_in[239] ^ data_in[238] ^ data_in[237] ^ data_in[236] ^ data_in[228] ^ data_in[227] ^ data_in[226] ^ data_in[225] ^ data_in[224] ^ data_in[223] ^ data_in[222] ^ data_in[221] ^ data_in[213] ^ data_in[212] ^ data_in[211] ^ data_in[210] ^ data_in[209] ^ data_in[208] ^ data_in[207] ^ data_in[206] ^ data_in[197] ^ data_in[196] ^ data_in[195] ^ data_in[194] ^ data_in[193] ^ data_in[192] ^ data_in[191] ^ data_in[190] ^ data_in[182] ^ data_in[181] ^ data_in[180] ^ data_in[179] ^ data_in[178] ^ data_in[177] ^ data_in[176] ^ data_in[175] ^ data_in[166] ^ data_in[165] ^ data_in[164] ^ data_in[163] ^ data_in[162] ^ data_in[161] ^ data_in[160] ^ data_in[159] ^ data_in[150] ^ data_in[149] ^ data_in[148] ^ data_in[147] ^ data_in[146] ^ data_in[145] ^ data_in[144] ^ data_in[143] ^ data_in[134] ^ data_in[133] ^ data_in[132] ^ data_in[131] ^ data_in[130] ^ data_in[129] ^ data_in[128] ^ data_in[127] ^ data_in[119] ^ data_in[118] ^ data_in[117] ^ data_in[116] ^ data_in[115] ^ data_in[114] ^ data_in[113] ^ data_in[112] ^ data_in[103] ^ data_in[102] ^ data_in[101] ^ data_in[100] ^ data_in[99] ^ data_in[98] ^ data_in[97] ^ data_in[96] ^ data_in[87] ^ data_in[86] ^ data_in[85] ^ data_in[84] ^ data_in[83] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[71] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[55] ^ data_in[54] ^ data_in[53] ^ data_in[52] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[4] = data_in[235] ^ data_in[234] ^ data_in[233] ^ data_in[232] ^ data_in[231] ^ data_in[230] ^ data_in[229] ^ data_in[228] ^ data_in[227] ^ data_in[226] ^ data_in[225] ^ data_in[224] ^ data_in[223] ^ data_in[222] ^ data_in[221] ^ data_in[205] ^ data_in[204] ^ data_in[203] ^ data_in[202] ^ data_in[201] ^ data_in[200] ^ data_in[199] ^ data_in[198] ^ data_in[197] ^ data_in[196] ^ data_in[195] ^ data_in[194] ^ data_in[193] ^ data_in[192] ^ data_in[191] ^ data_in[190] ^ data_in[174] ^ data_in[173] ^ data_in[172] ^ data_in[171] ^ data_in[170] ^ data_in[169] ^ data_in[168] ^ data_in[167] ^ data_in[166] ^ data_in[165] ^ data_in[164] ^ data_in[163] ^ data_in[162] ^ data_in[161] ^ data_in[160] ^ data_in[159] ^ data_in[142] ^ data_in[141] ^ data_in[140] ^ data_in[139] ^ data_in[138] ^ data_in[137] ^ data_in[136] ^ data_in[135] ^ data_in[134] ^ data_in[133] ^ data_in[132] ^ data_in[131] ^ data_in[130] ^ data_in[129] ^ data_in[128] ^ data_in[127] ^ data_in[111] ^ data_in[110] ^ data_in[109] ^ data_in[108] ^ data_in[107] ^ data_in[106] ^ data_in[105] ^ data_in[104] ^ data_in[103] ^ data_in[102] ^ data_in[101] ^ data_in[100] ^ data_in[99] ^ data_in[98] ^ data_in[97] ^ data_in[96] ^ data_in[79] ^ data_in[78] ^ data_in[77] ^ data_in[76] ^ data_in[75] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[71] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[47] ^ data_in[46] ^ data_in[45] ^ data_in[44] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[5] = data_in[220] ^ data_in[219] ^ data_in[218] ^ data_in[217] ^ data_in[216] ^ data_in[215] ^ data_in[214] ^ data_in[213] ^ data_in[212] ^ data_in[211] ^ data_in[210] ^ data_in[209] ^ data_in[208] ^ data_in[207] ^ data_in[206] ^ data_in[205] ^ data_in[204] ^ data_in[203] ^ data_in[202] ^ data_in[201] ^ data_in[200] ^ data_in[199] ^ data_in[198] ^ data_in[197] ^ data_in[196] ^ data_in[195] ^ data_in[194] ^ data_in[193] ^ data_in[192] ^ data_in[191] ^ data_in[190] ^ data_in[158] ^ data_in[157] ^ data_in[156] ^ data_in[155] ^ data_in[154] ^ data_in[153] ^ data_in[152] ^ data_in[151] ^ data_in[150] ^ data_in[149] ^ data_in[148] ^ data_in[147] ^ data_in[146] ^ data_in[145] ^ data_in[144] ^ data_in[143] ^ data_in[142] ^ data_in[141] ^ data_in[140] ^ data_in[139] ^ data_in[138] ^ data_in[137] ^ data_in[136] ^ data_in[135] ^ data_in[134] ^ data_in[133] ^ data_in[132] ^ data_in[131] ^ data_in[130] ^ data_in[129] ^ data_in[128] ^ data_in[127] ^ data_in[95] ^ data_in[94] ^ data_in[93] ^ data_in[92] ^ data_in[91] ^ data_in[90] ^ data_in[89] ^ data_in[88] ^ data_in[87] ^ data_in[86] ^ data_in[85] ^ data_in[84] ^ data_in[83] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[79] ^ data_in[78] ^ data_in[77] ^ data_in[76] ^ data_in[75] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[71] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[31] ^ data_in[30] ^ data_in[29] ^ data_in[28] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[6] = data_in[189] ^ data_in[188] ^ data_in[187] ^ data_in[186] ^ data_in[185] ^ data_in[184] ^ data_in[183] ^ data_in[182] ^ data_in[181] ^ data_in[180] ^ data_in[179] ^ data_in[178] ^ data_in[177] ^ data_in[176] ^ data_in[175] ^ data_in[174] ^ data_in[173] ^ data_in[172] ^ data_in[171] ^ data_in[170] ^ data_in[169] ^ data_in[168] ^ data_in[167] ^ data_in[166] ^ data_in[165] ^ data_in[164] ^ data_in[163] ^ data_in[162] ^ data_in[161] ^ data_in[160] ^ data_in[159] ^ data_in[158] ^ data_in[157] ^ data_in[156] ^ data_in[155] ^ data_in[154] ^ data_in[153] ^ data_in[152] ^ data_in[151] ^ data_in[150] ^ data_in[149] ^ data_in[148] ^ data_in[147] ^ data_in[146] ^ data_in[145] ^ data_in[144] ^ data_in[143] ^ data_in[142] ^ data_in[141] ^ data_in[140] ^ data_in[139] ^ data_in[138] ^ data_in[137] ^ data_in[136] ^ data_in[135] ^ data_in[134] ^ data_in[133] ^ data_in[132] ^ data_in[131] ^ data_in[130] ^ data_in[129] ^ data_in[128] ^ data_in[127] ^ data_in[63] ^ data_in[62] ^ data_in[61] ^ data_in[60] ^ data_in[59] ^ data_in[58] ^ data_in[57] ^ data_in[56] ^ data_in[55] ^ data_in[54] ^ data_in[53] ^ data_in[52] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[47] ^ data_in[46] ^ data_in[45] ^ data_in[44] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[31] ^ data_in[30] ^ data_in[29] ^ data_in[28] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[7] = data_in[126] ^ data_in[125] ^ data_in[124] ^ data_in[123] ^ data_in[122] ^ data_in[121] ^ data_in[120] ^ data_in[119] ^ data_in[118] ^ data_in[117] ^ data_in[116] ^ data_in[115] ^ data_in[114] ^ data_in[113] ^ data_in[112] ^ data_in[111] ^ data_in[110] ^ data_in[109] ^ data_in[108] ^ data_in[107] ^ data_in[106] ^ data_in[105] ^ data_in[104] ^ data_in[103] ^ data_in[102] ^ data_in[101] ^ data_in[100] ^ data_in[99] ^ data_in[98] ^ data_in[97] ^ data_in[96] ^ data_in[95] ^ data_in[94] ^ data_in[93] ^ data_in[92] ^ data_in[91] ^ data_in[90] ^ data_in[89] ^ data_in[88] ^ data_in[87] ^ data_in[86] ^ data_in[85] ^ data_in[84] ^ data_in[83] ^ data_in[82] ^ data_in[81] ^ data_in[80] ^ data_in[79] ^ data_in[78] ^ data_in[77] ^ data_in[76] ^ data_in[75] ^ data_in[74] ^ data_in[73] ^ data_in[72] ^ data_in[71] ^ data_in[70] ^ data_in[69] ^ data_in[68] ^ data_in[67] ^ data_in[66] ^ data_in[65] ^ data_in[64] ^ data_in[63] ^ data_in[62] ^ data_in[61] ^ data_in[60] ^ data_in[59] ^ data_in[58] ^ data_in[57] ^ data_in[56] ^ data_in[55] ^ data_in[54] ^ data_in[53] ^ data_in[52] ^ data_in[51] ^ data_in[50] ^ data_in[49] ^ data_in[48] ^ data_in[47] ^ data_in[46] ^ data_in[45] ^ data_in[44] ^ data_in[43] ^ data_in[42] ^ data_in[41] ^ data_in[40] ^ data_in[39] ^ data_in[38] ^ data_in[37] ^ data_in[36] ^ data_in[35] ^ data_in[34] ^ data_in[33] ^ data_in[32] ^ data_in[31] ^ data_in[30] ^ data_in[29] ^ data_in[28] ^ data_in[27] ^ data_in[26] ^ data_in[25] ^ data_in[24] ^ data_in[23] ^ data_in[22] ^ data_in[21] ^ data_in[20] ^ data_in[19] ^ data_in[18] ^ data_in[17] ^ data_in[16] ^ data_in[15] ^ data_in[14] ^ data_in[13] ^ data_in[12] ^ data_in[11] ^ data_in[10] ^ data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    parity_in = data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[10] ^ data_in[11] ^ data_in[12] ^ data_in[13] ^ data_in[14] ^ data_in[15] ^ data_in[16] ^ data_in[17] ^ data_in[18] ^ data_in[19] ^ data_in[20] ^ data_in[21] ^ data_in[22] ^ data_in[23] ^ data_in[24] ^ data_in[25] ^ data_in[26] ^ data_in[27] ^ data_in[28] ^ data_in[29] ^ data_in[30] ^ data_in[31] ^ data_in[32] ^ data_in[33] ^ data_in[34] ^ data_in[35] ^ data_in[36] ^ data_in[37] ^ data_in[38] ^ data_in[39] ^ data_in[40] ^ data_in[41] ^ data_in[42] ^ data_in[43] ^ data_in[44] ^ data_in[45] ^ data_in[46] ^ data_in[47] ^ data_in[48] ^ data_in[49] ^ data_in[50] ^ data_in[51] ^ data_in[52] ^ data_in[53] ^ data_in[54] ^ data_in[55] ^ data_in[56] ^ data_in[57] ^ data_in[58] ^ data_in[59] ^ data_in[60] ^ data_in[61] ^ data_in[62] ^ data_in[63] ^ data_in[64] ^ data_in[65] ^ data_in[66] ^ data_in[67] ^ data_in[68] ^ data_in[69] ^ data_in[70] ^ data_in[71] ^ data_in[72] ^ data_in[73] ^ data_in[74] ^ data_in[75] ^ data_in[76] ^ data_in[77] ^ data_in[78] ^ data_in[79] ^ data_in[80] ^ data_in[81] ^ data_in[82] ^ data_in[83] ^ data_in[84] ^ data_in[85] ^ data_in[86] ^ data_in[87] ^ data_in[88] ^ data_in[89] ^ data_in[90] ^ data_in[91] ^ data_in[92] ^ data_in[93] ^ data_in[94] ^ data_in[95] ^ data_in[96] ^ data_in[97] ^ data_in[98] ^ data_in[99] ^ data_in[100] ^ data_in[101] ^ data_in[102] ^ data_in[103] ^ data_in[104] ^ data_in[105] ^ data_in[106] ^ data_in[107] ^ data_in[108] ^ data_in[109] ^ data_in[110] ^ data_in[111] ^ data_in[112] ^ data_in[113] ^ data_in[114] ^ data_in[115] ^ data_in[116] ^ data_in[117] ^ data_in[118] ^ data_in[119] ^ data_in[120] ^ data_in[121] ^ data_in[122] ^ data_in[123] ^ data_in[124] ^ data_in[125] ^ data_in[126] ^ data_in[127] ^ data_in[128] ^ data_in[129] ^ data_in[130] ^ data_in[131] ^ data_in[132] ^ data_in[133] ^ data_in[134] ^ data_in[135] ^ data_in[136] ^ data_in[137] ^ data_in[138] ^ data_in[139] ^ data_in[140] ^ data_in[141] ^ data_in[142] ^ data_in[143] ^ data_in[144] ^ data_in[145] ^ data_in[146] ^ data_in[147] ^ data_in[148] ^ data_in[149] ^ data_in[150] ^ data_in[151] ^ data_in[152] ^ data_in[153] ^ data_in[154] ^ data_in[155] ^ data_in[156] ^ data_in[157] ^ data_in[158] ^ data_in[159] ^ data_in[160] ^ data_in[161] ^ data_in[162] ^ data_in[163] ^ data_in[164] ^ data_in[165] ^ data_in[166] ^ data_in[167] ^ data_in[168] ^ data_in[169] ^ data_in[170] ^ data_in[171] ^ data_in[172] ^ data_in[173] ^ data_in[174] ^ data_in[175] ^ data_in[176] ^ data_in[177] ^ data_in[178] ^ data_in[179] ^ data_in[180] ^ data_in[181] ^ data_in[182] ^ data_in[183] ^ data_in[184] ^ data_in[185] ^ data_in[186] ^ data_in[187] ^ data_in[188] ^ data_in[189] ^ data_in[190] ^ data_in[191] ^ data_in[192] ^ data_in[193] ^ data_in[194] ^ data_in[195] ^ data_in[196] ^ data_in[197] ^ data_in[198] ^ data_in[199] ^ data_in[200] ^ data_in[201] ^ data_in[202] ^ data_in[203] ^ data_in[204] ^ data_in[205] ^ data_in[206] ^ data_in[207] ^ data_in[208] ^ data_in[209] ^ data_in[210] ^ data_in[211] ^ data_in[212] ^ data_in[213] ^ data_in[214] ^ data_in[215] ^ data_in[216] ^ data_in[217] ^ data_in[218] ^ data_in[219] ^ data_in[220] ^ data_in[221] ^ data_in[222] ^ data_in[223] ^ data_in[224] ^ data_in[225] ^ data_in[226] ^ data_in[227] ^ data_in[228] ^ data_in[229] ^ data_in[230] ^ data_in[231] ^ data_in[232] ^ data_in[233] ^ data_in[234] ^ data_in[235] ^ data_in[236] ^ data_in[237] ^ data_in[238] ^ data_in[239] ^ data_in[240] ^ data_in[241] ^ data_in[242] ^ data_in[243] ^ data_in[244] ^ data_in[245] ^ data_in[246] ^ checker_in[0] ^ checker_in[1] ^ checker_in[2] ^ checker_in[3] ^ checker_in[4] ^ checker_in[5] ^ checker_in[6] ^ checker_in[7] ;

end // always(*)
endmodule


module data_mem_9(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [ 246:0] data_in;
output reg [ 246:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_9(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  7:0] checker_in;
input              parity_in;
output reg [  7:0] checker_out;
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


module ecc_corrector_9(data_out, checker_out, parity_out, correct_data_out, error);

input      [ 246:0] data_out;
input      [  7:0] checker_out;
input              parity_out;
output reg [ 246:0] correct_data_out;
output reg         error;

reg        [  7:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[246] ^ data_out[245] ^ data_out[243] ^ data_out[242] ^ data_out[240] ^ data_out[238] ^ data_out[236] ^ data_out[235] ^ data_out[233] ^ data_out[231] ^ data_out[229] ^ data_out[227] ^ data_out[225] ^ data_out[223] ^ data_out[221] ^ data_out[220] ^ data_out[218] ^ data_out[216] ^ data_out[214] ^ data_out[212] ^ data_out[210] ^ data_out[208] ^ data_out[206] ^ data_out[204] ^ data_out[202] ^ data_out[200] ^ data_out[198] ^ data_out[196] ^ data_out[194] ^ data_out[192] ^ data_out[190] ^ data_out[189] ^ data_out[187] ^ data_out[185] ^ data_out[183] ^ data_out[181] ^ data_out[179] ^ data_out[177] ^ data_out[175] ^ data_out[173] ^ data_out[171] ^ data_out[169] ^ data_out[167] ^ data_out[165] ^ data_out[163] ^ data_out[161] ^ data_out[159] ^ data_out[157] ^ data_out[155] ^ data_out[153] ^ data_out[151] ^ data_out[149] ^ data_out[147] ^ data_out[145] ^ data_out[143] ^ data_out[141] ^ data_out[139] ^ data_out[137] ^ data_out[135] ^ data_out[133] ^ data_out[131] ^ data_out[129] ^ data_out[127] ^ data_out[126] ^ data_out[124] ^ data_out[122] ^ data_out[120] ^ data_out[118] ^ data_out[116] ^ data_out[114] ^ data_out[112] ^ data_out[110] ^ data_out[108] ^ data_out[106] ^ data_out[104] ^ data_out[102] ^ data_out[100] ^ data_out[98] ^ data_out[96] ^ data_out[94] ^ data_out[92] ^ data_out[90] ^ data_out[88] ^ data_out[86] ^ data_out[84] ^ data_out[82] ^ data_out[80] ^ data_out[78] ^ data_out[76] ^ data_out[74] ^ data_out[72] ^ data_out[70] ^ data_out[68] ^ data_out[66] ^ data_out[64] ^ data_out[62] ^ data_out[60] ^ data_out[58] ^ data_out[56] ^ data_out[54] ^ data_out[52] ^ data_out[50] ^ data_out[48] ^ data_out[46] ^ data_out[44] ^ data_out[42] ^ data_out[40] ^ data_out[38] ^ data_out[36] ^ data_out[34] ^ data_out[32] ^ data_out[30] ^ data_out[28] ^ data_out[26] ^ data_out[24] ^ data_out[22] ^ data_out[20] ^ data_out[18] ^ data_out[16] ^ data_out[14] ^ data_out[12] ^ data_out[10] ^ data_out[8] ^ data_out[6] ^ data_out[4] ^ data_out[2] ^ data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[246] ^ data_out[244] ^ data_out[243] ^ data_out[241] ^ data_out[240] ^ data_out[237] ^ data_out[236] ^ data_out[234] ^ data_out[233] ^ data_out[230] ^ data_out[229] ^ data_out[226] ^ data_out[225] ^ data_out[222] ^ data_out[221] ^ data_out[219] ^ data_out[218] ^ data_out[215] ^ data_out[214] ^ data_out[211] ^ data_out[210] ^ data_out[207] ^ data_out[206] ^ data_out[203] ^ data_out[202] ^ data_out[199] ^ data_out[198] ^ data_out[195] ^ data_out[194] ^ data_out[191] ^ data_out[190] ^ data_out[188] ^ data_out[187] ^ data_out[184] ^ data_out[183] ^ data_out[180] ^ data_out[179] ^ data_out[176] ^ data_out[175] ^ data_out[172] ^ data_out[171] ^ data_out[168] ^ data_out[167] ^ data_out[164] ^ data_out[163] ^ data_out[160] ^ data_out[159] ^ data_out[156] ^ data_out[155] ^ data_out[152] ^ data_out[151] ^ data_out[148] ^ data_out[147] ^ data_out[144] ^ data_out[143] ^ data_out[140] ^ data_out[139] ^ data_out[136] ^ data_out[135] ^ data_out[132] ^ data_out[131] ^ data_out[128] ^ data_out[127] ^ data_out[125] ^ data_out[124] ^ data_out[121] ^ data_out[120] ^ data_out[117] ^ data_out[116] ^ data_out[113] ^ data_out[112] ^ data_out[109] ^ data_out[108] ^ data_out[105] ^ data_out[104] ^ data_out[101] ^ data_out[100] ^ data_out[97] ^ data_out[96] ^ data_out[93] ^ data_out[92] ^ data_out[89] ^ data_out[88] ^ data_out[85] ^ data_out[84] ^ data_out[81] ^ data_out[80] ^ data_out[77] ^ data_out[76] ^ data_out[73] ^ data_out[72] ^ data_out[69] ^ data_out[68] ^ data_out[65] ^ data_out[64] ^ data_out[61] ^ data_out[60] ^ data_out[57] ^ data_out[56] ^ data_out[53] ^ data_out[52] ^ data_out[49] ^ data_out[48] ^ data_out[45] ^ data_out[44] ^ data_out[41] ^ data_out[40] ^ data_out[37] ^ data_out[36] ^ data_out[33] ^ data_out[32] ^ data_out[29] ^ data_out[28] ^ data_out[25] ^ data_out[24] ^ data_out[21] ^ data_out[20] ^ data_out[17] ^ data_out[16] ^ data_out[13] ^ data_out[12] ^ data_out[9] ^ data_out[8] ^ data_out[5] ^ data_out[4] ^ data_out[1] ^ data_out[0] ^ checker_out[1] ;
    syndrome[2] = data_out[245] ^ data_out[244] ^ data_out[243] ^ data_out[239] ^ data_out[238] ^ data_out[237] ^ data_out[236] ^ data_out[232] ^ data_out[231] ^ data_out[230] ^ data_out[229] ^ data_out[224] ^ data_out[223] ^ data_out[222] ^ data_out[221] ^ data_out[217] ^ data_out[216] ^ data_out[215] ^ data_out[214] ^ data_out[209] ^ data_out[208] ^ data_out[207] ^ data_out[206] ^ data_out[201] ^ data_out[200] ^ data_out[199] ^ data_out[198] ^ data_out[193] ^ data_out[192] ^ data_out[191] ^ data_out[190] ^ data_out[186] ^ data_out[185] ^ data_out[184] ^ data_out[183] ^ data_out[178] ^ data_out[177] ^ data_out[176] ^ data_out[175] ^ data_out[170] ^ data_out[169] ^ data_out[168] ^ data_out[167] ^ data_out[162] ^ data_out[161] ^ data_out[160] ^ data_out[159] ^ data_out[154] ^ data_out[153] ^ data_out[152] ^ data_out[151] ^ data_out[146] ^ data_out[145] ^ data_out[144] ^ data_out[143] ^ data_out[138] ^ data_out[137] ^ data_out[136] ^ data_out[135] ^ data_out[130] ^ data_out[129] ^ data_out[128] ^ data_out[127] ^ data_out[123] ^ data_out[122] ^ data_out[121] ^ data_out[120] ^ data_out[115] ^ data_out[114] ^ data_out[113] ^ data_out[112] ^ data_out[107] ^ data_out[106] ^ data_out[105] ^ data_out[104] ^ data_out[99] ^ data_out[98] ^ data_out[97] ^ data_out[96] ^ data_out[91] ^ data_out[90] ^ data_out[89] ^ data_out[88] ^ data_out[83] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[75] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[59] ^ data_out[58] ^ data_out[57] ^ data_out[56] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[2] ;
    syndrome[3] = data_out[242] ^ data_out[241] ^ data_out[240] ^ data_out[239] ^ data_out[238] ^ data_out[237] ^ data_out[236] ^ data_out[228] ^ data_out[227] ^ data_out[226] ^ data_out[225] ^ data_out[224] ^ data_out[223] ^ data_out[222] ^ data_out[221] ^ data_out[213] ^ data_out[212] ^ data_out[211] ^ data_out[210] ^ data_out[209] ^ data_out[208] ^ data_out[207] ^ data_out[206] ^ data_out[197] ^ data_out[196] ^ data_out[195] ^ data_out[194] ^ data_out[193] ^ data_out[192] ^ data_out[191] ^ data_out[190] ^ data_out[182] ^ data_out[181] ^ data_out[180] ^ data_out[179] ^ data_out[178] ^ data_out[177] ^ data_out[176] ^ data_out[175] ^ data_out[166] ^ data_out[165] ^ data_out[164] ^ data_out[163] ^ data_out[162] ^ data_out[161] ^ data_out[160] ^ data_out[159] ^ data_out[150] ^ data_out[149] ^ data_out[148] ^ data_out[147] ^ data_out[146] ^ data_out[145] ^ data_out[144] ^ data_out[143] ^ data_out[134] ^ data_out[133] ^ data_out[132] ^ data_out[131] ^ data_out[130] ^ data_out[129] ^ data_out[128] ^ data_out[127] ^ data_out[119] ^ data_out[118] ^ data_out[117] ^ data_out[116] ^ data_out[115] ^ data_out[114] ^ data_out[113] ^ data_out[112] ^ data_out[103] ^ data_out[102] ^ data_out[101] ^ data_out[100] ^ data_out[99] ^ data_out[98] ^ data_out[97] ^ data_out[96] ^ data_out[87] ^ data_out[86] ^ data_out[85] ^ data_out[84] ^ data_out[83] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[71] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[55] ^ data_out[54] ^ data_out[53] ^ data_out[52] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[3] ;
    syndrome[4] = data_out[235] ^ data_out[234] ^ data_out[233] ^ data_out[232] ^ data_out[231] ^ data_out[230] ^ data_out[229] ^ data_out[228] ^ data_out[227] ^ data_out[226] ^ data_out[225] ^ data_out[224] ^ data_out[223] ^ data_out[222] ^ data_out[221] ^ data_out[205] ^ data_out[204] ^ data_out[203] ^ data_out[202] ^ data_out[201] ^ data_out[200] ^ data_out[199] ^ data_out[198] ^ data_out[197] ^ data_out[196] ^ data_out[195] ^ data_out[194] ^ data_out[193] ^ data_out[192] ^ data_out[191] ^ data_out[190] ^ data_out[174] ^ data_out[173] ^ data_out[172] ^ data_out[171] ^ data_out[170] ^ data_out[169] ^ data_out[168] ^ data_out[167] ^ data_out[166] ^ data_out[165] ^ data_out[164] ^ data_out[163] ^ data_out[162] ^ data_out[161] ^ data_out[160] ^ data_out[159] ^ data_out[142] ^ data_out[141] ^ data_out[140] ^ data_out[139] ^ data_out[138] ^ data_out[137] ^ data_out[136] ^ data_out[135] ^ data_out[134] ^ data_out[133] ^ data_out[132] ^ data_out[131] ^ data_out[130] ^ data_out[129] ^ data_out[128] ^ data_out[127] ^ data_out[111] ^ data_out[110] ^ data_out[109] ^ data_out[108] ^ data_out[107] ^ data_out[106] ^ data_out[105] ^ data_out[104] ^ data_out[103] ^ data_out[102] ^ data_out[101] ^ data_out[100] ^ data_out[99] ^ data_out[98] ^ data_out[97] ^ data_out[96] ^ data_out[79] ^ data_out[78] ^ data_out[77] ^ data_out[76] ^ data_out[75] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[71] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[47] ^ data_out[46] ^ data_out[45] ^ data_out[44] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[4] ;
    syndrome[5] = data_out[220] ^ data_out[219] ^ data_out[218] ^ data_out[217] ^ data_out[216] ^ data_out[215] ^ data_out[214] ^ data_out[213] ^ data_out[212] ^ data_out[211] ^ data_out[210] ^ data_out[209] ^ data_out[208] ^ data_out[207] ^ data_out[206] ^ data_out[205] ^ data_out[204] ^ data_out[203] ^ data_out[202] ^ data_out[201] ^ data_out[200] ^ data_out[199] ^ data_out[198] ^ data_out[197] ^ data_out[196] ^ data_out[195] ^ data_out[194] ^ data_out[193] ^ data_out[192] ^ data_out[191] ^ data_out[190] ^ data_out[158] ^ data_out[157] ^ data_out[156] ^ data_out[155] ^ data_out[154] ^ data_out[153] ^ data_out[152] ^ data_out[151] ^ data_out[150] ^ data_out[149] ^ data_out[148] ^ data_out[147] ^ data_out[146] ^ data_out[145] ^ data_out[144] ^ data_out[143] ^ data_out[142] ^ data_out[141] ^ data_out[140] ^ data_out[139] ^ data_out[138] ^ data_out[137] ^ data_out[136] ^ data_out[135] ^ data_out[134] ^ data_out[133] ^ data_out[132] ^ data_out[131] ^ data_out[130] ^ data_out[129] ^ data_out[128] ^ data_out[127] ^ data_out[95] ^ data_out[94] ^ data_out[93] ^ data_out[92] ^ data_out[91] ^ data_out[90] ^ data_out[89] ^ data_out[88] ^ data_out[87] ^ data_out[86] ^ data_out[85] ^ data_out[84] ^ data_out[83] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[79] ^ data_out[78] ^ data_out[77] ^ data_out[76] ^ data_out[75] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[71] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[31] ^ data_out[30] ^ data_out[29] ^ data_out[28] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[5] ;
    syndrome[6] = data_out[189] ^ data_out[188] ^ data_out[187] ^ data_out[186] ^ data_out[185] ^ data_out[184] ^ data_out[183] ^ data_out[182] ^ data_out[181] ^ data_out[180] ^ data_out[179] ^ data_out[178] ^ data_out[177] ^ data_out[176] ^ data_out[175] ^ data_out[174] ^ data_out[173] ^ data_out[172] ^ data_out[171] ^ data_out[170] ^ data_out[169] ^ data_out[168] ^ data_out[167] ^ data_out[166] ^ data_out[165] ^ data_out[164] ^ data_out[163] ^ data_out[162] ^ data_out[161] ^ data_out[160] ^ data_out[159] ^ data_out[158] ^ data_out[157] ^ data_out[156] ^ data_out[155] ^ data_out[154] ^ data_out[153] ^ data_out[152] ^ data_out[151] ^ data_out[150] ^ data_out[149] ^ data_out[148] ^ data_out[147] ^ data_out[146] ^ data_out[145] ^ data_out[144] ^ data_out[143] ^ data_out[142] ^ data_out[141] ^ data_out[140] ^ data_out[139] ^ data_out[138] ^ data_out[137] ^ data_out[136] ^ data_out[135] ^ data_out[134] ^ data_out[133] ^ data_out[132] ^ data_out[131] ^ data_out[130] ^ data_out[129] ^ data_out[128] ^ data_out[127] ^ data_out[63] ^ data_out[62] ^ data_out[61] ^ data_out[60] ^ data_out[59] ^ data_out[58] ^ data_out[57] ^ data_out[56] ^ data_out[55] ^ data_out[54] ^ data_out[53] ^ data_out[52] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[47] ^ data_out[46] ^ data_out[45] ^ data_out[44] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[31] ^ data_out[30] ^ data_out[29] ^ data_out[28] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[6] ;
    syndrome[7] = data_out[126] ^ data_out[125] ^ data_out[124] ^ data_out[123] ^ data_out[122] ^ data_out[121] ^ data_out[120] ^ data_out[119] ^ data_out[118] ^ data_out[117] ^ data_out[116] ^ data_out[115] ^ data_out[114] ^ data_out[113] ^ data_out[112] ^ data_out[111] ^ data_out[110] ^ data_out[109] ^ data_out[108] ^ data_out[107] ^ data_out[106] ^ data_out[105] ^ data_out[104] ^ data_out[103] ^ data_out[102] ^ data_out[101] ^ data_out[100] ^ data_out[99] ^ data_out[98] ^ data_out[97] ^ data_out[96] ^ data_out[95] ^ data_out[94] ^ data_out[93] ^ data_out[92] ^ data_out[91] ^ data_out[90] ^ data_out[89] ^ data_out[88] ^ data_out[87] ^ data_out[86] ^ data_out[85] ^ data_out[84] ^ data_out[83] ^ data_out[82] ^ data_out[81] ^ data_out[80] ^ data_out[79] ^ data_out[78] ^ data_out[77] ^ data_out[76] ^ data_out[75] ^ data_out[74] ^ data_out[73] ^ data_out[72] ^ data_out[71] ^ data_out[70] ^ data_out[69] ^ data_out[68] ^ data_out[67] ^ data_out[66] ^ data_out[65] ^ data_out[64] ^ data_out[63] ^ data_out[62] ^ data_out[61] ^ data_out[60] ^ data_out[59] ^ data_out[58] ^ data_out[57] ^ data_out[56] ^ data_out[55] ^ data_out[54] ^ data_out[53] ^ data_out[52] ^ data_out[51] ^ data_out[50] ^ data_out[49] ^ data_out[48] ^ data_out[47] ^ data_out[46] ^ data_out[45] ^ data_out[44] ^ data_out[43] ^ data_out[42] ^ data_out[41] ^ data_out[40] ^ data_out[39] ^ data_out[38] ^ data_out[37] ^ data_out[36] ^ data_out[35] ^ data_out[34] ^ data_out[33] ^ data_out[32] ^ data_out[31] ^ data_out[30] ^ data_out[29] ^ data_out[28] ^ data_out[27] ^ data_out[26] ^ data_out[25] ^ data_out[24] ^ data_out[23] ^ data_out[22] ^ data_out[21] ^ data_out[20] ^ data_out[19] ^ data_out[18] ^ data_out[17] ^ data_out[16] ^ data_out[15] ^ data_out[14] ^ data_out[13] ^ data_out[12] ^ data_out[11] ^ data_out[10] ^ data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[7] ;

    parity = data_out[0] ^ data_out[1] ^ data_out[2] ^ data_out[3] ^ data_out[4] ^ data_out[5] ^ data_out[6] ^ data_out[7] ^ data_out[8] ^ data_out[9] ^ data_out[10] ^ data_out[11] ^ data_out[12] ^ data_out[13] ^ data_out[14] ^ data_out[15] ^ data_out[16] ^ data_out[17] ^ data_out[18] ^ data_out[19] ^ data_out[20] ^ data_out[21] ^ data_out[22] ^ data_out[23] ^ data_out[24] ^ data_out[25] ^ data_out[26] ^ data_out[27] ^ data_out[28] ^ data_out[29] ^ data_out[30] ^ data_out[31] ^ data_out[32] ^ data_out[33] ^ data_out[34] ^ data_out[35] ^ data_out[36] ^ data_out[37] ^ data_out[38] ^ data_out[39] ^ data_out[40] ^ data_out[41] ^ data_out[42] ^ data_out[43] ^ data_out[44] ^ data_out[45] ^ data_out[46] ^ data_out[47] ^ data_out[48] ^ data_out[49] ^ data_out[50] ^ data_out[51] ^ data_out[52] ^ data_out[53] ^ data_out[54] ^ data_out[55] ^ data_out[56] ^ data_out[57] ^ data_out[58] ^ data_out[59] ^ data_out[60] ^ data_out[61] ^ data_out[62] ^ data_out[63] ^ data_out[64] ^ data_out[65] ^ data_out[66] ^ data_out[67] ^ data_out[68] ^ data_out[69] ^ data_out[70] ^ data_out[71] ^ data_out[72] ^ data_out[73] ^ data_out[74] ^ data_out[75] ^ data_out[76] ^ data_out[77] ^ data_out[78] ^ data_out[79] ^ data_out[80] ^ data_out[81] ^ data_out[82] ^ data_out[83] ^ data_out[84] ^ data_out[85] ^ data_out[86] ^ data_out[87] ^ data_out[88] ^ data_out[89] ^ data_out[90] ^ data_out[91] ^ data_out[92] ^ data_out[93] ^ data_out[94] ^ data_out[95] ^ data_out[96] ^ data_out[97] ^ data_out[98] ^ data_out[99] ^ data_out[100] ^ data_out[101] ^ data_out[102] ^ data_out[103] ^ data_out[104] ^ data_out[105] ^ data_out[106] ^ data_out[107] ^ data_out[108] ^ data_out[109] ^ data_out[110] ^ data_out[111] ^ data_out[112] ^ data_out[113] ^ data_out[114] ^ data_out[115] ^ data_out[116] ^ data_out[117] ^ data_out[118] ^ data_out[119] ^ data_out[120] ^ data_out[121] ^ data_out[122] ^ data_out[123] ^ data_out[124] ^ data_out[125] ^ data_out[126] ^ data_out[127] ^ data_out[128] ^ data_out[129] ^ data_out[130] ^ data_out[131] ^ data_out[132] ^ data_out[133] ^ data_out[134] ^ data_out[135] ^ data_out[136] ^ data_out[137] ^ data_out[138] ^ data_out[139] ^ data_out[140] ^ data_out[141] ^ data_out[142] ^ data_out[143] ^ data_out[144] ^ data_out[145] ^ data_out[146] ^ data_out[147] ^ data_out[148] ^ data_out[149] ^ data_out[150] ^ data_out[151] ^ data_out[152] ^ data_out[153] ^ data_out[154] ^ data_out[155] ^ data_out[156] ^ data_out[157] ^ data_out[158] ^ data_out[159] ^ data_out[160] ^ data_out[161] ^ data_out[162] ^ data_out[163] ^ data_out[164] ^ data_out[165] ^ data_out[166] ^ data_out[167] ^ data_out[168] ^ data_out[169] ^ data_out[170] ^ data_out[171] ^ data_out[172] ^ data_out[173] ^ data_out[174] ^ data_out[175] ^ data_out[176] ^ data_out[177] ^ data_out[178] ^ data_out[179] ^ data_out[180] ^ data_out[181] ^ data_out[182] ^ data_out[183] ^ data_out[184] ^ data_out[185] ^ data_out[186] ^ data_out[187] ^ data_out[188] ^ data_out[189] ^ data_out[190] ^ data_out[191] ^ data_out[192] ^ data_out[193] ^ data_out[194] ^ data_out[195] ^ data_out[196] ^ data_out[197] ^ data_out[198] ^ data_out[199] ^ data_out[200] ^ data_out[201] ^ data_out[202] ^ data_out[203] ^ data_out[204] ^ data_out[205] ^ data_out[206] ^ data_out[207] ^ data_out[208] ^ data_out[209] ^ data_out[210] ^ data_out[211] ^ data_out[212] ^ data_out[213] ^ data_out[214] ^ data_out[215] ^ data_out[216] ^ data_out[217] ^ data_out[218] ^ data_out[219] ^ data_out[220] ^ data_out[221] ^ data_out[222] ^ data_out[223] ^ data_out[224] ^ data_out[225] ^ data_out[226] ^ data_out[227] ^ data_out[228] ^ data_out[229] ^ data_out[230] ^ data_out[231] ^ data_out[232] ^ data_out[233] ^ data_out[234] ^ data_out[235] ^ data_out[236] ^ data_out[237] ^ data_out[238] ^ data_out[239] ^ data_out[240] ^ data_out[241] ^ data_out[242] ^ data_out[243] ^ data_out[244] ^ data_out[245] ^ data_out[246] ^ checker_out[0] ^ checker_out[1] ^ checker_out[2] ^ checker_out[3] ^ checker_out[4] ^ checker_out[5] ^ checker_out[6] ^ checker_out[7] ^ parity_out ;

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
            3: correct_data_out[246] = ~data_out[246];
            5: correct_data_out[245] = ~data_out[245];
            6: correct_data_out[244] = ~data_out[244];
            7: correct_data_out[243] = ~data_out[243];
            9: correct_data_out[242] = ~data_out[242];
            10: correct_data_out[241] = ~data_out[241];
            11: correct_data_out[240] = ~data_out[240];
            12: correct_data_out[239] = ~data_out[239];
            13: correct_data_out[238] = ~data_out[238];
            14: correct_data_out[237] = ~data_out[237];
            15: correct_data_out[236] = ~data_out[236];
            17: correct_data_out[235] = ~data_out[235];
            18: correct_data_out[234] = ~data_out[234];
            19: correct_data_out[233] = ~data_out[233];
            20: correct_data_out[232] = ~data_out[232];
            21: correct_data_out[231] = ~data_out[231];
            22: correct_data_out[230] = ~data_out[230];
            23: correct_data_out[229] = ~data_out[229];
            24: correct_data_out[228] = ~data_out[228];
            25: correct_data_out[227] = ~data_out[227];
            26: correct_data_out[226] = ~data_out[226];
            27: correct_data_out[225] = ~data_out[225];
            28: correct_data_out[224] = ~data_out[224];
            29: correct_data_out[223] = ~data_out[223];
            30: correct_data_out[222] = ~data_out[222];
            31: correct_data_out[221] = ~data_out[221];
            33: correct_data_out[220] = ~data_out[220];
            34: correct_data_out[219] = ~data_out[219];
            35: correct_data_out[218] = ~data_out[218];
            36: correct_data_out[217] = ~data_out[217];
            37: correct_data_out[216] = ~data_out[216];
            38: correct_data_out[215] = ~data_out[215];
            39: correct_data_out[214] = ~data_out[214];
            40: correct_data_out[213] = ~data_out[213];
            41: correct_data_out[212] = ~data_out[212];
            42: correct_data_out[211] = ~data_out[211];
            43: correct_data_out[210] = ~data_out[210];
            44: correct_data_out[209] = ~data_out[209];
            45: correct_data_out[208] = ~data_out[208];
            46: correct_data_out[207] = ~data_out[207];
            47: correct_data_out[206] = ~data_out[206];
            48: correct_data_out[205] = ~data_out[205];
            49: correct_data_out[204] = ~data_out[204];
            50: correct_data_out[203] = ~data_out[203];
            51: correct_data_out[202] = ~data_out[202];
            52: correct_data_out[201] = ~data_out[201];
            53: correct_data_out[200] = ~data_out[200];
            54: correct_data_out[199] = ~data_out[199];
            55: correct_data_out[198] = ~data_out[198];
            56: correct_data_out[197] = ~data_out[197];
            57: correct_data_out[196] = ~data_out[196];
            58: correct_data_out[195] = ~data_out[195];
            59: correct_data_out[194] = ~data_out[194];
            60: correct_data_out[193] = ~data_out[193];
            61: correct_data_out[192] = ~data_out[192];
            62: correct_data_out[191] = ~data_out[191];
            63: correct_data_out[190] = ~data_out[190];
            65: correct_data_out[189] = ~data_out[189];
            66: correct_data_out[188] = ~data_out[188];
            67: correct_data_out[187] = ~data_out[187];
            68: correct_data_out[186] = ~data_out[186];
            69: correct_data_out[185] = ~data_out[185];
            70: correct_data_out[184] = ~data_out[184];
            71: correct_data_out[183] = ~data_out[183];
            72: correct_data_out[182] = ~data_out[182];
            73: correct_data_out[181] = ~data_out[181];
            74: correct_data_out[180] = ~data_out[180];
            75: correct_data_out[179] = ~data_out[179];
            76: correct_data_out[178] = ~data_out[178];
            77: correct_data_out[177] = ~data_out[177];
            78: correct_data_out[176] = ~data_out[176];
            79: correct_data_out[175] = ~data_out[175];
            80: correct_data_out[174] = ~data_out[174];
            81: correct_data_out[173] = ~data_out[173];
            82: correct_data_out[172] = ~data_out[172];
            83: correct_data_out[171] = ~data_out[171];
            84: correct_data_out[170] = ~data_out[170];
            85: correct_data_out[169] = ~data_out[169];
            86: correct_data_out[168] = ~data_out[168];
            87: correct_data_out[167] = ~data_out[167];
            88: correct_data_out[166] = ~data_out[166];
            89: correct_data_out[165] = ~data_out[165];
            90: correct_data_out[164] = ~data_out[164];
            91: correct_data_out[163] = ~data_out[163];
            92: correct_data_out[162] = ~data_out[162];
            93: correct_data_out[161] = ~data_out[161];
            94: correct_data_out[160] = ~data_out[160];
            95: correct_data_out[159] = ~data_out[159];
            96: correct_data_out[158] = ~data_out[158];
            97: correct_data_out[157] = ~data_out[157];
            98: correct_data_out[156] = ~data_out[156];
            99: correct_data_out[155] = ~data_out[155];
            100: correct_data_out[154] = ~data_out[154];
            101: correct_data_out[153] = ~data_out[153];
            102: correct_data_out[152] = ~data_out[152];
            103: correct_data_out[151] = ~data_out[151];
            104: correct_data_out[150] = ~data_out[150];
            105: correct_data_out[149] = ~data_out[149];
            106: correct_data_out[148] = ~data_out[148];
            107: correct_data_out[147] = ~data_out[147];
            108: correct_data_out[146] = ~data_out[146];
            109: correct_data_out[145] = ~data_out[145];
            110: correct_data_out[144] = ~data_out[144];
            111: correct_data_out[143] = ~data_out[143];
            112: correct_data_out[142] = ~data_out[142];
            113: correct_data_out[141] = ~data_out[141];
            114: correct_data_out[140] = ~data_out[140];
            115: correct_data_out[139] = ~data_out[139];
            116: correct_data_out[138] = ~data_out[138];
            117: correct_data_out[137] = ~data_out[137];
            118: correct_data_out[136] = ~data_out[136];
            119: correct_data_out[135] = ~data_out[135];
            120: correct_data_out[134] = ~data_out[134];
            121: correct_data_out[133] = ~data_out[133];
            122: correct_data_out[132] = ~data_out[132];
            123: correct_data_out[131] = ~data_out[131];
            124: correct_data_out[130] = ~data_out[130];
            125: correct_data_out[129] = ~data_out[129];
            126: correct_data_out[128] = ~data_out[128];
            127: correct_data_out[127] = ~data_out[127];
            129: correct_data_out[126] = ~data_out[126];
            130: correct_data_out[125] = ~data_out[125];
            131: correct_data_out[124] = ~data_out[124];
            132: correct_data_out[123] = ~data_out[123];
            133: correct_data_out[122] = ~data_out[122];
            134: correct_data_out[121] = ~data_out[121];
            135: correct_data_out[120] = ~data_out[120];
            136: correct_data_out[119] = ~data_out[119];
            137: correct_data_out[118] = ~data_out[118];
            138: correct_data_out[117] = ~data_out[117];
            139: correct_data_out[116] = ~data_out[116];
            140: correct_data_out[115] = ~data_out[115];
            141: correct_data_out[114] = ~data_out[114];
            142: correct_data_out[113] = ~data_out[113];
            143: correct_data_out[112] = ~data_out[112];
            144: correct_data_out[111] = ~data_out[111];
            145: correct_data_out[110] = ~data_out[110];
            146: correct_data_out[109] = ~data_out[109];
            147: correct_data_out[108] = ~data_out[108];
            148: correct_data_out[107] = ~data_out[107];
            149: correct_data_out[106] = ~data_out[106];
            150: correct_data_out[105] = ~data_out[105];
            151: correct_data_out[104] = ~data_out[104];
            152: correct_data_out[103] = ~data_out[103];
            153: correct_data_out[102] = ~data_out[102];
            154: correct_data_out[101] = ~data_out[101];
            155: correct_data_out[100] = ~data_out[100];
            156: correct_data_out[99] = ~data_out[99];
            157: correct_data_out[98] = ~data_out[98];
            158: correct_data_out[97] = ~data_out[97];
            159: correct_data_out[96] = ~data_out[96];
            160: correct_data_out[95] = ~data_out[95];
            161: correct_data_out[94] = ~data_out[94];
            162: correct_data_out[93] = ~data_out[93];
            163: correct_data_out[92] = ~data_out[92];
            164: correct_data_out[91] = ~data_out[91];
            165: correct_data_out[90] = ~data_out[90];
            166: correct_data_out[89] = ~data_out[89];
            167: correct_data_out[88] = ~data_out[88];
            168: correct_data_out[87] = ~data_out[87];
            169: correct_data_out[86] = ~data_out[86];
            170: correct_data_out[85] = ~data_out[85];
            171: correct_data_out[84] = ~data_out[84];
            172: correct_data_out[83] = ~data_out[83];
            173: correct_data_out[82] = ~data_out[82];
            174: correct_data_out[81] = ~data_out[81];
            175: correct_data_out[80] = ~data_out[80];
            176: correct_data_out[79] = ~data_out[79];
            177: correct_data_out[78] = ~data_out[78];
            178: correct_data_out[77] = ~data_out[77];
            179: correct_data_out[76] = ~data_out[76];
            180: correct_data_out[75] = ~data_out[75];
            181: correct_data_out[74] = ~data_out[74];
            182: correct_data_out[73] = ~data_out[73];
            183: correct_data_out[72] = ~data_out[72];
            184: correct_data_out[71] = ~data_out[71];
            185: correct_data_out[70] = ~data_out[70];
            186: correct_data_out[69] = ~data_out[69];
            187: correct_data_out[68] = ~data_out[68];
            188: correct_data_out[67] = ~data_out[67];
            189: correct_data_out[66] = ~data_out[66];
            190: correct_data_out[65] = ~data_out[65];
            191: correct_data_out[64] = ~data_out[64];
            192: correct_data_out[63] = ~data_out[63];
            193: correct_data_out[62] = ~data_out[62];
            194: correct_data_out[61] = ~data_out[61];
            195: correct_data_out[60] = ~data_out[60];
            196: correct_data_out[59] = ~data_out[59];
            197: correct_data_out[58] = ~data_out[58];
            198: correct_data_out[57] = ~data_out[57];
            199: correct_data_out[56] = ~data_out[56];
            200: correct_data_out[55] = ~data_out[55];
            201: correct_data_out[54] = ~data_out[54];
            202: correct_data_out[53] = ~data_out[53];
            203: correct_data_out[52] = ~data_out[52];
            204: correct_data_out[51] = ~data_out[51];
            205: correct_data_out[50] = ~data_out[50];
            206: correct_data_out[49] = ~data_out[49];
            207: correct_data_out[48] = ~data_out[48];
            208: correct_data_out[47] = ~data_out[47];
            209: correct_data_out[46] = ~data_out[46];
            210: correct_data_out[45] = ~data_out[45];
            211: correct_data_out[44] = ~data_out[44];
            212: correct_data_out[43] = ~data_out[43];
            213: correct_data_out[42] = ~data_out[42];
            214: correct_data_out[41] = ~data_out[41];
            215: correct_data_out[40] = ~data_out[40];
            216: correct_data_out[39] = ~data_out[39];
            217: correct_data_out[38] = ~data_out[38];
            218: correct_data_out[37] = ~data_out[37];
            219: correct_data_out[36] = ~data_out[36];
            220: correct_data_out[35] = ~data_out[35];
            221: correct_data_out[34] = ~data_out[34];
            222: correct_data_out[33] = ~data_out[33];
            223: correct_data_out[32] = ~data_out[32];
            224: correct_data_out[31] = ~data_out[31];
            225: correct_data_out[30] = ~data_out[30];
            226: correct_data_out[29] = ~data_out[29];
            227: correct_data_out[28] = ~data_out[28];
            228: correct_data_out[27] = ~data_out[27];
            229: correct_data_out[26] = ~data_out[26];
            230: correct_data_out[25] = ~data_out[25];
            231: correct_data_out[24] = ~data_out[24];
            232: correct_data_out[23] = ~data_out[23];
            233: correct_data_out[22] = ~data_out[22];
            234: correct_data_out[21] = ~data_out[21];
            235: correct_data_out[20] = ~data_out[20];
            236: correct_data_out[19] = ~data_out[19];
            237: correct_data_out[18] = ~data_out[18];
            238: correct_data_out[17] = ~data_out[17];
            239: correct_data_out[16] = ~data_out[16];
            240: correct_data_out[15] = ~data_out[15];
            241: correct_data_out[14] = ~data_out[14];
            242: correct_data_out[13] = ~data_out[13];
            243: correct_data_out[12] = ~data_out[12];
            244: correct_data_out[11] = ~data_out[11];
            245: correct_data_out[10] = ~data_out[10];
            246: correct_data_out[9] = ~data_out[9];
            247: correct_data_out[8] = ~data_out[8];
            248: correct_data_out[7] = ~data_out[7];
            249: correct_data_out[6] = ~data_out[6];
            250: correct_data_out[5] = ~data_out[5];
            251: correct_data_out[4] = ~data_out[4];
            252: correct_data_out[3] = ~data_out[3];
            253: correct_data_out[2] = ~data_out[2];
            254: correct_data_out[1] = ~data_out[1];
            255: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_9(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [ 246:0] data_in;
output     [ 246:0] correct_data_out;
output             error;

wire       [ 246:0] data_out;
wire       [  7:0] checker_in;
wire       [  7:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_9 u_checker(data_in, checker_in, parity_in);
data_mem_9 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_9 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_9 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


