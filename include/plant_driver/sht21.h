#ifndef __SHT21_H__
#define __SHT21_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

// I2C acknowledge
typedef enum{
  ACK                      = 0,
  NO_ACK                   = 1,
}i2c_ack;

typedef enum{
  I2C_ADR_W                = 128,   // sensor I2C address + write bit
  I2C_ADR_R                = 129    // sensor I2C address + read bit
}i2c_header;

// sensor command
typedef enum{
  TRIG_T_MEASUREMENT_HM    = 0xE3, // command trig. temp meas. hold master
  TRIG_RH_MEASUREMENT_HM   = 0xE5, // command trig. humidity meas. hold master
  TRIG_T_MEASUREMENT_POLL  = 0xF3, // command trig. temp meas. no hold master
  TRIG_RH_MEASUREMENT_POLL = 0xF5, // command trig. humidity meas. no hold master
  USER_REG_W               = 0xE6, // command writing user register
  USER_REG_R               = 0xE7, // command reading user register
  SOFT_RESET               = 0xFE  // command soft reset
}sht21_command;
//  CRC
//const unsigned int SHT21_POLYNOMIAL = 0x131;  //P(x)=x^8+x^5+x^4+1 = 100110001

// measurement signal selection
typedef enum{
  HUMIDITY,
  TEMP
}sht21_measure_type_t;


// Error codes
typedef enum{
  ACK_ERROR                = 0x01,
  TIME_OUT_ERROR           = 0x02,
  CHECKSUM_ERROR           = 0x04,
  UNIT_ERROR               = 0x08
}sht21_error_t;

typedef enum {
  SHT2x_RES_12_14BIT       = 0x00, // RH=12bit, T=14bit
  SHT2x_RES_8_12BIT        = 0x01, // RH= 8bit, T=12bit
  SHT2x_RES_10_13BIT       = 0x80, // RH=10bit, T=13bit
  SHT2x_RES_11_11BIT       = 0x81, // RH=11bit, T=11bit
  SHT2x_RES_MASK           = 0x81  // Mask for res. bits (7,0) in user reg.
} sht21_resolution_t;

typedef enum {
  SHT2x_EOB_ON             = 0x40, // end of battery
  SHT2x_EOB_MASK           = 0x40, // Mask for EOB bit(6) in user reg.
}sht21_eob_t;

typedef struct 
{
    UCHAR_T SDA_PIN;            ///< SDA pin
    UCHAR_T SCL_PIN;            ///< SCL pin
    sht21_resolution_t RESOLUTION;   
}sht21_init_t;

UCHAR_T tuya_sht21_read_register(UCHAR_T *register_value);

UCHAR_T tuya_sht21_write_register(UCHAR_T *register_value);

USHORT_T tuya_sht21_measure(sht21_measure_type_t sht21_measure_type);

float tuya_sht21_cal_RH(SHORT_T u16sRH);

float tuya_sht21_cal_temperature(SHORT_T u16sT);

UCHAR_T tuya_sht21_reset(VOID);

UCHAR_T tuya_sht21_init(sht21_init_t* param);

#endif