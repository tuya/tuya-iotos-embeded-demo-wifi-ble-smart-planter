#ifndef __PLANT_PWM_H__
#define __PLANT_PWM_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"



/**
 * @brief pwm init data structure
 * pwm send data structure
 */
typedef struct 
{
    USHORT_T usFreq;            ///< PWM Frequency
    USHORT_T usDuty;            ///< PWM Init duty
    UCHAR_T ucList[2];          ///< GPIO List 
    UCHAR_T ucChannelNum;       ///< GPIO List length
    BOOL_T bPolarity;           ///< PWM output polarity
    BOOL_T  bCCTFlag;           ///< CCT drive mode flag
}USER_PWM_INIT_T;

typedef struct 
{
    USHORT_T duty_red;         ///< R value,rang from 0 to 1000
    USHORT_T duty_blue;       ///< G value,rang from 0 to 1000
}USER_PWM_DUTY_T;


OPERATE_RET plant_pwm_init(VOID);

OPERATE_RET plant_pwm_set(USER_PWM_DUTY_T *pDutyData);

#endif