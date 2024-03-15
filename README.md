# EC800M-CN

## 串口测试

- 波特率：115200
- 数据位：8
- 停止位：1

### 基础测试

- **AT 命令末尾需添加回车换行符`\r\n`**

- **AT 命令必须使用英文标点符号**

```AT
AT+CIMI   //测试是否识别到卡
AT+CSQ    //卡信号强度
```

- AT+CREG CS 域网络注册状态

```AT
AT+CREG?  //是否有服务
->+CREG: 1,1  //第一位含义为启用网络注册URC +CREG: <stat>，若为0则需运行下列命令

AT+CREG=1
->  OK

->  +CREG: 1      //URC 上报ME 已经注册到归属地网络。

AT+CREG=2      //启用带有位置信息的网络注册URC。
->  OK

->  +CREG: 1,''D509'',''80D413D'',7 //URC 上报带小区ID 和位置区号的运营商。
```

- 若返回`+CREG: 1,1`则表示网络 URC 注册到归属地网络

### 拨打和接听电话

```AT
ATD 10086;
ATH  //挂断电话
ATA  //接听电话
```

### 发送短信

```AT
AT+CMGF=1 //设置短信为文本格式
AT+CSCS="GSM"  //设置 TE 输入字符集格式为 GSM 格式。
AT+CMGS="15966607851"  //填写手机号。
```

### MQTT

- 以阿里云为例，须在阿里云公共实例中创建使用数据连接自定义的 MQTT 设备

- mqtt 相关指令

```AT
AT+QMTCFG MQTT 参数配置，选指令，连接发起前配置，如不配置则按默认值连接。

AT+QMTOPEN：打开 MQTT 客户端网络。

AT+QMTCLOSE：关闭 MQTT 客户端网络

AT+QMTCONN：连接 MQTT 服务器。

AT+QMTDISC：断开 MQTT 服务器。

AT+QMTSUB：订阅主题。

AT+QMTPUB：发布消息。

AT+QMTUNS：退订主题。
```

#### 1.配置网络

首先，您需要确保 EC800M 模块已经连接到网络。这通常涉及到设置 APN（接入点名称）。

```AT
AT+CGDCONT=1,"IP","CMNET"   //设置 APN。需要 your.apn.com 替换为你的 APN。

AT+CGACT=1,1//激活CID=1的数据承载

AT+CGATT?//检查网络状态，若返回 +CGATT: 1则表示设备已经连接到网络

```

#### 2.设置 MQTT 参数

接下来，您需要设置 MQTT 的服务器地址、端口、用户名和密码,参照下列格式，具体见阿里云配置页面

- 设备证书

```json
{
  "ProductKey": "k0leyWHxYT1",
  "DeviceName": "Cat1",
  "DeviceSecret": "3af7bc8812cb475e042a0a5ae377c6a1"
}
```

- MQTT 连接参数

```json
{
  "mqttHostUrl": "iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com",
  "port": 1883
}
```

- 配置接收模式。

```AT

AT+QMTCFG="recv/mode",0,0,1
->  OK
```

- 配置阿里云设备信息。

```AT

AT+QMTCFG="aliauth",0,"k0leyWHxYT1","Cat1","3af7bc8812cb475e042a0a5ae377c6a1"
->  OK
```

- 测试命令

```AT
AT+QMTOPEN=?
->  +QMTOPEN: (0-5),"hostname",(1-65535)
->  OK
```

- MQTT 客户端打开网络。

```AT

AT+QMTOPEN=0,"iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com",1883
->  OK
->  +QMTOPEN: 0,0             //MQTT 客户端成功打开网络。
```

- **在 mqtt 客户端打开网络命令发出后，尽快发出本条命令。否则可能会返回+QMTSTAT: 0,1 错误，就得重来一遍了命令解释：客户端端口号 0，因为服务器上 mqtt 并未设置其它参数，所以这里写 0**

```AT
AT+QMTCONN=0,0
->  OK
->  +QMTCONN: 0,0,0
```

- 可在阿里云查看该设备，若已上线，则说明配置成功

- 查询命令

```AT
AT+QMTOPEN?
->  +QMTOPEN: 0,"iot-as-mqtt.cn-shanghai.aliyuncs.com",1883
->  OK
```

- 测试命令

```AT
AT+QMTCONN=?
->  +QMTCONN: (0-5),"clientid","username","password"
```

- ~~配置设备信息测试时命令报错~~

```AT
//客户端连接MQTT 服务器。
// 若已连接阿里云，可使用 AT+QMTCFG="aliauth"提前配置设备信息，之后可省略<username>和<password>。
AT+QMTCONN=0,"clientExample"
-> OK
```

#### 3.订阅主题

在连接成功后，您可以订阅一个或多个主题以接收消息。

```AT
AT+QMTSUB=0,1,"/sys/k0leyWHxYT1/Cat1/thing/service/property/set",2
```

这里的参数解释如下：

- `0`：MQTT 客户端 ID，表示使用的是第一个（也是唯一的）MQTT 客户端。
- `1`：订阅的 QoS 级别，这里是 1，表示至少一次交付。
- `"/sys/k0leyWHxYT1/Cat1/thing/service/property/set`：要订阅的主题名称,具体参考阿里云物模型 topic。
- `2`：订阅的主题 ID，这里是 2。

请确保你已经正确配置了 MQTT 客户端的网络连接和认证信息，以便能够成功订阅主题。

#### 4.发布消息

您也可以将消息发布到某个主题。

Topic 使用阿里云物模型 Topic

数据格式应采用[阿里云标准数据格式](https://help.aliyun.com/zh/iot/user-guide/device-properties-events-and-services?spm=a2c4g.11186623.0.0.95c91bc8m3HmhW)

- 测试命令

```AT
AT+QMTPUBEX=?
-> +QMTPUBEX: (0-5),<msgid>,(0-2),(0,1),"topic","length"

-> OK
```

- 设置命令

```AT
AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30  //30代表字节长度
```

**响应`>`后，输入并发送 paload 字符串。以 json 格式发送，其中 params 记录的是属性及属性值**

- 标准 json 格式，但不可通过串口直接发送

```json
{
  "id": "123",
  "version": "1.0",
  "sys": {
    "ack": 0
  },
  "params": {
    "LINK_TEST": "126"
  },
  "method": "thing.event.property.post"
}
```

- 通过串口发送的 json 中不能包含换行符，否则阿里云物模型会出现 6207(非标准 json 格式)错误。

```json
{
  "id": "123",
  "version": "1.0",
  "sys": { "ack": 0 },
  "params": { "LINK_TEST": 126 },
  "method": "thing.event.property.post"
}
```

```AT
-> OK
-> +QMTPUBEX: 0,0,0
```

- 可通过阿里云日志服务查看，需确保设备到云端状态为`200`-Topic；物模型状态`200`-物模型成功解析 paload，此时可在设备页面查看数据

#### 5.退订主题

```AT
AT+QMTUNS=0,2,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post"
->  OK

->  +QMTUNS: 0,2,0
```

#### 6.关闭客户端

```AT
AT+QMTDISC=0   断开客户端
->  OK

->  +QMTDISC: 0,0
```

## 使用 esp32+EC800M-CN 实现(2G/3G/4G)MQTT
