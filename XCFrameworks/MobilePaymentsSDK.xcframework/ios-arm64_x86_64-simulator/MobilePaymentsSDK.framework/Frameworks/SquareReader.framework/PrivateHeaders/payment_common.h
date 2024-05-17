// Copyright 2017 Square, Inc.
#pragma once
#include <payment_generated_tags.h>

#ifndef SHORT2BYTEARRAY
    #define SHORT2BYTEARRAY(v, ba)                  {ba[0] = (uint8_t)(v >> 8); ba[1] = (uint8_t)v;}
#endif

#ifndef LONG2BYTEARRAY
    #define LONG2BYTEARRAY(v, ba)                  {ba[0] = (v >> 24); ba[1] = (uint8_t)(v >> 16);ba[2] = (uint8_t)(v >> 8); ba[3] = (uint8_t)v;}
#endif

#ifndef BYTE2SHORT
    #define BYTE2SHORT(x) ((uint16_t)((((const uint8_t*)(x))[0]) | (((const uint8_t*)(x))[1] << 8)))
#endif

#ifndef BYTE2LONG
    #define BYTE2LONG(x) ((((const uint8_t*)(x))[0]) | (((const uint8_t*)(x))[1] << 8) | (((const uint8_t*)(x))[2] << 16) | (((const uint8_t*)(x))[3] << 24))
#endif

#ifndef BYTE2SHORTLE
    #define BYTE2SHORTLE(x) ((uint16_t)((((const uint8_t*)(x))[1]) | (((const uint8_t*)(x))[0] << 8)))
#endif

#ifndef BYTE3LONGLE
    #define BYTE3LONGLE(x) ((((const uint8_t*)(x))[2]) |  (((const uint8_t*)(x))[1] << 8)|(((const uint8_t*)(x))[0] << 16))
#endif

#ifndef BYTE4LONGLE
    #define BYTE4LONGLE(x) ((((const uint8_t*)(x))[3]) | (((const uint8_t*)(x))[2] << 8) | (((const uint8_t*)(x))[1] << 16) | (((const uint8_t*)(x))[0] << 24))
#endif

#define SETVALUE(X,Y) (X |= (Y))
#define CLEARVALUE(X,Y) (X = (X & (~(uint32_t)(Y))))
#define TESTBITS(X,Y) ((X) && (Y) && (((X) & (Y)) == (Y)))
#define TESTMASK(X,Y) ((X) && (Y) && (((X) & (Y)) != (0)))

#define  LEN_DATE                  3
#define  LEN_TRACK2_DATE           2
#define  LEN_FULL_DATE             4
#ifndef LEN_TIME
    #define  LEN_TIME              3
#endif
#ifndef LEN_TRANSACTION_TYPE
    #define  LEN_TRANSACTION_TYPE  1
#endif
#ifndef  LEN_AMOUNT
    #define  LEN_AMOUNT            6
#endif
#define MAX_MONTH (12)
#define  LEN_ACCOUNT_TYPE          1
#define  MAX_LEN_ACCOUNT_TYPE_LIST 6
#define  LEN_ISSUER_ARC            2
#define LEN_PTAG_SONIC_BRANDING    1
#define LEN_TRANSACTION_MODE       1
#define LEN_CARD_INTERFACE         1
#define MAX_LEN_TMS_PROD_ID        31
#define MAX_LEN_KERNEL_KEY_DATA    132

#define TAG_TMSCAPK_STRUCT_VERSION          0x01

#define TMS_CAPK_STRUCT_VERSION             3
#define TMS_CAPK_STRUCT_VERSION_OFFSET      2
#define TMS_CAPK_STRUCT_VERSION_LEN         4

#define INVALID_REMOTE_PAYMENT_SESSION_ID   0

typedef enum
{
    ACCOUNT_TYPE_DEFAULT     = 0x00,
    ACCOUNT_TYPE_SAVINGS     = 0x10,
    ACCOUNT_TYPE_CHEQUING    = 0x20,
    ACCOUNT_TYPE_CREDIT      = 0x30,
    ACCOUNT_TYPE_INVALID     = 0xFF
} account_type_t;