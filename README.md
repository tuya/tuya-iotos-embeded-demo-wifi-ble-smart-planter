## Tuya IoTOS Embedded Demo Wi-Fi and BLE Indoor Garden

[中文版](README_zh.md) | [English](README.md)

## Overview

This Demo describes the implementation of a smart indoor gardan through the Tuya Smart Cloud platform, Tuya Smart app, and IoTOS embedded Wi-Fi and BLE SDK, using the Tuya Wi-Fi and Wi-Fi + BLE series modules to achieve smart functions such as remote control, data monitoring, and remote automatic control.

Implemented functions:

* Light monitoring
* Temperature and humidity monitoring
* Soil moisture monitoring
* Water level monitoring of water tank
* Automatic light filling
* Automatic humidification
* Automatic humidification
* Automatic ventilation
* Automatic filling of water tank
* Automatic watering



## Quick start

#### Download and compile

- Download [Tuya IoTOS embedded Wi-Fi & BLE SDK](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231t)

- Download the Demo to apps directory under the SDK directory

   ```bash
   $ cd apps
   $ git clone https://github.com/tuya tuya-iotos-embeded-demo-wifi-ble-smart-planter
   ```

- Execute the following command in the SDK root directory to start compiling:

   ```bash
   $ cd ../
   $ sh build_app.sh apps/tuya-iotos-embeded-demo-wifi-ble-smart-planter tuya-iotos-embeded-demo-wifi-ble-smart-planter 1.0.0
   ```

- For more information about firmware burning authorization, see [Burn and Authorize WB Series Modules](https://developer.tuya.com/en/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd)



#### Directory structure

```
├── src  
|    ├── plant_driver
|    |        ├── bh1750.c            // BH1750 light sensor module drive
|    |        ├── rs2255.c            // rs2255 analog switch chip related
|    |        ├── sht21.c             // SHT21 temperature and humidity sensor module driver
|    |        └── plant_pwm.c         // Driver light board related. Call the PWM API in SoC layer for repackaging 
|    ├── plant_soc                    // Tuya SDK API file in SoC layer
|    ├── tuya_device.c                // Application entry file
|    ├── app_plant.c                  // Main application layer of the smart indoor garden
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

Important function: device_init()

+ Call `tuya_iot_wf_soc_dev_init_param()` for SDK initialization to configure working mode and pairing mode, register callback functions, and save the firmware key and PID.

+ Call `tuya_iot_reg_get_wf_nw_stat_cb()` API to register callback of device network status.

+ All the initialization function in application payer `app_plant_init()`



#### DP related

+ Send DP data flow: `dev_obj_dp_cb() -> deal_dp_proc()`
+ Report DP API: `dev_report_dp_json_async()`

| Function name | `OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) ` |
| ------- | ------------------------------------------------------------ |
| devid | Device ID (devid = NULL if the device is a gateway, MCU, or SoC;  devid = sub-device_id if the device is sub-device) |
| dp_data | The name of DP struct array |
| cnt | The number of DP struct arrays. |
| Return | OPRT_OK: Succeeded  Other: Failed |



#### I/O list

| RS2255 | BH1750 | SHT21 | Water level sensor | Soil moisture sensor | Light board | Humidifier | Watering pump | Infrared bulb | Fan | Water tank pump |
| ------ | -------- | -------- | --------------- | --------------- | -------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| `A` P6 | `SDA` P6 | `SDA` P6 | `AO` channel_x1 | `AO` channel_x2 | `PON` P8 | `Relay` P24 | `Relay` P14 | `Relay` P20 | `Relay` P21 | `Relay` P22 |
| `B` P7 | `SCL` P7 | `SCL` P7 |  |  | `RON` P9 |  |  |  |  |  |

#### Evaluation Kits

For more information about SDK, see [Sandwich Evaluation Kits](https://developer.tuya.com/en/docs/iot/device-development/tuya-development-board-kit/tuya-sandwich-evaluation-kits/-tuya-sandwich-evaluation-kits?id=K97o0ixytemvr)

For detailed documentation, see [Fully-Functional Smart Indoor ](https://developer.tuya.com/en/demo/smart-planter)

## Technical support

You can get support from Tuya through the following accesses: 

- [Tuya IoT Developer Platform](https://developer.tuya.com/en/)
- [Tuya Smart Help Center](https://support.tuya.com/en/help)
- [Service & Support](https://service.console.tuya.com/)
