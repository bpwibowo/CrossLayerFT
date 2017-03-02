module ecc_checker_5(data_in, checker_in, parity_in);

input      [ 10:0] data_in;
output reg [  3:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[10] ^ data_in[9] ^ data_in[7] ^ data_in[6] ^ data_in[4] ^ data_in[2] ^ data_in[0] ;
    checker_in[1] = data_in[10] ^ data_in[8] ^ data_in[7] ^ data_in[5] ^ data_in[4] ^ data_in[1] ^ data_in[0] ;
    checker_in[2] = data_in[9] ^ data_in[8] ^ data_in[7] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    checker_in[3] = data_in[6] ^ data_in[5] ^ data_in[4] ^ data_in[3] ^ data_in[2] ^ data_in[1] ^ data_in[0] ;
    parity_in = data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[10] ^ checker_in[0] ^ checker_in[1] ^ checker_in[2] ^ checker_in[3] ;

end // always(*)
endmodule


module data_mem_5(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [ 10:0] data_in;
output reg [ 10:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_5(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  3:0] checker_in;
input              parity_in;
output reg [  3:0] checker_out;
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


module ecc_corrector_5(data_out, checker_out, parity_out, correct_data_out, error);

input      [ 10:0] data_out;
input      [  3:0] checker_out;
input              parity_out;
output reg [ 10:0] correct_data_out;
output reg         error;

reg        [  3:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[10] ^ data_out[9] ^ data_out[7] ^ data_out[6] ^ data_out[4] ^ data_out[2] ^ data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[10] ^ data_out[8] ^ data_out[7] ^ data_out[5] ^ data_out[4] ^ data_out[1] ^ data_out[0] ^ checker_out[1] ;
    syndrome[2] = data_out[9] ^ data_out[8] ^ data_out[7] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[2] ;
    syndrome[3] = data_out[6] ^ data_out[5] ^ data_out[4] ^ data_out[3] ^ data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[3] ;

    parity = data_out[0] ^ data_out[1] ^ data_out[2] ^ data_out[3] ^ data_out[4] ^ data_out[5] ^ data_out[6] ^ data_out[7] ^ data_out[8] ^ data_out[9] ^ data_out[10] ^ checker_out[0] ^ checker_out[1] ^ checker_out[2] ^ checker_out[3] ^ parity_out ;

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
            3: correct_data_out[10] = ~data_out[10];
            5: correct_data_out[9] = ~data_out[9];
            6: correct_data_out[8] = ~data_out[8];
            7: correct_data_out[7] = ~data_out[7];
            9: correct_data_out[6] = ~data_out[6];
            10: correct_data_out[5] = ~data_out[5];
            11: correct_data_out[4] = ~data_out[4];
            12: correct_data_out[3] = ~data_out[3];
            13: correct_data_out[2] = ~data_out[2];
            14: correct_data_out[1] = ~data_out[1];
            15: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_5(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [ 10:0] data_in;
output     [ 10:0] correct_data_out;
output             error;

wire       [ 10:0] data_out;
wire       [  3:0] checker_in;
wire       [  3:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_5 u_checker(data_in, checker_in, parity_in);
data_mem_5 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_5 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_5 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


