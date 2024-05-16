// Copyright 2018 Square, Inc.
#pragma once

#include "curve25519.h"

#include <stdint.h>

#define KEY_UPDATE_NONCE_LEN    (32)
#define PIN_RESPONSE_BLOCK_LEN  (16)
#define PIN_FORMAT4_PINBLOCK_RESPONSE_BLOCK_LEN (16)
#define PIN_FORMAT4_PSEUDOPANBLOCK_LEN (16)
#define SS_READER_SEED_LEN        (32)

// These defines intended as bridge to other repo's pre-exisiting defines
// (i.e. static assert these are the same as whatever else you're using)
#define SS_HWID_LEN               (8)
#define SS_AES_BLOCK_SIZE         (16)
#define SS_SHA256_DIGEST_LENGTH   (32)
#define SS_AES_GCM_NONCE_LEN      (12)
#define SS_AES_GCM_AUTH_TAG_LEN   (12)
#define SS_SERVER_KEY_LEN         (16)
#define SS_SESSION_KEY_LEN        (16)
#define SS_SHARED_SECRET_KDF_LEN  (16)
#define SS_TDES_KEY_LEN           (8)
#define SS_AES128_KEY_LEN         (16)
#define SS_TDES_BLOCK_SIZE        (8)

#define SECURESESSION_PROTOCOL_VERSION (2)
#define SECURESESSION_PROTOCOL_VERSION_SERVER (3)
#define SECURESESSION_PROTOCOL_VERSION_PROTO (4)
#define SECURESESSION_INVALID_SSID 0

typedef enum {
    SECURESESSION_STATUS            = 0,
    SECURESESSION_INIT              = 1,
    SECURESESSION_INIT_DATA         = 2,
    SECURESESSION_VALIDATE          = 3,
    SECURESESSION_APPROVE           = 4,
    SECURESESSION_DENY              = 5,
    SECURESESSION_KEY_UPDATE        = 6,
    SECURESESSION_PIN_RESPONSE      = 7,
    SECURESESSION_APPROVE_SERVER    = 8,
} securesession_msg_type_e;

typedef uint32_t securesession_id_t;
typedef uint32_t  securesession_version_t;
typedef uint8_t   key_update_nonce_t[KEY_UPDATE_NONCE_LEN];
_Static_assert(KEY_UPDATE_NONCE_LEN % SS_AES_BLOCK_SIZE == 0, "pin_nonce_len_is_aes_multiple");

typedef uint8_t   minesweeper_data_t[SS_SHA256_DIGEST_LENGTH];

// MESSAGE TYPEDEFS
typedef struct {
    securesession_version_t  protocol_version;
    securesession_id_t       session_id;
    uint32_t type; //securesession_msg_type_e
} securesession_msg_hdr_t;
_Static_assert(sizeof(securesession_msg_hdr_t) == 12, "securesession_msg_hdr_size_is_right");

// This header is used for encrypted+authenticated messaging within a session
typedef struct {
    securesession_id_t session_id;
    uint8_t nonce[SS_AES_GCM_NONCE_LEN];
    uint8_t auth_tag[SS_AES_GCM_AUTH_TAG_LEN];
} securesession_crypto_hdr_t;

typedef struct {
    securesession_msg_hdr_t  hdr;

    uint32_t                  rtc;
    uint16_t                  txn_count;
} securesession_status_t;
_Static_assert(sizeof(securesession_status_t) == 20, "securesession_status_size_is_right");

typedef struct {
    securesession_msg_hdr_t  hdr;

    minesweeper_data_t        minesweeper_data;
} securesession_init_t;
_Static_assert(sizeof(securesession_init_t) == 44, "securesession_init_size_is_right");

typedef struct {
    securesession_msg_hdr_t  hdr;

    uint32_t                  key_index;
    uint8_t                   hwid[SS_HWID_LEN];
    uint32_t                  rtc;
    uint8_t                   pk_reader[CURVE25519_KEY_LEN];
    uint32_t                  monotonic_ctr_high;
    uint32_t                  monotonic_ctr_low;
    minesweeper_data_t        minesweeper_data;
    uint8_t                   hmac[SS_SHA256_DIGEST_LENGTH];
} securesession_init_data_t;
_Static_assert(sizeof(securesession_init_data_t) == 132, "securesession_init_size_is_right");

