/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: soc_pwm.c
 * @Description: Realtek 8710bn PWM proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-24 20:42:29
 * @LastEditTime: 2019-04-25 17:54:43
 */

#include "BkDriverPwm.h"
#include "soc_pwm.h"
#include "tuya_gpio.h"


#include "uart_pub.h"

STATIC UCHAR_T *pChannelList = NULL;
STATIC USHORT_T usPeriod = 0;
STATIC UCHAR_T *pPwmEnablelist = NULL;
STATIC UCHAR_T ucChannelNumer = 0;


/**
 * @berief: PWM SOC Init
 * @param {IN USHORT_T frequency --> PWM frequency,unit:HZ, attention: range：500 ~ 20KHz }
 * @param {IN USHORT_T positive_duty_cycle --> PWM Init Duty(positive duty),range:0 ~ 1000}
 * @param {IN UCHAR_T channel_num --> PWM channel num}
 * @param {IN UCHART_T *gpio_list --> PWM gpio set}
 * @retval: OPERATE_RET
 */
OPERATE_RET opSocPwmInit(IN USHORT_T frequency, IN USHORT_T positive_duty_cycle, IN UCHAR_T channel_num, IN UCHAR_T *gpio_list)
{
    OPERATE_RET opRet = 0;
    UCHAR_T i = 0, j = 0;
    USHORT_T usDuty = 0;

    CONST UCHAR_T ucGpioAllowList[6][2] = {
                                                {6,  0},
                                                {7,  1},
                                                {8,  2},
                                                {9,  3},
                                                {24, 4},
                                                {26, 5}
                                            };

    if(pChannelList != NULL) {
        PR_NOTICE("SOC pwm already init.");
        return OPRT_OK;
    }

    pChannelList = (UCHAR_T *)Malloc( channel_num * SIZEOF(UCHAR_T) );
    pPwmEnablelist = (UCHAR_T *)Malloc( channel_num * SIZEOF(UCHAR_T) );
    
    if( (pChannelList == NULL) || (pPwmEnablelist == NULL) ) {
        PR_ERR("SOC PWM Init Error!");
        goto INIT_ERROR;
    }

    if(channel_num > 5) {
        PR_ERR("bk7231 enable max pwm num is 5! set channel num error!");
        goto INIT_ERROR;
    }

    if((frequency < 500) && (frequency > 20000)) {      /* frequency check range */
        PR_ERR("bk7231 pwm frequency range: 500 ~ 20KHz, set frequency error!");
        goto INIT_ERROR;
    }
    
    /* check gpio allowed to initilize pwm */
    for(i = 0; i < channel_num; i++) {
        for(j = 0; j < 6; j++) {
            if(gpio_list[i] == ucGpioAllowList[j][0]) {
                pChannelList[i] = ucGpioAllowList[j][1];
                break;
            }
        }
        if( j >= 6 ) {
            break;
        }
    }
    if( i < channel_num ) {
        PR_ERR("PWM GPIO List is illegal!");
        goto INIT_ERROR;
    }
    ucChannelNumer = channel_num;

    usPeriod = (USHORT_T) (26000000 / frequency);
    usDuty = (USHORT_T) (positive_duty_cycle / 1000.0 * usPeriod);
    
    for(i = 0; i < channel_num; i++ ) {
        bk_pwm_initialize(pChannelList[i], usPeriod, usDuty);
        bk_pwm_stop(pChannelList[i]);
        *(pPwmEnablelist + i) = FALSE;
    }
    if(usDuty <= 0) {
        return OPRT_OK;
    }

    for(i = 0; i < channel_num; i++ ) {
        bk_pwm_start(pChannelList[i]);
    }
    
    
    return OPRT_OK;

INIT_ERROR:

    if(pChannelList != NULL) {
        Free(pChannelList);
        pChannelList = NULL;
    }
    
    if(pPwmEnablelist != NULL) {
        Free(pPwmEnablelist);
        pPwmEnablelist = NULL;
    }

    return OPRT_INVALID_PARM;
}

/**
 * @berief: PWM SOC SET Duty
 * @param {IN UCHAR_T channel -> pwm send out channel num}
 * @param {IN USHORT_T duty ->  pwm send duty,rang: 0~1000}
 * @attention channel --> according to the initlize order
 * @return: none
 * @retval: none
 */
VOID vSocPwmSetDuty(IN UCHAR_T channel, IN USHORT_T duty) 
{
    FLOAT_T fPercent = 0.0;

    if( (pChannelList == NULL) || (pPwmEnablelist == NULL) ){
        PR_ERR("PWM not init or init error,please init firstly!");
        return;
    }

    if(channel >= ucChannelNumer) {
        PR_ERR("cann't set pwm, because this channel not init!");
        return;
    }

    if(duty <= 0) {    /* duty <= 0 stop pwm, to reduce power */
        if( TRUE == *(pPwmEnablelist + channel) ) {
            bk_pwm_stop(pChannelList[channel]);
            *(pPwmEnablelist + channel) = FALSE;
        } else {
            ;
        }

    } else {
    
        fPercent = (FLOAT_T) (duty / 1000.0);
        if( *(pPwmEnablelist + channel) != TRUE ) {    /* channel pwm already stop, init pwm func again! */
            bk_pwm_update_param(pChannelList[channel], usPeriod, fPercent * usPeriod);
            *(pPwmEnablelist + channel) = TRUE;
        }
        
#if 0
        if((ucChannelNumer == 2) && (channel == 1)) {       /* 2way, W need to send negative pwm */
            duty = 1000 - duty;
            pwmout_set_polarity( (pPwmInfo + channel), 0);
        }

        if((ucChannelNumer == 5) && (channel == 4)) {       /* 5way, W need to send negative pwm */
            duty = 1000 - duty;
            pwmout_set_polarity( (pPwmInfo + channel), 0);
        }
#endif
        
        bk_pwm_update_param(pChannelList[channel], usPeriod, fPercent * usPeriod);
        bk_pwm_start(pChannelList[channel]);
    }

}




