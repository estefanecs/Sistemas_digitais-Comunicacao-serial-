# Sistemas_digitais-Comunicacao-serial 
 Repositorio do problema 2 do MI sistemas digitais
<<<<<<< HEAD
<h1>Começando pessoal</h1>
<img src ="imagens/imagem3.png">

<img src ="imagens/requisicao.png">

<img src ="imagens/resposta.png">

<img src ="imagens/imagem1.png">

<img src ="imagens/imagem6.png">

<img src ="imagens/imagem5.png">
=======
 
<h1>Apresentação do sistema</h1>
O sistema proposto realiza a implementação de um protocolo de comunicação entre a Raspberry e a FPGA. 

O sistema realiza a medição de temperatura e umidade através do sensor DHT11, no qual o SBC envia requisições sobre qual informação deseja receber do sensor: situação, medida de temperatura e medida de umidade. Essa requisição é interpretada pelo sistema embarcado em FPGA, que realiza a obtenção dos dados e os envia como resposta para o SBC.

O sistema é modular, então aceita outros tipos de sensores, mas é necessário criar uma interface para o sensor que se deseja inserir. Essa interface deve respeitar as seguintes condições: receber requisições com comando de 8 bits e ser capaz de apresentar saídas agrupadas em bytes.
 
Foram utilizados para o desenvolvimento do sistema a linguagem de programação C, Verilog, Quartus Prime 21.1, o sensor DHT11, Raspberry PI 0 e Kit de desenvolvimento Mercury IV com o chip FPGA Cyclone IV 30K EP4CE30F23.

<h1>Diagrama de blocos</h1>
A imagem a seguir descreve em alto nível uma visão dos componentes do sistema desenvolvido.
<img src ="imagens/imagem3.png">

<h1>Single Board Computer (SBC)</h1>
Para a implementação do SBC, foi necessário realizar a configuração da UART. Utilizou-se as bibliotecas unistd, fcntl e termios, seguindo os passos:
<ul>
<li>Abertura do arquivo da uart: utiliza a função open(“/dev/serial0”, tipos de abertura). Os tipos de abertura foram O_RDWR, O_NOCTTY, O_NDELAY. Que abre no modo de leitura/gravação sem bloqueio;</li>
<li>E a configuração das flags, como valor de baud-rate, habilitação de paridade, tipo de paridade;</li>
 </ul>

O SBC solicita ao usuário que selecione o sensor que deseja receber os dados e em seguida, o tipo de informação. Nesse caso, o sensor é o DHT11 e as informações podem ser a situação do sensor, medida de temperatura e umidade. A opções aparecem em forma de menu no terminal e o usuário deve digitar o número correspondente.
De acordo com as opções selecionadas, a requisição é enviada pela UART da Raspberry para a UART da FPGA. A requisição é composta por 2 bytes, 1 byte que indica o código da requisição e 1 byte que indica o endereço do sensor. A imagem abaixo indica o formato das requisições.
<img src ="imagens/requisicao.png">
O envio de dados pela UART é feito utilizando a função write, no qual deve se indica o endereço do arquivo de mapeamento, o dado a ser enviado, e o tamanho do dado:
	<strong>write(enderecoArquivo,dado,tamanhoDado);<strong>

Para a leitura de dados enviados pela FPGA, a função read foi utilizada, que tem como parâmetros o endereço do arquivo da uart, o buffer para armazenar o dado a ser lido e o número máximo de bits a serem lidos. A função retorna o tamanho de bits lidos. 
 <strong>read(enderecoArquivo,buffer,tamanhoMaximo);</syrong>
 
As respostas recebidas podem ser compostas por 1 byte ou 3 bytes. As respostas relacionadas à situação do sensor possuem 1 byte que é composto pelo código de resposta. As respostas sobre a medição de temperatura ou/e umidade possuem 1 byte que indica o código de resposta, 1 byte da parte inteira e 1 byte da parte fracionária da medida. Ao receber a resposta, o SBC analisa o código recebido e exibe na tela a descrição correspondente ao código, e se houver dado de medida dos sensores exibe na tela.
 <img src ="imagens/resposta.png">
>>>>>>> 429df965496eb45b31a2a001c50a2d4718947f29
