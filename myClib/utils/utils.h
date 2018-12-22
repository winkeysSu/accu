#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stm32f1xx_hal.h"
#include "glb_cfg.h"
//#include "windebug.h"


//Debug conditional compiling flags
#define DISABLE_CKSUM_CHECK 1

#define HEX2ASC(h)  (((h)>9)?((h)+'A'-0x0a):((h)+'0'))

//从一个变量起始位置的偏移pos字节处取出值
#define GET_VAR_BYTE(var, pos) (*(((u8*)&var) + (pos)))

//取最大值
#define WMAX(x, y)   ((x) > (y)? (x): (y))

//取最小值
#define WMIN(x, y)   ((x) < (y)? (x): (y))

//取绝对值
#define ABS(x)  (((x) >= 0) ? (x): (-(x)))

//获取数组成员数量
#define ARRAY_LEN(array)    (u32)(sizeof((array))/sizeof((array)[0]))



//avoid this delay in release software
void SimpleDelay(uint32_t t);
void soft_delay(uint32_t cnt);
void delay_us(u32 us);

void hex2decstrII(u8 * const des, const u16* const src, u16 len);
void hex2decstr(u8 * const des, const u16* const src, u16 len);
s16 UTILITY_RevertByteSequence(void* const pDat, u32 unit_len, u32 total_len);
s16 UTILITY_FloatReverse(void *const pDst, float val);
char *itoa(int num, char *str, int radix);
bool is_bigEndian(void);
s16 UTILS_U16Average(const u16* const pbuf, u32 num, u16 * const pval);
s16 ReverseByteSequence(u16* const pDat, u32 total_len);
u16 crc_cal(const u8* const buf, u32 len, u8 ini_val);
s16 UTILS_U16MaxMin(const u16* const pbuf, u32 num, u16 * const pmax, u16 * const pmin);
s16 UTILS_U16Sum(const u16* const pbuf, u32 num, u32 * const pSum);


#ifdef __cplusplus
}
#endif

#endif //__UTILS_H__

