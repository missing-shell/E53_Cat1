#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ec800M_CN.h"

void app_main(void)
{
    ec800_uart_task_create();
}
