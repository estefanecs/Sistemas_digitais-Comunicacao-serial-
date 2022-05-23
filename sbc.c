#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	//Usado para UART
#include <fcntl.h>	//Usado para UART
#include <termios.h>	//Usado para UART

void uart_tx(char* tx_string, int uart_filestream); //Funcao para envio de dados
void uart_rx(int uart_filestream, char* dado); //Funcao para recebimento de dados

int main(){
	int sensor=0; //Armazena a opcao do sensor selecionado pelo usuÃ¡rio
	int comando=0; //Armazena a opcao de comando selecionado pelo usuÃ¡rio
 	unsigned char comandoResposta[9]; //Armazena o comando de resposta lido pelo rx

	char situacao[]="0x03"; //codigo da situacao aual do sensor.
	char temperatura[]="0x04"; //codigo da medida de temperatura.
	char umidade[]="0x05";//codigo da medida de umidade.	
    
//-----------------------------------------------------------------------------------------------------------------------------------
//Configuracao da UART
   
       //Abertura do arquivo da UART
	int uart_filestream = -1;
	uart_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);	//Abre em modo escrita/leitura bloqueado
	if (uart_filestream == -1){
		printf("\nErro: nÃ£o Ã© possÃ­vel abrir o arquivo da UART.\n");
	}
	//Struct para configuracao dos parametros de comunicacao
	struct termios options;
	tcgetattr(uart_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //Seta Baud-Rate para 9600, tamanho de 8 bits e sem paridade
	options.c_iflag = IGNPAR; //Ignora caracteres com erros de paridade
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart_filestream, TCIFLUSH); //Libera entrada pendente. Esvazia a saida nao transmitida.
	tcsetattr(uart_filestream, TCSANOW, &options);
	
//----------------------------------------------------------------------------------------------------------------------------------
//Solicitacao de dados para criar a requisicao
	while(repeticaoMenu==0){
	  
		//Solicita ao usuario o sensor que deseja obter informacoes
		printf("\nSelecione o sensor que deseja receber informacoes");
		printf("\n1 - DHT11\n");
		scanf("%i", &sensor);
		while(sensor!=1){ //Solicita novamente ate que a opcao seja valida.
			printf("\nOpcao de sensor invalida. Selecione o sensor corretamente");
			printf("\n1 - DHT11\n");
			scanf("%i", &sensor);
		}
		
		//Solicita ao usuario o tipo de informacao que deseja receber do sensor
		printf("\nSelecione o tipo de informacao que deseja receber");
		printf("\n1-Situacao atual do sensor.\n2-Medida de temperatura.\n3-Medida de umidade.\n");
		scanf("%i", &comando);
		while(comando<1 || comando>3){ //Solicita novamente ate que a opcao seja valida.
			printf("\nInformacao invalida, selecione uma disponivel:");
			printf("\n1-Situacao atual do sensor.\n2-Medida de temperatura.\n3-Medida de umidade.\n");
			scanf("%i", &comando);
		}
//----------------------------------------------------------------------------------------------------------------------------------	
//ENVIO DO CODIGO DA REQUISICAO
		switch (comando){
			case 1:
		    		//Envia o codigo da requisicao de situacao do sensor
		    		uart_tx(situacao,uart_filestream);
		  	break;
		  	case 2:
		  		//Envia o codigo da requisicao de medida de temperatura
		  		uart_tx(temperatura,uart_filestream);
		  	break;
		  	case 3:
		  		//Envia o codigo da requisicao de medida de umidade
		  		uart_tx(umidade,uart_filestream);	
		  	break;
		   	 default:
			    	printf("\n\nInformacao invalida, selecione uma disponivel:");
				printf("\n1-Situacao atual do sensor.\n2-Medida de temperatura.\n3-Medida de umidade.\n");
				scanf("%i", &comando);
		}
//----------------------------------------------------------------------------------------------------------------------------------		
//ENVIO DO ENDERECO DA REQUISICAO
		switch (sensor){
		    	case 1:
		    		//Envia o codigo de endereco do DHT11
		   		uart_tx("0x01",uart_filestream); 
		  	break;
		    	default:
			    	printf("\n\n Opcao de sensor invalida. Selecione o sensor corretamente");
				printf("\n1 - DHT11\n");
				scanf("%i", &sensor);
		}
		printf("Requisicao sendo enviada. Aguarde uns instantes");
		sleep(3);
//----------------------------------------------------------------------------------------------------------------------------------
//Leitura do byte de codigo de resposta	
		char dado[9];
		int rx_length;
		if (uart_filestream != -1){
			rx_length = read(uart_filestream, (void*)comandoResposta, 8); //Filestream, buffer para armazenar, numero maximo de bytes lidos
			if (rx_length < 0){
				printf("Ocorreu um erro na leitura de dados");
			}
			else if (rx_length == 0){
				printf("Nenhum dado lido");
			}
			else{
				//Byte recebido
				comandoResposta[rx_length] = '\0';
				if(strcmp(comandoResposta, "0x1F")==0){
			     	printf("\nO sensor esta com problema");
			  	}
			  	else if(strcmp(comandoResposta, "0x00")==0){
			  		printf("\nO sensor esta funcionando corretamente");
				}
			  	else if(strcmp(comandoResposta, "0x01")==0){
			  		printf("\nMedida de umidade");
					uart_rx(uart_filestream,dado); //Recebe o valor da umidade
					printf("%s ",dado); //Imprime valor de umidade
			  	}
			 	else if(strcmp(comandoResposta, "0x02")==0){
			 		printf("\nMedida de temperatura");
					uart_rx(uart_filestream,dado); //Recebe o valor da temperatura
					printf("%s ",dado); //Imprime valor da temperatura
			 	}
			}
		}
		else{
			printf("\nFalha na abertura do arquivo");
		}

		//Opcao de repetir o programa
		printf("\nDigite 0 se deseja continuar no programa\n");
		scanf("%i", &repeticaoMenu);
	}
		//Fecha a conexao com a UART
		close(uart_filestream); 
	return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------
