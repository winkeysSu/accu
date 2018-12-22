#ifndef __STD_DB_LOG_H__
#define __STD_DB_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "basic_f.h"

//macro

u32 HAL_GetTick(void);
//获取系统心跳的宏
#define LOG_GET_TICK()      HAL_GetTick()



//最大支持的log数量
#define LOG_NUM                     500

/*
由于保存数据的结构体必须保证4字节对齐，中间不能有空洞，建议采用以下配置
#define DELTA_CH                    ':'
#define LOG_FUNC_NAME_LEN           16
#define LOG_DESC_LEN                24
#define LOG_DESC_LEN_BEFORE_ERRNUM  10
*/

#define DELTA_CH                    ':'
#define LOG_FUNC_NAME_LEN           16
#define LOG_DESC_LEN                24
#define LOG_DESC_LEN_BEFORE_ERRNUM  10

//
#define DBLOG_MAGIC                 0xF8F90A88


//typedef
//该结构体必须保证4字节对齐，中间不能有空洞
typedef struct tagDbLogStruct
{
    u32 timestamp;
    u8 delt1;
    u8 func_name[LOG_FUNC_NAME_LEN];
    u8 delt2;
    u8 desc[LOG_DESC_LEN];
    u8 end[2];
}DbLogStruct;


typedef struct tagDbLogVV
{
    DbLogStruct back_log_array[LOG_NUM];
    u16 rd_idx;
    u16 wr_idx;
    u16 num;
}DbLogVV;


typedef struct tagDbLogUsage
{
    DbLogVV* pDbLogVV;
    u32 step;
    u32 flag;
    u32 rdx;
}DbLogUsage;


//var


//macro func


//mix


//func
void DbLog_Init(void);
void DbLog_Clear(void);
void DbLog_Add(const s8* const func, const s8* const desc);
const DbLogVV* DbLog_GetPtr(void);




#define DB_LOG_ADD(desc)            DbLog_Add(__FUNCTION__, (desc))


#ifdef __cplusplus
}
#endif
#endif

