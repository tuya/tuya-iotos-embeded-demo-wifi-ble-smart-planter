/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name:  soc_i2c.c
 * @Description: Realtek 8710bn IIC gpio proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-04-25 17:21:28
 */

#include "soc_i2c.h"
#include "tuya_gpio.h"



STATIC BOOL_T i2c_init_flag = FALSE;
STATIC i2c_pin_t g_i2c_gpio;
STATIC UCHAR_T ucSDAStatus = FALSE;

/**
 * @berief: SOC i2c init
 * @param {i2c_pin_t i2c_pin --> i2c init gpio struct}
 * @return: none
 * @retval: none
 */
OPERATE_RET opSocI2CInit(IN i2c_pin_t *i2c_pin)
{
    OPERATE_RET opRet = 0;

    if(i2c_init_flag != FALSE) {
        PR_NOTICE("SOC I2C already init!");
        return OPRT_OK;
    }
    
    opRet = tuya_gpio_inout_set(i2c_pin ->ucSDA_IO, TRUE);      /* input */
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c sda gpio init error!");
        return OPRT_INVALID_PARM;
    }

    
    opRet = tuya_gpio_inout_set(i2c_pin ->ucSCL_IO, TRUE);      /* input */
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c scl gpio init error!");
        return OPRT_INVALID_PARM;
    }
    

    
    memset(&g_i2c_gpio, 0, SIZEOF(i2c_pin_t));
    memcpy(&g_i2c_gpio, i2c_pin, SIZEOF(i2c_pin_t));

    i2c_init_flag = TRUE;
    
    return OPRT_OK;
}

/**
 * @berief: SOC i2c SDA reset proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSDAReset(VOID)
{
    OPERATE_RET opRet = 0;
    
    if(i2c_init_flag != TRUE){
        PR_ERR("I2C is not't init!");
        return ;
    }

    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSDA_IO, FALSE);      /* output */
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c sda gpio set output error!");
        return ;
    }
    
    opRet = tuya_gpio_write(g_i2c_gpio.ucSDA_IO, FALSE);
    if(opRet != OPRT_OK) {
        PR_ERR("I2C SDA gpio set error!");
        return ;
    }
    ucSDAStatus = FALSE;
}

/**
 * @berief: SOC i2c SDA set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSDASet(VOID)
{
    OPERATE_RET opRet = 0;
    
    if(i2c_init_flag != TRUE){
        PR_ERR("I2C is not't init!");
        return ;
    }
    
    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSDA_IO, TRUE);      /* input */
    
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c sda gpio set input error!");
        return ;
    }
    
    /*
    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSDA_IO, FALSE);      /* output */
    /*
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c sda gpio set output error!");
        return ;
    }
    
    opRet = tuya_gpio_write(g_i2c_gpio.ucSDA_IO, TRUE);
    if(opRet != OPRT_OK) {
        PR_ERR("I2C SDA gpio set error!");
        return ;
    }
    */
    ucSDAStatus = TRUE;
}

/**
 * @berief: SOC i2c SCL set proc
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
VOID vI2CSCLReset(VOID)
{
    OPERATE_RET opRet = 0;

    if(i2c_init_flag != TRUE){
        PR_ERR("I2C is not't init!");
        return ;
    }

    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSCL_IO, FALSE);      /* output */
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c scl gpio set output error!");
        return ;
    }
    
    opRet = tuya_gpio_write(g_i2c_gpio.ucSCL_IO, FALSE);
    if(opRet != OPRT_OK) {
        PR_ERR("I2C scl gpio set error!");
        return ;
    }
}

/**
 * @berief: SOC i2c SCL set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSCLSet(VOID)
{
    OPERATE_RET opRet = 0;

    if(i2c_init_flag != TRUE){
        PR_ERR("I2C is not't init!");
        return ;
    }
    
    
    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSCL_IO, TRUE);      /* input */
    
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c scl gpio set input error!");
        return ;
    }
    
    /*
    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSCL_IO, FALSE);      /* output */
    /*
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c sda gpio set output error!");
        return ;
    }
    
    opRet = tuya_gpio_write(g_i2c_gpio.ucSCL_IO, TRUE);
    if(opRet != OPRT_OK) {
        PR_ERR("I2C SDA gpio set error!");
        return ;
    }
    */
}

VOID vI2CSCLRelease(VOID)
{
    OPERATE_RET opRet = 0;

    if(i2c_init_flag != TRUE){
        PR_ERR("I2C is not't init!");
        return ;
    }

    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSCL_IO, TRUE);      /* input */
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c scl gpio release error!");
        return ;
    }
}

VOID vI2CSDARelease(VOID)
{
    OPERATE_RET opRet = 0;

    if(i2c_init_flag != TRUE){
        PR_ERR("I2C is not't init!");
        return ;
    }

    opRet = tuya_gpio_inout_set(g_i2c_gpio.ucSDA_IO, TRUE);      /* input */
    if(opRet != OPRT_OK){
        PR_ERR("soc i2c sda gpio release error!");
        return ;
    }
}

/**
 * @berief: SOC i2c SDA(output) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSDAOutputRead(VOID)
{
    return ucSDAStatus;
}

/**
 * @berief: SOC i2c SDA(input) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSDAInputRead(VOID)
{

    return tuya_gpio_read(g_i2c_gpio.ucSDA_IO);
    //return ucSDAStatus;
}

/**
 * @berief: SOC i2c SCL(input) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSCLInputRead(VOID)
{
    return tuya_gpio_read(g_i2c_gpio.ucSCL_IO);
}

/**
 * @berief: SOC i2c delay
 * @param {none}
 * @attention: delay time need to adapter according to system
 *              e.g: 
 *                  usDelayTime = usDelayTime * 2;
 *                  usDelayTime = usDelayTime * 0.6;
 * @retval: none
 */
VOID vI2CDelay(USHORT_T usDelayTime)
{
    while(usDelayTime)
    {
        usDelayTime --;
    }
}

