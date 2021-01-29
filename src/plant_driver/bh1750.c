/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: bh1750.h
 * @Description: bh1750 IIC drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 20120-12-16 18:51:29
 */

#include "bh1750.h"
#include "soc_i2c.h"

#define	  SLAVE_ADDRESS   0x46 // 器件从地址： ALT ADDRESS引脚接地时为 0x46 ,引脚接电源时地址为 0xB8;

// I2C acknowledge
typedef enum{
  ACK                      = 0,
  NO_ACK                   = 1,
}bh1750_ack;

STATIC UCHAR_T buf[8] = {0};
/**
 * @brief:  BH1750 IIC delay proc
 * @param:  delay time(us) 
 * @retval: none
 */
STATIC VOID __bh1750_delay(USHORT_T usTime)
{       
    vI2CDelay(usTime);
}

/**
 * @brief: BH1750 IIC start transport
 * @param {none}
 * @retval: 
 */
STATIC int __bh1750_start(VOID)
{
    vI2CSDASet();
    vI2CSCLSet();

    __bh1750_delay(5);

    vI2CSDAReset();

    __bh1750_delay(5);

    if(ucI2CSDAOutputRead()) {    /* confirm SDA status is set correct */
        PR_NOTICE("BH1750 start error!");
        return -1;
    }
    vI2CSCLReset();

    return 0;
}

/**
 * @brief: BH1750 IIC stop transport
 * @param {none}
 * @retval: 
 */
STATIC int __bh1750_stop(VOID)
{
    vI2CSDAReset();
    vI2CSCLReset();

    if(ucI2CSDAInputRead()) {    /* confirm SDA status is ready */
        PR_NOTICE("BH1750 stop1 error!");
        return -1;
    }
    
    vI2CSCLSet();

    __bh1750_delay(5);

    vI2CSDASet();

    if(!ucI2CSDAOutputRead()) {    /* confirm SDA status is set correct */
        PR_NOTICE("BH1750 stop2 error!");
        return -1;
    }

    __bh1750_delay(5);

    return 0;        
}

STATIC VOID __bh1750_send_ack(bh1750_ack ack)
{
    if(TRUE == ack) {
        vI2CSDASet();
    }else {
        vI2CSDAReset();
    }
    vI2CSCLSet();

    __bh1750_delay(5);

    vI2CSCLReset();

    __bh1750_delay(5);      
}

STATIC BOOL_T __bh1750_recv_ack(VOID)
{   
    BOOL_T recv_ack;

    vI2CSCLSet();

    __bh1750_delay(5);
    
    recv_ack = ucI2CSDAInputRead();
    vI2CSCLReset();

    __bh1750_delay(5);
    
    return recv_ack;
}

/**
 * @brief: BH1750 IIC send byte
 * @param {UCHAR_T sendbyte -> send one byte}
 * @retval: none
 */
STATIC VOID __bh1750_send_byte(UCHAR_T sendbyte)
{
    UCHAR_T i = 0;

    for(i = 0x80; i > 0; i >>= 1)
    {
        if((sendbyte & i) == 0) {
            vI2CSDAReset();
        } else {
            vI2CSDASet();
        }
        __bh1750_delay(1);   
        vI2CSCLSet();
        __bh1750_delay(5);
        vI2CSCLReset();
        __bh1750_delay(1);

    }

    vI2CSDASet();
    __bh1750_recv_ack();
}

/**
 * @brief: BH1750 IIC read byte
 * @param {none}
 * @retval: none
 */
STATIC UCHAR_T __bh1750_read_byte(VOID)
{
    UCHAR_T i = 0;
    
    UCHAR_T readbyte = 0;

    vI2CSDASet();

    for(i = 0x80; i > 0; i >>= 1)
    {
        vI2CSCLSet();
        __bh1750_delay(5);

        if(ucI2CSDAInputRead()) {
            readbyte |= i;    
        }

        vI2CSCLReset();
        __bh1750_delay(5);
    }
  
    return readbyte;
}

STATIC VOID __bh1750_send_cmd(bh1750_command cmd)
{
    __bh1750_start();                  
    __bh1750_send_byte(SLAVE_ADDRESS);
    __bh1750_send_byte(cmd);            
    __bh1750_stop();                  
}


STATIC VOID __bh1750_read_sensor_date(VOID)
{   
    UCHAR_T i = 0;

    __bh1750_start();
    __bh1750_send_byte(SLAVE_ADDRESS + 1);

    for(i = 0; i < 2; i++)
    {
        buf[i] = __bh1750_read_byte();
        if(i < 1) {
            __bh1750_send_ack(ACK);
        }else {
            __bh1750_send_ack(NO_ACK);
        }
    }

    __bh1750_stop();
    __bh1750_delay(2000);

}



UINT_T tuya_bh1750_get_bright_value(VOID)
{   
    UINT_T data;
    FLOAT_T temp;

    __bh1750_read_sensor_date();
    
    data = buf[0];
    data = (data<<8) + buf[1];

    temp = (float)data/1.2;
    
    data = (UINT_T)temp;

    return data;
}



UCHAR_T tuya_bh1750_init(bh1750_init_t* param)
{   
    UCHAR_T error = 0;

    int opRet = -1;

    i2c_pin_t i2c_config = {
        .ucSDA_IO = param ->SDA_PIN,
        .ucSCL_IO = param ->SCL_PIN,
    };
    opRet = opSocI2CInit(&i2c_config);          /* SDA&SCL GPIO INIT */
    PR_NOTICE("SocI2CInit = %d",opRet);

    __bh1750_send_cmd(COMMAND_POWER_ON);
    __bh1750_send_cmd(COMMAND_H_RESOLUTION_MODE);

}