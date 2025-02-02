# Semaforo_Temporizador
Repositório criado para a tarefa de Microcontroladores - Clock e Temporizadores U4C5

Grupo 2 - Subgrupo 6

Membros:

*Werliarlinson de Lima Sá Teles* - tic370101508

# Instruções de compilação

Neste repositório tem as duas atividades atribuidas em duas pastas separadas,
A pasta Semaforo_SB refere-se a atividade do temporizador periódico.
Enquanto a pasta Semaforo_CB está a atividade do temporizador de um disparo (One Shot).

Para compilar o código, são necessárias as seguintes extensões:

*Wokwi Simulator*

*Raspberry Pi Pico*

*Cmake*

Basta copiar os arquivos da pasta e movê-las para a parte de fora que está pronta para uso.

Após instalá-las basta buildar o projeto pelo CMake. A partir daí, abra o arquivo 
diagram.json e clique no botão verde para iniciar a simulação.

Na simulação, do Semaforo_SB os leds vão acionar automaticamente, seguindo a programação

Aciona o led vermelho, conta cada segundo quando completar 3s apaga,
Então acende o amarelo, no caso ativar led vermelho e verde simultaneamente espera os 3s apaga
E por fim aciona o verde espera os 3s então apaga, então reinicia o processo.

Na simulação do Semaforo_CB o usuário pode clicar no botão disposto na simulação
a fim de acender as luzes conectadas à placa.

Ao acionar o botão preto fará acender todos os leds (resultando em cor branca) e iniciar a sequência de apagá-los
A cada 3s irá apagar seguindo a seguinte ordem: 

Verde será apagado - resultando na cor purpura;
Azul será apagado - restando o vermelho;
E quando passar os 3s apagá-lo, esperando um novo comando do usuário.

O botão não acionará enquanto os leds estiverem ativos.

# Link Wokwi

Semaforo sem Botão:
https://wokwi.com/projects/421808598872155137

Semaforo com Botão:
https://wokwi.com/projects/421808187254658049