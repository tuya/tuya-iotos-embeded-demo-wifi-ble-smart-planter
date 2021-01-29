/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name:  soc_gpio.h
 * @Description: BK7231 gpio include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-06-14 09:58:37
 */
#ifndef __SOC_GPIO_H__
#define __SOC_GPIO_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @brief: SOC general pin Reset proc
 * @param {none}
 * @retval: none
 */
VOID vSocPinReset(UCHAR_T pin);

/**
 * @brief: SOC i2c ctrl set proc
 * @param {none}
 * @retval: none
 */
VOID vSocPinSet(UCHAR_T pin);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_GPIO_H__ */
