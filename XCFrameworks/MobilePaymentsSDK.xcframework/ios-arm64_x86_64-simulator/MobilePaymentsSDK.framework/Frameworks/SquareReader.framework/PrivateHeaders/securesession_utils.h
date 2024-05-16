// Copyright 2017 Square, Inc.

#pragma once

#include <securesession_msgs.h>
#include <stdlib.h>

#include <squareup/spe/k21/ritual.pb.h>

typedef enum {
    SECURESESSION_SUCCESS = 0,
    SECURESESSION_ERROR_GENERIC,
    SECURESESSION_ERROR_ARG,
    SECURESESSION_ERROR_SESSION_STATE,
    SECURESESSION_ERROR_INPUT_SIZE,
    SECURESESSION_ERROR_OUTPUT_SIZE,
    SECURESESSION_ERROR_MSG_TYPE,
    SECURESESSION_ERROR_SESSION_ID,
    SECURESESSION_ERROR_CURVE,
    SECURESESSION_ERROR_HKDF,
    SECURESESSION_ERROR_DENIED,
    SECURESESSION_ERROR_BAD_DIGIT,
    SECURESESSION_ERROR_PIN_FULL,
    SECURESESSION_ERROR_PIN_TOO_SHORT,
    SECURESESSION_ERROR_INVALID_PIN_REQUEST,
    SECURESESSION_ERROR_INVALID_KEY_UPDATE_MSG,
    SECURESESSION_ERROR_AES,
    SECURESESSION_ERROR_PROTOCOL_VERSION,
    SECURESESSION_ERROR_APPROVAL_MISMATCH,
    SECURESESSION_ERROR_APPROVAL_EXPIRED,
    SECURESESSION_ERROR_NO_TXN_LEFT,
    SECURESESSION_ERROR_API_CALL,
    SECURESESSION_ERROR_MINESWEEPER_CALL,
    SECURESESSION_ERROR_SHA256,
    SECURESESSION_ERROR_BAD_HMAC,
    SECURESESSION_ERROR_TDES,
    SECURESESSION_ERROR_ENCODE_FAILURE,
    SECURESESSION_ERROR_CONTEXT,
    SECURESESSION_ERROR_OUT_OF_CONTEXTS,
    SECURESESSION_ERROR_BAD_FIELD,
    SECURESESSION_ERROR_WHITEBOX_KEY_DESERIALIZE,
    SECURESESSION_ERROR_AES128ECB,
    SECURESESSION_ERROR_INVALID_PINBLOCK_FORMAT,
} securesession_result_t;

typedef enum {
    SECURESESSION_STATE_SESSION_INVALID = 0,
    SECURESESSION_STATE_SESSION_INITIALIZED,
    SECURESESSION_STATE_SESSION_PENDING,
    SECURESESSION_STATE_SESSION_VALID,
} securesession_state_t;

size_t securesession_utils_get_msg_size(securesession_msg_type_e msg);

securesession_result_t securesession_utils_msg_is_valid(uint8_t const *buf, size_t len, securesession_msg_type_e msg);

securesession_result_t securesession_utils_proto_is_valid(squareup_firmware_ritual_SecureSessionMsgHdr const *hdr,
                                                          securesession_msg_type_e msg_type);

extern uint8_t const secure_session_chain_key_label[32];
extern uint8_t const shared_secret_application_label[32];
extern uint8_t const pin_key_application_label[32];
extern uint8_t const pin_format4_key_application_label[32];

extern uint8_t const label_seed_for_shared_secret[SS_READER_SEED_LEN];
extern uint8_t const label_seed_for_pin_encryption_key[SS_READER_SEED_LEN];
extern uint8_t const label_seed_for_pin_format4_encryption_key[SS_READER_SEED_LEN];
extern uint8_t const label_seed_for_msw_prng[SS_READER_SEED_LEN];
