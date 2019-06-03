/******************Cloudoptek Raman project*********************
(c) 2018 Cloudoptek Co. Ltd.
 All rights reserved.
 Author: winkey su
 Date:2019.05.19
***************************************************************/
#include "basic_f.h"
#include "glb_cfg.h"
#include "utils.h"
#include "WinDebug.h"
#include "macro_func.h"
#include "StdDbLog.h"
#include "FreeRtos.h"
#include "ram_log.h"		


//***************************macro******************************
//验证字符
#define RAM_MAGIC               0x5232

//每条log起始符
#define LOG_ST_CH               0xEF

//在块尾无用区域的填充字符
#define PAD_BLK_UNUSED          ' '

//每条log的额外开销，1字节起始符 + 2字节长度
#define PAY_HEAD_SZ             1

//log中表示log内容的区域所占有的字节数
#define PAY_LEN_SZ              2

//log中维护每条log的开销总长度
#define PAY_LEN_PER_ITEM        (PAY_HEAD_SZ + PAY_LEN_SZ)

//***************************typedef******************************
#if PAY_LEN_SZ == 1
typedef u8 PAY_SZ_UNIT;
#elif PAY_LEN_SZ == 2
typedef u16 PAY_SZ_UNIT;
#elif PAY_LEN_SZ == 4
typedef u32 PAY_SZ_UNIT;
#endif

//RAM LOG HANDLE 的结构体
typedef struct __RAM_LOG_TYPEDEF
{
    s8 *wr;
    s8 *rd;
    s8* tail_blk;
    s8* head_blk;
    u32 remain_in_blk;
    u32 cur_blk_idx;

    u32 log_num_in_blk[LOG_BLOCK_NUM];
    
#ifdef RAM_LOG_LOCK_ENABLE
    bool lock;
#endif
    s8 buff[LOG_BLOCK_NUM][LOG_BLOCK_SZ];
}RAM_LOG_TYPEDEF;


//****************************var*******************************
//ram_log的句柄
static RAM_LOG_TYPEDEF s_ram_log_handle;


//*************************macro func***************************


//***************************mix********************************


//***************************func*******************************

/*************************************************************************************
  * @brief RamLog初始化
  * @param None.
  * @retval None.
**************************************************************************************/
void RamLog_Init(void)
{
    RamLog_Clear();
}


/*************************************************************************************
  * @brief 写入新log
  * @param  log - 待写入的log
            sz  - log的字节长度
  * @retval 0 - 成功； 非0 - 失败
**************************************************************************************/
s16 RamLog_Add(const s8* const log, u32 sz)
{
    PAY_SZ_UNIT tmpsz = sz;
    s16 err;

    if((sz == 0)||(NULL == log)||(sz>(LOG_BLOCK_SZ - PAY_LEN_PER_ITEM)))
    {
        err = -1;
        goto __exit;
    }

#ifdef RAM_LOG_LOCK_ENABLE
    if(s_ram_log_handle.lock)
    {
        err = -2;
        goto __exit;
    }
    else
    {
        s_ram_log_handle.lock = true;
    }
#endif
    
    if((sz + PAY_LEN_PER_ITEM) > s_ram_log_handle.remain_in_blk)
    {
        memset(s_ram_log_handle.wr, PAD_BLK_UNUSED, s_ram_log_handle.remain_in_blk);
        //更新块尾指针，用于写入新的log
        if(++s_ram_log_handle.cur_blk_idx >= LOG_BLOCK_NUM)
        {   
            s_ram_log_handle.cur_blk_idx = 0;
            s_ram_log_handle.tail_blk = s_ram_log_handle.buff[0];
        }
        else
        {
            s_ram_log_handle.tail_blk += LOG_BLOCK_SZ;
        }
        s_ram_log_handle.wr = s_ram_log_handle.tail_blk;
        s_ram_log_handle.remain_in_blk = LOG_BLOCK_SZ;
        s_ram_log_handle.log_num_in_blk[s_ram_log_handle.cur_blk_idx] = 0;

        //更新块头指针，旧log丢弃
        if(s_ram_log_handle.head_blk == s_ram_log_handle.tail_blk)
        {
            if(s_ram_log_handle.head_blk == s_ram_log_handle.buff[LOG_BLOCK_NUM-1])
            {
                s_ram_log_handle.head_blk = s_ram_log_handle.buff[0];
            }
            else
            {
                s_ram_log_handle.head_blk += LOG_BLOCK_SZ;
            }
            s_ram_log_handle.rd = s_ram_log_handle.head_blk;
        }
    }

    //输入前先检查一遍指针
    if((s_ram_log_handle.wr < s_ram_log_handle.buff[0])
        ||(s_ram_log_handle.wr >= s_ram_log_handle.buff[LOG_BLOCK_NUM]))
    {
        err = -3;
        goto __prepare_exit;
    }

    *s_ram_log_handle.wr = LOG_ST_CH;
    s_ram_log_handle.wr++;
    memcpy(s_ram_log_handle.wr, &tmpsz, sizeof(tmpsz));
    s_ram_log_handle.wr += sizeof(tmpsz);
    memcpy(s_ram_log_handle.wr, log, sz);
    s_ram_log_handle.wr += sz;

    s_ram_log_handle.remain_in_blk -= (1 + sizeof(tmpsz) + sz);

    s_ram_log_handle.log_num_in_blk[s_ram_log_handle.cur_blk_idx]++;

__prepare_exit:
#ifdef RAM_LOG_LOCK_ENABLE
    s_ram_log_handle.lock = false;
#endif

    err = 0;
__exit:
    return err;    
}


