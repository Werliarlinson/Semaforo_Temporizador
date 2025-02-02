#include <stdio.h>                          // Biblioteca padrão do C.
#include "pico/stdlib.h"                    // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "pico/time.h"                      // Biblioteca para gerenciamento de temporizadores e alarmes.

const uint LED_VERDE = 11;                  // Define o pino GPIO 11 para controlar o LED.
const uint LED_AZUL = 12;                   // Define o pino GPIO 12 para controlar o LED.
const uint LED_VERMELHO = 13;               // Define o pino GPIO 13 para controlar o LED.
const uint BUTTON_A = 5;                    // Define o pino GPIO 5 para ler o estado do botão.

bool led_on = false;                        // Variável global para armazenar o estado do LED (não utilizada neste código).
bool led_active = false;                    // Indica se o LED está atualmente aceso (para evitar múltiplas ativações).
absolute_time_t turn_off_time;              // Variável para armazenar o tempo em que o LED deve ser desligado (não utilizada neste código).
static volatile uint32_t last_time = 0;     // Armazena o tempo do último evento (em microssegundos)
int led_state = 0;                          // Armazena o estado atual do LED
uint32_t elapsed_time = 0;                       // Armazena o tempo decorrido em segundos
int tempo = 0;                              // Armazena o tempo programado para desligar o LED


// Função de callback para desligar o LED após o tempo programado.
int64_t turn_off_callback(alarm_id_t id, void *user_data) {

    // Desliga o LED com base no estado atual.
    switch (led_state)
    {
    case 2:
        gpio_put(LED_VERDE, false);
        led_state--;
        printf("LED Verde desligado\n");
        break;
    case 1:
        gpio_put(LED_AZUL, false);
        led_state--;
        printf("LED Azul desligado\n");
        break;
    case 0:
        gpio_put(LED_VERMELHO, false);
        led_active = false;
        printf("LED Vermelho desligado\n");
        printf("Fim do processo de contagem\n");
        return 0;
    }

    tempo = 0;       // Reseta o tempo decorrido
    // Agenda um novo alarme para desligar o próximo LED após 3 segundo (3000 ms).
    add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}

int main() {
    // Inicializa a comunicação serial para permitir o uso de printf.
    // Isso é útil para depuração, embora não seja usado neste exemplo.
    stdio_init_all();

    // Configura os pinos para o LED RGB (11, 12 e 13) como saída digital.
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);

    // Configura o pino BUTTON_A (5) como entrada digital.
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);

    // Habilita o resistor pull-up interno para o pino do botão.
    // Isso garante que o pino seja lido como alto (3,3 V) quando o botão não está pressionado.
    gpio_pull_up(BUTTON_A);

    // Loop principal do programa que verifica continuamente o estado do botão.
    while (true) {
        
        uint32_t current_time = to_us_since_boot(get_absolute_time());      // Obter o tempo atual em microssegundos

        // Verifica se o botão foi pressionado (nível baixo no pino) e se o LED não está ativo.
        if (gpio_get(BUTTON_A) == 0 && !led_active) {
            
            if(current_time - last_time > 300000) {                             // Evento para ignorar o debounce do botão
       
                last_time = current_time;
                elapsed_time = current_time;

                // Liga os LEDs configurando os pinos para nível alto.
                gpio_put(LED_AZUL, true);
                gpio_put(LED_VERDE, true);
                gpio_put(LED_VERMELHO, true);

                // Define 'led_active' como true para indicar que o LED está aceso.
                led_active = true;
                led_state = 2;
                tempo = 0;
                // Agenda um alarme para desligar o LED após 3 segundos (3000 ms).
                // A função 'turn_off_callback' será chamada após esse tempo.
                add_alarm_in_ms(3000, turn_off_callback, NULL, false);
                printf("Botão pressionado\n");
                printf("Começa o processo de contagem\n");
                //Implementação do temporizador dentro da função pois ele só iria reagir no proximo segundo apenas
                tempo++;
                printf("Tempo decorrido: %d segundos\n", tempo);
            }

        }
        
        if (current_time - elapsed_time >= 1000000 && led_active) {             // Evento para contar o tempo decorrido
            elapsed_time += 1000000;                                            //Atualiza o tempo do último segundo contado
            tempo++;
            printf("Tempo decorrido: %d segundos\n", tempo);
        }
        // Introduz uma pequena pausa de 10 ms para reduzir o uso da CPU.
        // Isso evita que o loop seja executado muito rapidamente e consuma recursos desnecessários.
        sleep_ms(10);
    }

    // Retorno de 0, que nunca será alcançado devido ao loop infinito.
    // Isso é apenas uma boa prática em programas com um ponto de entrada main().
    return 0;
}
