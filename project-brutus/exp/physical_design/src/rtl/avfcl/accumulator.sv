`define ACC_TOTAL_WIDTH 25 // assume 32-bit is enough, minus 7 bits LSB dropped
`define VBITS_TOTAL_WIDTH 16 // 8 bits vbits * up to 256 insts
`define DURATION_WIDTH (14-4) // drop 4 LSBs

module avfcl_acc_iq (
    input             clk,
    input             reset,
    input [3:0]       valid,
    input [3:0]       fast_or_slow,
    input [`DURATION_WIDTH-1:0] 	  duration_fast,
    input [`DURATION_WIDTH-1:0]      duration_slow,
    input [3:0][7:0]  vlt_values,
    input [7:0]       counter256,

    output [`ACC_TOTAL_WIDTH-1:0]     iq_acc
);

logic [`ACC_TOTAL_WIDTH-1:0] acc;
logic [`VBITS_TOTAL_WIDTH-1:0] vbits_fast, vbits_fast_snapshot;
logic [`VBITS_TOTAL_WIDTH-1:0] vbits_slow, vbits_slow_snapshot;
logic snapshot_ready;

assign iq_acc = acc;

always_ff @(posedge clk) begin : proc_iq_acc
	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_fast_total;
	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_slow_total;
	bit [31:0] i;

	if(reset) begin
		acc <= 0;
		vbits_fast <= 0;
		vbits_slow <= 0;
		vbits_fast_snapshot <= 0;
		vbits_slow_snapshot <= 0;
		snapshot_ready <= 0;
	end else begin
		vbits_fast_total = 0;
		vbits_slow_total = 0;

		for (i=0; i<4; i=i+1) begin
			if (valid[i]) begin
				if (fast_or_slow[i]) begin
					vbits_fast_total = vbits_fast_total + vlt_values[i];
				end
				else begin
					vbits_slow_total = vbits_slow_total + vlt_values[i];
				end
			end
		end

		if (counter256 == 0) begin
			vbits_fast <= 0;
			vbits_slow <= 0;
			vbits_fast_snapshot <= vbits_fast + vbits_fast_total;
			vbits_slow_snapshot <= vbits_slow + vbits_slow_total;
			snapshot_ready <= 1;
		end
		else begin
			vbits_fast <= vbits_fast + vbits_fast_total;
			vbits_slow <= vbits_slow + vbits_slow_total;
			snapshot_ready <= 0;
		end

		if (snapshot_ready) begin
			 acc <= acc + (vbits_fast_snapshot)*duration_fast +
			 	(vbits_slow_snapshot)*duration_slow;
		end
	end
end

endmodule // avfcl_acc_iq

module avfcl_acc_rob (
    input             clk,
    input             reset,
    input [3:0]       valid,
    input [3:0]       fast_or_slow,
    input [`DURATION_WIDTH-1:0]      duration_dp,
    input [`DURATION_WIDTH-1:0] 	  duration_fast,
    input [`DURATION_WIDTH-1:0]      duration_slow,
    input [3:0][7:0]  vlt_dp_values,
    input [3:0][7:0]  vlt_wb_values,
    input [7:0]       counter256,

    output [`ACC_TOTAL_WIDTH-1:0]     rob_acc
);

logic [`ACC_TOTAL_WIDTH-1:0] acc;
logic [`VBITS_TOTAL_WIDTH-1:0] vbits_fast, vbits_fast_snapshot;
logic [`VBITS_TOTAL_WIDTH-1:0] vbits_slow, vbits_slow_snapshot;
logic [`VBITS_TOTAL_WIDTH-1:0] vbits, vbits_snapshot;
logic snapshot_ready;

assign rob_acc = acc;

always_ff @(posedge clk) begin : proc_rob_acc
	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_fast_total;
	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_slow_total;
	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_total;
	bit [31:0] i;

	if(reset) begin
		acc <= 0;
		vbits_fast <= 0;
		vbits_slow <= 0;
		vbits <= 0;
		vbits_fast_snapshot <= 0;
		vbits_slow_snapshot <= 0;
		vbits_snapshot <= 0;
		snapshot_ready <= 0;
	end else begin
		vbits_fast_total = 0;
		vbits_slow_total = 0;
		vbits_total = 0;

		for (i=0; i<4; i=i+1) begin
			if (valid[i]) begin
				vbits_total = vbits_total + vlt_dp_values[i];
				if (fast_or_slow[i]) begin
					vbits_fast_total = vbits_fast_total + vlt_wb_values[i];
				end
				else begin
					vbits_slow_total = vbits_slow_total + vlt_wb_values[i];
				end
			end
		end

		if (counter256 == 0) begin
			vbits_fast <= 0;
			vbits_slow <= 0;
			vbits <= 0;
			vbits_fast_snapshot <= vbits_fast + vbits_fast_total;
			vbits_slow_snapshot <= vbits_slow + vbits_slow_total;
			vbits_snapshot <= vbits + vbits_total;
			snapshot_ready <= 1;
		end
		else begin
			vbits_fast <= vbits_fast + vbits_fast_total;
			vbits_slow <= vbits_slow + vbits_slow_total;
			vbits <= vbits + vbits_total;
			snapshot_ready <= 0;
		end

		if (snapshot_ready) begin
			 acc <= acc + (vbits_fast_snapshot)*duration_fast +
			 	(vbits_slow_snapshot)*duration_slow +
			 	(vbits_snapshot)*duration_dp;
		end
	end
end

endmodule // avfcl_acc_rob



module avfcl_acc_lq (
    input             clk,
    input             reset,
    input [3:0]       valid,
    input [`DURATION_WIDTH-1:0]      duration_dp,
    input [`DURATION_WIDTH-1:0]      duration_wb,
    input [7:0]       vlt_dp_values,
    input [7:0]       vlt_wb_values,

    output [`ACC_TOTAL_WIDTH-1:0]     lq_acc
);

