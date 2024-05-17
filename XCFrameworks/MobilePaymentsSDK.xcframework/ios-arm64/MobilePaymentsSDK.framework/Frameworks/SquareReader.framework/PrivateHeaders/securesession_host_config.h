// Copyright 2017 Square, Inc.

#pragma once

#include <securesession_curve25519.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


// The USE_ARXAN flag must be explicitly set to 0 or 1 when compiling this code
#if !defined(USE_ARXAN)
#error "Arxan, use or use not, there is no try."
#endif

//
// When the host is compiled with Arxan, we include additional items
//
#if USE_ARXAN

// Include headers from the Arxan TransformIT libraries
#include "TFIT_ECC_DH_iECDHredSSfastCurve25519.h"
#include "TFIT_DK_AES_GCM_Encrypt_iAES16_red.h"
#include "TFIT_AES_GCM_Encrypt_iAES16.h"
#include "TFIT_DK_AES_ECB_Encrypt_iAES11_red.h"
#include "TFIT_AES_ECB_Encrypt_iAES11.h"
#include "TFIT_DK_AES_GCM_Decrypt_iAES17_red.h"
#include "TFIT_AES_GCM_Decrypt_iAES17.h"
#include "TFIT_AES_NKDF_iNKDF128redOK.h"
#include "TFIT_DK_AES_NKDF_iNKDF128redOK_green.h"
#include "TFIT_Slice_red_green.h"
#include "TFIT_SHA_Digest_iSHAredMgreenD.h"
#include "TFIT_Slice_green_green.h"
#include "TFIT_Slice_red_green.h"
#include "TFIT_XLAT_Data_Encode_iXENCred.h"
#include "TFIT_SHA_HMAC_iHMACgreenK.h"
#include "TFIT_SHA_HMAC_iHMACgreenK-key.h"
#include "TFIT_hmac_key_iHMACgreenK_validate.h"
#include "TFIT_hmac_key_iHMACgreenK_approve.h"
#include "TFIT_DK_DES_Triple_Encrypt_i3DES1_copper.h"
#include "TFIT_DK_DES_Triple_Decrypt_i3DES2_copper.h"
#include "TFIT_DES_Triple_Encrypt_i3DES1.h"
#include "TFIT_DES_Triple_Decrypt_i3DES2.h"
#include "TFIT_XLAT_Data_Recode_iXRECredXIcopperXO.h"

#include <securesession_host_build_id.h>
#define wb_hmac_validate_key      &TFIT_key_iHMACgreenK_validate
#define wb_hmac_validate_key_len  TFIT_key_iHMACgreenK_validate_len
#define wb_hmac_approve_key       &TFIT_key_iHMACgreenK_approve
#define wb_hmac_approve_key_len   TFIT_key_iHMACgreenK_approve_len

#define SQ_TFIT_SLICE_DATA_GREEN_GREEN  (&TFIT_Slice_data_green_green)
#define SQ_TFIT_SLICE_DATA_RED_GREEN    (&TFIT_Slice_data_red_green)

// This type is just aliased when using TFIT, and the internal private key is managed by the whitebox functions
typedef TFIT_ctx_iECDHredSSfastCurve25519_t securesession_host_curve_context_t;

#else // USE_ARXAN = 0

// Include some substitute functions for TFIT (all from tomcrypt)
#include <lib/tomcryptlite/gcm.h>
#include <lib/tomcryptlite/kdf.h>
#include <lib/tomcryptlite/hmac-sha256.h>
#include <lib/tomcryptlite/des.h>

// Since the whitebox functions otherwise manage the private key, we have to manage it ourselves when we
// aren't using whitebox crypto (e.g. testing, and cardreader-terminal)
//
// This includes for unit tests
//
// The keys are included directly from securesession_host.c, but we configure the macro here
#define wb_hmac_validate_key      debug_hmac_validate_key
#define wb_hmac_validate_key_len  debug_hmac_validate_key_len
#define wb_hmac_approve_key       debug_hmac_approve_key
#define wb_hmac_approve_key_len   debug_hmac_approve_key_len

typedef struct securesession_host_curve_context_t {
    curve_private_t priv_key;
} securesession_host_curve_context_t;

#define WBSLICE_ZERO_PAD_AT_HIGHEST_ADDRESS (0)
#define SQ_TFIT_SLICE_DATA_GREEN_GREEN      (NULL)
#define SQ_TFIT_SLICE_DATA_RED_GREEN        (NULL)

#define SECURESESSION_HOST_WRAPPER_wb_hmac_sha256             sq_hmac_sha256
#define SECURESESSION_HOST_WRAPPER_wbaes_gcm_encrypt_redK     sq_aes_gcm_encrypt
#define SECURESESSION_HOST_WRAPPER_wbaes_gcm_decrypt_redK     sq_aes_gcm_decrypt
#define SECURESESSION_HOST_WRAPPER_kdf_nist108_greenK_redOK   sq_ctr_kdf_cmac_aes

#endif // endif for USE_ARXAN block

#include <securesession_host_tfit_wrappers.h>
