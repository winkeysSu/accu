//  Cloudoptek Raman V2 project
//
// utility, helper and debug routines.
//
// (c) 2018 Cloudoptek Co. Ltd.
// All rights reserved.
//
//  Author: winkey
//  Date:   6:16 PM 3/13/2018
//

#include "utils.h"

void SimpleDelay(u32 t)
{
//Simple Block Delay, for debug, don't use it in release version
//Delay about 1 mini Second
//ARMCC 5.06, -O0, microlib
    //Cortex M4: 6950
#define NOP_DELAY_CONST 6950
    u32 d = SystemCoreClock / NOP_DELAY_CONST;
    u32 e;

    while(--d)
    {
        e = t;

        while(--e)
        {
            __NOP();
        }
    }

#undef NOP_DELAY_CONST
}



void soft_delay(u32 cnt)
{
    u32 i;
    for(i = 0; i < cnt; i++)
    {
        __NOP();
    }
}


/*************************************************************************************
  * @brief  16λ����ת��Ϊ�ɼ�decimal�ַ�����.
  *
  * @param None.
  * @retval None.
**************************************************************************************/
void hex2decstr(u8 * const des, const u16* const src, u16 len)
{
    u16 i;
    const u16 *src_pt;
    u8* des_pt;
    s8 tmp_buf[20];
    
    src_pt = src;
    des_pt = des;
    *des_pt = NULL;
    for(i = 0; i < len; i++)
    {
        sprintf(tmp_buf, "%05d\r\n",*(src_pt + i));
        strcat((s8*)des_pt, tmp_buf);
    }
}


void delay_us(u32 us)
{
    for(volatile u32 i = 0; i < us; i++)
    {
        for(u32 j = 0; j < 50; j++)
        {
            __NOP();
        }
    }

}



/*************************************************************************************
  * @brief  16λ����ת��Ϊ�ɼ�decimal�ַ�����,�汾��
  *
  * @param None.
  * @retval None.
**************************************************************************************/
void hex2decstrII(u8 * const des, const u16* const src, u16 len)
{
    u16 i;
    const u16 *src_pt;
    u8* des_pt;
    s8 tmp_buf[20];
    
    src_pt = src;
    des_pt = des;
    *des_pt = NULL;
    for(i = 0; i < len; i++)
    {
        sprintf(tmp_buf, "%05d ",*(src_pt + i));
        strcat((s8*)des_pt, tmp_buf);
    }
}


/*************************************************************************************
* @brief  �ֽ���ת
*
* @param 
pDat - ת��������bufferָ�룬ת�������ݱ�����ԭ��
unit_len - ת�������ݵ�λ����(��λ���ֽ�)
total_len - ���ٸ����ݵ�λ��ת��
* @retval 0 ��ȷ����
         ��0 ����
**************************************************************************************/
s16 UTILITY_RevertByteSequence(void* const pDat, u32 unit_len, u32 total_len)
{
    u32 i, trlen, j;
    u8 tmp, *pUhead;

    if(NULL == pDat)
    {
        return -1;
    }
    if((0 == unit_len) || (0 == total_len))
    {
        return -2;
    }

    trlen = unit_len / 2;
    if(0 == trlen)
    {
        return -3;
    }

    pUhead = (u8*)pDat;
    
    for(i = 0; i < total_len; i++)
    {
        for(j = 0; j < trlen; j++)
        {
            tmp = *(pUhead + j);
            *(pUhead + j) = *(pUhead + unit_len - j - 1);
            *(pUhead + unit_len - j - 1) = tmp;
        }
        pUhead += unit_len;
    }

    return 0;
}


/*************************************************************************************
* @brief  ���������ֽ���ת�����浽���ֽ���
*
* @param dst 
* @retval 0 ��ȷ����
         ��0 ����
**************************************************************************************/
s16 UTILITY_FloatReverse(void *const pDst, float val)
{
    if(NULL == pDst)
    {
        return -1;
    }

    u8* const tp = (u8*)pDst;
    tp[0] = *((u32*)&val) >> 24;
    tp[1] = *((u32*)&val) >> 16;
    tp[2] = *((u32*)&val) >> 8;
    tp[3] = *((u32*)&val) >> 0;
    return 0;
}


/**
 * @brief  ����ת�ַ���
 * @param[in,out]
 num - ����ֵ
 str - Ŀ��buffer
 radix - ת��Ϊ�Ľ��ƣ�10����16
 * @param[in]
 * @return Ŀ��bufferָ��
 */
