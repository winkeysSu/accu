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
  * @brief led��ʼ��(û�в�����ȫ��������ϵͳ��ʼ��ʱ����)
  * @param  handle  - led�ľ��
            sw_on   - led���Ƶĺ���
            sw_off  - ledϨ��ĺ���
  * @retval 0 - �ɹ��� ��0 - ʧ��
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
  * @brief led��һ����˸ģʽ�ĳ�ʼ��(û�в�����ȫ��������ϵͳ��ʼ��ʱ����)
  * @param  pMode - ��˸ģʽ��ָ��
            pSrcArray - ��˸ģʽ�����飬ÿ����Ա�����ÿ����״̬������ʱ��(msΪ��λ)
            srcLen - ��˸ģʽ�еĵƵ�״̬��
  * @retval 0 - �ɹ��� ��0 - ʧ��
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
  * @brief ����led����˸ģʽ�����led�Ѿ�����������˸ģʽ��ͬ��
            �����ú󲻸ı�led��ǰ�Ĺ���״̬
  * @param  handle - led�ľ��
            pMode - ��˸ģʽ��ָ��
            loop - ��˸���ڡ�LOOP_PERMANENT������ѭ��
  * @retval 0 - �ɹ��� 
            1 - �Ѿ������˸���˸ģʽ,��������ͬ
            2 - �Ѿ������˸���˸ģʽ,�����ڲ�ͬ
            <0 - ʧ��
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

    //����Ѿ�����Ϊ����˸ģʽ�������ڹ������͸������ڣ����˳�
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
  * @brief ����led
  * @param  handle - led�ľ��
            init_light_state - led��ʼ������״̬
  * @retval 0 - �ɹ��� 
            1 - �Ѿ�������
            <0 - ʧ��
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
  * @brief ֹͣled��Ϩ��
  * @param  handle - led�ľ��
            init_light_state - led��ʼ������״̬
  * @retval 0 - �ɹ�ֹͣled
            1 - �Ѿ�ֹͣ�˵�
            < 0 - ʧ��
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
  * @brief  ����led��tick
  * @param  handle - led�ľ��
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
  * @brief  ����led��tick����
  * @param  handle - led�ľ��
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



