module vlt (
    input             isLoad,
    input             isStore,
    input             isControl,
    input [3:0]       vop,
    input             important,

    output reg [7:0]  iq_vbit,
    output reg [7:0]  rob_wb_vbit,
    output reg [7:0]  rob_dp_vbit,
    output reg [7:0]  ib_vbit
);

always_comb begin
	iq_vbit = 23;
	case ({isLoad,isStore,isControl,vop})
		{1'h0,1'h0,1'h0,4'd0}: begin
			if (important) iq_vbit = 50; else iq_vbit = 34;
		end
		{1'h0,1'h0,1'h0,4'd1}: begin
			if (important) iq_vbit = 57; else iq_vbit = 33;
		end
		{1'h0,1'h0,1'h0,4'd2}: begin
			if (important) iq_vbit = 42; else iq_vbit = 26;
		end
		{1'h0,1'h0,1'h0,4'd3}: begin
			if (important) iq_vbit = 23; else iq_vbit = 23;
		end
		{1'h0,1'h0,1'h0,4'd4}: begin
			if (important) iq_vbit = 42; else iq_vbit = 26;
		end
		{1'h0,1'h0,1'h0,4'd5}: begin
			if (important) iq_vbit = 49; else iq_vbit = 25;
		end
		{1'h0,1'h0,1'h0,4'd6}: begin
			if (important) iq_vbit = 23; else iq_vbit = 23;
		end
		{1'h0,1'h0,1'h0,4'd7}: begin
			if (important) iq_vbit = 57; else iq_vbit = 33;
		end
		{1'h0,1'h0,1'h0,4'd8}: begin
			if (important) iq_vbit = 41; else iq_vbit = 33;
		end
		{1'h0,1'h0,1'h0,4'd9}: begin
			if (important) iq_vbit = 23; else iq_vbit = 23;
		end
		{1'h0,1'h0,1'h0,4'd10}: begin
			if (important) iq_vbit = 66; else iq_vbit = 34;
		end
		{1'h0,1'h0,1'h0,4'd11}: begin
			if (important) iq_vbit = 48; else iq_vbit = 32;
		end
		{1'h0,1'h0,1'h0,4'd12}: begin
			if (important) iq_vbit = 50; else iq_vbit = 34;
		end
		{1'h0,1'h0,1'h0,4'd13}: begin
			if (important) iq_vbit = 48; else iq_vbit = 24;
		end
		{1'h1,1'h0,1'h0,4'd0}: begin
			if (important) iq_vbit = 65; else iq_vbit = 41;
		end
		{1'h1,1'h0,1'h0,4'd1}: begin
			if (important) iq_vbit = 74; else iq_vbit = 42;
		end
		{1'h1,1'h0,1'h0,4'd2}: begin
			if (important) iq_vbit = 66; else iq_vbit = 34;
		end
		{1'h0,1'h1,1'h0,4'd0}: begin
			if (important) iq_vbit = 41; else iq_vbit = 33;
		end
		{1'h0,1'h1,1'h0,4'd1}: begin
			if (important) iq_vbit = 66; else iq_vbit = 34;
		end
		{1'h0,1'h0,1'h1,4'd0}: begin
			if (important) iq_vbit = 166; else iq_vbit = 30;
		end
		{1'h0,1'h0,1'h1,4'd1}: begin
			if (important) iq_vbit = 192; else iq_vbit = 31;
		end
		{1'h0,1'h0,1'h1,4'd2}: begin
			if (important) iq_vbit = 183; else iq_vbit = 30;
		end
		{1'h0,1'h0,1'h1,4'd3}: begin
			if (important) iq_vbit = 174; else iq_vbit = 38;
		end
		{1'h0,1'h0,1'h1,4'd4}: begin
			if (important) iq_vbit = 191; else iq_vbit = 38;
		end
		{1'h0,1'h0,1'h1,4'd5}: begin
			if (important) iq_vbit = 93; else iq_vbit = 29;
		end
		{1'h0,1'h0,1'h1,4'd6}: begin
			if (important) iq_vbit = 101; else iq_vbit = 37;
		end
	endcase
end

