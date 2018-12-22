#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "WinDebug.h"

#ifdef DEBUG_MODE
#warning "it is debug version"
#endif


//***************************macro******************************


//***************************typedef******************************


//****************************var*******************************
u32 g_show_lev = SHOW_LEVEL;
s8 g_show_buffer[BACKEND_BUFFER_LEN];
s32 g_hope_wrlen;

//*************************macro func***************************


//***************************mix********************************


//***************************func*******************************


/*************************************************************************************
  * @brief ʮ������debug�������
  * @param lev - ���level����
  addr - ��ʼ��ַ
  bytenum - ��������ֽ���
  scale - λ�����ֱ�Ϊ8�� 16 ��32
  str - ��ʼ���ַ�����ʶ���
  * @retval None.
**************************************************************************************/
void WDebugHex(u32 lev, u32 addr, u32 bytenum, u8 scale,const u8 * const str)
{
    u32 i;

    if((8 == scale) || (16 == scale) || (32 == scale))
    {
        
        WDEBUG(lev, "%s hex%d:\r\n", str, (u32)scale);
        
        if(8 == scale)
        {
            u8 *ptr = (u8*)addr;
            for(i = 0; i < bytenum; i++)
            {
                WDEBUG(lev, "%02x ", *ptr);
                ptr++;
            }
        }
        else if(16 == scale)
        {
            u16 *ptr = (u16*)addr;
            for(i = 0; i < bytenum; i+=2)
            {
                WDEBUG(lev, "%04x ", *ptr);
                ptr++;
            }
        }
        else if(32 == scale)
        {
            u32 *ptr = (u32*)addr;
            for(i = 0; i < bytenum; i+=4)
            {
                WDEBUG(lev, "%08x ", *ptr);
                ptr++;
            }
        }
        else
        {}
        
        WDEBUG(lev, "\r\n");
    }
}


/*************************************************************************************
  * @brief  ���ÿ���ʾlog����͵ȼ�
  * @param None.
  * @retval None.
**************************************************************************************/
void WDebug_SetShowLev(u32 lev)
{
    g_show_lev = lev;
}


void WDebugtest(u32 lev, u32 val)
{
#if 0
    wDebug("ut:0x%08x\r\n", val);

    wLog("waht:%d\r\n", lev);

    wErr("abcdefj mm o xyz\r\n");

    wErr("val:0x%08x\r\n", val);
#endif
}