typedef struct {
    securesession_msg_hdr_t     hdr;
    securesession_init_data_t   init_data;
    uint32_t                    build_id;
    uint8_t                     pk_app[CURVE25519_KEY_LEN];
    uint8_t                     app_hmac[SS_SHA256_DIGEST_LENGTH];
} securesession_validate_t;
_Static_assert(sizeof(securesession_validate_t) == 212, "securesession_validate_size_is_right");

// In SecureSession v2, there is only one approval message,
// sent to the Register, who forwards it to the Reader.
// In SecureSession v3, there are two approval messages:
//  - One meant for the Reader
//  - One meant for the Register
// The one for the Reader is idential to the v2 approval message
// while the Register approval message wraps this v2 message, and adds
// the reader's public key, the Register's ms_data_hash, and
// an HMAC from the server over the whole message with a key shared with Register.
// This is the v2 (meant for the Reader in v3) approval message:
typedef struct {
    securesession_msg_hdr_t  hdr;

    uint32_t                  key_index;
    uint8_t                   init_data_hmac[SS_SHA256_DIGEST_LENGTH];
    uint32_t                  expiry;
    uint32_t                  txn_count;
    curve_public_t            pk_app;
    key_update_nonce_t        r_enc; // See protocol document (beginning of hash chain)
    key_update_nonce_t        r_txn; // See protocol document (end of hash chain)
    uint8_t                   hmac[SS_SHA256_DIGEST_LENGTH];
} securesession_approval_v2_t;
_Static_assert(sizeof(securesession_approval_v2_t) == 184, "securesession_approve_size_is_right");

// SecureSession v3 approval message: a wrapped v2 message (for the Reader),
// that includes Reader's public key, hash of the ms_data sent in the ms_comms frame,
// and an hmac from the Server (verified only by the Register)
typedef struct {
    securesession_msg_hdr_t      hdr;
    securesession_approval_v2_t  v2;
    uint8_t                      pk_reader[CURVE25519_KEY_LEN];
    uint8_t                      ms_data_hash[SS_SHA256_DIGEST_LENGTH];
    uint8_t                      host_hmac[SS_SHA256_DIGEST_LENGTH];
} securesession_approval_t;
_Static_assert(sizeof(securesession_approval_t) == 292, "securesession_approve_size_is_right");

typedef struct {
    securesession_msg_hdr_t  hdr;
} securesession_deny_t;
_Static_assert(sizeof(securesession_deny_t) == 12, "securesession_deny_size_is_right");

// This type is used to notify the securesession host that the reader has updated its session key
typedef struct {
    securesession_msg_hdr_t hdr;
    key_update_nonce_t nonce;
} securesession_key_update_t;
_Static_assert(sizeof(securesession_key_update_t) == 44, "key_update_size_is_right");

typedef struct {
    uint8_t                   nonce[SS_AES_GCM_NONCE_LEN];
    uint8_t                   auth_tag[SS_AES_GCM_AUTH_TAG_LEN];
    uint8_t                   pinblock[PIN_RESPONSE_BLOCK_LEN];
} securesession_pin_response_t;
_Static_assert(sizeof(securesession_pin_response_t) == 40, "pin_response_size_is_right");

// PIN block response for ISO 9564-1 format 4 pinblock ("pinv4").
typedef struct {
    uint8_t                   pseudo_panblock[PIN_FORMAT4_PSEUDOPANBLOCK_LEN];
    // we are saving ourselves space for the future with this reserved1 block.
    // the weird size is to match the size of the whole struct
    // securesession_newpin_response_t to the previous structure securesession_pin_response_t.
    // We do not really need to do that, but it wouldn't hurt either.
    uint8_t                   reserved1[(SS_AES_GCM_NONCE_LEN+SS_AES_GCM_AUTH_TAG_LEN)-PIN_FORMAT4_PSEUDOPANBLOCK_LEN];
    uint8_t                   pinblock_format4[PIN_FORMAT4_PINBLOCK_RESPONSE_BLOCK_LEN];
} securesession_pinv4_response_t;
_Static_assert(sizeof(securesession_pinv4_response_t) == 40, "pinv4_response_size_is_right");
