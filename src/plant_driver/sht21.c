/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sht21.h
 * @Description: SHT21 IIC drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 20120-12-16 18:51:29
 */

#include "sht21.h"
#include "soc_i2c.h"



/**
 * @brief:  SHT21 IIC delay proc
 * @param:  delay time(us) 
 * @retval: none
 */
STATIC VOID __sht21_delay(USHORT_T usTime)
{       
    vI2CDelay(usTime);
}

/**
 * @brief: SHT21 IIC start transport
 * @param {none}
 * @retval: 
 */
STATIC int __sht21_start(VOID)
{
    vI2CSDASet();
    vI2CSCLSet();

    vI2CSDAReset();

    __sht21_delay(10);

    if(ucI2CSDAOutputRead()) {    /* confirm SDA status is set correct */
        PR_DEBUG("SHT21 start2 error!");
        return -1;
    }
    vI2CSCLReset();
    __sht21_delay(10);

    return 0;
}

/**
 * @brief: SHT21 IIC stop transport
 * @param {none}
 * @retval: 
 */
STATIC int __sht21_stop(VOID)
{
    vI2CSDAReset();
    vI2CSCLReset();

    if(ucI2CSDAInputRead()) {    /* confirm SDA status is ready */
        PR_DEBUG("SHT21 stop1 error!");
        return -1;
    }
    
    vI2CSCLSet();

    __sht21_delay(10);

    vI2CSDASet();

    if(!ucI2CSDAOutputRead()) {    /* confirm SDA status is set correct */
        PR_DEBUG("SHT21 stop2 error!");
        return -1;
    }

    __sht21_delay(10);

    return 0;        
}  

/**
 * @brief: SHT21 IIC send byte
 * @param {UCHAR_T sendbyte -> send one byte}
 * @retval: none
 */
STATIC int __sht21_send_byte(UCHAR_T sendbyte)
{
    UCHAR_T i = 0;

    for(i = 0x80; i > 0; i >>= 1)
    {
        if((sendbyte & i) == 0) {
            vI2CSDAReset();
        } else {
            vI2CSDASet();
        }
        __sht21_delay(1);   
        vI2CSCLSet();
        __sht21_delay(5);
        vI2CSCLReset();
        __sht21_delay(1);

    }

    vI2CSDASet();
    vI2CSCLSet();
    __sht21_delay(1);

    if(ucI2CSDAInputRead()) {
        PR_DEBUG("i2c send byte error");
    }
    vI2CSCLReset();

    __sht21_delay(20);

    return 0;
}

/**
 * @brief: SHT21 IIC read byte
 * @param {none}
 * @retval: none
 */
STATIC UCHAR_T __sht21_read_byte(i2c_ack ack)
{
    UCHAR_T i = 0;
    
    UCHAR_T readbyte = 0;

    vI2CSDASet();

    for(i = 0x80; i > 0; i >>= 1)
    {
        vI2CSCLSet();
        __sht21_delay(4);

        if(ucI2CSDAInputRead()) {
            readbyte |= i;    
        }

        vI2CSCLReset();
        __sht21_delay(1);
    }
    
    if(ack) {
        vI2CSDASet();
    } else {
        vI2CSDAReset();    
    }

    __sht21_delay(1);
    vI2CSCLSet();
    __sht21_delay(5);

    vI2CSCLReset();
    vI2CSDASet();
    __sht21_delay(20);    
    
    return readbyte;
}

STATIC UCHAR_T __sht21_check_crc(UCHAR_T data[], UCHAR_T byte_num, UCHAR_T check_sum)
{
    UCHAR_T crc = 0;
    UCHAR_T byte_ctr;
    UCHAR_T bit;
    for(byte_ctr = 0; byte_ctr < byte_num; ++byte_ctr)
    {
        crc ^= (data[byte_ctr]);
        for(bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x131;
            } else {
                crc = (crc << 1);    
            }
        }
    }

    if(crc != check_sum) {
        PR_DEBUG("SHT21 check crc error!");
        return -1;
    } else {
        return 0;
    } 
}




