#include <stdio.h>                          // Biblioteca padrão do C.
#include "pico/stdlib.h"                    // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "pico/time.h"                      // Biblioteca para gerenciamento de temporizadores e alarmes.

const uint LED_VERDE = 11;                  // Define o pino GPIO 11 para controlar o LED.
const uint LED_VERMELHO = 13;               // Define o pino GPIO 13 para controlar o LED.

bool led_on = false;                        // Variável global para armazenar o estado do LED (não utilizada neste código).
bool led_active = false;                    // Indica se o LED está atualmente aceso (para evitar múltiplas ativações).
absolute_time_t turn_off_time;              // Variável para armazenar o tempo em que o LED deve ser desligado (não utilizada neste código).
static volatile uint32_t last_time = 0;     // Armazena o tempo do último evento (em microssegundos)
int led_state = 0;                          // Armazena o estado atual do LED
uint32_t elapsed_time = 0;                       // Armazena o tempo decorrido em segundos
int tempo = 0;                              // Armazena o tempo programado para desligar o LED
volatile uint8_t estado = 0;                // 0: LED_AZUL, 1: LED_VERDE, 2: LED_AMARELO, 3: LED_VERMELHO


// Função de callback para desligar o LED após o tempo programado.
bool repeating_timer_callback(struct repeating_timer *t) {

    tempo++;
    printf("Tempo decorrido: %d segundos\n", tempo);

    if (tempo >= 3) {
        tempo = 0;
        estado = (estado + 1) % 3;

        gpio_put(LED_VERDE, estado == 1 || estado == 2);
        gpio_put(LED_VERMELHO, estado == 1 || estado == 0);

        switch (estado) {
            case 0:
                printf("LED Vermelho ativo\n");
                break;
            case 1:
                printf("LED Amarelo ativo\n");
                break;
            case 2:
                printf("LED Verde ativo\n");
                printf("Reiniciando ciclo do semáforo\n");
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
