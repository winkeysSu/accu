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
//��֤�ַ�
#define RAM_MAGIC               0x5232

//ÿ��log��ʼ��
#define LOG_ST_CH               0xEF

//�ڿ�β�������������ַ�
#define PAD_BLK_UNUSED          ' '

//ÿ��log�Ķ��⿪����1�ֽ���ʼ�� + 2�ֽڳ���
#define PAY_HEAD_SZ             1

//log�б�ʾlog���ݵ�������ռ�е��ֽ���
#define PAY_LEN_SZ              2

//log��ά��ÿ��log�Ŀ����ܳ���
#define PAY_LEN_PER_ITEM        (PAY_HEAD_SZ + PAY_LEN_SZ)

//***************************typedef******************************
#if PAY_LEN_SZ == 1
typedef u8 PAY_SZ_UNIT;
#elif PAY_LEN_SZ == 2
typedef u16 PAY_SZ_UNIT;
#elif PAY_LEN_SZ == 4
typedef u32 PAY_SZ_UNIT;
#endif

//RAM LOG HANDLE �Ľṹ��
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
//ram_log�ľ��
static RAM_LOG_TYPEDEF s_ram_log_handle;


//*************************macro func***************************


//***************************mix********************************


//***************************func*******************************

/*************************************************************************************
  * @brief RamLog��ʼ��
  * @param None.
  * @retval None.
**************************************************************************************/
void RamLog_Init(void)
{
    RamLog_Clear();
}


/*************************************************************************************
  * @brief д����log
  * @param  log - ��д���log
            sz  - log���ֽڳ���
  * @retval 0 - �ɹ��� ��0 - ʧ��
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
        //���¿�βָ�룬����д���µ�log
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

        //���¿�ͷָ�룬��log����
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

    //����ǰ�ȼ��һ��ָ��
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
  * @brief  ��ȡ����log
    logͨ������buff���أ�������Ӧ����ʼ��ַ
  * @param  p1st - �����һ��log buff����ʼ��ַ��ָ��
            p1stSz - �����һ��log buff���ֽڳ��ȵ�ָ��
            pSec - ����ڶ���log buff����ʼ��ַ��ָ��
            pSecSz - �����һ��log buff���ֽڳ��ȵ�ָ��
  * @retval 0 - �ɹ��� ��0 - ʧ��
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
  * @brief ���log
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
  * @brief ��ȡlog������
  * @param None.
  * @retval log����
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
  * @brief ��ʼ��ȡlog
    RamLog_GetStart(), RamLog_GetNext()��RamLog_GetEnd()�����������ʹ��,
    �ο�����RamLog_SelfTest()��������������ʹ������
  * @param pCure - ά����ȡlog���̵�һ������ָ��
  * @retval  0 - �ɹ��� ��0 - ʧ��
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
  * @brief ������ȡlog
    RamLog_GetStart(), RamLog_GetNext()��RamLog_GetEnd()�����������ʹ��,
    �ο�����RamLog_SelfTest()��������������ʹ������
  * @param pCure - ά����ȡlog���̵�һ������ָ��
           ptr - ����ָ�룬���ڱ����ȡ����log�ĵ�ַ
           pSz - ���ڱ����ȡ����log���ֽڳ���
           with_log_head - ���ص�log�Ƿ���Ϲ̶���ʽ��head
  * @retval  0 - �ɹ�,��ȡ��һ��log
             1 - û���µ�log���Ի�ȡ
             <0 - ʧ��
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
            //Խ����Ӧ��log���ݵĳ��Ⱥ����û�п�����ʼ����
            //���߸���log����Խ����ǰblock�����Ѿ����˸�block�����һ��log��

            if(next_blk >= s_ram_log_handle.buff[LOG_BLOCK_NUM])
            {
                //�������������blk
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
  * @brief log��ȡ���
    RamLog_GetStart(), RamLog_GetNext()��RamLog_GetEnd()�����������ʹ��,
    �ο�����RamLog_SelfTest()��������������ʹ������
  * @param pCure - ά����ȡlog���̵�һ������ָ��
           bClear - �Ƿ����log
  * @retval  0 - �ɹ�,��ȡ��һ��log
             ��0 - ʧ��
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
  * @brief log������ȡ�Բ���
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


