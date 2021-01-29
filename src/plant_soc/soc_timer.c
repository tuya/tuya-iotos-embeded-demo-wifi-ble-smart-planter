/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: soc_timer.c
 * @Description: Realtek 8710bn soft timer proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-05-06 10:03:57
 */
#include "FreeRTOS.h"
#include "timers.h"
#include "uni_time_queue.h"
#include "rtos_error.h"
#include "BkDriverTimer.h"


#define SOFTWARE_TIMER_MAX      20

/// software timer handle
STATIC TimerHandle_t xSoftWareTimers[SOFTWARE_TIMER_MAX] = {NULL};

/**
 * @brief: SOC hardware time start
 * @param {IN UINT_T cycle_us -> cycle time,unit:us}
 * @param {IN VOID* callback -> callback}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocHWTimerStart(IN UINT_T cycle_us, IN VOID* callback)
{
    UINT_T uiReloadCnt = 0;
    INT_T iRet = 0;
    
    uiReloadCnt = cycle_us / 1000 ;        /* use time0,unit:ms */
    iRet = bk_timer_initialize(0, uiReloadCnt, callback);
    if(iRet != kNoErr) {
        return OPRT_COM_ERROR;
    }
    
    return OPRT_OK;
}

/**
 * @brief: SOC hardware time stop
 * @param {none} 
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocHWTimerStop(VOID)
{   
    INT_T iRet = 0;

    iRet = bk_timer_stop(0);
    return OPRT_OK;
}

/**
 * @brief: SOC hardware time start
 * @param {IN UINT_T cycle_us -> cycle time,unit:us}
 * @param {IN VOID* callback -> callback}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocHWTimerUSStart(IN UINT_T cycle_us, IN VOID* callback)
{
    UINT_T uiReloadCnt = 0;
    INT_T iRet = 0;
    
    uiReloadCnt = cycle_us ;        /* use time0,unit:ms */

    iRet = bk_timer_initialize_us(1, uiReloadCnt, callback);
    if(iRet != kNoErr) {
        return OPRT_COM_ERROR;
    }
    
    return OPRT_OK;
}

/**
 * @brief: SOC hardware time stop
 * @param {none} 
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocHWTimerUSStop(VOID)
{   
    INT_T iRet = 0;

    iRet = bk_timer_stop(1);
    return OPRT_OK;
}

/**
 * @berief: SOC software time start
 * @param {IN UCHAR_T timer_id -> timer id}
 * @param {IN UINT_T cycle_ms -> cycle time,unit:ms}
 * @param {IN VOID* callback -> callback}
 * @attention: timer_id must < SOFTWARE_TIMER_MAX(5)
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocSWTimerStart(IN UCHAR_T timer_id, IN UINT_T cycle_ms, IN VOID* callback)
{
    CHAR_T ucTemp[4] = {0};
    
    if(timer_id > SOFTWARE_TIMER_MAX) {
        PR_ERR("Software timer is overflow,max timer count is %d",SOFTWARE_TIMER_MAX);
        return OPRT_INVALID_PARM;
    }

    if(NULL == xSoftWareTimers[timer_id]) {
        vNum2Str(0, timer_id, 4, ucTemp);
        xSoftWareTimers[timer_id] = xTimerCreate(ucTemp, (cycle_ms / portTICK_PERIOD_MS), pdFAIL, timer_id, (TimerHandle_t)callback);
        if(NULL == xSoftWareTimers[timer_id]) {
            PR_ERR("Create software time %d error",timer_id);
            return OPRT_COM_ERROR;
        }

        if(xTimerStart(xSoftWareTimers[timer_id], 0) != pdPASS) {
            PR_ERR("Start software timer %d error", timer_id);
            return OPRT_COM_ERROR;
        }

    } else {
        if(xTimerChangePeriod(xSoftWareTimers[timer_id], cycle_ms / portTICK_PERIOD_MS, 0) != pdPASS) {
            PR_ERR("Start software timer %d error", timer_id);
            return OPRT_COM_ERROR;
        }
    }

    return OPRT_OK;
}

/**
 * @berief: SOC software time stop
 * @param {IN UCHAR_T timer_id -> timer id} 
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocSWTimerStop(IN UCHAR_T timer_id)
{
    if(timer_id >= SOFTWARE_TIMER_MAX) {
        PR_ERR("Software timer is overflow,max timer count is %d",SOFTWARE_TIMER_MAX);
        return OPRT_INVALID_PARM;
    }

    if(NULL == xSoftWareTimers[timer_id]) {
        return OPRT_OK;
    }
    
    if(xTimerIsTimerActive(xSoftWareTimers[timer_id]) != pdFAIL) {
        xTimerDelete(xSoftWareTimers[timer_id], 0);
        xSoftWareTimers[timer_id] = NULL;
    } else {
        if(xTimerStop(xSoftWareTimers[timer_id], 0) != pdPASS) {
            PR_ERR("Software timer stop error!");
            return OPRT_COM_ERROR;
        }
    }

    return OPRT_OK;
}

/**
 * @description: Check if the timer is running
 * @param {timer_id} timer to be check
 * @return: BOOL_T: true: running, false : not running
 */
BOOL_T bSocSWTimerStartCheck(IN UCHAR_T timer_id)
{
    return xTimerIsTimerActive(xSoftWareTimers[timer_id]);
}



