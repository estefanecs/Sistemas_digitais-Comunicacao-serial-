module checksum(
input [39:0] data,
output reg error,
output reg[31:0] out

);

	wire[7:0] check = data[39:32]; // checksum
	wire[7:0] data3 = data[31:24]; // low temp
	wire[7:0] data2 = data[23:16]; // high temp
	wire[7:0] data1 = data[15:8]; // low humidity
	wire[7:0] data0 = data[7:0]; // high humidity
	
	


always @(*) begin

	reg[7:0] aux;
	
	aux = data1 + data0 + data2 + data3;
	
	if(check == aux) begin
		error = 0;
	end
	else begin
		error = 1;
	end
	
	out[31:24] = data3;
	out[23:16] = data2;
	out[15:8] = data1;
	out[7:0] = data0;
end

endmodule
