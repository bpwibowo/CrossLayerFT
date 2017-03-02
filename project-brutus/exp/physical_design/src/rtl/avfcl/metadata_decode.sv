`define LC_QUEUE_SIZE 64
`define IMPQ_SIZE 128 // in term of 5-bits word

`define MD_SIZE_0 0
`define MD_SIZE_5 1
`define MD_SIZE_4 2
`define MD_SIZE_10 3
`define MD_SIZE_15 4
`define MD_SIZE_13 5
`define MD_SIZE_19 6

module metadata_decode #(
    /* Parameters */
    parameter FETCH_WIDTH = 2,
    parameter COMMIT_WIDTH = 4
    ) (
    input               clk,
    input               reset,
    input               stall,
    input [1:0][4:0]    fetch2decode_metadata,
    input               fetch2decode_valid,
    input [31:0]        inst_code [0:FETCH_WIDTH-1],
    input [4:0]         retire_cnt,
    input [7:0]         retire_iscontrol,
    input [7:0]         retire_alid,

    output reg [COMMIT_WIDTH-1:0]  importance_flags,
    output reg                     lc_valid,
    output reg [4:0]               lc_value
);

logic [4:0]             md_size [0:FETCH_WIDTH-1];
logic [9:0]             md_vector [0:FETCH_WIDTH-1];
logic [FETCH_WIDTH-1:0] inst_is_control;

logic [7:0]             last_start_bb;
logic [7:0]             last_start_bb_next;

logic [4:0]             imp_queue [0:`IMPQ_SIZE-1];
logic [7:0]             imp_queue_head;
logic [7:0]             imp_queue_tail;
logic [10:0]            imp_queue_size;

logic [4:0]             lc_queue [0:`LC_QUEUE_SIZE-1];
logic [7:0]             lc_queue_alid [0:`LC_QUEUE_SIZE-1];
logic [5:0]             lc_queue_head;
logic [5:0]             lc_queue_tail;

logic                   next_is_start_bb;
logic [10:0]            bb_inst_cnt;
logic [10:0]            bb_inst_cnt_next;
logic [4:0]             bb_inst_cnt_retired;
logic [4:0]             bb_inst_cnt_retired_next;


genvar i;
generate
  for (i=0; i<FETCH_WIDTH; i=i+1) begin : METADATA_DECODER
    mdecode mdec(
        .inst(inst_code[i]),
        .metadata_size(md_size[i]),
        .metadata_vector(md_vector[i]),
        .is_control(inst_is_control[i])
        );
  end
endgenerate

always_ff @(posedge clk) begin : proc_last_start_bb
    if(reset) begin
        last_start_bb <= 0;
    end else begin
        last_start_bb <= last_start_bb_next;
    end
end

always_ff @(posedge clk) begin : proc_next_is_start_bb
    if(reset) begin
        next_is_start_bb <= 1;
    end else if (!stall) begin
        if (inst_is_control[FETCH_WIDTH-2]) begin
            next_is_start_bb <= 1;
        end else begin
            next_is_start_bb <= 0;
        end
    end
end

always_ff @(posedge clk) begin : proc_bb_inst_cnt
    if(reset) begin
        bb_inst_cnt <= 0;
        bb_inst_cnt_retired <= 0;
    end else begin
        bb_inst_cnt <= bb_inst_cnt_next;
        bb_inst_cnt_retired <= bb_inst_cnt_retired_next;
    end
end

