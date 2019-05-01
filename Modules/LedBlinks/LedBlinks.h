#ifndef __LEDBLINKS_H__
#define __LEDBLINKS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "basic_f.h"

//macro

//�����������õ�ʱ��������λ:ms��
#define LED_TICK_TIME       10        

//����ѭ��
#define LOOP_PERMANENT      (~0)

//led����״̬��
#define LED_ON                      1
#define LED_OFF                     0
#define LED_UNKNOW                  2

//typedef

typedef void (*LED_OP)(void);

//�������ڵ���˸ģʽ����
typedef struct __BLinkModeTypedef
{
    u32*    pBlinkBuffer;
    u16     sect_num;
}BLinkModeTypedef;

//led�������״̬
typedef enum
{
    LedUnInit = 0,
    LedInitOK,
    LedSetModeOK,
    LedWorking,
    LedStopWorking,
    LedStateMax,
}LedState;


//led�ľ������
typedef struct __LED_HandleTypedef
{
    struct  __LED_HandleTypedef * next;

    BLinkModeTypedef*   pBlinkMode;     //һ�������ڵ���˸ģʽbufferָ��
    
    LED_OP  switch_on;                  //��led�ĺ���ָ��

    LED_OP  switch_off;                 //Ϩ��led�ĺ���ָ��    

    u32     curSectTick;                //��ǰ��˸�ڵ��ڵĵδ����

    u16     curSectIdx;               //��ǰ��˸�ڵ������    

    u16     totalLoop;                  //����
    u16     curLoop;                    //���ڼ���

    LedState workState;                 //�����������״̬

    u8      initLightState;             //�Ƴ�ʼ״̬
    u8      curLightState;              //�Ƶ�ǰ��״̬    
}LED_HandleTypedef;


//var


//macro func
#define  LED_BLINKS_LOCK()
#define  LED_BLINKS_UNLOCK()


//mix


//func
s16 LED_Init(LED_HandleTypedef *handle, LED_OP sw_on, LED_OP sw_off);
s16 LED_InitModeStatic(BLinkModeTypedef* mode, u32* pBlinkBuff, 
    u32 blinkBuffLen, u32 *srcArray, u32 srcLen);
s16 LED_InitMode(BLinkModeTypedef* mode, u32 *srcArray, u32 srcLen);
s16 LED_SetMode(LED_HandleTypedef* handle, BLinkModeTypedef* pMode, u16 loop);
s16 LED_Start(LED_HandleTypedef* handle, u8 init_light_state);
s16 LED_Stop(LED_HandleTypedef *led_handle);
void LED_Ticks (void);



#ifdef __cplusplus
}
#endif
#endif


