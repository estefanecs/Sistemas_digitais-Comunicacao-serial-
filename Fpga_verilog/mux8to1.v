// modulo mux8 para 1
module mux8to1(d0,d1,d2,d3,d4,d5,d6,d7,S,Y);
	input wire [7:0] d0,d1,d2,d3,d4,d5,d6,d7;
	input wire [2:0] S;
	output reg [7:0] Y;
	
	always@(S) begin
		case(S)
			3'b000: Y= d0;
			3'b001: Y= d1;
			3'b010: Y= d2;
			3'b011: Y= d3;
			3'b100: Y= d4;
			3'b101: Y= d5;
			3'b110: Y= d6;
			3'b111: Y= d7;
		endcase
	end
endmodule