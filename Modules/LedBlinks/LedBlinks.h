#ifndef __LEDBLINKS_H__
#define __LEDBLINKS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "basic_f.h"

//macro

//心跳函数调用的时间间隔（单位:ms）
#define LED_TICK_TIME       10        

//永久循环
#define LOOP_PERMANENT      (~0)

//led亮灭状态宏
#define LED_ON                      1
#define LED_OFF                     0
#define LED_UNKNOW                  2

//typedef

typedef void (*LED_OP)(void);

//单个周期的闪烁模式定义
typedef struct __BLinkModeTypedef
{
    u32*    pBlinkBuffer;
    u16     sect_num;
}BLinkModeTypedef;

//led句柄所处状态
typedef enum
{
    LedUnInit = 0,
    LedInitOK,
    LedSetModeOK,
    LedWorking,
    LedStopWorking,
    LedStateMax,
}LedState;


//led的句柄定义
typedef struct __LED_HandleTypedef
{
    struct  __LED_HandleTypedef * next;

    BLinkModeTypedef*   pBlinkMode;     //一个周期内的闪烁模式buffer指针
    
    LED_OP  switch_on;                  //亮led的函数指针

    LED_OP  switch_off;                 //熄灭led的函数指针    

    u32     curSectTick;                //当前闪烁节点内的滴答计数

    u16     curSectIdx;               //当前闪烁节点的索引    

    u16     totalLoop;                  //周期
    u16     curLoop;                    //周期计数

    LedState workState;                 //句柄所处工作状态

    u8      initLightState;             //灯初始状态
    u8      curLightState;              //灯当前的状态    
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