/*************************************************************************************
  * @brief  获取所有log
    log通过两个buff返回，返回相应的起始地址
  * @param  p1st - 保存第一个log buff的起始地址的指针
            p1stSz - 保存第一个log buff的字节长度的指针
            pSec - 保存第二个log buff的起始地址的指针
            pSecSz - 保存第一个log buff的字节长度的指针
  * @retval 0 - 成功； 非0 - 失败
**************************************************************************************/
s16 RamLog_GetAll(s8 **p1st, u32 *p1stSz, s8 **pSec, u32* pSecSz)
{
    if(!(p1st && p1stSz && pSec && pSecSz))
    {
        return -1;
    }

#ifdef RAM_LOG_LOCK_ENABLE
    u16 ts = 0;
    do
    {
        if(false == s_ram_log_handle.lock)
        {
            break;
        }
        OS_DELAY(1);  
    }
    while(ts < 1000);
    if(ts >= 1000)
    {
        return -2;
    }
#endif

    *p1st = s_ram_log_handle.rd;
    if(s_ram_log_handle.wr < s_ram_log_handle.rd)
    {
        *p1stSz = s_ram_log_handle.buff[LOG_BLOCK_NUM] - s_ram_log_handle.rd;
        *pSec = s_ram_log_handle.buff[0];
        *pSecSz = s_ram_log_handle.wr - s_ram_log_handle.buff[0];
    }
    else
    {
        *p1stSz = s_ram_log_handle.wr - s_ram_log_handle.rd;
        *pSec = NULL;
        *pSecSz = 0;
    }
    return 0;
}

/*************************************************************************************
  * @brief 清空log
  * @param None.
  * @retval None.
**************************************************************************************/
void RamLog_Clear(void)
{
#ifdef RAM_LOG_LOCK_ENABLE
    s_ram_log_handle.lock = true;
#endif
    
    s_ram_log_handle.cur_blk_idx = 0;
    s_ram_log_handle.head_blk = s_ram_log_handle.buff[0];
    s_ram_log_handle.tail_blk = s_ram_log_handle.buff[0];
    s_ram_log_handle.wr = s_ram_log_handle.tail_blk;
    s_ram_log_handle.rd = s_ram_log_handle.head_blk;
    s_ram_log_handle.remain_in_blk = LOG_BLOCK_SZ;

#ifdef RAM_LOG_LOCK_ENABLE
    s_ram_log_handle.lock = false;
#endif
    
    memset(s_ram_log_handle.log_num_in_blk, 0x00, sizeof(s_ram_log_handle.log_num_in_blk));
}

/*************************************************************************************
  * @brief 获取log的数量
  * @param None.
  * @retval log数量
**************************************************************************************/
u16 RamLog_GetNum(void)
{
    u32 num = 0;
    for(u32 i = 0; i < LOG_BLOCK_NUM; i++)
    {
        num += s_ram_log_handle.log_num_in_blk[i];
    }
    return num;
}


