/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name:  soc_i2c.h
 * @Description: bk7213 iic include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-06-14 09:58:37
 */
#ifndef __SOC_I2C_H__
#define __SOC_I2C_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @brief IIC gpio structure
 */
typedef struct 
{
    UCHAR_T ucSDA_IO;
    UCHAR_T ucSCL_IO;
}i2c_pin_t;

/**
 * @berief: SOC i2c init
 * @param {i2c_pin_t i2c_pin --> i2c init gpio struct}
 * @return: none
 * @retval: none
 */
OPERATE_RET opSocI2CInit(IN i2c_pin_t *i2c_pin);

/**
 * @berief: SOC i2c SDA reset proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSDAReset(VOID);

/**
 * @berief: SOC i2c SDA set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSDASet(VOID);

/**
 * @berief: SOC i2c SCL set proc
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
VOID vI2CSCLReset(VOID);

/**
 * @berief: SOC i2c SCL set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSCLSet(VOID);

VOID vI2CSCLRelease(VOID);

VOID vI2CSDARelease(VOID);
/**
 * @berief: SOC i2c SDA(output) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSDAOutputRead(VOID);

/**
 * @berief: SOC i2c SDA(input) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSDAInputRead(VOID);

/**
 * @berief: SOC i2c SCL(input) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSCLInputRead(VOID);

/**
 * @berief: SOC i2c delay
 * @param {none}
 * @attention: delay time need to adapter according to system
 *              e.g: 
 *                  usDelayTime = usDelayTime * 2;
 *                  usDelayTime = usDelayTime * 0.6;
 * @retval: none
 */
VOID vI2CDelay(USHORT_T usDelayTime);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_I2C_H__ */