char *itoa(int num, char *str, int radix)
{
    char index[] = "0123456789ABCDEF";
    unsigned unum;
    int i = 0, j, k;
    char temp;

    if((radix == 10) && (num < 0))
    {
        unum = (unsigned) - num;
        str[i++] = '-';
    }
    else
    {
        unum = (unsigned)num;
    }

    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= (u32)radix;
    }
    while(unum);

    str[i] = '\0';

    if(str[0] == '-')
    {
        k = 1;
    }
    else
    {
        k = 0;
    }

    for(j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }

    return str;
}


/**
 * @brief  �ֽ����ж�
 * @param none
 * @return true - ����ֽ���false - С��
 */
bool is_bigEndian(void)
{
    u16 test = 0x8800;
    return (((*(u8*)&test & 0xff) == 0x88)? true : false);
}

/**
 * @brief  ��Ǹ�16λ��������ľ�ֵ
 * @param 
 * @return  0 -�ɹ�������ʧ��
 */
s16 UTILS_U16Average(const u16* const pbuf, u32 num, u16 * const pval)
{
    u32 total;
    
    if((0 == num) || (NULL == pbuf))
    {
        return -1;
    }
    
    total = 0;
    for(u32 i = 0; i < num; i++)
    {
        total += pbuf[i];
    }
    total /= num;
    *pval = (u16)total;
    return 0;
}


/**
 * @brief  ��Ǹ�16λ��������ĺ�
 * @param 
 * @return  0 -�ɹ�������ʧ��
 */
s16 UTILS_U16Sum(const u16* const pbuf, u32 num, u32 * const pSum)
{
    u32 total;
    
    if((0 == num) || (NULL == pbuf))
    {
        return -1;
    }
    
    total = 0;
    for(u32 i = 0; i < num; i++)
    {
        total += pbuf[i];
    }
    *pSum = total;
    return 0;
}


/**
 * @brief  ��Ǹ�16λ�����������ֵ
 * @param 
 * @return  0 -�ɹ�������ʧ��
 */
s16 UTILS_U16MaxMin(const u16* const pbuf, u32 num, u16 * const pmax, u16 * const pmin)
{
    u32 i;
    u16 max, min;

    if((0 == num) || (NULL == pbuf))
    {
        return -1;
    }

    max = pbuf[0];
    min = pbuf[0];

    for(i = 1; i < num; i++)
    {
        max = WMAX(max, pbuf[i]);
        min = WMIN(min, pbuf[i]);
    }
    *pmin = min;
    *pmax = max;
    return 0.;
}


/*************************************************************************************
   * @brief  ˫�ֽڵ��ֽ���ת
   *
   * @param pDat bufferĿ��bufferָ��
            total_len Ҫ�ı�ĳ���,���ж��ٸ�˫�ֽ�
   * @retval 0 ��ȷ����
             ��0 ����
 **************************************************************************************/
s16 ReverseByteSequence(u16* const pDat, u32 total_len)
{
    u32 i;
    u8 tmp;
    
    if(NULL == pDat)
    {
        return -1;
    }
    if(0 == total_len)
    {
        return -2;
    }

    for(i = 0; i < total_len; i++)
    {
        tmp = *(u8*)(pDat + i);
        *(u8*)(pDat + i) = *((u8*)(pDat + i) + 1);
        *((u8*)(pDat + i) + 1) = tmp;
    }

    return 0;
}


/*************************************************************************************
  * @brief ����crc16
  * @param None.
  * @retval None.
**************************************************************************************/
u16 crc_cal(const u8* const buf, u32 len, u8 ini_val)
{
    u32 i;
    u16 val = ini_val;

    for(i = 0; i < len; i++)
    {
        val += *(buf + i);
    }

    return val;
}


/*************************************************************************************
  * @brief  16�����ַ�����ת��Ϊ�ɼ�ASCII�ַ�����
  *
  * @param None.
  * @retval None.
**************************************************************************************/
s16 hex8ToASCII(u8 * const des, const u8* const src, u16 dlen, u16 slen)
{
    u16 i;
    const u8 *src_pt;
    u8* des_pt;
    s8 tmp_buf[4];

    if((NULL == des) && (NULL == src))
    {
        return -1;
    }

    if(dlen <= (slen * 2))
    {
        return -2;
    }

    src_pt = src;
    des_pt = des;
    *des_pt = NULL;
    for(i = 0; i < slen; i++)
    {
        memset(tmp_buf, 0x00, sizeof(tmp_buf));
        sprintf(tmp_buf, "%02x ",*(src_pt + i));
        strcat((s8*)des_pt, tmp_buf);
    }
    return 0;
}





