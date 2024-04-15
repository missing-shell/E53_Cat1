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

static const char *TAG = "ec800M-CN";

const int CS_STATE = 1;

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
    attr.retry = 1;
    static const char *cmds[] = {
        "ATE0"       // 关闭指令回显，避免影响解析
        "AT+CIMI",   // 测试是否识别到卡
        "AT+QPOWD ", // 关闭模块"
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
static void read_creg(void)
{
    at_attr_t attr;
    // 设置属性为默认值
    at_attr_deinit(&attr);
    attr.cb = read_creg_callback; // 设置回调处理程序
    attr.timeout = 500;
    attr.retry = 1;

    // 发送CREG查询命令,超时时间为1000ms,重发次数为2
    at_send_singlline(at_obj, &attr, "AT+CREG?");
}
/**
 * @brief  设置注册状态
 */
static void set_creg(void)
{
    at_attr_t attr;
    // 设置属性为默认值
    at_attr_deinit(&attr);
    attr.cb = read_creg_callback; // 设置回调处理程序
    attr.timeout = 500;
    attr.retry = 1;

    if (CS_STATE == 1)
    { // 发送CREG查询命令,超时时间为1000ms,重发次数为2
        at_send_singlline(at_obj, &attr, "AT+CREG=1");
    }
    else if (CS_STATE == 2)
    {
        at_send_singlline(at_obj, &attr, "AT+CREG=2");
    }
}

/**
 * @brief  定义PDP上下文回调
 */
static void set_pdp_callback(at_response_t *r)
{
    ESP_LOGI(TAG, "Setting PDP %s!\r\n", r->code == AT_RESP_OK ? "ok" : "error");
}
/**
 * @brief  定义PDP上下文
 */
static void set_pdp(const int cid, const char *pdp, const char *apn)
{
    at_attr_t attr;
    attr.retry = 1;
    at_attr_deinit(&attr);
    attr.cb = set_pdp_callback; // 设置命令回调
    at_exec_cmd(at_obj, &attr, "AT+CGDCONT=\"%d\",\"%s\",\"%s\"", cid, pdp, apn);
}
/**
 * @brief 激活CID=1的数据承载回调
 */
static void set_cgact_callback(at_response_t *r)
{
    ESP_LOGI(TAG, "Setting CGACT %s!\r\n", r->code == AT_RESP_OK ? "ok" : "error");
}
/**
 * @brief  激活CID=1的数据承载
 */
static void set_cgact(const int cid, const int act)
{
    at_attr_t attr;
    attr.retry = 1;
    at_attr_deinit(&attr);
    attr.cb = set_cgact_callback; // 设置命令回调
    at_exec_cmd(at_obj, &attr, "AT+CGDCONT=\"%d\",\"%d\"", cid, act);
}

/**
 * @brief  命令响应处理程序
 */
static void mqtt_init_callback(at_response_t *r)
{
    // printf("MQTT Init %s!\r\n", r->code == AT_RESP_OK ? "ok" : "error");
}
/*
 * @brief 模块初始化
 */
static void mqtt_init(void)
{
    at_attr_t attr;
    attr.retry = 1;
    static const char *cmds[] = {
        "AT+QMTCFG=\"recv/mode\",0,0,1",

        "AT + QMTCFG = \"aliauth\",0,\"k0leyWHxYT1\",\"Cat1\",\"3af7bc8812cb475e042a0a5ae377c6a1\"",

        "AT+QMTOPEN=0,\"iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com\",1883",

        "AT+QMTCONN=0,0",

        "AT + QMTPUBEX = 0,0,0,0,\"/sys/k0leyWHxYT1/Cat1/thing/event/property/post\",140",

        NULL};
    at_attr_deinit(&attr);
    attr.cb = mqtt_init_callback; // 设置命令回调
    at_send_multiline(at_obj, &attr, cmds);
}

static void at_obj_init(void)
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
static void ec800_uart_task(void *pvParameters)
{
    while (1)
    {
        at_obj_process(at_obj);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

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
    set_cgact(1, 1);
    read_creg();
    set_pdp(1, "IP", "CMNET");

    /*MQTT 任务*/
    mqtt_init();

    xTaskCreate(ec800_uart_task, "ec800_uart_task", 4096, NULL, 10, NULL);
}
