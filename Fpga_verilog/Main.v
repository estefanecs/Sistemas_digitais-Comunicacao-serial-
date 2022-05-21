// Modulo principal
// Circuito modular de controle e comunicação de sensores

module Main #(
		// Parametro para os módulos UartRx e UartTx
		// A comunicação serial não é sincronzida ao clock interno da placa
		// Para um comunicação entre dois dispositivos eles devem estar na mesma frequencia de baud rate
		// Os módulos realizam a função de divir o clock internamente, é nescessario apenas informar o CLKS_PER_BIT
		// que é calculado: CLKS_PER_BIT = (Frequency of i_Clock)/(Frequency of UART)
		// Para a configuração utilizada 50 MHz Clock, 115200 baud UART
		//(50000000)/(115200) = 457
			parameter CLKS_PER_BIT = 434
	)(
			input wire     Clock,		// clock de entrada da placa
			input wire  		i_Rx_Serial,  // entrada de dados
			output      o_Tx_Serial,	// Saida serial da menssagem
			output reg [2:0] rgb
		);
		
		// Saidas do módulo uartRx
		wire rx_done;
		wire [7:0] rx_byte;
		
		// Entradas do módulo uartTx
		 wire tx_en;
		 wire [7:0] tx_byte;
		// Saidas do módulo uartRx
		wire tx_active;
		wire tx_done;
		
		// registradores
		reg rx_on;
		reg tx_on, tx_Odone;
		
		//variavel da maquina de estados
		reg [2:0] SM_control;
		
		// instanciando módulo uartRx
		uart_rx #(
		 .CLKS_PER_BIT(CLKS_PER_BIT) 
		 ) receptor (
		.i_Clock(Clock),
		.i_Rx_Serial(i_Rx_Serial),
		.o_Rx_DV(rx_done),
		.o_Rx_Byte(rx_byte)
	);
	
	// instanciando módulo uartTx
	uart_tx #(
		.CLKS_PER_BIT(CLKS_PER_BIT)
		) transmissor( 
		.i_Clock(Clock),
		.i_Tx_DV(tx_en),
		.i_Tx_Byte(tx_byte),
		.o_Tx_Active(tx_active),
		.o_Tx_Serial(o_Tx_Serial),
		.o_Tx_Done(tx_done)
	);
	
	initial begin
		rx_on <= rx_done;
		tx_on <= tx_active;
		tx_Odone <= tx_done;
		tx_byte = 8'b00001111;
		rgb =3'b000;
		tx_en <=1;
	end
		
	always@(posedge Clock) begin
		if(rx_byte == 8'b00001111)
			rgb = 3'b100;
	end
	
endmodule
