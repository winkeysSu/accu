/******************Cloudoptek Raman project*********************
 main.c
(c) 2018 Cloudoptek Co. Ltd.
 All rights reserved.
 Author: winkey su
 Date:2018.8.7
***************************************************************/
#include "StdDbLog.h"
#include "WinDebug.h"
#include <string.h>

//***************************macro******************************


//***************************typedef******************************


//****************************var*******************************
static volatile DbLogVV s_DbLog;


//*************************macro func***************************


//***************************mix********************************


//***************************func*******************************

/*************************************************************************************
  * @brief 初始化
  * @param None.
  * @retval None.
**************************************************************************************/
void DbLog_Init(void)
{
    s_DbLog.rd_idx = 0;

    WDEBUG(10,"sizeof(DbLogStruct):%d, s_DbLog:%d\r\n", sizeof(DbLogStruct), sizeof(s_DbLog));
}


/*************************************************************************************
  * @brief 清空
  * @param None.
  * @retval None.
**************************************************************************************/
void DbLog_Clear(void)
{
    s_DbLog.rd_idx = 0;
    s_DbLog.wr_idx = 0;
    s_DbLog.num = 0;
    memset((void*)s_DbLog.back_log_array, 0x00, sizeof(s_DbLog.back_log_array));

    //wDebug("sizeof(DbLogStruct):%d, s_DbLog:%d\r\n", sizeof(DbLogStruct), sizeof(s_DbLog));
}



/*************************************************************************************
  * @brief log添加
  * @param None.
  * @retval None.
**************************************************************************************/
void DbLog_Add(const s8* const func, const s8* const desc)
{
    u16 wr_dx, len;
    
    s_DbLog.wr_idx = ((s_DbLog.wr_idx >= LOG_NUM)? 0: s_DbLog.wr_idx);
    wr_dx = s_DbLog.wr_idx;

    if((LOG_NUM <= s_DbLog.num) && (s_DbLog.wr_idx == s_DbLog.rd_idx))
    {
        //数组满了，覆盖最早用过的数组
        s_DbLog.rd_idx++;
        if(s_DbLog.rd_idx >= LOG_NUM)
        {
            s_DbLog.rd_idx = 0;
        }
    }
    else
    {
        ++s_DbLog.num;
    }

    memset((void*)&s_DbLog.back_log_array[wr_dx], 0x20, sizeof(DbLogStruct));
    s_DbLog.back_log_array[wr_dx].timestamp = LOG_GET_TICK();

    len = strlen(func);
    len = (len > LOG_FUNC_NAME_LEN? LOG_FUNC_NAME_LEN: len);
    memcpy((s8*)s_DbLog.back_log_array[wr_dx].func_name, func, len);

    len = strlen(desc);
    len = (len > LOG_DESC_LEN? LOG_DESC_LEN: len);
    memcpy((s8*)s_DbLog.back_log_array[wr_dx].desc, desc, len);
    s_DbLog.back_log_array[wr_dx].delt1 = DELTA_CH;
    s_DbLog.back_log_array[wr_dx].delt2 = DELTA_CH;
    s_DbLog.back_log_array[wr_dx].end[0] = '\r';
    s_DbLog.back_log_array[wr_dx].end[1] = '\n';
    s_DbLog.wr_idx++;    
    return;
}



/*************************************************************************************
  * @brief log指针获取
  * @param None.
  * @retval None.
**************************************************************************************/
const DbLogVV* DbLog_GetPtr(void)
{
    return (const DbLogVV*)&s_DbLog;
}

/*************************************************************************************
  * @brief 开始获取log
  * @param dbl - 获取log的一个结构体指针
  * @retval 0 - 正确
**************************************************************************************/
s16 DbLogGetStart(DbLogUsage * const dbl)
{
    if(NULL == dbl)
    {
        return -1;
    }

    dbl->pDbLogVV = (DbLogVV*)&s_DbLog;
    dbl->step = 0;
    dbl->flag = DBLOG_MAGIC;
    dbl->rdx = s_DbLog.rd_idx;
    return 0;
}

/*************************************************************************************
  * @brief 获取下一个log
  * @param dbl - 从DbLogGetStart()获取的结构体
            plen - 指针，保存获取的log字节数
            pErr - 指针，保存错误信息
  * @retval 0 - 正确获取到log
            1 - 没有log了
            <0 - 错误
**************************************************************************************/
u8* Dblog_GetNext(DbLogUsage * const dbl, u32* const plen, s16* const pErr)
{    
    *pErr = 0;
    if(DBLOG_MAGIC != dbl->flag)
    {
        *pErr = -1;
        return NULL;
    }
    else
    {
        if(dbl->step >= dbl->pDbLogVV->num)
        {
            //log已经读取到最后
            *pErr = 1;
            return NULL;
        }
        else
        {
            if(dbl->rdx >= LOG_NUM)
            {
                dbl->rdx = 0;
            }
            dbl->step++;
            *plen = sizeof(dbl->pDbLogVV->back_log_array[0]);
            *pErr = 0;
            return (u8*)&dbl->pDbLogVV->back_log_array[dbl->rdx++];
        }
    }
}


/*************************************************************************************
  * @brief log获取结束
  * @param dbl - 从DbLogGetStart()获取的结构体
  * @retval 0 - 正确获取到log
            <0 - 错误
**************************************************************************************/
s16 Dblog_End(DbLogUsage * const dbl)
{
    if(NULL == dbl)
    {
        return -1;
    }
    
    DbLog_Clear();
    memset(dbl, 0, sizeof(DbLogUsage));
    return 0;
}


