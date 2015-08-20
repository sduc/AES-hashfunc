#ifndef __CONFIG_H__
#define __CONFIG_H__

// standard definition if microsoft system is used

#ifdef _MSC_VER

#if (_MSC_VER < 1300)
typedef signed char       int8_t;
typedef signed short      int16_t;
typedef signed int        int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      uint32_t;
#else//_MSC_VER >= 1300
typedef signed __int8     int8_t;
typedef signed __int16    int16_t;
typedef signed __int32    int32_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
#endif//_MSC_VER >= 1300

typedef signed __int64    int64_t;
typedef unsigned __int64  uint64_t;

#else//!_MSC_VER
#include <stdint.h>
#endif//!_MSC_VER


// DEBUG related macro

#define DEBUG 1
#define PARA 1

#if DEBUG 
#define LOG_STRING(s,len)\
    printf("LOG :: %s (%d):\n",#s,len);\
    fwrite(s,1,len,stdout);\
    printf("\n")
#else
#define LOG_STRING(s,len)
#endif

#endif//__CONFIG_H__
