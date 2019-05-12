#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "basic_f.h"

extern uint8_t retUSER; /* Return value for USER */
extern s8 USERPath[4]; /* USER logical drive path */
extern FATFS USERFatFS; /* File system object for USER logical drive */
extern FIL USERFile; /* File object for USER */

void FatfsApp_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
