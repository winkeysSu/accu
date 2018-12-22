#ifndef __W_DEBUG_H__
#define __W_DEBUG_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "basic_f.h"
#include <stdio.h>
#include "glb_cfg.h"
#include "StdDbLog.h"


//对外发布时，应该注释掉宏定义DEBUG_MODE，注释掉宏DEBUG_SHOW_EN

//#define DEBUG_MODE

//#define LLDB_SHOW_EN
//#define DEBUG_SHOW_EN

#ifdef DEBUG_MODE
#define DEBUG_SHOW_EN
#endif

#define LOG_SHOW_EN
#define ERR_SHOW_EN
//#define BACK_DB_EN

//默认值，最低的可显示log的level，比这个数字小的值的对应log都不可以显示
#define SHOW_LEVEL              3

//调试信息实时显示的宏
#define REAL_TIME_MSG           0

//调试信息保存在内存的宏
#define BACKEND_MSG             1

//调试用缓存buffer最大长度
#define BACKEND_BUFFER_LEN      256


#ifdef DEBUG_MODE
//debug模式下才有断言
#define ASSERT(expr) \
if((expr) == false)\
{\
    printf("%s err:%s %d %s\r\n", #expr, __FILE__, __LINE__, __FUNCTION__);  \
    while(1); \
}

#define ASSERT_MSG(err_msg) \
do  \
{\
    printf(err_msg);   \
    printf("err:%s %d %s\r\n", __FILE__, __LINE__, __FUNCTION__);  \
    while(1);   \
}while(0);

#else
#define ASSERT(expr)
#define ASSERT_MSG(err_msg)
#endif

extern u32 g_show_lev;
extern s8 g_show_buffer[BACKEND_BUFFER_LEN];
extern s32 g_hope_wrlen;

/**
 * @brief  output accroding to the level
    只支持实时输出log
  */
#define WDEBUG(level, fmt, ...) \
do \
{ \
    if(level > g_show_lev) \
    { \
        g_hope_wrlen = snprintf(g_show_buffer, sizeof(g_show_buffer),fmt, ##__VA_ARGS__); \
        printf("%s", g_show_buffer);  \
    } \
}while(0)


/**
 * @brief  output accroding to the level and channel
  */
#define WDEBUG_ACC_CHANNEL(level, channel, fmt, ...) \
do \
{ \
    if(level > g_show_lev) \
    { \
        g_hope_wrlen = snprintf(g_show_buffer, sizeof(g_show_buffer),fmt, ##__VA_ARGS__); \
        if(REAL_TIME_MSG == channel) printf("%s\r\n", g_show_buffer); \
        else { DB_LOG_ADD(g_show_buffer); } \
    } \
}while(0)


/**
 * @brief  output accroding to the level and channel, with function name first
  */
#define WDEBUG_FUNCNAME_ACC_CHANNEL(level, channel, fmt, ...) \
do \
{ \
    if(level > g_show_lev) \
    { \
        g_hope_wrlen = snprintf(g_show_buffer, sizeof(g_show_buffer),fmt, ##__VA_ARGS__); \
        if(REAL_TIME_MSG == channel) {printf("%s():", __FUNCTION__);  printf("%s\r\n", g_show_buffer);} \
        else { DB_LOG_ADD(g_show_buffer); } \
    } \
}while(0)


//各个调试打印信息的级别
#define LLDB_LEV                        4
#define DEBUG_LEV                       6
#define LOG_LEV                         9
#define ERR_LEV                         10



//对各个调试接口的定义

#ifdef LLDB_SHOW_EN
#define wLMSG(fmt, a...)        WDEBUG_ACC_CHANNEL(LLDB_LEV, REAL_TIME_MSG, fmt, ##a)
#else
#define wLMSG(fmt, a...)
#endif

#ifdef DEBUG_SHOW_EN
#define wDebug(fmt, a...)       WDEBUG_ACC_CHANNEL(DEBUG_LEV, BACKEND_MSG, fmt, ##a)
#else
#define wDebug(fmt, a...)
#endif


#ifdef DEBUG_MODE
#define LOG_CHANNEL     REAL_TIME_MSG
#else
#define LOG_CHANNEL     BACKEND_MSG
#endif


#ifdef LOG_SHOW_EN
#define wLog(fmt, a...)         WDEBUG_FUNCNAME_ACC_CHANNEL(LOG_LEV, LOG_CHANNEL, fmt, ##a)
#else
#define wLog(fmt, a...)
#endif

#ifdef ERR_SHOW_EN
#define wErr(fmt, a...)         WDEBUG_FUNCNAME_ACC_CHANNEL(ERR_LEV, LOG_CHANNEL, fmt, ##a)
#else
#define wErr(fmt, a...)
#endif


#ifdef BACK_DB_EN
#define wBackDB(fmt, a...)         WDEBUG_FUNCNAME_ACC_CHANNEL(LOG_LEV, BACKEND_MSG, fmt, ##a)
#else
#define wBackDB(fmt, a...)
#endif


///////////////////////////////////////////////
void WDebug_SetShowLev(u32 lev);
void WDebugHex(u32 lev, u32 addr, u32 bytenum, u8 scale,const u8 * const str);


//下面这3个api必须同时使用,使用方法如下
/*
    DbLogUsage dbu;
    s16 err;
    u32 len;
    u8 *pLog = (u8*)1;
    
    DbLogGetStart(&dbu);
    while(NULL != pLog)
    {
        pLog = Dblog_GetNext(&dbu, &len, &err);
        //在此处理log，可以从pLog中拷贝log数据
    }
    Dblog_End(&dbu);
*/

s16 DbLogGetStart(DbLogUsage * const dbl);
u8* Dblog_GetNext(DbLogUsage * const dbl, u32* const plen, s16* const pErr);
s16 Dblog_End(DbLogUsage * const dbl);



#ifdef __cplusplus
}
#endif
#endif

