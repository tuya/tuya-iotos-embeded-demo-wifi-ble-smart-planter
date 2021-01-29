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

#include "plant_control.h"
#include "sht21.h"
#include "bh1750.h"
#include "soc_adc.h"
#include "rs2255.h"
#include "tuya_gpio.h"
#include "plant_pwm.h"
#include "sys_timer.h"
/***********************************************************
*************************types define***********************
***********************************************************/
typedef enum
{
    LOW = 0,
    HIGH,
}default_level;


typedef struct {
    float temperature;
    float humidity;
    USHORT_T water_tank_value;
    USHORT_T light_intensity_value;
    USHORT_T soil_humidity;
}DEVICE_DATA_T;


USER_PWM_DUTY_T user_pwm_duty = {0,0};

DEVICE_DATA_T device_data = {0};
PLANT_CTRL_DATA_T plant_ctrl_data = {FALSE, FALSE, FALSE, 0, 0, 0, 30, 20, 60,40,2000};
PLANT_REPORT_DATA_T plant_report_data = {0};

STATIC UINT8 ADD_WATER_COUNT = 0;
STATIC UINT8 ADD_WATER_READY = 1;

STATIC UINT8 IIC_SELECT_FLAG = 0;
STATIC UINT8 ADC_SELECT_FLAG = 0;

STATIC TIMER_ID light_timer;

STATIC USHORT_T light_flag_min = 0;


#define ACCURACY                         (2000)  // 误差范围值
#define light_value_set                  (12000) // unit:lux
/***********************************************************
*************************IO control device define***********
***********************************************************/
#define HUMIDIFIER_PORT                     (24)
#define HUMIDIFIER_LEVEL                    LOW

#define WATER_VALVE_PORT                    (14)
#define WATER_VALVE_LEVEL                   LOW

#define HEATING_ROD_PORT                    (20)
#define HEATING_ROD_LEVEL                   LOW

#define COOL_DOWN_FAN_PORT                  (21)
#define COOL_DOWN_FAN_LEVEL                 LOW

#define WATER_PUMP_PORT                     (22)
#define WATER_PUMP_LEVEL                    LOW

#define WATER_SENSOR_PORT              channel_x1
#define SOIL_MOISTURE_SENSOR_PORT      channel_x2

#define IIC_SDA_PIN                         (6)
#define IIC_SCL_PIN                         (7)

/***********************************************************
*************************about adc init*********************
***********************************************************/


#define TEMP_ADC_DATA_LEN           (4)

tuya_adc_dev_t tuya_adc;

/***********************************************************
*************************about iic init*********************
***********************************************************/
STATIC sht21_init_t sht21_int_param = {IIC_SDA_PIN, IIC_SCL_PIN, SHT2x_RES_10_13BIT};

STATIC bh1750_init_t bh1750_int_param = {IIC_SDA_PIN, IIC_SCL_PIN};

/***********************************************************
*************************function***************************
***********************************************************/

STATIC VOID __ctrl_gpio_init(CONST TY_GPIO_PORT_E port, CONST BOOL_T high)
{   
    // 设置IO口为输出模式
    tuya_gpio_inout_set(port, FALSE);
    // 设置IO口电平
    tuya_gpio_write(port, high);
}

STATIC VOID light_timer_cb(UINT_T timerID, PVOID_T pTimerArg)
{
    light_flag_min--;
    plant_report_data.Countdown_left = light_flag_min;
    if(!light_flag_min) {
        plant_ctrl_data.Countdown_set = cancel;
    }
}

