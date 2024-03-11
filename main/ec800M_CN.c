#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "ec800M_CN.h"

#define UART_PORT_NUM (1)
#define ec800M_BAND (115200)
#define ec800M_TX (5)
#define ec800M_RX (4)
#define UART_PORT_NUM (1) // 使用UART1

#define UART_BUF_SIZE (1024)

static const char *TAG = "ec800M-CN"; // Initialize the log tag

static void parse_payload();

static void uart_event_init(void) // Define the echo task function
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        // Define and initialize the UART configuration
        .baud_rate = ec800M_BAND,        // Set the baud rate
        .data_bits = UART_DATA_8_BITS,   // Set the data bits
        .parity = UART_PARITY_DISABLE,   // Disable parity
        .stop_bits = UART_STOP_BITS_1,   // Set the stop bits
        .source_clk = UART_SCLK_DEFAULT, // Set the source clock
    };
    int intr_alloc_flags = 0; // Define the interrupt allocation flags

    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, UART_BUF_SIZE * 2, UART_BUF_SIZE * 2, 0, NULL, intr_alloc_flags)); // Install the UART driver
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));                                                      // Configure the UART parameters
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, ec800M_TX, ec800M_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));           // Set the UART pins
}

static void uart_event_task(void *pvParameters)
{
    uint8_t *data = (uint8_t *)malloc(UART_BUF_SIZE);
    while (1)
    {
        // Read data from the UART
        int len = uart_read_bytes(UART_PORT_NUM, data, (UART_BUF_SIZE - 1), 20 / portTICK_PERIOD_MS); // Read bytes from the UART
        if (len)
        {                                                // If data was read
            data[len] = '\0';                            // Null-terminate the data
            ESP_LOGI(TAG, "Recv str: %s", (char *)data); // Log the received data
        }
    }
}

void uart_task_create(void)
{
    uart_event_init();
    // Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 10, NULL);
}
