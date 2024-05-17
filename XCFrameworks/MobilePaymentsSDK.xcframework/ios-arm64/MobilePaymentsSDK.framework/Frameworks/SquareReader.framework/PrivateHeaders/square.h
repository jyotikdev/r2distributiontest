// Copyright 2019 Square, Inc.

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SQ_LSB(x) ((x) ^ ((x) & ((x)-1)))
#define SQ_ROUNDUP(x, y) ((((x) + (y)-1)) & ~((y)-1))

#define SQ_UNUSED(x) ((void)(x))

#if defined(__GNUC__) || defined(__clang__)
#define SQ_ALIGN(n) __attribute__((aligned(n)))
#else
#error "SQ_ALIGN not supported!"
#endif

#define SQ_NELEM(x) (sizeof(x) / sizeof((x)[0]))

#if defined(__GNUC__) || defined(__clang__)
#define SQ_NORETURN __attribute__((noreturn))
#else
#error "SQ_NORETURN not supported!"
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SQ_SECTION(s) __attribute__((section(s)))
#else
#error "SQ_SECTION not supported!"
#endif

#if defined(__clang__)
#define SQ_NO_OPTIMIZE __attribute__((optnone))
#elif defined(__GNUC__)
#define SQ_NO_OPTIMIZE __attribute__((optimize("O0")))
#else
#error "SQ_NO_OPTIMIZE not supported!"
#endif

#if defined(__GNUC__) || defined(__clang__)
/*
 * Counts the number of bits set in a byte
 */
#define SQ_BITS_SET(x) ({ \
        ((x >> 0) & 1) + \
        ((x >> 1) & 1) + \
        ((x >> 2) & 1) + \
        ((x >> 3) & 1) + \
        ((x >> 4) & 1) + \
        ((x >> 5) & 1) + \
        ((x >> 6) & 1) + \
        ((x >> 7) & 1); \
    })
#else
#error "SQ_BITS_SET not supported!"
#endif

/*
 * Note: SQ_STACK should eventually be extended to all other embedded
 * targets to aid in stack isolation when using the MPU. For the moment,
 * this is enabled on K450 to relocate stacks to (non-executable) data
 * memory.
 */
#if ((TARGET_DEVICE_K450 || TARGET_DEVICE_K480) && TARGET_EMBEDDED)
#define SQ_STACK SQ_SECTION(".stack")
#else
#define SQ_STACK
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SQ_WEAK __attribute__((weak))
#else
#error "SQ_WEAK not supported!"
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SQ_MAX(x, y) ({ \
        __typeof__(x) const x_eval = (x); \
        __typeof__(y) const y_eval = (y); \
        x_eval > y_eval ? x_eval : y_eval; \
    })
#else
#define SQ_MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SQ_MIN(x, y) ({ \
        __typeof__(x) const x_eval = (x); \
        __typeof__(y) const y_eval = (y); \
        x_eval < y_eval ? x_eval : y_eval; \
    })
#else
#define SQ_MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SQ_CLAMP(x, max, min) ({\
        __typeof__(x) const x_eval = (x); \
        __typeof__(max) const max_eval = (max); \
        __typeof__(min) const min_eval = (min); \
        (x_eval > min_eval ? (x_eval < max_eval ? x_eval : max_eval) : min_eval); \
        })
#else
#define SQ_CLAMP(x, max, min) (SQ_MIN(max, SQ_MAX(min, x)))
#endif

/*
 *  Used to get the size of a struct member when you don't have an instance of
 *  the struct on hand.
 */
#define SQ_MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

/*
 *  Used to cause a warning when the return value of a function is not checked.
 */
#if defined(__GNUC__) || defined(__clang__)
    #define SQ_MUST_CHECK __attribute__((warn_unused_result))
#else
    #define SQ_MUST_CHECK
#endif

/*
 * Indirect, non-inlined calls are employed for assertions to avoid overzealous
 * compiler optimizations that result in confusing stack traces. This requires
 * some amount of trickery to make unit tests happy. Only include this on
 * Platforms that have uCOS call().
 */
#if OS_UCOSIII
#include <uCOS-III/call.h>
#else
#define call(f) (f)()
#endif

#if USE_FUZZING_COMPATIBILITY
#include <pthread.h>

