## Tuya IoTOS Embeded  Demo WiFi & BLE  Smart-Planter

[中文版](README_zh.md)|[English](README.md)

## Introduction

This Demo implements a smart WiFi plant grower through the Tuya Smart Cloud Platform, Tuya Smart APP, and IoTOS Embeded WiFi &BLE SDK, using the Tuya WiFi/WiFi+BLE series modules to achieve intelligent functions such as remote control, data monitoring, and remote automatic control.

Implemented functions include.

* Light monitoring
* Temperature and humidity monitoring
* Soil moisture monitoring
* Water level monitoring of water tank
* Automatic light replenishment
* Automatic humidification
* Automatic heating
* Automatic ventilation
* Automatic filling of water tank
* Automatic watering



## Get Started Quickly

### Download & Compile

- Download [Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231t)

- Download the Demo to the apps directory in the SDK directory

  ``` bash
  $ cd apps
  $ git clone https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-smart-planter
  ```

- Execute the following command in the SDK root directory to start compiling：

  ``` bash
  $ cd ../
  $ sh build_app.sh apps/tuya-iotos-embeded-demo-wifi-ble-smart-planter tuya-iotos-embeded-demo-wifi-ble-smart-planter 1.0.0
  ```

- For license information of firmware download, please refer to：[Wi-Fi + BLE series moduleBurning and Authorization](https://developer.tuya.com/en/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd)



### Directory & file

```
├── src	
|    ├── plant_driver
|    |        ├── bh1750.c            //BH1750 light sensing module driver
|    |        ├── rs2255.c            //rs2255 analog switch chip related
|    |        ├── sht21.c             //SHT21 temperature and humidity sensing module driver
|    |        └── plant_pwm.c         //Drive light board related, call the soc layer pwm interface 
                                        for repackaging
|    ├── plant_soc               //tuya SDK soc layer interface related files
|    ├── tuya_device.c           //Application entry file
|    ├── app_plant.c             //Main application layer of plant growth machine
|    └── plant_control.c         //The control logic of each functional component of the smart planter
                                   is related
|
├── include				//Header file directory
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
└── output              //Compilation products
```



### Demo entrance

Entrance file：tuya_device.c

Important Functions：`device_init()`

+ Call the `tuya_iot_wf_soc_dev_init_param()` interface to initialize the SDK, configure the operating mode, the mating mode, and register various callback functions and store the firmware key and PID.

+ Calling the `tuya_iot_reg_get_wf_nw_stat_cb()` interface to register the device network status callback functions.

+ Calling the application layer initialization function `app_plant_init()`

### DP related

+ Downstream dp point data: `dev_obj_dp_cb() -> deal_dp_proc()`
+ Report dp point interface: `dev_report_dp_json_async()`

| Function Name | `OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T` `*dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt`) |
| ------------- | ------------------------------------------------------------ |
| devid         | device id (if gateway, MCU, SOC class device then devid = NULL; if sub-device, then devid = sub-device_id) |
| dp_data       | dp structure array name                                      |
| cnt           | Number of elements of the dp structure array                 |
| Return        | OPRT_OK: Success Other: Failure                              |



##### I/O List

| RS2255 | BH1750   | SHT21    | Liquid level sensor | Soil moisture sensor | Supplementary light board | Humidifier  | Watering pump | Heat lamp   | Fan         | 水箱水泵    |
| ------ | -------- | -------- | ------------------- | -------------------- | ------------------------- | ----------- | ------------- | ----------- | ----------- | ----------- |
| `A` P6 | `SDA` P6 | `SDA` P6 | `AO` channel_x1     | `AO` channel_x2      | `PON` P8                  | `Relay` P24 | `Relay` P14   | `Relay` P20 | `Relay` P21 | `Relay` P22 |
| `B` P7 | `SCL` P7 | `SCL` P7 |                     |                      | `RON` P9                  |             |               |             |             |             |

### Evaluation Kits

For details on the development board kit, please refer to：[Tuya Sandwich Evaluation Kits](https://developer.tuya.com/en/docs/iot/device-development/tuya-development-board-kit/tuya-sandwich-evaluation-kits/-tuya-sandwich-evaluation-kits?id=K97o0ixytemvr)

For more detailed documentation, please refer to the Tuya Demo Center：https://developer.tuya.com/en/demo/smart-planter

## Technical Support

You can get support for Tuya by using the following methods:

- Developer Center：[https://developer.tuya.com/en/](https://developer.tuya.com/en/)
- Help Center: [https://support.tuya.com/en/help](https://support.tuya.com/en/help)
- Technical Support Console:[https://service.console.tuya.com](https://service.console.tuya.com/)
