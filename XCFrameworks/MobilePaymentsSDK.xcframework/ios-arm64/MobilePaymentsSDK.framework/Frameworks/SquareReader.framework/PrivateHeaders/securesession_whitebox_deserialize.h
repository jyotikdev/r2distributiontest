// Copyright(C) 2017 Square, Inc. All Rights Reserved.
#pragma once
#if PAYSDK_BUILD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <util.h>

#include <lib/tomcryptlite/tomcrypt.h>

#define KEY_SIZE 272
#define HMAC_SIZE 32

#define OK 0
#define ERR -1

#define DEBUG_PRINT_ENABLED (false)

#if DEBUG_PRINT_ENABLED == true
	#include <android/log.h>
    #include <inttypes.h>
    #define DEBUG_printf(msg, args...) __android_log_print(ANDROID_LOG_DEBUG, "LCR", "(%s) " msg, __func__, ##args);
#else
	#define DEBUG_printf(...)
#endif

/* This structure is our internal representation of
 * the data stored in the serialized on disk. This struct
 * will need to be accessed by libsecuresession. The
 * structure is designed to hold both keys for both iOS and
 * Android but in practice only 2 are present and must
 * be for the same architecture */
typedef struct __attribute__((__packed__)) {
	uint64_t application_id;
	uint8_t validate_key[KEY_SIZE];
	uint8_t approve_key[KEY_SIZE];
	uint8_t hmac[HMAC_SIZE];
} whitebox_keys_appid;

__attribute__((always_inline)) static inline int ms_memcmp_const_time(const void *one, const void *two, size_t n);
__attribute__((always_inline)) static inline void deobfuscator(uint8_t *key, size_t key_size, uint8_t *outkey);
uint32_t hmac_sha256_wb(uint8_t *in, size_t in_sz, uint8_t *out, size_t out_sz);
__attribute__ ((visibility ("default"))) whitebox_keys_appid *deserialize_wbkeys(uint8_t *buffer);
#endif
