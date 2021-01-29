#ifndef __BH1750_H__
#define __BH1750_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"




// sensor command
typedef enum{
    COMMAND_POWER_OFF                        = 0x00,
    COMMAND_POWER_ON                         = 0x01,
    COMMAND_RESET                            = 0x07,
    COMMAND_H_RESOLUTION_MODE                = 0x10,
    COMMAND_H_RESOLUTION_MODE_2              = 0x11,
    COMMAND_L_RESOLUTION_MODE                = 0x13,
    COMMAND_ONETIME_H_RESOLUTION_MODE        = 0x20,
    COMMAND_ONETIME_H_RESOLUTION_MODE_2      = 0x21,
    COMMAND_ONETIME_L_RESOLUTION_MODE        = 0x23,
}bh1750_command;



typedef struct 
{
    UCHAR_T SDA_PIN;            ///< SDA pin
    UCHAR_T SCL_PIN;            ///< SCL pin 
}bh1750_init_t;

UINT_T tuya_bh1750_get_bright_value(VOID);

UCHAR_T tuya_bh1750_init(bh1750_init_t* param);

#endif