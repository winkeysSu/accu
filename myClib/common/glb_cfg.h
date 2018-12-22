#ifndef __GLB_CFG_H__
#define __GLB_CFG_H__

#include "basic_f.h"
//macro


//var



//macro func


//func
#define NUCLEAR_RADIATION


//ccd定时清空数据使能
#define CCD_CLEAR_ENABLE    1



//rtos版本app，该宏设置为1；只是app中暂时调试就可以暂时为0
//printf在串口1输出使能
#define PRINTF_UART1_ENABLE             1


//
#define APP_TEST_ONLINE_ENABLE          0

//使能通过DMA发送使能,
//注意：积分时间太小时，不能通过DMA来串口传输
#define CCD_TRANSMIT_DMA_ENABLE         0


//读取温度缓存buffer数据使能
#define GET_TEMPER_RAW_DATA_ENABLE      1


//初始化时低功耗停止模式的调试使能
#define STOP_MODE_DBG_ENABLE            1


//初始化时读保护使能
#define POWER_RDP_ENABLE                0


//总log开关
#define LOG_DEBUG_ENABLE                0


#if LOG_DEBUG_ENABLE

//For Debug
#define RAW_PROTOCOL_DEBUG              1

//初始化log使能
#define BEGIN_LOG_ENALBE                1


//更新boot的log使能
#define UPGREATE_BOOT_LOG_ENABLE        1

#define UPGREATE_BOOT_LOG_ENABLE2       1

//写flash参数的log使能
#define WRITE_FLASH_LOG_ENABLE          1

//电源管理的log使能
#define POWER_MGR_LOG_ENABLE            1

//轮询的log使能
#define POLL_LOG_ENABLE                 0

#define WRTIE_FLASH_LOG_LEV             9

#define TMP_LOG_LEV                     9


#define RAW_REC_LEV                     9

//初始化log
#define BEGIN_LOG_LEV                   9

//
#define SETTING_LOG_LEV                 9

//温度采集log
#define TEMP_SAMP_LOG_LEV               9

//电压采集log
#define VOL_SAMP_LOG_LEV                9


#define GETTING_LOG_LEV                 9


#else

#define UPGREATE_APP_LOG_ENABLE         0


//写flash参数的log使能
#define WRITE_FLASH_LOG_ENABLE          0

//更新boot的log使能
#define UPGREATE_BOOT_LOG_ENABLE        0

#define UPGREATE_BOOT_LOG_ENABLE2       0


//初始化log使能
#define BEGIN_LOG_ENALBE                0

//电源管理的log使能
#define POWER_MGR_LOG_ENABLE            0

//轮询的log使能
#define POLL_LOG_ENABLE                 0


//初始化log
#define BEGIN_LOG_LEV                   1


#define WRTIE_FLASH_LOG_LEV             1

#define TMP_LOG_LEV                     1

#define RAW_REC_LEV                     1

#define SETTING_LOG_LEV                 1

//温度采集log
#define TEMP_SAMP_LOG_LEV               1

//电压采集log
#define VOL_SAMP_LOG_LEV                1

#define GETTING_LOG_LEV                 1

#endif

#define BACK_DB_HOOK_EN

#ifdef BACK_DB_HOOK_EN
#define BQ24296_DB_HOOK_EN
#define EASY_PWR_DB_HOOK_EN
#define EASY_PWR_INTG_DB_HOOK_EN


#endif




//默认波特率
#define DEF_BANDRATE    460800

//串口接收超时检测时间,ms
#define UART1_TIMEOUT   50

//ccd定时清空数据间隔,ms
#define CCD_CLEAR_TIME  50


//电路基本板号
#define BASE_BOARD_102     102

#define BASE_BOARD_103     103

//产品号
#define RAMAN_PURE          01

#define RAMAN_RAD           02


#define IS_BOARD_103_SUPP_RAD()     \
    ((BASE_BOARD_103 == g_bd_cfg) && (RAMAN_RAD == g_product_id))


extern volatile u16 g_bd_cfg;

extern volatile u8 g_product_id;



#endif

