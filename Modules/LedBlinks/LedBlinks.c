/******************Cloudoptek Raman project*********************
 All rights reserved.
 Author: winkey su
 Date:2019.05.01
***************************************************************/
#include "LedBlinks.h"
#include <stdlib.h>

//***************************macro******************************


//***************************typedef******************************


//****************************var*******************************
static LED_HandleTypedef* s_head_led_handle = NULL;
static LED_HandleTypedef* s_tail_led_handle = NULL;


//*************************macro func***************************


//***************************mix********************************


//***************************func*******************************

/*************************************************************************************
  * @brief led初始化(没有并发安全，建议在系统初始化时调用)
  * @param  handle  - led的句柄
            sw_on   - led亮灯的函数
            sw_off  - led熄灭的函数
  * @retval 0 - 成功； 非0 - 失败
**************************************************************************************/
s16 LED_Init(LED_HandleTypedef *handle, LED_OP sw_on, LED_OP sw_off)
{
    s16 err;

    LED_BLINKS_LOCK();
    if(NULL == handle)
    {
        err = -1;
        goto LED_InitErr;
    }
    
    if(LedUnInit == handle->workState)
    {
        handle->switch_on = sw_on;
        handle->switch_off = sw_off;
        
        handle->pBlinkMode = NULL;
        handle->curSectTick = 0;
        handle->curSectIdx = 0;
        handle->totalLoop = 0;
        handle->curLoop = 0;
        handle->initLightState = LED_UNKNOW;
        handle->curLightState = LED_UNKNOW;

        handle->next = NULL;
        if(NULL == s_head_led_handle)
        {
            s_head_led_handle = handle;
        }
        else
        {
            s_tail_led_handle->next = handle;
        }        
        s_tail_led_handle = handle;

        handle->workState = LedInitOK;
        err = 0;
        goto LED_InitErr;
    }
    else
    {
        err = -2;
        goto LED_InitErr;
    }
    
LED_InitErr:
    LED_BLINKS_UNLOCK();
    return err;
}


/*************************************************************************************
  * @brief led的一种闪烁模式的初始化(没有并发安全，建议在系统初始化时调用)
  * @param  pMode - 闪烁模式的指针
            pSrcArray - 闪烁模式的数组，每个成员项代表每个灯状态持续的时间(ms为单位)
            srcLen - 闪烁模式中的灯的状态数
  * @retval 0 - 成功； 非0 - 失败
**************************************************************************************/
s16 LED_InitMode(BLinkModeTypedef* pMode, u32 *pSrcArray, u32 srcLen)
{
    u16 i;
    s16 err;

    LED_BLINKS_LOCK();
    if((NULL == pMode) || (NULL == pSrcArray) || (0 == srcLen))
    {
        err = -1;
        goto LED_InitModeErr;
    }

    if(pMode->pBlinkBuffer)
    {
        err = -2;
        goto LED_InitModeErr;
    }
    
    pMode->pBlinkBuffer = malloc(sizeof(u32)*srcLen);
    if(NULL == pMode->pBlinkBuffer)
    {
        err = -3;
        goto LED_InitModeErr;
    }

    for(i = 0; i < srcLen; i++)
    {
        pMode->pBlinkBuffer[i] = (u32)(pSrcArray[i]/LED_TICK_TIME);
    }
    pMode->sect_num = srcLen;

    err = 0;
LED_InitModeErr:
    LED_BLINKS_UNLOCK();
    return err;
}


/*************************************************************************************
  * @brief 设置led的闪烁模式。如果led已经工作，且闪烁模式相同，
            则设置后不改变led当前的工作状态
  * @param  handle - led的句柄
            pMode - 闪烁模式的指针
            loop - 闪烁周期。LOOP_PERMANENT：永久循环
  * @retval 0 - 成功； 
            1 - 已经设置了该闪烁模式,且周期相同
            2 - 已经设置了该闪烁模式,且周期不同
            <0 - 失败
**************************************************************************************/
s16 LED_SetMode(LED_HandleTypedef* handle, BLinkModeTypedef* pMode, u16 loop)
{
    s16 err;
    
    LED_BLINKS_LOCK();
    if((NULL == handle) || (NULL == pMode) || (LedUnInit == handle->workState))
    {
        err = -1;
        goto LED_SetModeErr;
    }

    //如果已经设置为该闪烁模式，且正在工作，就更新周期，再退出
    if((handle->pBlinkMode == pMode)&&(LedWorking == handle->workState))
    {
        err = ((handle->totalLoop == loop)? 1: 2);
        if(handle->totalLoop == loop)
        {
            err = 1;
        }
        else
        {
            handle->totalLoop = loop;
            err = 2;
        }
        
        handle->curLoop = 0;
        goto LED_SetModeErr;
    }
    else
    {
        handle->curLoop = 0;
        handle->curSectIdx = 0;
        handle->curSectTick = 0;
        
        handle->pBlinkMode = pMode;
        handle->totalLoop = loop;
        
        handle->workState = LedSetModeOK;
        err = 0;
        goto LED_SetModeErr;
    }

LED_SetModeErr:
    LED_BLINKS_UNLOCK();
    return err;
}

