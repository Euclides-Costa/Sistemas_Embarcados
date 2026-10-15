#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

// Definições de hardware
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define LED_PIN (GPIO_NUM_4) 

#define UART_NUM UART_NUM_2
#define BUF_SIZE (1024)

void init_hardware(void) {
    // 1. Inicializa e configura o pino do LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0); 

    // 2. Configura a UART2 com os parâmetros solicitados
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Instala o driver
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    
    // Aplica as configurações do periférico
    uart_param_config(UART_NUM, &uart_config);
    
    // Mapeia os pinos TX e RX
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void app_main(void) {
    init_hardware();

    // Aloca memória para o buffer de recepção
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    bool flag_ligar = true; // Variável de controle para alternar os comandos

    while (1) {
        // Seleciona a mensagem do ciclo atual
        const char *mensagem_envio = flag_ligar ? "LIGAR" : "DESLIGAR";
        flag_ligar = !flag_ligar; // Alterna o estado para o próximo ciclo

        // Envia a mensagem pela UART2
        printf("==> Enviando comando: '%s'\n", mensagem_envio);
        uart_write_bytes(UART_NUM, mensagem_envio, strlen(mensagem_envio));

        // Aguarda e lê os dados do RX usando timeout da API (500 ms)
        int rx_bytes = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(500));

        if (rx_bytes > 0) {
            // Requisito: Manipulação Segura de Memória
            data[rx_bytes] = '\0';
            
            printf("<== Recebido no Loopback: '%s'\n", (char *)data);

            // Requisito: Processamento e Ação por Recepção
            if (strcmp((char *)data, "LIGAR") == 0) {
                gpio_set_level(LED_PIN, 1);
                printf("[AÇÃO] LED foi ACESO\n\n");
            } 
            else if (strcmp((char *)data, "DESLIGAR") == 0) {
                gpio_set_level(LED_PIN, 0);
                printf("[AÇÃO] LED foi APAGADO\n\n");
            } 
        } else {
            printf("Timeout: Nenhuma resposta. O jumper entre TX(17) e RX(16) está conectado?\n\n");
        }

        // Aguarda 2 segundos antes de enviar o próximo comando no loop
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    free(data);
}