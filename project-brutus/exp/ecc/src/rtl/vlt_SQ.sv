module vlt_SQ (
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
    // SWL
    15: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SDR
    18: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SC_0
    21: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SW
    8: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SH
    7: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SCD_0
    23: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SDL
    17: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SB
    6: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 4;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SWR
    16: begin
      if (important_i) begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 5;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
    // SD
    10: begin
      if (important_i) begin
        shift1_o = 7;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
      else begin
        shift1_o = 6;
        shift1_v_o = 1;
        shift2_o = 3;
        shift2_v_o = 1;
      end
    end
  endcase
end

endmodule
