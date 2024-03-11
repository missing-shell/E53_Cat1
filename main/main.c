#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ec800M_CN.h"

#define BMD_RESET (22)
#define BMD_CS (21)

// static void BM101_init(void)
// {
//     /*配置BMD引脚*/
//     gpio_config_t bmdconfig = {
//         .pin_bit_mask = (1ULL << BMD_RESET) | (1ULL << BMD_CS),
//         .mode = GPIO_MODE_OUTPUT};
//     gpio_config(&bmdconfig);

//     /*使能CS 高电平有效*/
//     gpio_set_level(BMD_CS, 1);

//     /*使能RESET 低电平有效*/
//     gpio_set_level(BMD_RESET, 1);
//     vTaskDelay(pdMS_TO_TICKS(1000)); // 延迟
//     gpio_set_level(BMD_RESET, 0);
//     vTaskDelay(pdMS_TO_TICKS(500)); // 延迟
//     gpio_set_level(BMD_RESET, 1);
// }
void app_main(void)
{
    // BM101_init();

    uart_task_create();
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
