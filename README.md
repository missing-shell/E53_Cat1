# Cat1 与 4G

Cat.1 的全称是 LTE UE-Category1，在 2009 年，Cat.1-5 是 3GPP 专门划分出来，面向于未来物联网应用市场的类别，Cat.1 的最终目标是服务于物联网并实现低功耗和低成本 LTE 连接。

LTE 英文 “Long-Term Evolution” ，中文名称为长期演进技术。它是一种用于移动通信的无线网络标准，也是 4G（第四代）移动通信技术的主要标准之一。
UE 英文 “User Equipment” 指的是用户终端，它是 LTE 网络下用户终端设备的无线性能的分类。3GPP 用 Cat.1~20 来衡量用户终端设备的无线性能，也就是划分终端速率等级。

Cat.1 是属于 4G 系列，可以完全重用现有的 4G 资源。Cat.1 是配置为最低版本参数的用户终端级别，可让业界以低成本设计低端 4G 终端。

随着现在物联网的发展，Cat.1 在物联网领域越发重要，与 NB-IoT 和 2G 模块相比，Cat.1 在网络覆盖范围，速度和延迟方面具有优势。与传统的 Cat.4 模块相比，它具有成本低，功耗低的优点。

## 基础测试

```AT
AT+CPIN?  //测试是否识别到卡
AT+CSQ    //卡信号强度
AT+CREG?  //是否有服务
```

## 拨打和接听电话

```AT
ATD 10086;
ATH  //挂断电话
ATA  //接听电话
```

## 发送短信

```AT
AT+CMGF=1 //设置短信为文本格式
AT+CSCS="GSM"  //设置 TE 输入字符集格式为 GSM 格式。
AT+CMGS="15966607851"  //填写手机号。
```

## MQTT

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

### 1.配置网络

首先，您需要确保 EC800M 模块已经连接到网络。这通常涉及到设置 APN（接入点名称）。

```AT
AT+CGDCONT=1,"IP","CMNET"   //设置 APN。需要 your.apn.com 替换为你的 APN。

AT+CGACT=1,1//激活CID=1的数据承载

AT+CGATT?//检查网络状态，若返回 +CGATT: 1则表示设备已经连接到网络

```

### 2.设置 MQTT 参数

接下来，您需要设置 MQTT 的服务器地址、端口、用户名和密码

```json
//设备证书
{
  "ProductKey": "k0leyWHxYT1",
  "DeviceName": "Cat1",
  "DeviceSecret": "3af7bc8812cb475e042a0a5ae377c6a1"
}

//MQTT连接参数
{
  "mqttHostUrl":"iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com",
  "port":1883
}

```

```json
//配置接收模式。
AT+QMTCFG="recv/mode",0,0,1
->  OK
```

```json
//配置阿里云设备信息。
AT+QMTCFG="aliauth",0,"k0leyWHxYT1","Cat1","3af7bc8812cb475e042a0a5ae377c6a1"
->  OK
```

```json
AT+QMTOPEN=?
->  +QMTOPEN: (0-5),"hostname",(1-65535)
->  OK
```

```json
//MQTT 客户端打开网络。
AT+QMTOPEN=0,"iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com",1883
->  OK
->  +QMTOPEN: 0,0             //MQTT 客户端成功打开网络。
```

```json
//在mqtt客户端打开网络命令发出后，尽快发出本条命令，否则可能会返回+QMTSTAT: 0,1 错误，就得重来一遍了命令解释：客户端端口号0，因为服务器上mqtt并未设置其它参数，所以这里写0
AT+QMTCONN=0,0
->  OK
->  +QMTCONN: 0,0,0
```

```json
AT+QMTOPEN?
->  +QMTOPEN: 0,"iot-as-mqtt.cn-shanghai.aliyuncs.com",1883
->  OK
```

```json
AT+QMTCONN=?
->  +QMTCONN: (0-5),"clientid","username","password"
```

```json
//客户端连接MQTT 服务器。
// 若已连接阿里云，可使用 AT+QMTCFG="aliauth"提前配置设备信息，之后可省略<username>和<password>。
AT+QMTCONN=0,"clientExample"
-> OK
```

### 3.订阅主题

在连接成功后，您可以订阅一个或多个主题以接收消息。

```AT
AT+MQTTSUB="your/topic/sub",0
AT+MQTTSUB="your/topic/sub",0：订阅一个主题。必然 your/topic/sub 替换为你想要的订阅主题。

AT+MQTTSUB="/sys/k0leyWHxYT1/Cat1/thing/event/property/post_reply",0

	AT+MQTTSUB="/sys/k0leyWHxYT1/"Cat1"/thing/event/property/post",0


```

### 4.发布消息

您也可以将消息发布到某个主题。

```AT
AT+MQTTPUB="your/topic/pub","Hello, Aliyun!"
AT+MQTTPUB="your/topic/pub","Hello, Aliyun!"：发布一个消息到一个主题。your/topic/pub 替换为你想要发布的主题，"Hello, Aliyun!"替换为你想要发送的消息。
```

### 5.接收消息

当您订阅的主题有新消息时，EC800M 模块会通过串口发送一个通知。您可以通过读取串口数据来获取这些消息。

注意事项
确保您的设备已经正确连接到 EC800M 模块，并且可以通过串口与模块通信。
请替换上述命令中的占位符位（如 your.apn.com、your.iot.endpoint、your_username、your_password 和 your/topic/sub）为实际的值。

### 6.关闭客户端

```json
AT+QMTDISC=0   断开客户端
```
