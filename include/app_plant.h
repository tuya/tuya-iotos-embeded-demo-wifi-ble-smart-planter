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

#ifndef __APP_PLANT_H__
#define __APP_PLANT_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/***********************************************************
*************************variable define********************
***********************************************************/
typedef enum{
    APP_PLANT_NORMAL,       // 正常工作模式
    APP_PLANT_PRODTEST      // 产测模式
}APP_PLANT_MODE;

#define DPID_SWITCH_P                1      // 设备总开关
#define DPID_PUMP                    2      // 水泵开关
#define DPID_TEMP_CURRENT            6      // 当前温度
#define DPID_HUMIDITY_CURRENT        7      // 当前湿度
#define DPID_COUNTDOWN_SET           8      // 定时设定值
#define DPID_COUNTDOWN_LEFT          9      // 定时剩余值
#define DPID_FAULT                   10     // 故障码
#define DPID_TEMP_MAX                101    // 最大温度设定值
#define DPID_HUMIDITY_MAX            102    // 最大湿度设定值
#define DPID_WATER_REMAIN            103    // 水箱水量
#define DPID_LIGHT_COLOR             104    // 灯光颜色
#define DPID_TEMP_MIN                105    // 最小温度设定值
#define DPID_HUMIDITY_MIN            106    // 最小湿度设定值
#define DPID_AUTO_LIGHT              107    // 自动补光开关


/*********************************************************************************
 * FUNCTION:       app_plant_init
 * DESCRIPTION:    应用初始化
 * INPUT:          mode:应用模式
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
OPERATE_RET app_plant_init(IN APP_PLANT_MODE mode);

/*********************************************************************************
 * FUNCTION:       deal_dp_proc
 * DESCRIPTION:    处理app下发数据
 * INPUT:          root:app下发数据结构体
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

/*********************************************************************************
 * FUNCTION:       app_report_all_dp_status
 * DESCRIPTION:    上报所有dp点数据
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         新增或者删除dp点功能时需修改变量dp_cnt
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID app_report_all_dp_status(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_INIT_H__ */