//Funcao que envia os dados na UART
void uart_tx(char* tx_string, int uart_filestream){
	if (uart_filestream != -1){//Se abriu o arquivo da UART
		write(uart_filestream, tx_string, strlen(tx_string)); //Filestream,mensagem enviada,tamanho da mensagem
	}	
	else{
		printf("\nFalha na abertura do arquivo");
	}
}

//----------------------------------------------------------------------------------------------------------------------------------
//Funcao que recebe os dados de temperatura ou umidade
void uart_rx(int uart_filestream, char* dado){
	int rx_length; //armazena o tamanho do dado lido
	unsigned char dadoInteiro[9]; //armazena a parte inteira do dado
	unsigned char dadoFracao[9]; //armazena a parte fracionaria do dado
	//leitura do byte de dado 1
	if (uart_filestream != -1){
		rx_length = read(uart_filestream, (void*)dadoInteiro, 8); //Filestream, buffer para armazenar, nÃºmero maximo de bytes lidos
		if (rx_length < 0){
			printf("Ocorreu um erro na leitura de dados");
		}
		else if (rx_length == 0){
			printf("Nenhum dado lido");
		}
		else{//Se tem dado lido
			//Byte recebido
			dadoInteiro[rx_length] = '\0';
		}
		sleep(1);
	
		//leitura do byte de dado 2
		rx_length = read(uart_filestream, (void*)dadoFracao, 8); //Filestream, buffer para armazenar, numero maximo de bytes lidos
		if (rx_length < 0){
			printf("Ocorreu um erro na leitura de dados");
		}
		else if (rx_length == 0){
			printf("Nenhum dado lido");
		}
		else{
			//Byte recebido
			dadoFracao[rx_length] = '\0';
		}
	}
	else{
		printf("\nFalha na abertura do arquivo");
	}
	//Concatena os valores da parte inteira e fracionaria da medicao
	strcat(dadoInteiro, dadoFracao);
	strcpy(dado, dadoInteiro);
}
