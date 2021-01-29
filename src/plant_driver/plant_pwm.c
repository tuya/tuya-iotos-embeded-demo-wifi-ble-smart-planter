/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: plant_pwm.c
 * @Description: plant user pwm drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 20120-12-16 18:51:29
 */

#include "plant_pwm.h"
#include "BkDriverPwm.h"
#include "tuya_gpio.h"
#include "BkDriverGpio.h"

/***********************************************************
*************************pwm define*************************
***********************************************************/

#define PWM_RED_PORT                (8)
#define PWM_BLUE_PORT               (9)

#define PWM_FREQ                    10000


//STATIC USER_PWM_DUTY_T user_pwm_duty = {100,100};

#define PWM_MAX_FREQ    20000                   ///< PWM MAX Frequency 20K
#define PWM_MIN_FREQ    100                     ///< PWM MIN Frequency 100
#define PWM_MAX_DUTY    1000                    ///< PWM MAX Duty 1000 --> Precision 0.1%

STATIC USER_PWM_INIT_T pwm_init_config ;          ///< PWM Settings
STATIC UCHAR_T user_pwm_init_flag = FALSE;
STATIC UCHAR_T *pChannelList = NULL;


STATIC USHORT_T pwm_period = 0;

STATIC CONST UCHAR_T gpio_allow_list[6][2] = {
                                                {6,  0},
                                                {7,  1},
                                                {8,  2},
                                                {9,  3},
                                                {24, 4},
                                                {26, 5}
                                            };

OPERATE_RET plant_pwm_init(VOID)
{  
    
    OPERATE_RET opRet = -1;
    UCHAR_T i = 0, j = 0;
    USHORT_T duty = 0;
    
    pwm_init_config.usFreq = PWM_FREQ;
    pwm_init_config.usDuty = 100;
    pwm_init_config.ucList[0] = PWM_RED_PORT;
    pwm_init_config.ucList[1] = PWM_BLUE_PORT;
    pwm_init_config.ucChannelNum = 2;
    pwm_init_config.bPolarity = 1;

    pChannelList = (UCHAR_T *)Malloc( pwm_init_config.ucChannelNum * SIZEOF(UCHAR_T) );
    
    //BkGpioInitialize(pwm_init_config.ucList[0],GPIO_SECOND_FNNC);
    //BkGpioInitialize(pwm_init_config.ucList[1],GPIO_SECOND_FNNC);

    for(i = 0; i < pwm_init_config.ucChannelNum; i++) {
        for(j = 0; j < 6; j++) {
            if(pwm_init_config.ucList[i] == gpio_allow_list[j][0]) {
                pChannelList[i] = gpio_allow_list[j][1];
                break;
            }
        }
        if( j >= 6 ) {
            break;
        }
    }
    
    pwm_period = (USHORT_T) (26000000 / pwm_init_config.usFreq);
    duty = (USHORT_T) (pwm_init_config.usDuty / 1000.0 * pwm_period);

    for(i = 0; i < pwm_init_config.ucChannelNum; i++ ) {
        bk_pwm_initialize(pChannelList[i], pwm_period, duty);
        bk_pwm_stop(pChannelList[i]);
    }
    for(i = 0; i < pwm_init_config.ucChannelNum; i++ ) {
        bk_pwm_start(pChannelList[i]);
    }

    user_pwm_init_flag = TRUE;

    return OPRT_OK;
}

OPERATE_RET plant_pwm_set(USER_PWM_DUTY_T *pDutyData)
{
    FLOAT_T percent = 0.0;

    if(user_pwm_init_flag != TRUE){
        PR_ERR("user pwm not init!");
        return OPRT_INVALID_PARM;
    }

    if((pDutyData ->duty_red > PWM_MAX_DUTY) || (pDutyData ->duty_blue > PWM_MAX_DUTY)) {
        PR_ERR("USER PWM data is invalid!");
        return OPRT_INVALID_PARM;    
    }

    percent = (FLOAT_T)(pDutyData->duty_red/1000.0);
    bk_pwm_update_param(pChannelList[0], pwm_period, percent * pwm_period);
    bk_pwm_start(pChannelList[0]); 

    percent = (FLOAT_T)(pDutyData->duty_blue/1000.0);
    bk_pwm_update_param(pChannelList[1], pwm_period, percent * pwm_period);
    bk_pwm_start(pChannelList[1]);

    return OPRT_OK;
}