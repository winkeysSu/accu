
#ifndef __BASIC_F_H__
#define __BASIC_F_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"

//macro


//var
#ifndef u8
typedef unsigned char u8;
#endif

#ifndef s8
typedef char s8;
#endif

#ifndef u16
typedef unsigned short int u16;
#endif

#ifndef s16
typedef short int s16;
#endif

#ifndef u32
typedef unsigned int u32;
#endif

#ifndef s32
typedef int s32;
#endif

#ifndef bool
#ifdef _Bool
typedef _Bool bool
#define true    1
#define false   0
#else
typedef enum
{
    false = 0,
    true = !false,
}bool;
#endif
#endif


#ifndef NULL
#define NULL 0
#endif


//macro func


//func


#ifdef __cplusplus
}
#endif

#endif

