/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: rs2255.h
 * @Description: rs2255 drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 20120-12-16 18:51:29
 */

#include "tuya_gpio.h"
#include "rs2255.h"

#define RS2255_IN_A        (6)    // PWM0--PA6        
#define RS2255_IN_B        (7)    // PWM0--PA7

VOID rs2255_init(VOID)
{
    tuya_gpio_inout_set(RS2255_IN_A, FALSE);

    tuya_gpio_inout_set(RS2255_IN_B, FALSE);
}

OPERATE_RET rs2255_channel_checkout(rs2255_channel channel)
{   
    OPERATE_RET ret = 0;
    if(channel >= 4) {
        PR_ERR("channel out of range!");
        ret = -1;
        return ret;
    }

    switch (channel)
    {
    case channel_x0:
        ret = tuya_gpio_write(RS2255_IN_A, FALSE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x0 error!");
            return ;
        }

        ret = tuya_gpio_write(RS2255_IN_B, FALSE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x0 error!");
            return ;
        }
        break;
    
    case channel_x1:
        ret = tuya_gpio_write(RS2255_IN_A, FALSE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x1 error!");
            return ;
        }

        ret = tuya_gpio_write(RS2255_IN_B, TRUE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x1 error!");
            return ;
        }
        break;
    
    case channel_x2:
        ret = tuya_gpio_write(RS2255_IN_A, TRUE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x2 error!");
            return ;
        }

        ret = tuya_gpio_write(RS2255_IN_B, FALSE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x2 error!");
            return ;
        }
        break;
    
    case channel_x3:
        ret = tuya_gpio_write(RS2255_IN_A, TRUE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x3 error!");
            return ;
        }

        ret = tuya_gpio_write(RS2255_IN_B, TRUE);
        if(ret != OPRT_OK) {
            PR_ERR("checkout channel x3 error!");
            return ;
        }        
        break;
    
    default:
        break;
    }

    return ret;
}