/*************************************************************************************
  * @brief 启动led
  * @param  handle - led的句柄
            init_light_state - led初始的亮灭状态
  * @retval 0 - 成功； 
            1 - 已经启动了
            <0 - 失败
**************************************************************************************/
s16 LED_Start(LED_HandleTypedef* handle, u8 init_light_state)
{
    s16 err;
    
    LED_BLINKS_LOCK();

    if((NULL == handle) || (handle->workState < LedSetModeOK))
    {
        err = -1;
        goto LED_StartErr;
    }
    else
    {        
        handle->initLightState = init_light_state;
        if(LedWorking == handle->workState)
        {
            err = 1;
        }
        else
        {      
            handle->curSectTick = 0;
            handle->curLoop = 0;
            handle->curSectIdx = 0;

            handle->initLightState = init_light_state;
            handle->curLightState = handle->initLightState;

            handle->workState = LedWorking;
            err = 0;
        }
    }

LED_StartErr:
    LED_BLINKS_UNLOCK();
    return err;
}

/*************************************************************************************
  * @brief 停止led，熄灭
  * @param  handle - led的句柄
            init_light_state - led初始的亮灭状态
  * @retval 0 - 成功停止led
            1 - 已经停止了的
            < 0 - 失败
**************************************************************************************/
s16 LED_Stop(LED_HandleTypedef * handle)
{
    s16 err;
    
    LED_BLINKS_LOCK();

    if((NULL == handle) || (handle->workState < LedSetModeOK))
    {
        err = -1;
        goto LED_StopErr;
    }
    else
    {
        handle->workState = LedStopWorking;
    }

    err = 0;
LED_StopErr:
    LED_BLINKS_UNLOCK();
    return err;
}

/*************************************************************************************
  * @brief  单个led的tick
  * @param  handle - led的句柄
  * @retval None
**************************************************************************************/
static void LED_SingleTick(LED_HandleTypedef * handle)
{
    BLinkModeTypedef* pMode;

    if((LedWorking == handle->workState)&&(handle->pBlinkMode))
    {
        pMode = handle->pBlinkMode;

        if(handle->curSectTick >= pMode->pBlinkBuffer[handle->curSectIdx])
        {
            handle->curSectIdx++;
            if(handle->curSectIdx >= pMode->sect_num)
            {
                handle->curLoop++;
                if((handle->curLoop >= handle->totalLoop)
                    && ((u16)LOOP_PERMANENT != handle->totalLoop))
                {
                    handle->workState = LedStopWorking;
                    handle->curLoop = 0;
                }
                handle->curSectIdx = 0;
            }
            
            handle->curSectTick = 0;
        }
        if(0 == handle->curSectTick)
        {
            if(0 == handle->curSectIdx)
            {
                if(LED_ON == handle->initLightState)
                {
                    handle->switch_on();
                }
                else
                {
                    handle->switch_off();
                }
                handle->curLightState = handle->initLightState;
            }
            else
            {
                if(LED_ON == handle->curLightState)
                {
                    handle->switch_off();
                    handle->curLightState = LED_OFF;
                }
                else
                {
                    handle->switch_on();
                    handle->curLightState = LED_ON;
                }
            }
        }
        
        handle->curSectTick++;
    
    }
}

/*************************************************************************************
  * @brief  所有led的tick处理
  * @param  handle - led的句柄
  * @retval None
**************************************************************************************/
void LED_Ticks(void)
{
    LED_HandleTypedef* handle;

    if(s_head_led_handle)
    {
        LED_BLINKS_LOCK();
        for(handle = s_head_led_handle; handle; handle = handle->next)
        {
            LED_SingleTick(handle);
        }
        LED_BLINKS_UNLOCK();
    }
}



