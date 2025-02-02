#include <stdio.h>                          // Biblioteca padrão do C.
#include "pico/stdlib.h"                    // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "pico/time.h"                      // Biblioteca para gerenciamento de temporizadores e alarmes.

const uint LED_VERDE = 11;                  // Define o pino GPIO 11 para controlar o LED.
const uint LED_VERMELHO = 13;               // Define o pino GPIO 13 para controlar o LED.

int tempo = 0;                              // Armazena o tempo programado para desligar o LED
volatile uint8_t estado = 0;                // Variável para verificar o estado atual dos leds e ajudar no controle deles 
                                            // 0: LED_VERMELHO, 1: LED_VERDE + LED_VERMELHO, 2: LED_VERDE

// Função de callback para desligar o LED após o tempo programado.
bool repeating_timer_callback(struct repeating_timer *t) {
    
    //Verificação do tempo transcorrido em segundos
    tempo++;
    printf("Tempo decorrido: %d segundos\n", tempo);

    // Irá mudar o estado do semáforo a cada 3 segundos
    if (tempo >= 3) {
        
        tempo = 0;
        estado = (estado + 1) % 3;

        // Atualiza o estado dos LEDs de acordo com o estado do semáforo de uma maneira mais eficiente
        gpio_put(LED_VERDE, estado == 1 || estado == 2); 
        gpio_put(LED_VERMELHO, estado == 1 || estado == 0);

        switch (estado) {
            case 0:
                printf("Reiniciando ciclo do semáforo\n");
                printf("LED Vermelho ativo\n");
                break;
            case 1:
                printf("LED Amarelo ativo\n");
                break;
            case 2:
                printf("LED Verde ativo\n");
                break;
        }
    }

    // Retorna true para manter o temporizador repetindo. Se retornar false, o temporizador para.
    return 1;
}

int main() {
    
    // Inicializa a comunicação serial para permitir o uso de printf.
    stdio_init_all();

    // Configura os pinos para o LED RGB (11, 12 e 13) como saída digital.
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    // Já para inicializar o semáforo com o LED vermelho ativo.
    gpio_put(LED_VERDE, false);
    gpio_put(LED_VERMELHO, true);
    printf("LED Vermelho ativo\n");

    struct repeating_timer timer;                                           // Estrutura para armazenar o temporizador repetitivo
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);   // Adiciona um temporizador repetitivo de 1 segundo
    
    // Loop principal do programa.
    while (true) {
        // Introduz uma pequena pausa de 10 s para reduzir o uso da CPU.
        sleep_ms(10000);
    }

    // Retorno de 0, que nunca será alcançado devido ao loop infinito.
    // Isso é apenas uma boa prática em programas com um ponto de entrada main().
    return 0;
}
