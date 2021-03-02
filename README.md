## Tuya IoTOS Wi-Fi and BLE Indoor Garden Demo

[中文版](README_zh.md) | [English](README.md)

## Overview

This demo describes how to implement a smart indoor garden through Tuya IoTOS SDK. With Tuya Cloud, Tuya Smart app, and Tuya Wi-Fi or Wi-Fi+BLE modules, you can easily implement IoT-enabled functions such as remote control, data monitoring, and remote control.

Implemented functions:

* Light monitoring
* Temperature and humidity monitoring
* Soil moisture monitoring
* Water tank level monitoring
* Automatic light filling
* Automatic spraying
* Automatic heating
* Automatic ventilating
* Automatic water tank filling
* Automatic watering



## Quick start

#### Download and compile

- Download [Tuya IoTOS Wi-Fi and BLE SDK](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231t)

- Download the demo to `apps` directory under the SDK directory

  ``` bash
  $ cd apps
  $ git clone https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-smart-planter
  ```

- Execute the following command in the SDK root directory to start compiling:

  ``` bash
  $ cd ../
  $ sh build_app.sh apps/tuya-iotos-embeded-demo-wifi-ble-smart-planter tuya-iotos-embeded-demo-wifi-ble-smart-planter 1.0.0
  ```

- For more information about burning and authorizing module, see [Burn and Authorize WB Series Modules](https://developer.tuya.com/en/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd).



#### Directory structure

```
├── src  
|    ├── plant_driver
|    |        ├── bh1750.c            // BH1750 light sensor driver
|    |        ├── rs2255.c            // RS2255 analog multiplexer
|    |        ├── sht21.c             // SHT21 humidity and temperature sensor driver
|    |        └── plant_pwm.c         // Light board driver. Call the PWM API in the SoC layer for repackaging
|    ├── plant_soc                    // Tuya SDK APIs in the SoC layer
|    ├── tuya_device.c                // Application entry file
|    ├── app_plant.c                  // Main application layers of the smart indoor garden
|    └── plant_control.c              // The control logic of each functional component of the smart indoor garden
|
├── include                           // Header file directory
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
└── output                            // Compilation output
```

#### Demo entry 

Entry file: tuya_device.c

Important functions: `device_init()`

+ Call `tuya_iot_wf_soc_dev_init_param()` to initialize the SDK, configure the operating mode and pairing mode, register callback functions, and store the firmware key and PID.
+ Call `tuya_iot_reg_get_wf_nw_stat_cb()` to register the network status callback function.
+ Call application layer initialization function `app_plant_init()`.


#### DP related

+ Data flow of DP sending: `dev_obj_dp_cb() -> deal_dp_proc()`
+ API of DP reporting: `dev_report_dp_json_async()`

| Function name | `OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) ` |
| ------- | ------------------------------------------------------------ |
| devid | Device ID. If the device is a gateway, MCU, or SoC, `devid` is `NULL`. If the device is sub-device, `devid` is `sub-device_id`.|
| dp_data | The name of DP struct array. |
| cnt | The number of DP struct arrays. |
| Return | OPRT_OK: Succeeded. Other: Failed. |



#### I/O list

| RS2255 | BH1750 | SHT21 | Water level sensor | Soil moisture sensor | Light board | Mist maker | Watering pump | Infrared bulb | Fan | Water tank pump |
| ------ | -------- | -------- | --------------- | --------------- | -------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `A` P6 | `SDA` P6 | `SDA` P6 | `AO` channel_x1 | `AO` channel_x2 | `PON` P8 | `Relay` P24 | `Relay` P14 | `Relay` P20 | `Relay` P21 | `Relay` P22 |
| `B` P7 | `SCL` P7 | `SCL` P7 |  |  | `RON` P9 |  |  |  |  |  |

#### Evaluation kits

For more information about SDK, see [Sandwich Evaluation Kits](https://developer.tuya.com/en/docs/iot/device-development/tuya-development-board-kit/tuya-sandwich-evaluation-kits/-tuya-sandwich-evaluation-kits?id=K97o0ixytemvr).

For the full document, see [Fully-Functional Smart Indoor Garden](https://developer.tuya.com/en/demo/smart-planter).

## Technical support

You can get support from Tuya through the following accesses: 

- [Tuya IoT Developer Platform](https://developer.tuya.com/en/)
- [Tuya Smart Help Center](https://support.tuya.com/en/help)
- [Service & Support](https://service.console.tuya.com/)
