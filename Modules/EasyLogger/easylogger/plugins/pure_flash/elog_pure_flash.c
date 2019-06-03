/******************Cloudoptek Raman project*********************
(c) 2018 Cloudoptek Co. Ltd.
 All rights reserved.
 Author: winkey su
 Date:2019.05.19
***************************************************************/
#include "basic_f.h"
#include "glb_cfg.h"
#include "utils.h"
#include "elog_pure_flash.h"		


//***************************macro******************************


//***************************typedef******************************


//****************************var*******************************
//PURE FLASH LOG HANDLE ȫ�ֱ�������
static PF_LOG_HANDLE s_pf_handle;


//*************************macro func***************************


//***************************mix********************************


//***************************func*******************************
u32 elog_pf_port_clear(void);
s16 elog_pf_port_init(void);
void elog_pf_port_output(const char *log, size_t size);
void elog_pf_port_lock(void);
void elog_pf_port_unlock(void);
s16 elog_pf_read(u8* const pBuff, u32 start, u32 sz);




/*************************************************************************************
  * @brief ��ʼ��
  * @param None.
  * @retval None.
**************************************************************************************/
void PFLog_Init(void)
{
    s_pf_handle.bEnable = false;
    s_pf_handle.wr = 0;
    elog_pf_port_init();
}

/*************************************************************************************
  * @brief �������
  * @param None.
  * @retval None.
**************************************************************************************/
void PFLog_Clear(void)
{
    s_pf_handle.bEnable = false;
    s_pf_handle.total_sz = elog_pf_port_clear();
    s_pf_handle.wr = 0;
    s_pf_handle.bEnable = true;
}


/**
 * Write log to flash. The flash write use buffer mode.
 *
 * @param log log
 * @param size log size
 */
s16 PFLog_write(const char *log, size_t size)
{
    if(s_pf_handle.bEnable)
    {
        if((s_pf_handle.wr + size) > s_pf_handle.total_sz)
        {
            return -1;
        }
        else
        {
            elog_pf_port_output(log, size);
            s_pf_handle.wr += size;
            return 0;
        }
    }
    else
    {
        return -2;
    }
}


/*************************************************************************************
  * @brief ��ȡlog����
  * @param  pBuf - ��ȡ���������ݴ��buffer
            start - ��ʼ���ĵ�ַ
            size - �������������ֽ���
  * @retval -
**************************************************************************************/
s16 PFLog_Read(u8 * const pBuf, u32 start, size_t size)
{
    s16 ret;
    
    ret = elog_pf_read(pBuf, start, size);
    return ret;
}