#define SQ_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "assertion \"%s\" failed: " \
                    "file \"%s\", line %d, function: %s\n", \
                    #expr, __FILE__, __LINE__, __func__); \
            pthread_exit(0); \
        } \
    } while (false)
#else
/*
 * Due to limited flash in debug builds, we are no longer able to enable assert
 * output. Once the text segment is trimmed up, this can be re-enabled.
 */
#if !TARGET_EMBEDDED && !NDEBUG
#define SQ_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "assertion \"%s\" failed: " \
                    "file \"%s\", line %d, function: %s\n", \
                    #expr, __FILE__, __LINE__, __func__); \
            call(abort); \
        } \
    } while (false)
#else

#if (TARGET_K400 || FW_CFG_R12C_CPU0 || FW_CFG_R12D_CPU0 || FW_CFG_T2B_CPU0 || FW_CFG_X2B_CPU0 || FW_CFG_S3_K450_CPU0 || FW_CFG_K480_CPU0 || FW_CFG_K480_FPGA_CPU0)
// FIXME: K400 libc on GCC 4.8 has issues with abort/__builtin_trap, so trigger
// an arithmetic exception directly instead of aborting.
// Added k450 cpu0 for now as coredump only reports EPC, can be removed once stackstrace implemented
#define SQ_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            uint32_t denominator = 0; \
            uint32_t ans = 1/denominator; \
            while (ans); \
        } \
    } while (false)
#else
#define SQ_ASSERT(expr) \
    do { \
        if (!(expr)) { \
            call(abort); \
        } \
    } while (false)
#endif

#endif
#endif

#if defined(NDEBUG)
#define SQ_DEBUG_ASSERT(COND)
#else
#define SQ_DEBUG_ASSERT(COND)           SQ_ASSERT(COND)
#endif


/*
 * Only enable printf when debugging locally; this should never be enabled by
 * default, even for debug builds as this can affect timing due to semihosting
 * latency.
 */
#if 0
#define SQ_PRINTF(...) printf(__VA_ARGS__)
#else
#define SQ_PRINTF(...)
#endif

#ifndef NDEBUG
#define SQ_PRINT_BYTES(bp, n) \
    do { \
        uint8_t *p = (void *)(bp); \
        uint8_t *o = p; \
        uint8_t *e = p+(n); \
        while (p < e) { \
            if (p > o) { \
                printf(" "); \
            } \
            printf("%02x", *p++); \
        } \
    } while (0)
#else
#define SQ_PRINT_BYTES(bp, n)
#endif

/*
 * Endian "functions" are different per platform.
 * BSD (MacOS X) uses the host-to-network / network-to-host functions
 * Linux uses the host-to-big or little-to-host style functions
 * uCOS-III does it's own thing.
 * Code in riker has been written against the Linux style function names, and must compile and run on each platform
 * as either FW or a unit test.
 */

#if __linux__
// Use the system's endian.h since it matches our calling convention
#include <endian.h>
#elif __APPLE__ && __MACH__

// Bring in the system's definitions and redefine into the names used
#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#elif TARGET_EMBEDDED && OS_UCOSIII
// Bring in uCOS-III's lib_mem and then redefine the linux style function names to Micrium's style
#include <lib_mem.h>

#define htobe16(x) (MEM_VAL_HOST_TO_BIG_16(x))
#define htole16(x) (MEM_VAL_HOST_TO_LITTLE_16(x))
#define be16toh(x) (MEM_VAL_BIG_TO_HOST_16(x))
#define le16toh(x) (MEM_VAL_LITTLE_TO_HOST_16(x))

#define htobe32(x) (MEM_VAL_HOST_TO_BIG_32(x))
#define htole32(x) (MEM_VAL_HOST_TO_LITTLE_32(x))
#define be32toh(x) (MEM_VAL_BIG_TO_HOST_32(x))
#define le32toh(x) (MEM_VAL_LITTLE_TO_HOST_32(x))

#define htobe64(x) (MEM_VAL_HOST_TO_BIG_64(x))
#define htole64(x) (MEM_VAL_HOST_TO_LITTLE_64(x))
#define be64toh(x) (MEM_VAL_BIG_TO_HOST_64(x))
#define le64toh(x) (MEM_VAL_LITTLE_TO_HOST_64(x))
#endif