/*************************************************************************************
  * @brief 开始获取log
    RamLog_GetStart(), RamLog_GetNext()，RamLog_GetEnd()三个必须配合使用,
    参考下面RamLog_SelfTest()对这三个函数的使用例子
  * @param pCure - 维护获取log过程的一个变量指针
  * @retval  0 - 成功； 非0 - 失败
**************************************************************************************/
s16 RamLog_GetStart(RAM_LOG_CURE * const pCure)
{
    if(NULL == pCure)
    {
        return -1;
    }
    
    pCure->cur_rd = s_ram_log_handle.rd;
    pCure->cur_rd_blk = s_ram_log_handle.head_blk;
    pCure->cur_idx = 0;
    pCure->total_num = RamLog_GetNum();
    pCure->magic = RAM_MAGIC;
    return 0;
}


/*************************************************************************************
  * @brief 逐条获取log
    RamLog_GetStart(), RamLog_GetNext()，RamLog_GetEnd()三个必须配合使用,
    参考下面RamLog_SelfTest()对这三个函数的使用例子
  * @param pCure - 维护获取log过程的一个变量指针
           ptr - 二级指针，用于保存获取到的log的地址
           pSz - 用于保存获取到的log的字节长度
           with_log_head - 返回的log是否带上固定格式的head
  * @retval  0 - 成功,获取到一条log
             1 - 没有新的log可以获取
             <0 - 失败
**************************************************************************************/
s16 RamLog_GetNext(
RAM_LOG_CURE * const pCure, 
s8** ptr,
u32 *pSz, 
bool with_log_head)
{
    PAY_SZ_UNIT sz;
    s8 *next, *next_blk;

    if((NULL == pCure)||(NULL == ptr)||(NULL == pSz)
        ||(RAM_MAGIC != pCure->magic))
    {
        return -1;
    }

    *ptr = NULL;
    *pSz = 0;

    if(pCure->cur_idx >= pCure->total_num)
    {
        return 1;
    }

    if(LOG_ST_CH == *(pCure->cur_rd))
    {
        sz = *(PAY_SZ_UNIT*)(pCure->cur_rd + 1);
        if((sz <= 2)||(sz > LOG_BLOCK_SZ))
        {
            return -3;
        }
        
        if(with_log_head)
        {
            *ptr = pCure->cur_rd;
            *pSz = sz + PAY_LEN_PER_ITEM;
        }
        else
        {
            *ptr = pCure->cur_rd + PAY_LEN_PER_ITEM;
            *pSz = sz;
        }

        next = pCure->cur_rd + PAY_LEN_PER_ITEM + sz;
        next_blk = pCure->cur_rd_blk + LOG_BLOCK_SZ;
        if((*next != LOG_ST_CH)||(next >= next_blk))
        {
            //越过相应的log内容的长度后，如果没有看到开始符，
            //或者该条log长度越过当前block，则已经读了该block中最后一条log了

            if(next_blk >= s_ram_log_handle.buff[LOG_BLOCK_NUM])
            {
                //读到了最下面的blk
                next_blk = s_ram_log_handle.buff[0];                
            }
            next = next_blk;
            pCure->cur_rd_blk = next;
        }

        pCure->cur_rd = next;
        pCure->cur_idx++;
        return 0;
    }
    else
    {
        //log have err
        return -2;
    }
    
}

/*************************************************************************************
  * @brief log获取完成
    RamLog_GetStart(), RamLog_GetNext()，RamLog_GetEnd()三个必须配合使用,
    参考下面RamLog_SelfTest()对这三个函数的使用例子
  * @param pCure - 维护获取log过程的一个变量指针
           bClear - 是否清空log
  * @retval  0 - 成功,获取到一条log
             非0 - 失败
**************************************************************************************/
s16 RamLog_GetEnd(RAM_LOG_CURE * const pCure, bool bClear)
{
    if((NULL == pCure)||(RAM_MAGIC != pCure->magic))
    {
        return -1;
    }

    if(bClear)
    {
        RamLog_Clear();
    }

    memset(pCure, 0x00, sizeof(pCure));
    
    return 0;
}

/*************************************************************************************
  * @brief log逐条获取自测试
  * @param None
  * @retval None
**************************************************************************************/
void RamLog_SelfTest(void)
{
    RAM_LOG_CURE cure;
    s16 ret = 0;
    s8* pwr;
    u32 len, ts, chknum;
    
    RamLog_GetStart(&cure);
    ts = 0;
    while(0 == ret)
    {
        ret = RamLog_GetNext(&cure, &pwr, &len, false);
        if(0 == ret)
        {
            ts++;
        }
    }
    chknum = RamLog_GetNum();
    
    RamLog_GetEnd(&cure, true);
    printf("\r\nts:%u, total:%u, ret:%d\r\n", ts, chknum, ret);
}


