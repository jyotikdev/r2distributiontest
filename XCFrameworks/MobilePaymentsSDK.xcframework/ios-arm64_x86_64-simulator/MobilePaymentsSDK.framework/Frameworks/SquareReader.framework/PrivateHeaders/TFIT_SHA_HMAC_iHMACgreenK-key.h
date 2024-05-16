/*
 * TransformIT 8.3.0 Production-Kit for Square8
 *
 * Copyright (C) Arxan Technologies Inc. 2017.
 * All rights Reserved.
 *
 *
 * Portions of the information disclosed herein are protected by
 * U.S. Patent No. 6,941,463, U.S. Patent No. 6,957,341, U.S. Patent 7,287,166,
 * U.S. Patent 7,707,433, U.S. Patent 7,757,097, U.S. Patent 7,853,018,
 * U.S. Patent 8,510,571, U.S. Patent 9,262,600, and Patents Pending.
 *
 */

#ifndef __TFIT_SHA_HMAC_iHMACgreenK_KEY_H__
#define __TFIT_SHA_HMAC_iHMACgreenK_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TFIT_key_iHMACgreenK{
    unsigned char uuid[16];
    unsigned char hmac_key[256];
} TFIT_key_iHMACgreenK_t;

#ifdef __cplusplus
}
#endif

#endif /* __TFIT_SHA_HMAC_iHMACgreenK_KEY_H__ */
