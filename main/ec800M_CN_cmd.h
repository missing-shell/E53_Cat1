#ifndef EC800M_CN_CMD_H
#define EC800M_CN_CMD_H

#ifdef __cplusplus
extern "C"
{
#endif

    void at_obj_init(void);
    void ec800M_CN_init(void);
    void read_creg(void);
    void ec800_uart_task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif
