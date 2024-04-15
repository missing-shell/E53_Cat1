# 代码结构

* 参考[AT Command](https://gitee.com/moluo-tech/AT-Command)

从[AT Command](https://gitee.com/moluo-tech/AT-Command)中导入了`include`和`src`目录，并将`src`目录重命名为`at`，并修改了部分代码。

    重写了`at_port.c`中的系统接口。并添加了`at_device.c`文件用做定义`at`命令函数接口。

## 注意

`ec800m_cn.c`和`at_device.c`的区别不够明确，两者都定义了`at`命令函数。

```c
.recv_bufsize = UART_BUF_SIZE, // 接收缓冲区大小
.urc_bufsize = UART_BUF_SIZE
```

UART_BUF_SIZE不可定义太大，否则可能创建`at`对象不成功。

`cmakelist`中需要注册为`src_at`,否则可能出现链接失败。