logic [`ACC_TOTAL_WIDTH-1:0] acc;

assign lq_acc = acc;

always_ff @(posedge clk) begin : proc_lq_acc
	logic [`ACC_TOTAL_WIDTH-1:0] acc_total;
	bit [31:0] i;

	if(reset) begin
		acc <= 0;
	end else begin
		acc_total = 0;

		for (i=0; i<4; i=i+1) begin
			if (valid[i]) begin
				acc_total = acc_total +
					(duration_dp * vlt_dp_values[i]);
				acc_total = acc_total +
					(duration_wb * vlt_wb_values[i]);
			end
		end

		acc <= lq_acc + acc_total;
	end
end

endmodule // avfcl_acc_lq


module avfcl_acc_sq (
    input             clk,
    input             reset,
    input [3:0]       valid,
    input [`DURATION_WIDTH-1:0]      duration_dp,
    input [`DURATION_WIDTH-1:0]      duration_wb,
    input [7:0]       vlt_dp_values,
    input [7:0]       vlt_wb_values,

    output [`ACC_TOTAL_WIDTH-1:0]     sq_acc
);

logic [`ACC_TOTAL_WIDTH-1:0] acc;

assign sq_acc = acc;

always_ff @(posedge clk) begin : proc_sq_acc
	logic [`ACC_TOTAL_WIDTH-1:0] acc_total;
	bit [31:0] i;

	if(reset) begin
		acc <= 0;
	end else begin
		acc_total = 0;

		for (i=0; i<4; i=i+1) begin
			if (valid[i]) begin
				acc_total = acc_total +
					(duration_dp * vlt_dp_values[i]);
				acc_total = acc_total +
					(duration_wb * vlt_wb_values[i]);
			end
		end

		acc <= sq_acc + acc_total;
	end
end

endmodule // avfcl_acc_sq


module avfcl_acc_ib (
    input             clk,
    input             reset,
    input [3:0]       valid,
    input [`DURATION_WIDTH-1:0]      duration,
    input [3:0][7:0]  vlt_values,
    input [7:0]       counter256,

    output [`ACC_TOTAL_WIDTH-1:0]     ib_acc
);

logic [`ACC_TOTAL_WIDTH-1:0] acc;
logic [`VBITS_TOTAL_WIDTH-1:0] vbits;

assign ib_acc = acc;

always_ff @(posedge clk) begin : proc_ib_acc
	bit [31:0] i;
	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_total;

	if(reset) begin
		acc <= 0;
		vbits <= 0;
	end else begin
		for (i=0; i<4; i=i+1) begin
			if (valid[i]) begin
				vbits_total = vbits_total + vlt_values[i];
			end
		end

		if (counter256 == 0) begin
			acc <= acc + (vbits_total+vbits)*duration;
			vbits <= 0;
		end
		else begin
			vbits <= vbits + vbits_total;
		end
	end
end

endmodule // avfcl_acc_ib

// module avfcl_acc_ib (
//     input             clk,
//     input             reset,
//     input [3:0]       valid,
//     input [15:0]      duration,
//     input [3:0][7:0]  vlt_values,
//     input [7:0]       counter256,

//     output [`ACC_TOTAL_WIDTH-1:0]     ib_acc
// );

// logic [`ACC_TOTAL_WIDTH-1:0] acc;
// logic [`VBITS_TOTAL_WIDTH-1:0] vbits, vbits_snapshot;
// logic snapshot_ready;

// assign ib_acc = acc;

// always_ff @(posedge clk) begin : proc_ib_acc
// 	bit [31:0] i;
// 	logic [`VBITS_TOTAL_WIDTH-1:0] vbits_total;

// 	if(reset) begin
// 		acc <= 0;
// 		vbits <= 0;
// 		vbits_snapshot <= 0;
// 		snapshot_ready <= 0;
// 	end else begin
// 		vbits_total = 0;

// 		for (i=0; i<4; i=i+1) begin
// 			if (valid[i]) begin
// 				vbits_total = vbits_total + vlt_values[i];
// 			end
// 		end

// 		if (counter256 == 0) begin
// 			vbits <= 0;
// 			vbits_snapshot <= vbits + vbits_total;
// 			snapshot_ready <= 1;
// 		end
// 		else begin
// 			vbits <= vbits + vbits_total;
// 			snapshot_ready <= 0;
// 		end

// 		if (snapshot_ready) begin
// 			 acc <= acc + (vbits_snapshot)*duration;
// 		end
// 	end
// end

// endmodule // avfcl_acc_ib


module avfcl_acc_prf (
    input             clk,
    input             reset,
    input             valid,
    input [5:0]       next_rlc,

    output [`ACC_TOTAL_WIDTH-1:0]     prf_acc
);

logic [`ACC_TOTAL_WIDTH-1:0] acc;
logic [5:0] rlc;

assign prf_acc = acc;

always_ff @(posedge clk) begin : proc_prf_acc
	logic [`ACC_TOTAL_WIDTH-1:0] acc_total;

	if(reset) begin
		rlc <= 0;
		acc <= 0;
	end else begin
		acc <= acc + {rlc,6'b0};

		if (valid) begin
			rlc <= next_rlc;
		end
	end
end

endmodule // avfcl_acc_prf