VOID plant_device_init(VOID)
{
    INT_T op_ret = 0;
    
    op_ret = sys_add_timer(light_timer_cb, NULL, &light_timer);
    if(op_ret != OPRT_OK) {
        PR_ERR("add light timer failed");
    }
    
    // SHT21 IIC driver init 
    tuya_sht21_init(&sht21_int_param);
    vTaskDelay(200);
    tuya_bh1750_init(&bh1750_int_param);
    vTaskDelay(200);
    plant_pwm_init();
    // gpio init
    __ctrl_gpio_init(HUMIDIFIER_PORT, HUMIDIFIER_LEVEL);

    __ctrl_gpio_init(COOL_DOWN_FAN_PORT, COOL_DOWN_FAN_LEVEL);  

    __ctrl_gpio_init(WATER_VALVE_PORT, WATER_VALVE_LEVEL);

    __ctrl_gpio_init(HEATING_ROD_PORT, HEATING_ROD_LEVEL);
    
    __ctrl_gpio_init(WATER_PUMP_PORT, WATER_PUMP_LEVEL);
    // adc driver init
    tuya_adc.priv.pData = Malloc(TEMP_ADC_DATA_LEN * sizeof(USHORT_T));
    memset(tuya_adc.priv.pData, 0, TEMP_ADC_DATA_LEN*sizeof(USHORT_T));
    tuya_adc.priv.data_buff_size = TEMP_ADC_DATA_LEN; //设置数据缓存个数

}



VOID plant_get_iic_sensor_data(VOID)
{

    SHORT_T hum;
    SHORT_T temp;

    switch (IIC_SELECT_FLAG)
    {
    case 0:    
        tuya_sht21_init(&sht21_int_param);

        hum = tuya_sht21_measure(HUMIDITY);
        device_data.humidity = tuya_sht21_cal_RH(hum);
        if(device_data.humidity > 0){ // 剔除小于0的无效湿度值
            plant_report_data.Humidity_current = (UCHAR_T)device_data.humidity;
            PR_NOTICE("humidity = %d",plant_report_data.Humidity_current);
        }

        temp = tuya_sht21_measure(TEMP);
        device_data.temperature = tuya_sht21_cal_temperature(temp);
        plant_report_data.Temp_current = (UCHAR_T)device_data.temperature;
        PR_NOTICE("tempre = %d",plant_report_data.Temp_current);
        
        IIC_SELECT_FLAG = 1;

        break;
    case 1:    
        tuya_bh1750_init(&bh1750_int_param);

        device_data.light_intensity_value = tuya_bh1750_get_bright_value();
        PR_NOTICE("light_intensity_value = %d",device_data.light_intensity_value);
        
        IIC_SELECT_FLAG = 0;

        break;

    default:
        break;
    }

}

VOID plant_get_adc_sensor_data(VOID)
{
    rs2255_init();

    switch (ADC_SELECT_FLAG)
    {
    case 0:    

        rs2255_channel_checkout(WATER_SENSOR_PORT);
        tuya_hal_adc_init(&tuya_adc);
        tuya_hal_adc_value_get(TEMP_ADC_DATA_LEN, &device_data.water_tank_value);
        PR_NOTICE("water_tank_value = %d",device_data.water_tank_value);

        ADC_SELECT_FLAG = 1;

        break;
    case 1:    
        
        rs2255_channel_checkout(SOIL_MOISTURE_SENSOR_PORT);
        tuya_hal_adc_init(&tuya_adc);
        tuya_hal_adc_value_get(TEMP_ADC_DATA_LEN, &device_data.soil_humidity);
        PR_NOTICE("soil_humidity = %d",device_data.soil_humidity);

        ADC_SELECT_FLAG = 0;

        break;

    default:
        break;
    }
    
    tuya_hal_adc_finalize(&tuya_adc);

}

STATIC VOID __passive_ctrl_module_soil_humidity(VOID)
{   
    if(device_data.soil_humidity > plant_ctrl_data.Soil_humidity_threshold) {

        if(ADD_WATER_READY) {

            tuya_gpio_write(WATER_VALVE_PORT, !WATER_VALVE_LEVEL);

            ADD_WATER_COUNT++;
            if(ADD_WATER_COUNT > 5) {
                ADD_WATER_READY = 0;
            }

        } else{

            tuya_gpio_write(WATER_VALVE_PORT, WATER_VALVE_LEVEL);
            ADD_WATER_COUNT++;
            if(ADD_WATER_COUNT >15) {
                ADD_WATER_READY = 1;
                ADD_WATER_COUNT = 0;
            }

        }
    }else {

        ADD_WATER_READY = 1;
        ADD_WATER_COUNT = 0;
        tuya_gpio_write(WATER_VALVE_PORT, WATER_VALVE_LEVEL);

    }
}

