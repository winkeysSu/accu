#ifndef __RAM_LOG_H__
#define __RAM_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "basic_f.h"
#include "glb_cfg.h"

//macro

//ÿ��log��Ĵ�С
#define LOG_BLOCK_SZ            2048

//�ܵ�log������
#define LOG_BLOCK_NUM           10

//�Ƿ�����ramLog��
#define RAM_LOG_LOCK_ENABLE   


//typedef



//������ȡlogʱ�����õ��Ľṹ��
typedef struct __RAM_LOG_CURE
{
    s8* cur_rd;
    s8* cur_rd_blk;
    u32 cur_idx;
    u32 total_num;
    u16 magic;
}RAM_LOG_CURE;

//var


//macro func


//mix


//func
void RamLog_Clear(void);
void RamLog_Init(void);
s16 RamLog_Add(const s8* const log, u32 sz);
s16 RamLog_GetAll(s8 **p1st, u32 *p1stSz, s8 **pSec, u32* pSecSz);
void RamLog_Clear(void);
u16 RamLog_GetNum(void);


s16 RamLog_GetStart(RAM_LOG_CURE * const pCure);
s16 RamLog_GetNext(
RAM_LOG_CURE * const pCure, 
s8** ptr,
u32 *pSz, 
bool with_log_head);
s16 RamLog_GetEnd(RAM_LOG_CURE * const pCure, bool bClear);

void RamLog_SelfTest(void);

#ifdef __cplusplus
}
#endif
#endif


