#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"

#include "at_chat.h"
#include "at_port.h"
#include "at_device.h"
#include "linux_list.h"
#include "ec800M_CN_cmd.h"

static const char *TAG = "ec800M_CN_CMD";

/**
 * @brief   定义AT控制器
 */
static at_obj_t *at_obj;

/**
 * @brief AT适配器
 */
static const at_adapter_t at_adapter = {
    .lock = at_mutex_lock,     // 多任务上锁(非OS下填NULL)
    .unlock = at_mutex_unlock, // 多任务解锁(非OS下填NULL)
    .write = at_device_write,  // 数据写接口
    .read = at_device_read,    // 数据读接口
    .debug = at_debug,
    .recv_bufsize = UART_BUF_SIZE, // 接收缓冲区大小
    .urc_bufsize = UART_BUF_SIZE

};

/**
 * @brief  模块初始化回调
 */
static void ec800M_CN_init_callback(at_response_t *r)
{
    ESP_LOGI(TAG, "ec800M_CN_init_callback Init %s!", r->code == AT_RESP_OK ? "ok" : "error");
}
/*
 * @brief 模块初始化
 */
void ec800M_CN_init(void)
{
    at_attr_t attr;
    static const char *cmds[] = {
        "ATE0"     // 关闭指令回显，避免影响解析
        "AT+CIMI", // 测试是否识别到卡
        NULL};
    at_attr_deinit(&attr);
    attr.cb = ec800M_CN_init_callback; // 设置命令回调
    at_send_multiline(at_obj, &attr, cmds);
}
/**
 * @brief 读网络注册状态回调
 */
static void read_creg_callback(at_response_t *r)
{

    int mode, state;
    if (r->code == AT_RESP_OK)
    {
        /**
         *    \r\n
         *    OK\r\n
         *    \r\n
         *    +CREG: 0,1\r\n
         *    \r\n
         *    OK\r\n
         *    AT指令格式，`sscanf`并不会忽略掉回车换行符，所以需要确保格式完全一致，
         */
        if (sscanf(r->prefix, "\r\n+CREG:%d,%d\r\n", &mode, &state) == 2)
        {
            ESP_LOGI(TAG, "Mode:%d, state:%d", mode, state);
        }
    }
    else
    {
        ESP_LOGE(TAG, "'CREG' command response failed!");
    }
}

/**
 * @brief  读网络注册状态
 */
void read_creg(void)
{
    at_attr_t attr;
    // 设置属性为默认值
    at_attr_deinit(&attr);
    attr.cb = read_creg_callback; // 设置回调处理程序
    attr.timeout = 500;
    attr.retry = 2;

    // 发送CREG查询命令,超时时间为1000ms,重发次数为2
    at_send_singlline(at_obj, &attr, "AT+CREG?");
}

void at_obj_init(void)
{
    // 初始化互斥锁
    at_mutex_init();

    at_obj = at_obj_create(&at_adapter);

    if (at_obj == NULL)
    {
        ESP_LOGE(TAG, "at object create failed");
    }
}
/**
 * 创建EC800 UART任务
 * 该函数初始化UART并创建一个任务来处理UART通信。
 *
 * @param
 * @return
 */
void ec800_uart_task(void *pvParameters)
{
    while (1)
    {
        at_obj_process(at_obj);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}