always_ff @(posedge clk) begin : proc_imp_lc_queue
    int i, j, offset, new_size, inst_md_size;
    logic [5:0][4:0] new_metadata_vector;
    int retire_control_cnt;

    if(reset) begin
        imp_queue_head <= 0;
        imp_queue_tail <= 0;
        lc_queue_head <= 0;
        lc_queue_tail <= 0;
        imp_queue_size <= 0;
    end else begin
        retire_control_cnt = 0;
        for (i=0; i<COMMIT_WIDTH; i=i+1) begin
            retire_control_cnt = retire_control_cnt + retire_iscontrol[i];
        end

        if (!stall) begin
            new_size = 0;
            offset = 0;
            if (fetch2decode_valid) begin
                new_metadata_vector[0] = fetch2decode_metadata[0];
                new_metadata_vector[1] = fetch2decode_metadata[1];
                new_size = new_size + 2;
            end
            for (j=0; j<FETCH_WIDTH; j=j+1) begin
                inst_md_size = 0;
                if (md_size[j] == `MD_SIZE_10) begin
                    new_metadata_vector[new_size] = md_vector[i][0];
                    new_metadata_vector[new_size+1] = md_vector[i][1];
                    new_size = new_size + 2;
                end
                else if (md_size[j] == `MD_SIZE_5) begin
                    new_metadata_vector[new_size] = md_vector[i][0];
                    new_size = new_size + 1;
                end
            end

            if (next_is_start_bb && new_size >= 5) begin
                // this new_metadata_vector has live vector count
                lc_queue[lc_queue_tail] <= new_metadata_vector[0];
                offset = offset + 1;
                lc_queue_tail <= lc_queue_tail + 1;
            end

            if (new_size-offset > 0 && (imp_queue_size+new_size-offset) < (`IMPQ_SIZE-1)) begin
                imp_queue[imp_queue_tail+0] <= new_metadata_vector[offset+0];
            end
            if (new_size-offset > 1 && (imp_queue_size+new_size-offset) < (`IMPQ_SIZE-2)) begin
                imp_queue[imp_queue_tail+1] <= new_metadata_vector[offset+1];
            end
            if (new_size-offset > 2 && (imp_queue_size+new_size-offset) < (`IMPQ_SIZE-3)) begin
                imp_queue[imp_queue_tail+2] <= new_metadata_vector[offset+2];
            end
            if (new_size-offset > 3 && (imp_queue_size+new_size-offset) < (`IMPQ_SIZE-4)) begin
                imp_queue[imp_queue_tail+3] <= new_metadata_vector[offset+3];
            end
            if (new_size-offset > 4 && (imp_queue_size+new_size-offset) < (`IMPQ_SIZE-5)) begin
                imp_queue[imp_queue_tail+4] <= new_metadata_vector[offset+4];
            end
            if (new_size-offset > 5 && (imp_queue_size+new_size-offset) < (`IMPQ_SIZE-6)) begin
                imp_queue[imp_queue_tail+5] <= new_metadata_vector[offset+5];
            end

            if (next_is_start_bb) begin
                imp_queue_tail <= last_start_bb + bb_inst_cnt;
                imp_queue_size <= imp_queue_size + (imp_queue_tail-(last_start_bb+bb_inst_cnt)) - retire_control_cnt;
            end else begin
                imp_queue_tail <= imp_queue_tail + offset;
                imp_queue_size <= imp_queue_size + new_size - offset - retire_control_cnt;
            end
        end

        // update head pointer logic
        if (imp_queue_size == 0) begin
            // don't update head pointer
        end
        else begin
            imp_queue_head <= imp_queue_head + retire_control_cnt;
        end
        for (i=0; i<retire_cnt; i=i+1) begin
            if (retire_alid+i == lc_queue_alid[lc_queue_head]) begin
                lc_queue_head <= lc_queue_head + 1;
            end
        end
    end
end

always_comb begin
    last_start_bb_next = last_start_bb;
    bb_inst_cnt_next = bb_inst_cnt;

    if (!stall) begin
        if (next_is_start_bb) begin
            bb_inst_cnt_next = 2;
            last_start_bb_next = last_start_bb + bb_inst_cnt;
        end
        else begin
            bb_inst_cnt_next = bb_inst_cnt + 2;
        end
    end
end

always_comb begin
    int retire_control_cnt, head_offset;
    bit[31:0] i;

    lc_value = 0;
    lc_valid = 0;

    retire_control_cnt = 0;
    head_offset = 0;
    bb_inst_cnt_retired_next = bb_inst_cnt_retired;
    for (i=0; i<COMMIT_WIDTH; i=i+1) begin
        importance_flags[i] = imp_queue[imp_queue_head+head_offset][bb_inst_cnt_retired_next];
        if (retire_iscontrol[i]) begin
            bb_inst_cnt_retired_next = 0;
        end
        else begin
            bb_inst_cnt_retired_next = bb_inst_cnt_retired_next + 1;
        end
    end

    for (i=0; i<2; i=i+1) begin
        importance_flags[i] = imp_queue[imp_queue_head+i];
    end
    for (i=0; i<COMMIT_WIDTH; i=i+1) begin
        if (retire_alid+i == lc_queue_alid[lc_queue_head]) begin
            lc_value = lc_queue[lc_queue_head];
            lc_valid = 1;
        end
    end
end

endmodule
