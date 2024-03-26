#ifndef __AT_DEVICE_H__
#define __AT_DEVICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define ec800M_BAND (115200)
#define ec800M_TX (5)
#define ec800M_RX (4)
#define UART_PORT_NUM (1) // 使用UART1
#define UART_BUF_SIZE (1024)
#define DEBUG_BUF_SIZE (512)

    void at_mutex_init(void);

    void at_mutex_lock(void);

    void at_mutex_unlock(void);

    void at_device_init(void);

    void at_device_open(void);

    void at_device_close(void);

    unsigned int at_device_write(const void *buf, unsigned int len);

    unsigned int at_device_read(void *buf, unsigned int len);

    void at_debug(const char *fmt, ...);

    void at_device_emit_urc(const void *urc, int size);

#ifdef __cplusplus
}
#endif

#endif
