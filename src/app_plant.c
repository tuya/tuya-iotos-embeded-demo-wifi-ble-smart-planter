/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: light_init.h
 * @Description: light init process include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-21 11:30:03
 * @LastEditTime: 2020-12-21 11:30:03
 */

#include "app_plant.h"
#include "plant_control.h"
#include "uni_time_queue.h"
#include "sys_timer.h"
#include "tuya_iot_wifi_api.h"
#include "FreeRTOS.h"
#include "tuya_hal_thread.h"
#include "uni_thread.h"
#include "queue.h"

#define TASKDELAY_SEC         500

extern PLANT_CTRL_DATA_T plant_ctrl_data;
extern PLANT_REPORT_DATA_T plant_report_data;

STATIC VOID sensor_data_get_iic_theard(PVOID_T pArg);
STATIC VOID sensor_data_get_adc_theard(PVOID_T pArg);
STATIC VOID sensor_data_deal_theard(PVOID_T pArg);
STATIC VOID sensor_data_report_theard(PVOID_T pArg);



OPERATE_RET app_plant_init(IN APP_PLANT_MODE mode)
{
    OPERATE_RET op_ret = OPRT_OK;


    if(APP_PLANT_NORMAL == mode) {
        
        plant_device_init();
        
        // 创建I2C类传感器数据采集任务
        tuya_hal_thread_create(NULL, "thread_data_get_iic", 512*4, TRD_PRIO_3, sensor_data_get_iic_theard, NULL);

        // 创建ADC类传感器数据采集任务
        tuya_hal_thread_create(NULL, "thread_data_get_adc", 512*4, TRD_PRIO_4, sensor_data_get_adc_theard, NULL);

        // 创建数据处理任务
        tuya_hal_thread_create(NULL, "thread_data_deal", 512*4, TRD_PRIO_4, sensor_data_deal_theard, NULL);

        // 创建dp点数据定时循环上报任务
        tuya_hal_thread_create(NULL, "thread_data_report", 512*4, TRD_PRIO_4, sensor_data_report_theard, NULL);
    }else {
        // 非产测模式
    }

    return op_ret;
}

STATIC VOID sensor_data_get_iic_theard(PVOID_T pArg)
{   
    while(1) {

        PR_DEBUG("plant_get_i2c_sensor_data");
        vTaskDelay(TASKDELAY_SEC);

        if(TRUE == plant_ctrl_data.Switch) {    
            plant_get_iic_sensor_data();
        }
        
    }
}

STATIC VOID sensor_data_get_adc_theard(PVOID_T pArg)
{   
    while(1) {

        PR_DEBUG("plant_get_adc_sensor_data");
        vTaskDelay(TASKDELAY_SEC*2);

        if(TRUE == plant_ctrl_data.Switch) {
            plant_get_adc_sensor_data();
        }
        
    }
}

STATIC VOID sensor_data_deal_theard(PVOID_T pArg)
{   
    while(1) {

        vTaskDelay(TASKDELAY_SEC);

        if(TRUE == plant_ctrl_data.Switch) {
            plant_ctrl_handle();
        }else {
            plant_ctrl_all_off();
        }
        
    }

}

STATIC VOID sensor_data_report_theard(PVOID_T pArg)
{   
    while(1) {

        vTaskDelay(TASKDELAY_SEC*5);

        app_report_all_dp_status();
    }

}