UCHAR_T tuya_sht21_read_register(UCHAR_T *register_value)
{
    UCHAR_T check_sum;
    UCHAR_T error = 0;

    __sht21_start();
    __sht21_send_byte(I2C_ADR_W);
    __sht21_send_byte(USER_REG_R);
    __sht21_start();
    __sht21_send_byte(I2C_ADR_R);
    *register_value = __sht21_read_byte(ACK);
    check_sum = __sht21_read_byte(NO_ACK);
    __sht21_check_crc(register_value, 1, check_sum);
    __sht21_stop();

    return error;
}

UCHAR_T tuya_sht21_write_register(UCHAR_T *register_value)
{
    UCHAR_T error = 0;

    __sht21_start();
    error |= __sht21_send_byte(I2C_ADR_W);
    error |= __sht21_send_byte(USER_REG_W);
    error |= __sht21_send_byte(*register_value);
    __sht21_stop();

    return error;
}

USHORT_T tuya_sht21_measure(sht21_measure_type_t sht21_measure_type)
{   
    SHORT_T  measurand = 0;
    UCHAR_T  checksum;   //checksum
    UCHAR_T  data[2];    //data array for checksum verification
    UCHAR_T  error=0;    //error variable
    int i=0;        //counting variable

    __sht21_start();
    error |= __sht21_send_byte (I2C_ADR_W); // I2C Adr
    switch (sht21_measure_type)
    {
    case HUMIDITY:
        error |= __sht21_send_byte (TRIG_RH_MEASUREMENT_HM);
        break;
    
    case TEMP:
        error |= __sht21_send_byte (TRIG_T_MEASUREMENT_HM);
        break;
    default:
        break;
    }

    __sht21_start();
    error |= __sht21_send_byte (I2C_ADR_R);
    vI2CSCLSet();
    for(i = 0; i<100000; i++)
    {
        __sht21_delay(100);
        if(ucI2CSCLInputRead()) {
            break;
        }
    }

    if(!ucI2CSCLInputRead()) {
        error |= TIME_OUT_ERROR; 
        PR_DEBUG("TIME_OUT_ERROR");
    }

    //-- read two data bytes and one checksum byte --
    data[0] = __sht21_read_byte(ACK);
    data[1] = __sht21_read_byte(ACK);
    checksum = __sht21_read_byte(NO_ACK);
    //-- verify checksum --
    error |= __sht21_check_crc(data,2,checksum);

    PR_DEBUG("error = %d",error);
    __sht21_stop();

    measurand |= (data[0]<<8);
    measurand |= data[1];
    
    PR_DEBUG("measurand = %d",measurand);

    return measurand;
}

float tuya_sht21_cal_RH(SHORT_T u16sRH)
{
  float humidity_RH;              // variable for result
  
  u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
  //-- calculate relative humidity [%RH] --
  humidity_RH = ((125.0 * (float)u16sRH) / 65536) - 6; // RH= -6 + 125 * SRH/2^16

  return humidity_RH;
}

float tuya_sht21_cal_temperature(SHORT_T u16sT)
{
    float temperatureC;            // variable for result

    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [C] --
    temperatureC= ((175.72 * (float)u16sT) / 65536) - 46.85; //T= -46.85 + 175.72 * ST/2^16
    return temperatureC;
}

UCHAR_T tuya_sht21_reset(VOID)
{
    UCHAR_T error = 0;

    __sht21_start();
    error |= __sht21_send_byte(I2C_ADR_W);
    error |= __sht21_send_byte(SOFT_RESET);
    __sht21_stop();

    __sht21_delay(15000);

    return error;
}

UCHAR_T tuya_sht21_init(sht21_init_t* param)
{   
    UCHAR_T error = 0;

    int opRet = -1;

    UCHAR_T user_register;

    i2c_pin_t i2c_config = {
        .ucSDA_IO = param ->SDA_PIN,
        .ucSCL_IO = param ->SCL_PIN,
    };
    opRet = opSocI2CInit(&i2c_config);          /* SDA&SCL GPIO INIT */
    PR_DEBUG("SocI2CInit = %d",opRet);
    
    error = tuya_sht21_read_register(&user_register);
    user_register = (user_register & ~SHT2x_RES_MASK) | param->RESOLUTION;

    error = tuya_sht21_write_register(&user_register);
}