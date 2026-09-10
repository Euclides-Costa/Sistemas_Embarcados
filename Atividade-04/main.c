#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Definição dos pinos GPIO para os LEDs (Bits 3 a 0)
#define GREEN_LED_GPIO  GPIO_NUM_1 
#define YELLOW_LED_GPIO GPIO_NUM_2 
#define RED_LED_GPIO    GPIO_NUM_35 
#define BLUE_LED_GPIO   GPIO_NUM_48 

// Definição dos pinos GPIO para os Botões
#define BUTTON_A_GPIO   GPIO_NUM_4  // Incrementa o contador
#define BUTTON_B_GPIO   GPIO_NUM_9  // Alterna o passo de incremento (+1 ou +2)

// Configuração dos pinos (Entradas e Saídas)
void configure_pins(void)
{
    // Configura os LEDs como SAÍDA
    gpio_reset_pin(GREEN_LED_GPIO);
    gpio_set_direction(GREEN_LED_GPIO, GPIO_MODE_OUTPUT);
    
    gpio_reset_pin(YELLOW_LED_GPIO);
    gpio_set_direction(YELLOW_LED_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(RED_LED_GPIO);
    gpio_set_direction(RED_LED_GPIO, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BLUE_LED_GPIO);
    gpio_set_direction(BLUE_LED_GPIO, GPIO_MODE_OUTPUT);

    // Configura os Botões como ENTRADA com Pull-Up
    gpio_reset_pin(BUTTON_A_GPIO);
    gpio_set_direction(BUTTON_A_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_A_GPIO, GPIO_PULLUP_ONLY);

    gpio_reset_pin(BUTTON_B_GPIO);
    gpio_set_direction(BUTTON_B_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_B_GPIO, GPIO_PULLUP_ONLY);
}

// Atualiza o estado dos 4 LEDs baseado no valor binário passado
void update_leds(uint8_t value)
{
    // Usa operação de bitwise (deslocamento e máscara) para ligar o LED correto
    gpio_set_level(BLUE_LED_GPIO,   (value >> 0) & 1); // Extrai bit 0
    gpio_set_level(RED_LED_GPIO,    (value >> 1) & 1); // Extrai bit 1
    gpio_set_level(YELLOW_LED_GPIO, (value >> 2) & 1); // Extrai bit 2
    gpio_set_level(GREEN_LED_GPIO,  (value >> 3) & 1); // Extrai bit 3
}

void app_main(void)
{
    // Inicializa as portas
    configure_pins();

    uint8_t counter = 0;        // Contador de 4 bits (0 a 15)
    uint8_t increment_step = 1; // Passo inicial (+1)

    // Atualiza os LEDs para o estado inicial (tudo apagado -> 0)
    update_leds(counter);

    // Variáveis para guardar o estado anterior dos botões (tudo -> 1)
    int last_btn_a_state = 1; 
    int last_btn_b_state = 1; 

    // Variáveis para o Debounce via software
    TickType_t last_debounce_time_a = 0;
    TickType_t last_debounce_time_b = 0;
    const TickType_t debounce_delay = pdMS_TO_TICKS(50); // 50 milissegundos de filtro

    while (1)
    {
        // Lê o estado atual dos botões
        int current_btn_a_state = gpio_get_level(BUTTON_A_GPIO);
        int current_btn_b_state = gpio_get_level(BUTTON_B_GPIO);
        TickType_t current_time = xTaskGetTickCount(); // Tempo atual do sistema

        // LÓGICA DO BOTÃO A (Incrementar Contador)
        // Transição de 1 para 0 E tempo passado desde o último clique válido é maior que o debounce
        if (last_btn_a_state == 1 && current_btn_a_state == 0 && (current_time - last_debounce_time_a) > debounce_delay) {
            
            // Soma o passo e limita a 4 bits usando resto da divisão (Módulo 16)
            counter = (counter + increment_step) % 16;
            
            update_leds(counter); // Atualiza as luzes
            
            last_debounce_time_a = current_time; // Salva o tempo deste clique
        }

        // LÓGICA DO BOTÃO B (Alternar Unidade de Incremento)
        if (last_btn_b_state == 1 && current_btn_b_state == 0 && (current_time - last_debounce_time_b) > debounce_delay) {
            
            // Se o passo for 1, vira 2. Se for 2, vira 1.
            if (increment_step == 1) {
                increment_step = 2;
            } else {
                increment_step = 1;
            }
            
            last_debounce_time_b = current_time; // Salva o tempo deste clique
        }

        // Atualiza os estados para a próxima iteração
        last_btn_a_state = current_btn_a_state;
        last_btn_b_state = current_btn_b_state;
        
        // Pequeno atraso para alívio de sistema do FreeRTOS (necessário para não disparar Watchdog Timer)
        vTaskDelay(pdMS_TO_TICKS(10));     
    }
}