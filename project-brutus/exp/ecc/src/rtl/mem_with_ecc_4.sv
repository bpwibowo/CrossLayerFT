module ecc_checker_4(data_in, checker_in, parity_in);

input      [  3:0] data_in;
output reg [  2:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[3] ^ data_in[2] ^ data_in[0] ;
    checker_in[1] = data_in[3] ^ data_in[1] ^ data_in[0] ;
    checker_in[2] = data_in[2] ^ data_in[1] ^ data_in[0] ;
    parity_in = data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ checker_in[0] ^ checker_in[1] ^ checker_in[2] ;

end // always(*)
endmodule


module data_mem_4(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [  3:0] data_in;
output reg [  3:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_4(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  2:0] checker_in;
input              parity_in;
output reg [  2:0] checker_out;
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


module ecc_corrector_4(data_out, checker_out, parity_out, correct_data_out, error);

input      [  3:0] data_out;
input      [  2:0] checker_out;
input              parity_out;
output reg [  3:0] correct_data_out;
output reg         error;

reg        [  2:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[3] ^ data_out[2] ^ data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[3] ^ data_out[1] ^ data_out[0] ^ checker_out[1] ;
    syndrome[2] = data_out[2] ^ data_out[1] ^ data_out[0] ^ checker_out[2] ;

    parity = data_out[0] ^ data_out[1] ^ data_out[2] ^ data_out[3] ^ checker_out[0] ^ checker_out[1] ^ checker_out[2] ^ parity_out ;

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
            3: correct_data_out[3] = ~data_out[3];
            5: correct_data_out[2] = ~data_out[2];
            6: correct_data_out[1] = ~data_out[1];
            7: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_4(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [  3:0] data_in;
output     [  3:0] correct_data_out;
output             error;

wire       [  3:0] data_out;
wire       [  2:0] checker_in;
wire       [  2:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_4 u_checker(data_in, checker_in, parity_in);
data_mem_4 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_4 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_4 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


