#include "at_device.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h" // 用于ESP_LOGI
#include <stdarg.h>  // 用于va_list, va_start, va_end
#include <stdio.h>   // 用于vsnprintf

static const char *TAG = "AT_Device";

// 声明一个全局的互斥锁变量
SemaphoreHandle_t at_mutex;

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
 * @brief 初始化AT设备使用的互斥锁
 *
 * 该函数负责在运行时创建并初始化一个互斥锁对象（at_mutex），用于同步对AT设备的访问，防止多线程环境下的竞态条件。
 *
 * @return 无
 */
void at_mutex_init(void)
{
    // 在运行时创建互斥锁
    at_mutex = xSemaphoreCreateMutex();
    if (at_mutex == NULL)
    {
        // 如果创建互斥锁失败，这里可以处理错误
    }
}
/**
 * @brief 尝试获取互斥锁。
 * 这个函数用于在多任务环境中保护共享资源，确保在同一时刻只有一个任务可以访问该资源。
 *
 * @note 该函数没有参数，也不返回任何值。
 */
void at_mutex_lock(void)
{
    // 尝试获取互斥锁
    if (xSemaphoreTake(at_mutex, portMAX_DELAY) != pdTRUE)
    {
        // 如果获取互斥锁失败，这里可以处理错误
    }
}
/**
 * @brief 释放互斥锁
 *
 * 此函数用于在多任务环境下释放先前获取的互斥锁，允许其他任务尝试获取并访问受保护的共享资源，以实现线程安全控制。
 *
 * @note 本函数不接受任何参数，且无返回值。
 */
void at_mutex_unlock(void)
{
    // 释放互斥锁
    xSemaphoreGive(at_mutex);
}

/**
 * @brief AT模拟器初始化()
 */
void at_device_init(void)
{
    // 未使用，直接使用ec800_uart_init()初始化
}

/*
 * @brief	    向串口发送缓冲区内写入数据并启动发送
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    实际写入长度(如果此时缓冲区满,则返回len)
 */
unsigned int at_device_write(const void *buf, unsigned int len)
{
    // 参数校验
    if (buf == NULL)
    {
        ESP_LOGE(TAG, "Error: Buffer is NULL in at_device_write");
        return 0; // 返回 0 表示没有字节被写入
    }

    // 使用 uart_write_bytes 函数将数据写入到 UART 缓冲区
    int ret = uart_write_bytes(UART_PORT_NUM, buf, len);

    // 检查 uart_write_bytes 是否返回了错误
    if (ret == -1)
    {

        ESP_LOGE(TAG, "Parameter error in uart_write_bytes");
        ret = 0;
    }

    // 如果没有发生错误，返回实际写入的字节数
    assert(ret >= 0); // 确保 ret 是非负值，尽管ESP_OK保证了这一点，但增加断言以提高代码健壮性
    return (unsigned int)ret;
}

/*
 * @brief	    读取串口接收缓冲区的数据
 * @param[in]   buf       -  数据缓存
 * @param[in]   size       -  数据长度
 * @return 	    (实际读取长度)如果接收缓冲区的有效数据大于len则返回len否则返回缓冲
 *              区有效数据的长度
 */
unsigned int at_device_read(void *buf, unsigned int len)
{
    // 使用 uart_read_bytes 函数从 UART 缓冲区读取数据
    int ret = uart_read_bytes(UART_PORT_NUM, buf, len, 20 / portTICK_PERIOD_MS);
    // 检查 uart_write_bytes 是否返回了错误
    if (ret == -1)
    {

        ESP_LOGE(TAG, "Parameter error in uart_read_bytes");
        ret = 0;
    }
    // else if (ret)
    // {
    //     char *mutable_buf = (char *)buf;
    //     mutable_buf[ret] = '\0';                          // Null-terminate the data
    //     ESP_LOGI(TAG, "Recv  str: %s", mutable_buf); // Log the received data
    // }
    return (unsigned int)ret;
}

/**
 * @brief AT命令调试输出函数
 *
 * 该函数用于格式化和输出AT命令相关的调试信息。它使用了可变参数列表，允许调用者传递任意数量的参数。
 * 格式化字符串`fmt`用于指定输出格式，与`printf`函数的格式化字符串用法相同。
 *
 * @param fmt 格式化字符串，支持%符号作为格式化占位符。
 * @param ... 可变参数列表，根据格式化字符串`fmt`的需求传递相应参数。
 *
 * 该函数内部使用了`vsnprintf`函数进行参数的格式化处理，并将结果输出到ESP32的日志系统中。
 * 为了控制内存使用，这里使用了一个固定大小DEBUG_BUF_SIZE的缓冲区来存储格式化后的字符串。
 */
void at_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buffer[DEBUG_BUF_SIZE]; // 假设日志消息不会超过512字节
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    ESP_LOGI(TAG, "%s", buffer); // 使用ESP_LOGI处理格式化后的字符串
    va_end(args);
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
