# Cat1 与 4G

Cat.1 的全称是 LTE UE-Category1，在 2009 年，Cat.1-5 是 3GPP 专门划分出来，面向于未来物联网应用市场的类别，Cat.1 的最终目标是服务于物联网并实现低功耗和低成本 LTE 连接。

LTE 英文 “Long-Term Evolution” ，中文名称为长期演进技术。它是一种用于移动通信的无线网络标准，也是 4G（第四代）移动通信技术的主要标准之一。
UE 英文 “User Equipment” 指的是用户终端，它是 LTE 网络下用户终端设备的无线性能的分类。3GPP 用 Cat.1~20 来衡量用户终端设备的无线性能，也就是划分终端速率等级。

Cat.1 是属于 4G 系列，可以完全重用现有的 4G 资源。Cat.1 是配置为最低版本参数的用户终端级别，可让业界以低成本设计低端 4G 终端。

随着现在物联网的发展，Cat.1 在物联网领域越发重要，与 NB-IoT 和 2G 模块相比，Cat.1 在网络覆盖范围，速度和延迟方面具有优势。与传统的 Cat.4 模块相比，它具有成本低，功耗低的优点。

## CS 域 和 PS 域

在移动通信网络中，CS（Circuit Switched）域和 PS（Packet Switched）域是两个不同的网络域，它们分别处理不同类型的服务。

### CS 域（电路交换域）

CS 域是电路交换网络的一部分，它主要用于处理语音和电话服务。在 CS 域中，当两个设备（例如手机）想要进行通信时，网络会为它们建立一个专用的电路。这个电路在通信期间保持开启，直到通信结束。CS 域的主要特点是它提供了一个稳定的、具有固定带宽的连接，这对于实时语音通信非常重要。

### PS 域（分组交换域）

PS 域是分组交换网络的一部分，它主要用于处理数据服务，如互联网访问、移动数据和 MMS（多媒体消息服务）。在 PS 域中，数据被分割成小块（分组），并通过网络进行传输。每个分组可以独立地通过网络路由，这意味着它们可以选择最佳的路径，而不是固定的电路。PS 域的主要特点是它提供了一个更加灵活和可扩展的连接，这对于数据服务非常重要。

在现代的移动通信网络中，CS 域和 PS 域通常是集成的，这意味着设备可以同时使用这两个域来提供服务。例如，一个设备可以在 CS 域中进行电话通话，同时在 PS 域中进行互联网浏览。这种集成允许设备在不同的网络域之间进行无缝切换，以提供更好的用户体验。

### 参考数据

