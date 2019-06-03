#ifndef __ELOG_PURE_FLASH_H__
#define __ELOG_PURE_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "basic_f.h"
#include "glb_cfg.h"

//macro


//typedef
//PURE FLASH LOG HANDLE 结构体定义
typedef struct __PF_LOG_HANDLE
{
    u32 wr;
    u32 total_sz;
    u32 remain_sz;

    bool bEnable;
}PF_LOG_HANDLE;

//var


//macro func


//mix


//func
void PFLog_Init(void);
void PFLog_Clear(void);
s16 PFLog_write(const char *log, size_t size);
s16 PFLog_Read(u8 * const pBuf, u32 start, size_t size);



#ifdef __cplusplus
}
#endif
#endif