always_comb begin
	rob_wb_vbit = 16;
	case ({isLoad,isStore,isControl,vop})
		{1'h0,1'h0,1'h0,4'd0}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
		{1'h0,1'h0,1'h0,4'd1}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
		{1'h0,1'h0,1'h0,4'd2}: begin
			if (important) rob_wb_vbit = 16; else rob_wb_vbit = 16;
		end
		{1'h0,1'h0,1'h0,4'd3}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h0,4'd4}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h0,4'd5}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h0,4'd6}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
		{1'h0,1'h0,1'h0,4'd7}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h0,1'h0,1'h0,4'd8}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
		{1'h0,1'h0,1'h0,4'd9}: begin
			if (important) rob_wb_vbit = 16; else rob_wb_vbit = 16;
		end
		{1'h0,1'h0,1'h0,4'd10}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
		{1'h0,1'h0,1'h0,4'd11}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
		{1'h0,1'h0,1'h0,4'd12}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h0,1'h0,1'h0,4'd13}: begin
			if (important) rob_wb_vbit = 16; else rob_wb_vbit = 16;
		end
		{1'h1,1'h0,1'h0,4'd0}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h1,1'h0,1'h0,4'd1}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h1,1'h0,1'h0,4'd2}: begin
			if (important) rob_wb_vbit = 16; else rob_wb_vbit = 16;
		end
		{1'h0,1'h1,1'h0,4'd0}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h0,1'h1,1'h0,4'd1}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h1,4'd0}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h1,4'd1}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h1,4'd2}: begin
			if (important) rob_wb_vbit = 80; else rob_wb_vbit = 80;
		end
		{1'h0,1'h0,1'h1,4'd3}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h0,1'h0,1'h1,4'd4}: begin
			if (important) rob_wb_vbit = 94; else rob_wb_vbit = 94;
		end
		{1'h0,1'h0,1'h1,4'd5}: begin
			if (important) rob_wb_vbit = 16; else rob_wb_vbit = 16;
		end
		{1'h0,1'h0,1'h1,4'd6}: begin
			if (important) rob_wb_vbit = 30; else rob_wb_vbit = 30;
		end
	endcase
end

always_comb begin
	rob_dp_vbit = 5;
	case ({isLoad,isStore,isControl,vop})
		{1'h0,1'h0,1'h0,4'd0}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h0,4'd1}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h0,4'd2}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h0,1'h0,4'd3}: begin
			if (important) rob_dp_vbit = 69; else rob_dp_vbit = 69;
		end
		{1'h0,1'h0,1'h0,4'd4}: begin
			if (important) rob_dp_vbit = 69; else rob_dp_vbit = 69;
		end
		{1'h0,1'h0,1'h0,4'd5}: begin
			if (important) rob_dp_vbit = 69; else rob_dp_vbit = 69;
		end
		{1'h0,1'h0,1'h0,4'd6}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h0,4'd7}: begin
			if (important) rob_dp_vbit = 83; else rob_dp_vbit = 83;
		end
		{1'h0,1'h0,1'h0,4'd8}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h0,4'd9}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h0,1'h0,4'd10}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h0,4'd11}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h0,4'd12}: begin
			if (important) rob_dp_vbit = 83; else rob_dp_vbit = 83;
		end
		{1'h0,1'h0,1'h0,4'd13}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h1,1'h0,1'h0,4'd0}: begin
			if (important) rob_dp_vbit = 83; else rob_dp_vbit = 83;
		end
		{1'h1,1'h0,1'h0,4'd1}: begin
			if (important) rob_dp_vbit = 83; else rob_dp_vbit = 83;
		end
		{1'h1,1'h0,1'h0,4'd2}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h1,1'h0,4'd0}: begin
			if (important) rob_dp_vbit = 83; else rob_dp_vbit = 83;
		end
		{1'h0,1'h1,1'h0,4'd1}: begin
			if (important) rob_dp_vbit = 69; else rob_dp_vbit = 69;
		end
		{1'h0,1'h0,1'h1,4'd0}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h0,1'h1,4'd1}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h0,1'h1,4'd2}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h0,1'h1,4'd3}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h1,4'd4}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
		{1'h0,1'h0,1'h1,4'd5}: begin
			if (important) rob_dp_vbit = 5; else rob_dp_vbit = 5;
		end
		{1'h0,1'h0,1'h1,4'd6}: begin
			if (important) rob_dp_vbit = 19; else rob_dp_vbit = 19;
		end
	endcase
end

always_comb begin
	ib_vbit = 21;
	case ({isLoad,isStore,isControl,vop})
		{1'h0,1'h0,1'h0,4'd0}: begin
			if (important) ib_vbit = 39; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd1}: begin
			if (important) ib_vbit = 49; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd2}: begin
			if (important) ib_vbit = 33; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h0,4'd3}: begin
			if (important) ib_vbit = 85; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h0,4'd4}: begin
			if (important) ib_vbit = 97; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h0,4'd5}: begin
			if (important) ib_vbit = 107; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h0,4'd6}: begin
			if (important) ib_vbit = 21; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h0,4'd7}: begin
			if (important) ib_vbit = 113; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd8}: begin
			if (important) ib_vbit = 33; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd9}: begin
			if (important) ib_vbit = 21; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h0,4'd10}: begin
			if (important) ib_vbit = 55; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd11}: begin
			if (important) ib_vbit = 43; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd12}: begin
			if (important) ib_vbit = 103; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h0,4'd13}: begin
			if (important) ib_vbit = 43; else ib_vbit = 21;
		end
		{1'h1,1'h0,1'h0,4'd0}: begin
			if (important) ib_vbit = 113; else ib_vbit = 27;
		end
		{1'h1,1'h0,1'h0,4'd1}: begin
			if (important) ib_vbit = 119; else ib_vbit = 27;
		end
		{1'h1,1'h0,1'h0,4'd2}: begin
			if (important) ib_vbit = 49; else ib_vbit = 21;
		end
		{1'h0,1'h1,1'h0,4'd0}: begin
			if (important) ib_vbit = 97; else ib_vbit = 27;
		end
		{1'h0,1'h1,1'h0,4'd1}: begin
			if (important) ib_vbit = 113; else ib_vbit = 21;
		end
		{1'h0,1'h0,1'h1,4'd0}: begin
			if (important) ib_vbit = 97; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h1,4'd1}: begin
			if (important) ib_vbit = 120; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h1,4'd2}: begin
			if (important) ib_vbit = 114; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h1,4'd3}: begin
			if (important) ib_vbit = 103; else ib_vbit = 33;
		end
		{1'h0,1'h0,1'h1,4'd4}: begin
			if (important) ib_vbit = 120; else ib_vbit = 33;
		end
		{1'h0,1'h0,1'h1,4'd5}: begin
			if (important) ib_vbit = 27; else ib_vbit = 27;
		end
		{1'h0,1'h0,1'h1,4'd6}: begin
			if (important) ib_vbit = 33; else ib_vbit = 33;
		end
	endcase
