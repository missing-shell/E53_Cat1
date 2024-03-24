#include "at_device.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// 声明一个全局的互斥锁变量
SemaphoreHandle_t at_mutex;

void at_mutex_init(void)
{
    // 在运行时创建互斥锁
    at_mutex = xSemaphoreCreateMutex();
    if (at_mutex == NULL)
    {
        // 如果创建互斥锁失败，这里可以处理错误
    }
}

void at_mutex_lock(void)
{
    // 尝试获取互斥锁
    if (xSemaphoreTake(at_mutex, portMAX_DELAY) != pdTRUE)
    {
        // 如果获取互斥锁失败，这里可以处理错误
    }
}

void at_mutex_unlock(void)
{
    // 释放互斥锁
    xSemaphoreGive(at_mutex);
}
/**
 * @brief AT设备
 */
typedef struct
{
    // ring_buf_t rb_tx;
    // ring_buf_t rb_rx;
    unsigned char txbuf[UART_BUF_SIZE];
    unsigned char rxbuf[UART_BUF_SIZE];
} at_device_t;

static at_device_t at_device;

/**
 * @brief AT模拟器初始化()
 */
void at_device_init(void)
{
}
/**
 * @brief 数据写操作
 * @param buf  数据缓冲区
 * @param size 缓冲区长度
 * @retval 实际写入数据的大小
 */
unsigned int at_device_write(const void *buf, unsigned int size)
{
    // 使用 uart_write_bytes 函数将数据写入到 UART 缓冲区
    unsigned int ret = uart_write_bytes(UART_PORT_NUM, buf, size);
    return ret;
}

/**
 * @brief 数据读操作
 * @param buf  数据缓冲区
 * @param size 缓冲区长度
 * @retval 实际读到的数据
 */
unsigned int at_device_read(void *buf, unsigned int size)
{
    // 使用 uart_read_bytes 函数从 UART 缓冲区读取数据
    unsigned ret = uart_read_bytes(UART_PORT_NUM, buf, size, 20 / portTICK_PERIOD_MS);
    return ret;
}

/**
 * @brief 触发生成URC消息
 */
void at_device_emit_urc(const void *urc, int size)
{
    // 使用 uart_write_bytes 函数将数据写入到 UART 端口
    uart_write_bytes(UART_PORT_NUM, urc, size);
}

/**
 * @brief 打开AT设备(将触发 "+POWER:1" URC消息)
 */
void at_device_open(void)
{
    char *s = "+POWER:1\r\n";
    at_device_emit_urc(s, strlen(s));
}

/**
 * @brief 关闭AT设备(将触发 "+POWER:0" URC消息)
 */
void at_device_close(void)
{
    char *s = "+POWER:0\r\n";
    at_device_emit_urc(s, strlen(s));
}
