#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "ec800M_CN.h"

#include "at_chat.h"
#include "at_port.h"
#include "linux_list.h"
#include "at_device.h"
#include "ec800M_CN_cmd.h"

static const char *TAG = "ec800M-CN";

/*
 * 函数名称：ec800_uart_init
 * 功能描述：初始化UART通信，配置UART参数，安装UART驱动并设置相关引脚
 *
 *  @param
 *  @return
 */
static void ec800_uart_init(void) // Define the echo task function
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

/**
 * 创建EC800 UART任务
 * 该函数初始化UART并创建一个任务来处理UART通信。
 *
 * @param
 * @return
 */
void ec800_uart_task_create(void)
{
    ec800_uart_init();
    at_obj_init(); // 初始化AT对象

    ec800M_CN_init();
    read_creg();
    xTaskCreate(ec800_uart_task, "ec800_uart_task", 4096, NULL, 10, NULL);
}