VOID app_report_all_dp_status(VOID)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 0;
    dp_cnt = 12;

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_SWITCH_P;
    dp_arr[0].type = PROP_BOOL;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_value = plant_ctrl_data.Switch;

    dp_arr[1].dpid = DPID_PUMP;
    dp_arr[1].type = PROP_BOOL;
    dp_arr[1].time_stamp = 0;
    dp_arr[1].value.dp_value = plant_ctrl_data.Pump;

    dp_arr[2].dpid = DPID_AUTO_LIGHT;
    dp_arr[2].type = PROP_BOOL;
    dp_arr[2].time_stamp = 0;
    dp_arr[2].value.dp_value = plant_ctrl_data.Auto_switch;

    dp_arr[3].dpid = DPID_TEMP_CURRENT;
    dp_arr[3].type = PROP_VALUE;
    dp_arr[3].time_stamp = 0;
    dp_arr[3].value.dp_value = plant_report_data.Temp_current;

    dp_arr[4].dpid = DPID_HUMIDITY_CURRENT;
    dp_arr[4].type = PROP_VALUE;
    dp_arr[4].time_stamp = 0;
    dp_arr[4].value.dp_value = plant_report_data.Humidity_current;

    dp_arr[5].dpid = DPID_TEMP_MAX;
    dp_arr[5].type = PROP_VALUE;
    dp_arr[5].time_stamp = 0;
    dp_arr[5].value.dp_value = plant_ctrl_data.Temp_max;

    dp_arr[6].dpid = DPID_TEMP_MIN;
    dp_arr[6].type = PROP_VALUE;
    dp_arr[6].time_stamp = 0;
    dp_arr[6].value.dp_value = plant_ctrl_data.Temp_min;

    dp_arr[7].dpid = DPID_HUMIDITY_MAX;
    dp_arr[7].type = PROP_VALUE;
    dp_arr[7].time_stamp = 0;
    dp_arr[7].value.dp_value = plant_ctrl_data.Humidity_max;

    dp_arr[8].dpid = DPID_HUMIDITY_MIN;
    dp_arr[8].type = PROP_VALUE;
    dp_arr[8].time_stamp = 0;
    dp_arr[8].value.dp_value = plant_ctrl_data.Humidity_min;

    dp_arr[9].dpid = DPID_COUNTDOWN_LEFT;
    dp_arr[9].type = PROP_VALUE;
    dp_arr[9].time_stamp = 0;
    dp_arr[9].value.dp_value = plant_report_data.Countdown_left;

    dp_arr[10].dpid = DPID_COUNTDOWN_SET;
    dp_arr[10].type = PROP_ENUM;
    dp_arr[10].time_stamp = 0;
    dp_arr[10].value.dp_value = plant_ctrl_data.Countdown_set;

    dp_arr[11].dpid = DPID_WATER_REMAIN;
    dp_arr[11].type = PROP_VALUE;
    dp_arr[11].time_stamp = 0;
    dp_arr[11].value.dp_value = plant_report_data.Water_remain;
    
    op_ret = dev_report_dp_json_async(NULL,dp_arr,dp_cnt);
    Free(dp_arr);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    PR_DEBUG("dp_query report_all_dp_data");
    return;
}


VOID deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    UCHAR_T dpid;

    dpid = root->dpid;
    PR_DEBUG("dpid:%d",dpid);
    
    switch (dpid) {
    
    case DPID_SWITCH_P:
        PR_DEBUG("set switch:%d",root->value.dp_bool);
        plant_ctrl_data.Switch = root->value.dp_bool;
        break;
        
    case DPID_PUMP:
        PR_DEBUG("set pump:%d",root->value.dp_bool);
        plant_ctrl_data.Pump = root->value.dp_bool;
        break;
    
    case DPID_TEMP_MAX:
        PR_DEBUG("set temp max:%d",root->value.dp_value);
        plant_ctrl_data.Temp_max = root->value.dp_value;
        break;
    
    case DPID_HUMIDITY_MAX:
        PR_DEBUG("set humidity max:%d",root->value.dp_value);
        plant_ctrl_data.Humidity_max = root->value.dp_value;
        break;
    
    case DPID_TEMP_MIN:
        PR_DEBUG("set temp min:%d",root->value.dp_value);
        plant_ctrl_data.Temp_min = root->value.dp_value;
        break;

    case DPID_HUMIDITY_MIN:
        PR_DEBUG("set humidity min:%d",root->value.dp_value);
        plant_ctrl_data.Humidity_min = root->value.dp_value;
        break;
    
    case DPID_LIGHT_COLOR:
        PR_DEBUG("set light color:%d",root->value.dp_enum);
        plant_ctrl_data.Light_color = root->value.dp_enum;
        break;
    
    case DPID_AUTO_LIGHT:
        PR_DEBUG("set auto light:%d",root->value.dp_bool);
        plant_ctrl_data.Auto_switch = root->value.dp_bool;
        break;
        
    case DPID_COUNTDOWN_SET:
        PR_DEBUG("set timr:%d",root->value.dp_enum);
        plant_ctrl_data.Countdown_set = root->value.dp_enum;
        break;
    default:
        break;
    }

    return;

}