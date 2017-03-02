module vlt_LQ (
    input important_i
  , input [8:0] opcode_i
  , input [9:0] start_ts_i
  , input [9:0] end_ts_i
  , output reg [3:0] shift1_o
  , output reg [3:0] shift2_o
  , output reg shift1_v_o
  , output reg shift2_v_o
  , output reg [9:0] duration_o
  );

always @(*)
begin
  // initialize outputs to zero
  shift1_o = 0;
  shift1_v_o = 0;
  shift2_o = 0;
  shift2_v_o = 0;

  // calculate duration
  duration_o = end_ts_i - start_ts_i;

  case (opcode_i)
    // LD
    9: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LBU
    1: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LW
    4: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LWR
    12: begin
      if (important_i) begin // 160
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LL
    19: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // PREF
    132: begin
      if (important_i) begin // 20
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
      else begin // 20
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // LH
    2: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LWU
    5: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // PREFX
    133: begin
      if (important_i) begin // 20
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
      else begin // 20
        shift1_o = 4;
        shift1_v_o = 1;
        shift2_o = 2;
        shift2_v_o = 1;
      end
    end
    // LDR
    14: begin
      if (important_i) begin // 160
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LHU
    3: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LWL
    11: begin
      if (important_i) begin // 160
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LB
    0: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LDL
    13: begin
      if (important_i) begin // 160
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
    // LLD
    20: begin
      if (important_i) begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin // 96
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
    end
  endcase
end

endmodule