[14:13:00.296] AT+CIMI
[14:13:00.296]
[14:13:17.546] AT+CIMI
[14:13:31.496] AT
[14:14:26.497] ATI
[14:14:48.796] ATI
[14:14:48.804] ATI
[14:14:48.804] Quectel
[14:14:48.804] EC800M
[14:14:48.804] Revision: EC800MCNGAR06A05M08
[14:14:48.804]
[14:14:48.804] OK
[14:15:29.896] AT+CIMI
[14:15:29.903] AT+CIMI
[14:15:29.903] 460003625090007
[14:15:29.903]
[14:15:29.903] OK
[14:15:41.296] AT+CSQ
[14:15:41.303] AT+CSQ
[14:15:41.303] +CSQ: 29,99
[14:15:41.303]
[14:15:41.303] OK
[14:15:59.346] AT+CREG?  
[14:15:59.353] AT+CREG?  
[14:15:59.353] +CREG: 0,1
[14:15:59.353]
[14:15:59.353] OK
[14:16:11.296] AT+CREG=1
[14:16:11.304] AT+CREG=1
[14:16:11.304] OK
[14:16:19.846] AT+CREG?
[14:16:19.853] AT+CREG?
[14:16:19.853] +CREG: 1,1
[14:16:19.853]
[14:16:19.853] OK
[14:16:46.496] AT+CGDCONT=1,"IP","CMNET"
[14:16:46.505] AT+CGDCONT=1,"IP","CMNET"
[14:16:46.505] OK
[14:16:57.246] AT+CGACT=1,1
[14:16:57.254] AT+CGACT=1,1
[14:16:57.254] OK
[14:17:10.345] AT+QMTCFG="recv/mode",0,0,1
[14:17:10.356] AT+QMTCFG="recv/mode",0,0,1
[14:17:10.356] OK
[14:17:19.796] AT+QMTCFG="aliauth",0,"k0leyWHxYT1","Cat1","3af7bc8812cb475e042a0a5ae377c6a1"
[14:17:19.808] AT+QMTCFG="aliauth",0,"k0leyWHxYT1","Cat1","3af7bc8812cb475e042a0a5ae377c6a1"
[14:17:19.808] OK
[14:17:21.646] AT+QMTCFG="aliauth",0,"k0leyWHxYT1","Cat1","3af7bc8812cb475e042a0a5ae377c6a1"
[14:17:21.658] AT+QMTCFG="aliauth",0,"k0leyWHxYT1","Cat1","3af7bc8812cb475e042a0a5ae377c6a1"
[14:17:21.658] OK
[14:17:35.895] AT+QMTOPEN=0,"iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com",1883
[14:17:35.904] AT+QMTOPEN=0,"iot-06z00i8mcbcop1x.m
[14:17:35.908] qtt.iothub.aliyuncs.com",1883
[14:17:35.908] OK
[14:17:36.124]
[14:17:36.124] +QMTOPEN: 0,0
[14:17:43.147] AT+QMTCONN=0,0
[14:17:43.155] AT+QMTCONN=0,0
[14:17:43.155] OK
[14:17:43.454]
[14:17:43.454] +QMTCONN: 0,0,0
[14:17:54.846] AT+QMTOPEN?
[14:17:54.863] AT+QMTOPEN?
[14:17:54.863] +QMTOPEN: 0,"iot-06z00i8mcbcop1x.mqtt.iothub.aliyuncs.com",1883
[14:17:54.863]
[14:17:54.863] OK
[14:18:09.346] AT+QMTSUB=0,1,"/sys/k0leyWHxYT1/Cat1/thing/service/property/set",2
[14:18:09.360] AT+QMTSUB=0,1,"/sys/k0leyWHxYT1/Cat
[14:18:09.362] 1/thing/service/property/set",2
[14:18:09.362] OK
[14:18:09.532]
[14:18:09.532] +QMTSUB: 0,1,0,1
[14:18:31.346] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30
[14:18:31.357] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30
[14:18:31.357] >
[14:18:58.546] {
[14:18:58.546]   "id": "123",
[14:18:58.546]   "version": "1.0",
[14:18:58.546]   "sys": { "ack": 0 },
[14:18:58.546]   "params": { "LINK_TEST": 126 },
[14:18:58.546]   "method": "thing.event.property.post"
[14:18:58.546] }
[14:18:58.554] {
[14:18:58.554]   "id": "123",
[14:18:58.554]   "version":
[14:18:58.651]
[14:18:58.651] OK
[14:18:58.655]
[14:18:58.655] +QMTPUBEX: 0,0,0
[14:20:33.046]  AT+QMTPUBEX=?
[14:20:33.060] AT+QMTPUBEX=?
[14:20:33.060] +QMTPUBEX: (0-5), <msgid>,(0-2),(0,1),"topic","length"
[14:20:33.060]
[14:20:33.060] OK
[14:20:46.295] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30
[14:20:46.308] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30
[14:20:46.308] >
[14:20:55.896] {
[14:20:55.896]   "id": "123",
[14:20:55.896]   "version": "1.0",
[14:20:55.896]   "sys": { "ack": 0 },
[14:20:55.896]   "params": { "LINK_TEST": 129 },
[14:20:55.896]   "method": "thing.event.property.post"
[14:20:55.896] }
[14:20:55.904] {
[14:20:55.904]   "id": "123",
[14:20:55.904]   "version":
[14:20:56.002]
[14:20:56.002] OK
[14:20:56.006]
[14:20:56.006] +QMTPUBEX: 0,0,0
[14:23:19.496] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30
[14:23:19.507] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",30
[14:23:19.507] >
[14:23:28.396] {"id": "123","version": "1.0","sys": { "ack": 0 },"params": { "LINK_TEST": 129 },"method": "thing.event.property.post"}
[14:23:28.403] {"id": "123","version": "1.0",
[14:23:28.499]
[14:23:28.499] OK
[14:23:28.504]
[14:23:28.504] +QMTPUBEX: 0,0,0
[14:24:53.198] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",120
[14:24:53.210] AT+QMTPUBEX=0,0,0,0,"/sys/k0leyWHxYT1/Cat1/thing/event/property/post",120
[14:24:53.210] >
[14:25:02.946] {"id": "123","version": "1.0","sys": { "ack": 0 },"params": { "LINK_TEST": 129 },"method": "thing.event.property.post"}
[14:25:02.962] {"id": "123","version": "1.0","sys": { "ack": 0 },"params": { "LINK_TEST": 129 },"method": "thing.event.property.post"}
[14:25:03.057]
[14:25:03.057] OK
[14:25:03.061]
[14:25:03.061] +QMTPUBEX: 0,0,0