end

endmodule // vlt

module vlt_lq (
    input [3:0]       opcode,
    input             important,
    output reg [7:0]  lq_vbit
);

always_comb begin
	lq_vbit = 19;
	case (opcode)
		0: begin // LB
			if (important) lq_vbit = 94; else lq_vbit = 92;
		end
		1: begin // LBU
			if (important) lq_vbit = 94; else lq_vbit = 92;
		end
		2: begin // LH
			if (important) lq_vbit = 94; else lq_vbit = 92;
		end
		3: begin // LHU
			if (important) lq_vbit = 94; else lq_vbit = 92;
		end
		4: begin // LW
			if (important) lq_vbit = 96; else lq_vbit = 92;
		end
		5: begin // LWU
			if (important) lq_vbit = 96; else lq_vbit = 92;
		end
		6: begin // LD
			if (important) lq_vbit = 96; else lq_vbit = 92;
		end
		7: begin // LWL
			if (important) lq_vbit = 160; else lq_vbit = 92;
		end
		8: begin // LWR
			if (important) lq_vbit = 160; else lq_vbit = 92;
		end
		9: begin // LDL
			if (important) lq_vbit = 160; else lq_vbit = 92;
		end
		10: begin // LDR
			if (important) lq_vbit = 160; else lq_vbit = 92;
		end
		11: begin // LL
			if (important) lq_vbit = 94; else lq_vbit = 92;
		end
		12: begin // LLD
			if (important) lq_vbit = 94; else lq_vbit = 92;
		end
		13: begin // PREF
			if (important) lq_vbit = 19; else lq_vbit = 19;
		end
		14: begin // PREFX
			if (important) lq_vbit = 19; else lq_vbit = 19;
		end
	endcase // opcode
end

endmodule

module vlt_lq_dp (
    input [3:0]       opcode,
    input             important,
    output reg [7:0]  lq_dp_vbit
);

always_comb begin
	lq_dp_vbit = 9;
	case (opcode)
		0: begin // LB
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		1: begin // LBU
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		2: begin // LH
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		3: begin // LHU
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		4: begin // LW
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		5: begin // LWU
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		6: begin // LD
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		7: begin // LWL
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		8: begin // LWR
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		9: begin // LDL
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		10: begin // LDR
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		11: begin // LL
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		12: begin // LLD
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		13: begin // PREF
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
		14: begin // PREFX
			if (important) lq_dp_vbit = 9; else lq_dp_vbit = 9;
		end
	endcase // opcode
end

endmodule


module vlt_sq (
    input [3:0]       opcode,
    input             important,
    output reg [7:0]  sq_vbit
);

always_comb begin
	sq_vbit = 74;
	case (opcode)
		0: begin
			if (important) sq_vbit = 84; else sq_vbit = 74;
		end
		1: begin
			if (important) sq_vbit = 92; else sq_vbit = 74;
		end
		2: begin
			if (important) sq_vbit = 108; else sq_vbit = 74;
		end
		3: begin
			if (important) sq_vbit = 140; else sq_vbit = 74;
		end
		4: begin
			if (important) sq_vbit = 108; else sq_vbit = 74;
		end
		5: begin
			if (important) sq_vbit = 108; else sq_vbit = 74;
		end
		6: begin
			if (important) sq_vbit = 140; else sq_vbit = 74;
		end
		7: begin
			if (important) sq_vbit = 140; else sq_vbit = 74;
		end
		8: begin
			if (important) sq_vbit = 140; else sq_vbit = 74;
		end
		9: begin
			if (important) sq_vbit = 140; else sq_vbit = 74;
		end
	endcase // opcode
end

endmodule

module vlt_sq_dp (
    input [3:0]       opcode,
    input             important,
    output reg [7:0]  sq_dp_vbit
);

always_comb begin
	sq_dp_vbit = 7; // everything is a constant, apparently
	case (opcode)
		0: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		1: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		2: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		3: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		4: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		5: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		6: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		7: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		8: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
		9: begin
			if (important) sq_dp_vbit = 7; else sq_dp_vbit = 7;
		end
	endcase // opcode
end

endmodule
