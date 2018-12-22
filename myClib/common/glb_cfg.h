#ifndef __GLB_CFG_H__
#define __GLB_CFG_H__

#include "basic_f.h"
//macro


//var



//macro func


//func
#define NUCLEAR_RADIATION


//ccd��ʱ�������ʹ��
#define CCD_CLEAR_ENABLE    1



//rtos�汾app���ú�����Ϊ1��ֻ��app����ʱ���ԾͿ�����ʱΪ0
//printf�ڴ���1���ʹ��
#define PRINTF_UART1_ENABLE             1


//
#define APP_TEST_ONLINE_ENABLE          0

//ʹ��ͨ��DMA����ʹ��,
//ע�⣺����ʱ��̫Сʱ������ͨ��DMA�����ڴ���
#define CCD_TRANSMIT_DMA_ENABLE         0


//��ȡ�¶Ȼ���buffer����ʹ��
#define GET_TEMPER_RAW_DATA_ENABLE      1


//��ʼ��ʱ�͹���ֹͣģʽ�ĵ���ʹ��
#define STOP_MODE_DBG_ENABLE            1


//��ʼ��ʱ������ʹ��
#define POWER_RDP_ENABLE                0


//��log����
#define LOG_DEBUG_ENABLE                0


#if LOG_DEBUG_ENABLE

//For Debug
#define RAW_PROTOCOL_DEBUG              1

//��ʼ��logʹ��
#define BEGIN_LOG_ENALBE                1


//����boot��logʹ��
#define UPGREATE_BOOT_LOG_ENABLE        1

#define UPGREATE_BOOT_LOG_ENABLE2       1

//дflash������logʹ��
#define WRITE_FLASH_LOG_ENABLE          1

//��Դ�����logʹ��
#define POWER_MGR_LOG_ENABLE            1

//��ѯ��logʹ��
#define POLL_LOG_ENABLE                 0

#define WRTIE_FLASH_LOG_LEV             9

#define TMP_LOG_LEV                     9


#define RAW_REC_LEV                     9

//��ʼ��log
#define BEGIN_LOG_LEV                   9

//
#define SETTING_LOG_LEV                 9

//�¶Ȳɼ�log
#define TEMP_SAMP_LOG_LEV               9

//��ѹ�ɼ�log
#define VOL_SAMP_LOG_LEV                9


#define GETTING_LOG_LEV                 9


#else

#define UPGREATE_APP_LOG_ENABLE         0


//дflash������logʹ��
#define WRITE_FLASH_LOG_ENABLE          0

//����boot��logʹ��
#define UPGREATE_BOOT_LOG_ENABLE        0

#define UPGREATE_BOOT_LOG_ENABLE2       0


//��ʼ��logʹ��
#define BEGIN_LOG_ENALBE                0

//��Դ�����logʹ��
#define POWER_MGR_LOG_ENABLE            0

//��ѯ��logʹ��
#define POLL_LOG_ENABLE                 0


//��ʼ��log
#define BEGIN_LOG_LEV                   1


#define WRTIE_FLASH_LOG_LEV             1

#define TMP_LOG_LEV                     1

#define RAW_REC_LEV                     1

#define SETTING_LOG_LEV                 1

//�¶Ȳɼ�log
#define TEMP_SAMP_LOG_LEV               1

//��ѹ�ɼ�log
#define VOL_SAMP_LOG_LEV                1

#define GETTING_LOG_LEV                 1

#endif

#define BACK_DB_HOOK_EN

#ifdef BACK_DB_HOOK_EN
#define BQ24296_DB_HOOK_EN
#define EASY_PWR_DB_HOOK_EN
#define EASY_PWR_INTG_DB_HOOK_EN


#endif




//Ĭ�ϲ�����
#define DEF_BANDRATE    460800

//���ڽ��ճ�ʱ���ʱ��,ms
#define UART1_TIMEOUT   50

//ccd��ʱ������ݼ��,ms
#define CCD_CLEAR_TIME  50


//��·�������
#define BASE_BOARD_102     102

#define BASE_BOARD_103     103

//��Ʒ��
#define RAMAN_PURE          01

#define RAMAN_RAD           02


#define IS_BOARD_103_SUPP_RAD()     \
    ((BASE_BOARD_103 == g_bd_cfg) && (RAMAN_RAD == g_product_id))


extern volatile u16 g_bd_cfg;

extern volatile u8 g_product_id;



#endif

