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
		
		// entradas do módulo uartRx
		input   i_Rx_Serial,  // entrada de dados
		
		// Saidas do módulo uartRx
		output       o_Rx_DV,	// Não sei
		output [7:0] o_Rx_Byte,		// Byte recebido pelo receptor
		
		// Entradas do módulo uartTx
		input       i_Tx_DV,			// Não sei
		input [7:0] i_Tx_Byte, 		// Byte da menssagem a ser enviada
		
		// Saidas do módulo uartRx
		output      o_Tx_Active,	// Sinal indicando que uma transmissão está acontecendo
		output      o_Tx_Serial,	// Saida serial da menssagem
		output      o_Tx_Done		// Sinal indicando que ma tranmissão foi realizada e finalizada
		);
		
	// mensagem de teste
		reg [7:0] mensagem = 8'b00001111;
	// instanciando módulo uartRx
	
	wire rx,tx;
	
	uart_rx #(
		.CLKS_PER_BIT(CLKS_PER_BIT)
		)receptor 
		(
		.i_Clock(Clock),
		.i_Rx_Serial(rx),
		.o_Rx_DV(o_Rx_DV),
		.o_Rx_Byte(o_Rx_Byte)
	);
	
	// instanciando módulo uartTx
	uart_tx #(
		.CLKS_PER_BIT(CLKS_PER_BIT)
		)transmissor 
		(
		.i_Clock(Clock),
		.i_Tx_DV(i_Tx_DV),
		.i_Tx_Byte(i_Tx_Byte),
		.o_Tx_Active(o_Tx_Active),
		.o_Tx_Serial(tx),
		.o_Tx_Done(o_Tx_Done)
	);
	
		
endmodule
