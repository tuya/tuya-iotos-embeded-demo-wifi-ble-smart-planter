#ifndef __RS2255_H__
#define __RS2255_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

typedef enum{
    channel_x0,
    channel_x1,
    channel_x2,
    channel_x3
}rs2255_channel;

VOID rs2255_init(VOID);
OPERATE_RET rs2255_channel_checkout(rs2255_channel channel);

#endif