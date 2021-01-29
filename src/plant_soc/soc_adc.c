#include "typedef.h"
#include "saradc_pub.h"
#include "drv_model_pub.h"
#include "soc_adc.h"


#include "gpio.h"
#include "arm_arch.h"


#define ADC_TEMP_BUFFER_SIZE  20

#define TEMP_ADC_DATA_LEN     4


UINT_T adc_isr_callback_num = 0;

static saradc_desc_t adc_desc;
static DD_HANDLE adc_hdl;
/***********************************************************
****************************温度检测************************
***********************************************************/
UINT16 tmp_detect_buff[ADC_TEMP_BUFFER_SIZE];

STATIC VOID bk_adc_timer_cb(UINT_T timerID,LPVOID pTimerArg);


INT_T tuya_hal_adc_init(tuya_adc_dev_t *adc)
{
    adc_priv_config_t *adc_priv_cfg = &adc->priv;
    INT_T ret = 0;
    
    //REG_WRITE(REG_GPIO_23_CONFIG, 0x08);
    adc_desc.channel = 3;
    adc_desc.current_read_data_cnt = 0;
    adc_desc.current_sample_data_cnt = 0;
    adc_desc.pData = adc_priv_cfg->pData;
    adc_desc.data_buff_size = adc_priv_cfg->data_buff_size;
    adc_desc.mode = 3;//(3<<0)|(0<<2)|(0<<4);
    adc_desc.filter = 0;
    adc_desc.has_data = 0;
    adc_desc.all_done = 0;
    adc_desc.pre_div = 8;
    adc_desc.samp_rate = 2;//16;
    adc_desc.p_Int_Handler = saradc_disable;

    PR_NOTICE("=============tuya_hal_adc_init enter=============channel:%d pData:%x data_buff_size:%d mode:%d", 
        adc_desc.channel, adc_desc.pData, adc_desc.data_buff_size, adc_desc.mode);

    
#if 0
	adc_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT_T)&adc_desc);
	if(status)
	{
	    PR_ERR("tuya_hal_adc_init  ddev_open fail:%d", status);
		ret = -1;
	}
#endif
	return ret;
}


#if 0
INT_T tuya_hal_adc_value_get(UINT8_T read_cnt, void *output)
{
	INT_T ret = -1;
    INT_T i = 0;
    UINT_T status;
    UINT8_T param = 1;
    STATIC  USHORT_T last_adc = 0;
    *(USHORT_T*)output = 0xFFFF;
    INT_T value_sum = 0;

 
    adc_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT_T)&adc_desc); 
    for(int i = 0; i < 5; i++){
        if(adc_desc.has_data && adc_desc.current_sample_data_cnt >= read_cnt)
        {
            status = 0;
            *(USHORT_T*)output = adc_desc.pData[0];
            adc_desc.has_data = 0;
            adc_desc.current_sample_data_cnt = 0;
            last_adc = *(USHORT_T*)output;
            ddev_close(adc_hdl);
            break;
        } else {     
            PR_NOTICE("read ADC error");
            *(USHORT_T*)output = last_adc;
        }
    }
    if(status)
        ddev_close(adc_hdl);
    //PR_NOTICE("read ADC %d", last_adc);

	return 0;
}
#endif


INT_T tuya_hal_adc_value_get(UINT8_T read_cnt, void *output)
{
    INT_T i = 0;
    INT_T ret = -1;
    UINT_T status;
    UINT8_T param = 1;
    STATIC  USHORT_T last_adc = 0;

    GLOBAL_INT_DECLARATION();
    
    *(USHORT_T*)output = 0xFFFF;
    INT_T value_sum = 0;
    memset(adc_desc.pData, 0, TEMP_ADC_DATA_LEN*sizeof(USHORT_T));

    GLOBAL_INT_DISABLE();
    adc_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT_T)&adc_desc); 
    if ((DD_HANDLE_UNVALID == adc_hdl) || (SARADC_SUCCESS != status))
    {
        if (SARADC_SUCCESS != status)
        {
            ddev_close(adc_hdl);
        }
        adc_hdl = DD_HANDLE_UNVALID;
        GLOBAL_INT_RESTORE();
        PR_ERR("tuya_hal_adc_value_get ddev_open error:%d", status);
        return -1;
    }
    GLOBAL_INT_RESTORE();

    while (adc_desc.all_done == 0)
    {
        i ++;
        if (i > 100)
        {
            PR_ERR("tuya_hal_adc_value_get: timeout!");
            break;
        }
        vTaskDelay(1);
    }
    if(adc_desc.current_sample_data_cnt >= read_cnt)
    {
        *(USHORT_T*)output = adc_desc.pData[0];//(USHORT_T)(value_sum / read_cnt);//adc_desc.pData[3];
        adc_desc.has_data = 0;
        adc_desc.current_sample_data_cnt = 0;
        last_adc = *(USHORT_T*)output;
    } else {
        *(USHORT_T*)output = last_adc;
        PR_DEBUG("last adclast adclast adc %d", last_adc);
    }
     (adc_hdl);
   // PR_DEBUG("last adc %d", last_adc);
    return 0;
}




INT_T tuya_hal_adc_finalize(tuya_adc_dev_t *adc)
{
	UINT8_T param;

	param = 0;
	ddev_control(adc_hdl, SARADC_CMD_RUN_OR_STOP_ADC, (void *)&param);
	ddev_close(adc_hdl);

	return 0;
}






VOID tuya_hal_adc_isr_calbak(VOID)
{
    INT_T out_put = 0;

    adc_isr_callback_num++;

    PR_DEBUG("tuya_hal_adc_finalize");

    //tuya_hal_adc_value_get(1, &out_put);

    PR_DEBUG("tuya_hal_adc_finalize:%d", out_put);
}

VOID vTuyaIsrDisable(VOID)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();
}


VOID vTuyaIsrEnable(VOID)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_RESTORE();
}


