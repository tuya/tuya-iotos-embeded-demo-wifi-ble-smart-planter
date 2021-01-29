#include "uni_log.h"
#include "tuya_cloud_error_code.h"

#define BK_ADC_ENABLE 1

#define BK_ADC_ENABLE_TEST 1

#if BK_ADC_ENABLE_TEST
#define BK_ADC_ENABLE_DIV_NUM 10
#else
#define BK_ADC_ENABLE_DIV_NUM 1
#endif

/***********************************************************
*************************variable define********************
***********************************************************/
typedef struct {
    UINT16_T *pData;
    UINT8_T data_buff_size;
    /* mode:     ADC mode
     * bit[0:1]: ADC operation mode
     *          00:  ADC power down mode
     *          01:  ADC one-step mode
     *          10:  ADC software control mode
     *          11:  ADC continuous mode
     * bit[3:2]: ADC sample rate selection
     *          00: adc_clk/36
     *          01: adc_clk/18
     *          10: Reserved
     *          11: Reserved
     * bit[7:4]: reserved
     */
    UINT8_T mode;
    void (*p_Int_Handler)(void);
} adc_priv_config_t;


typedef struct {
    UINT16_T port;
    adc_priv_config_t priv;
} tuya_adc_dev_t;



