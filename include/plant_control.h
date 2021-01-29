#ifndef __PLANT_CONTROL_H__
#define __PLANT_CONTROL_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

typedef enum {
    both = 0,
    red,
    blue
}LIGHT_COLOR;


typedef enum {
    cancel = 0,
    set_1h,
    set_2h,
    set_3h,
    set_4h,
    set_5h,
    set_6h,
    set_7h,
    set_8h,
    set_9h,
    set_10h,
    set_11h,
    set_12h
}COUNTDOWN_SET;

typedef struct {
    BOOL_T Switch;
    BOOL_T Pump;
    BOOL_T Auto_switch;
    USHORT_T Bright_value;
    LIGHT_COLOR Light_color; 
    COUNTDOWN_SET Countdown_set;
    UCHAR_T Temp_max;
    UCHAR_T Temp_min;
    UCHAR_T Humidity_max;
    UCHAR_T Humidity_min;
    USHORT_T Soil_humidity_threshold;
}PLANT_CTRL_DATA_T;

typedef struct {
    UCHAR_T Temp_current;
    UCHAR_T Humidity_current;
    USHORT_T Countdown_left;
    UCHAR_T Water_remain;
    UCHAR_T fault;
}PLANT_REPORT_DATA_T;


/*********************************************************************************
 * FUNCTION:       plant_device_init
 * DESCRIPTION:    设备、IO初始化
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID plant_device_init(VOID);

/*********************************************************************************
 * FUNCTION:       plant_get_iic_sensor_data
 * DESCRIPTION:    获取I2C类传感器数据
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID plant_get_iic_sensor_data(VOID);

/*********************************************************************************
 * FUNCTION:       plant_get_adc_sensor_data
 * DESCRIPTION:    获取ADC类传感器数据
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID plant_get_adc_sensor_data(VOID);

/*********************************************************************************
 * FUNCTION:       plant_ctrl_handle
 * DESCRIPTION:    传感器数据解析、业务逻辑处理
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID plant_ctrl_handle(VOID);

/*********************************************************************************
 * FUNCTION:       plant_ctrl_all_off
 * DESCRIPTION:    关闭所有控制组件
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         在设备开关为FALSE时执行
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID plant_ctrl_all_off(VOID);

#endif