module ecc_checker_3(data_in, checker_in, parity_in);

input      [  0:0] data_in;
output reg [  1:0] checker_in;
output reg         parity_in;

always @(*) begin
    checker_in[0] = data_in[0] ;
    checker_in[1] = data_in[0] ;
    parity_in = data_in[0] ^ checker_in[0] ^ checker_in[1] ;

end // always(*)
endmodule


module data_mem_3(clock, reset, we, data_in, data_out);

input              clock;
input              reset;
input              we;
input      [  0:0] data_in;
output reg [  0:0] data_out;

always @(posedge clock) begin
    if (reset) begin
        data_out <= 0;
    end
    else if (we) begin
        data_out <= data_in;
    end
end
endmodule


module ecc_mem_3(clock, reset, we, checker_in, parity_in, checker_out, parity_out);

input              clock;
input              reset;
input              we;
input      [  1:0] checker_in;
input              parity_in;
output reg [  1:0] checker_out;
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


module ecc_corrector_3(data_out, checker_out, parity_out, correct_data_out, error);

input      [  0:0] data_out;
input      [  1:0] checker_out;
input              parity_out;
output reg [  0:0] correct_data_out;
output reg         error;

reg        [  1:0] syndrome;
reg                parity;

always @(*) begin
    syndrome[0] = data_out[0] ^ checker_out[0] ;
    syndrome[1] = data_out[0] ^ checker_out[1] ;

    parity = data_out[0] ^ checker_out[0] ^ checker_out[1] ^ parity_out ;

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
            3: correct_data_out[0] = ~data_out[0];
        endcase
    end
end // always(*)
endmodule


module mem_with_ecc_3(clock, reset, we, data_in, correct_data_out, error);

input              clock;
input              reset;
input              we;
input      [  0:0] data_in;
output     [  0:0] correct_data_out;
output             error;

wire       [  0:0] data_out;
wire       [  1:0] checker_in;
wire       [  1:0] checker_out;
wire               parity_in;
wire               parity_out;

ecc_checker_3 u_checker(data_in, checker_in, parity_in);
data_mem_3 u_datamem(clock, reset, we, data_in, data_out);
ecc_mem_3 u_eccmem(clock, reset, we, checker_in, parity_in, checker_out, parity_out);
ecc_corrector_3 u_corrector(data_out, checker_out, parity_out, correct_data_out, error);

endmodule


