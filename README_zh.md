## Tuya IoTOS Embeded  Demo WiFi & BLE  Smart-Planter

[中文版](README_zh.md) | [English](README.md)

## 简介

本Demo通过涂鸦智能云平台、涂鸦智能APP、IoTOS Embeded WiFi &Ble SDK实现一款智能WiFi植物生长机，使用涂鸦WiFi/WiFi+BLE系列模组，实现远程遥控、数据监测、远程自动控制等智能化功能。

已实现功能包括：

* 光照监测
* 温湿度监测
* 土壤湿度监测
* 水箱水位监测
* 自动补光
* 自动加湿
* 自动加温
* 自动通风
* 水箱自动加水
* 自动浇水



## 快速上手

### 下载与编译

- 下载[Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231t)

- 下载Demo至SDK目录的apps目录下

  ``` bash
  $ cd apps
  $ git clone https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-smart-planter
  ```

- 在SDK根目录下执行以下命令开始编译：

  ``` bash
  $ cd ../
  $ sh build_app.sh apps/tuya-iotos-embeded-demo-wifi-ble-smart-planter tuya-iotos-embeded-demo-wifi-ble-smart-planter 1.0.0
  ```

- 固件烧录授权相关信息请参考：[Wi-Fi + BLE 系列模组烧录授权](https://developer.tuya.com/cn/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd)



### 目录结构

```
├── src	
|    ├── plant_driver
|    |        ├── bh1750.c            //BH1750光照传感模块驱动
|    |        ├── rs2255.c            //rs2255模拟开关芯片相关
|    |        ├── sht21.c             //SHT21温湿度传感模块驱动
|    |        └── plant_pwm.c         //驱动灯板相关，调用soc层的pwm接口进行再封装
|    ├── plant_soc               //tuya SDK soc层接口相关文件
|    ├── tuya_device.c           //应用入口文件
|    ├── app_plant.c             //植物生长机主要应用层
|    └── plant_control.c         //植物生长机各个功能组件的控制逻辑相关
|
├── include				//头文件目录
|    ├── plant_driver
|    |        ├── bh1750.h
|    |        ├── rs2255.h
|    |        ├── sht21.h
|    |        └── plant_pwm.h
|    ├── plant_soc
|    ├── tuya_device.h
|    ├── app_plant.h
|    └── plant_control.h
|
└── output              //编译产物
```



### Demo入口

入口文件：tuya_device.c

重要函数：device_init()

+ 调用 `tuya_iot_wf_soc_dev_init_param()` 接口进行SDK初始化，配置了工作模式、配网模式，同时注册了各种回调函数并存入了固件key和PID。

+ 调用 `tuya_iot_reg_get_wf_nw_stat_cb()` 接口注册设备网络状态回调函数。

+ 调用应用层初始化函数 `app_plant_init()`

  

### DP点相关

+ 下发dp点数据流：`dev_obj_dp_cb() -> deal_dp_proc()`
+ 上报dp点接口: `dev_report_dp_json_async()`

| 函数名  | `OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) `|
| ------- | ------------------------------------------------------------ |
| devid   | 设备id（若为网关、MCU、SOC类设备则devid = NULL;若为子设备，则devid = sub-device_id) |
| dp_data | dp结构体数组名                                               |
| cnt     | dp结构体数组的元素个数                                       |
| Return  | OPRT_OK: 成功  Other: 失败                                   |



##### I/O 列表

| RS2255 | BH1750   | SHT21    | 水位传感器      | 土壤湿度传感    | 补光灯板 | 加湿器      | 浇水水泵    | 加热灯      | 风扇        | 水箱水泵    |
| ------ | -------- | -------- | --------------- | --------------- | -------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `A` P6 | `SDA` P6 | `SDA` P6 | `AO` channel_x1 | `AO` channel_x2 | `PON` P8 | `Relay` P24 | `Relay` P14 | `Relay` P20 | `Relay` P21 | `Relay` P22 |
| `B` P7 | `SCL` P7 | `SCL` P7 |                 |                 | `RON` P9 |             |             |             |             |             |

### 开发套件

开发板套件相关详情请参考：[三明治开发套件](https://developer.tuya.com/cn/docs/iot/device-development/tuya-development-board-kit/tuya-sandwich-evaluation-kits/-tuya-sandwich-evaluation-kits?id=K97o0ixytemvr)

更多详细文档，请参考涂鸦Demo中心：[https://developer.tuya.com/cn/demo/smart-planter](https://developer.tuya.com/cn/demo/smart-planter)

## 技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心:  [https://developer.tuya.com/cn/] (https://developer.tuya.com/cn/)
- 帮助中心: [https://support.tuya.com/en/help](https://support.tuya.com/cn/help)
- 技术支持工单中心:[https://service.console.tuya.com](https://service.console.tuya.com/)