STATIC VOID __passive_ctrl_module_temp_humidity(VOID)
{   
    if(device_data.humidity < plant_ctrl_data.Humidity_min) { // 打开加湿器
        tuya_gpio_write(HUMIDIFIER_PORT, !HUMIDIFIER_LEVEL);
    }else {
        tuya_gpio_write(HUMIDIFIER_PORT, HUMIDIFIER_LEVEL);
    }

    if(device_data.temperature < plant_ctrl_data.Temp_min) {  // 打开加热
        tuya_gpio_write(HEATING_ROD_PORT, !HEATING_ROD_LEVEL);
    }else {
        tuya_gpio_write(HEATING_ROD_PORT, HEATING_ROD_LEVEL);
    }

    if((device_data.temperature > plant_ctrl_data.Temp_max)||(device_data.humidity > plant_ctrl_data.Humidity_max)) { // 打开风扇
        tuya_gpio_write(COOL_DOWN_FAN_PORT,!COOL_DOWN_FAN_LEVEL);
    }else {
        tuya_gpio_write(COOL_DOWN_FAN_PORT,COOL_DOWN_FAN_LEVEL);
    }
    
}

STATIC VOID __passive_ctrl_module_light(VOID)
{   
    if(IIC_SELECT_FLAG){ // 若上一次启用的iic传感器为温湿度传感器
        return;
    }

    if((TRUE == plant_ctrl_data.Auto_switch)) { // 自动补光开关为开
        USHORT_T current = device_data.light_intensity_value;
        USHORT_T set = light_value_set;

        if((current - set) > ACCURACY) { // 当前光照强度大于设定值且不在误差范围内
            if((current - set) >= 200) {
                if(plant_ctrl_data.Bright_value >= 50)plant_ctrl_data.Bright_value -= 50;
            }else if((current - set) > 150) {
                if(plant_ctrl_data.Bright_value >= 20)plant_ctrl_data.Bright_value -= 20;
            }else {
                if(plant_ctrl_data.Bright_value >= 1)plant_ctrl_data.Bright_value--;
            }
        }else if((set - current) > ACCURACY) { // 当前光照强度小于设定值且不在误差范围内
            if((set - current) >= 200) {
                if(plant_ctrl_data.Bright_value <= 950)plant_ctrl_data.Bright_value += 50;
            }else if((set - current) > 150) {
                if(plant_ctrl_data.Bright_value <= 980)plant_ctrl_data.Bright_value += 20;
            }else {
                if(plant_ctrl_data.Bright_value <= 999)plant_ctrl_data.Bright_value++;
            }
        }
    } 
}

STATIC VOID __initiative_ctrl_module_pump(VOID)
{   
    // 根据水位传感器值转换剩余水量百分比，用于上报
    if(device_data.water_tank_value < 1700) {
        plant_report_data.Water_remain = 10;
    }else if(device_data.water_tank_value < 2500) {
        plant_report_data.Water_remain = 25;
    }else if(device_data.water_tank_value < 2700) {
        plant_report_data.Water_remain = 50;
    }else if(device_data.water_tank_value < 2900) {
        plant_report_data.Water_remain = 75;
    }else if(device_data.water_tank_value >= 3000) {
        plant_report_data.Water_remain = 100;
    }

    if(TRUE == plant_ctrl_data.Pump){ // 若水泵开关为开
        PR_DEBUG("water pump open !!!!");
        tuya_gpio_write(WATER_PUMP_PORT,!WATER_PUMP_LEVEL);
    }else { // 若水泵开关为关时主动关闭水泵
        tuya_gpio_write(WATER_PUMP_PORT,WATER_PUMP_LEVEL);
    }
    if(device_data.water_tank_value >= 3000) { // 当水量接近测量上限时自动关闭水泵
        PR_DEBUG("water tank is full !!!!");
        tuya_gpio_write(WATER_PUMP_PORT,WATER_PUMP_LEVEL);
        plant_ctrl_data.Pump = FALSE;
    }
                                                                      
}

