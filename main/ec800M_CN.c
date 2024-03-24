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
#include "at_device.h"
#include "linux_list.h"

#define UART_BUF_SIZE (1024)

static const char *TAG = "ec800M-CN"; // Initialize the log tag

/* Private variables ---------------------------------------------------------*/
/**
 * @brief   定义AT控制器
 */
static const at_obj_t *at_obj;
/**
 * @brief AT适配器
 */
static const at_adapter_t at_adapter = {
    .lock = at_mutex_lock,     // 多任务上锁(非OS下填NULL)
    .unlock = at_mutex_unlock, // 多任务解锁(非OS下填NULL)
    .write = at_device_write,  // 数据写接口
    .read = at_device_read,    // 数据读接口
    //.debug = at_debug,
    .recv_bufsize = UART_BUF_SIZE, // 接收缓冲区大小
    .urc_bufsize = UART_BUF_SIZE

};

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
 * @brief  命令响应处理程序
 */
static void read_creg_callback(at_response_t *r)
{
    int mode, state;
    if (r->code == AT_RESP_OK)
    {
        // 提取出工作模式及注册状态
        if (sscanf(r->prefix, "+CREG:%d,%d", &mode, &state) == 2)
        {
            ESP_LOGI("TAG", "Mode:%d, state:%d\r\n", mode, state);
        }
    }
    else
    {
        ESP_LOGE("TAG", "'CREG' command response failed!");
    }
}

/**
 * @brief  读网络注册状态
 */
static void read_creg(void)
{
    at_attr_t attr;
    // 设置属性为默认值
    at_attr_deinit(&attr);
    attr.cb = read_creg_callback; // 设置回调处理程序
    attr.timeout = 500;
    attr.retry = 2;

    // 发送CREG查询命令,超时时间为1000ms,重发次数为1
    at_send_singlline(at_obj, &attr, "AT+CREG?");
}

/**
 * 创建EC800 UART任务
 * 该函数初始化UART并创建一个任务来处理UART通信。
 *
 * @param
 * @return
 */
static void ec800_uart_task(void *pvParameters)
{
    uint8_t *data = (uint8_t *)malloc(UART_BUF_SIZE);

    at_obj_process(at_obj);
    while (1)
    {
        // 读取 UART 数据
        int len = uart_read_bytes(UART_PORT_NUM, data, (UART_BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        if (len)
        {
            data[len] = '\0'; // 确保数据以 null 结尾
            ESP_LOGI(TAG, "Recv str: %s", (char *)data);

            // 检查接收到的数据是否包含发送的命令
            if (strstr((char *)data, "AT+CIMI\r\n") != NULL)
            {
                // 如果包含，那么只处理响应部分
                char *response = strstr((char *)data, "OK");
                if (response != NULL)
                {
                    // 处理响应
                    ESP_LOGI(TAG, "Response: %s", response);
                }
            }
        }
    }
    vTaskDelete(NULL);
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

    // 初始化互斥锁
    at_mutex_init();

    at_obj = at_obj_create(&at_adapter);

    if (at_obj == NULL)
    {
        ESP_LOGE("TAG", "at object create failed");
    }
    read_creg();

    xTaskCreate(ec800_uart_task, "ec800_uart_task", 2048, NULL, 10, NULL);
}
