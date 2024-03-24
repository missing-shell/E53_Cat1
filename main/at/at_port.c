/**
 * @Brief: The AT component drives the interface implementation
 * @Author: roger.luo
 * @Date: 2021-04-04
 * @Last Modified by: roger.luo
 * @Last Modified time: 2021-11-27
 */
#include <stddef.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"

/**
 * @brief Custom malloc for AT component.
 */
void *at_malloc(unsigned int nbytes)
{
    // 使用 ESP-IDF 的内存管理函数
    return pvPortMalloc(nbytes);
}
/**
 * @brief Custom free for AT component.
 */
void at_free(void *ptr)
{
    vPortFree(ptr);
}

/**
 * @brief Gets the total number of milliseconds in the system.
 */
unsigned int at_get_ms(void)
{
    // 获取自 FreeRTOS 启动以来经过的滴答数
    TickType_t ticks = xTaskGetTickCount();

    // 将滴答数转换为毫秒数
    
    unsigned int ms = ticks * 1000 / configTICK_RATE_HZ;

    return ms;
}