STATIC VOID __initiative_ctrl_module_light(VOID)
{   
    
    if(TRUE == plant_ctrl_data.Auto_switch) { // 自动补光开关为开
        PR_NOTICE("Ligth open !!!!");
        if(plant_ctrl_data.Light_color == red) { // 灯光颜色设为红灯
            user_pwm_duty.duty_red = plant_ctrl_data.Bright_value;
            user_pwm_duty.duty_blue = 0;
        }else if(plant_ctrl_data.Light_color == blue) { // 灯光颜色设为蓝灯
            user_pwm_duty.duty_blue = plant_ctrl_data.Bright_value;
            user_pwm_duty.duty_red = 0;
        }else {
            user_pwm_duty.duty_blue = plant_ctrl_data.Bright_value;
            user_pwm_duty.duty_red = user_pwm_duty.duty_blue;
        }
        plant_pwm_set(&user_pwm_duty);
    }else { // 自动补光开关为关 用户手动定时控制
        if(plant_ctrl_data.Light_color == red) {
            user_pwm_duty.duty_red = 1000;
            user_pwm_duty.duty_blue = 0;
        }else if(plant_ctrl_data.Light_color == blue) {
            user_pwm_duty.duty_blue = 1000;
            user_pwm_duty.duty_red = 0;
        }else {
            user_pwm_duty.duty_red = 1000;
            user_pwm_duty.duty_blue = 1000;
        }
        if((IsThisSysTimerRun(light_timer) == FALSE)&&(plant_ctrl_data.Countdown_set != cancel)) {
            light_flag_min = (USHORT_T)plant_ctrl_data.Countdown_set * 60;
            plant_pwm_set(&user_pwm_duty);
            sys_start_timer(light_timer,1000*60,TIMER_CYCLE);
        }else if(plant_ctrl_data.Countdown_set == cancel) {
            user_pwm_duty.duty_blue = 0;
            user_pwm_duty.duty_red = 0;
            plant_pwm_set(&user_pwm_duty);
            light_flag_min = 0;
            sys_stop_timer(light_timer);
        }else if(IsThisSysTimerRun(light_timer) == TRUE) {
            plant_pwm_set(&user_pwm_duty);
        }
        // 保存定时剩余时间 单位分钟
        plant_report_data.Countdown_left = light_flag_min;
    }
                                                                           
}

VOID plant_ctrl_handle(VOID)
{   
    PR_DEBUG("enter ctrl handle");
    
    // 温湿度自动控制
    __passive_ctrl_module_temp_humidity();

    // 土壤湿度自动控制
    __passive_ctrl_module_soil_humidity();

    // 灯板自动控制
    __passive_ctrl_module_light();
    
    // 水泵主动控制
    __initiative_ctrl_module_pump();

    // 灯板主动控制
    __initiative_ctrl_module_light();
}

VOID plant_ctrl_all_off(VOID)
{   
    // 关闭所有设备
    tuya_gpio_write(HUMIDIFIER_PORT, HUMIDIFIER_LEVEL);
    tuya_gpio_write(HEATING_ROD_PORT, HEATING_ROD_LEVEL);
    tuya_gpio_write(COOL_DOWN_FAN_PORT, COOL_DOWN_FAN_LEVEL);
    tuya_gpio_write(WATER_VALVE_PORT, WATER_VALVE_LEVEL);
    user_pwm_duty.duty_blue = 0;
    user_pwm_duty.duty_red = user_pwm_duty.duty_blue;
    plant_pwm_set(&user_pwm_duty);
}

 