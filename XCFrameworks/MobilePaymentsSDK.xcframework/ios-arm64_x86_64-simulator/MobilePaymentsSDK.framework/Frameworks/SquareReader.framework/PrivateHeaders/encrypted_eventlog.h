// Copyright 2021 Square, Inc.

/**
 * @file
 *
 * @brief Encrypted Eventlog Library
 *
 * @details Provides functions for encrypting eventlog data.
 *          Key data is prepared once per boot.
 *
 * @author Eldon Rivers <eldon@squareup.com>
 *
 * @see https://docs.google.com/document/d/1rILXe4jZfWF6FYqdc46xpwbPK2l813FF6yrPkhFC6YQ
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <cardreader_shared/crs_pack.h>

#include <module/curve25519_wrappers.h>
#include <lib/tomcryptlite/sha256.h>

/**
 * @brief Enum of result codes used in this library
 */
typedef enum {
    /**
     * @brief Success result code
     */
    ENCRYPTED_EVENTLOG_SUCCESS = 0,
    /**
     * @brief Generic error
     */
    ENCRYPTED_EVENTLOG_ERROR_GENERIC,
    /**
     * @brief Invalid header
     */
    ENCRYPTED_EVENTLOG_ERROR_INVALID_HEADER,
} encrypted_eventlog_result_t;

/**
 * @brief Context for this system. Key details change each boot cycle.
 */
typedef struct {
    /**
     * @brief Flag indicating the system has been initialized
     */
    bool initialized;
    /**
     * @brief Our encryption key which can be derived by recipient using our public
     * key and their private key (Diffie–Hellman key exchange)
     */
    uint8_t aes_key[SHA256_DIGEST_LENGTH];
    /**
     * @brief Our generated public key, used by recipient to derive shared secret
     */
    curve_public_t firmware_public_key;
} encrypted_eventlog_context_t;

/**
 * @brief Enum containing constants used by encryption header
 */
enum {
    /**
     * @brief Length of Nonce
     */
    ENCRYPTED_EVENTLOG_NONCE_LEN = 12,
    /**
     * @brief Dangerous short tag.
     * @see https://docs.google.com/document/d/1rILXe4jZfWF6FYqdc46xpwbPK2l813FF6yrPkhFC6YQ
     */
    ENCRYPTED_EVENTLOG_AUTH_LEN = 4,
    /**
     * @brief Header version identifier: 'EVT1'
     */
    ENCRYPTED_EVENTLOG_HDR_VERSION = 0x31545645,
};

/**
 * @brief Eventlog encryption header struct
 */
typedef struct SQ_PACK() {
    /**
     * @brief Firmware's public key, used by recipient to derive shared secret
     */
    uint8_t firmware_public_key[CURVE25519_KEY_LEN];
    /**
     * @brief Nonce (one-time random data used to prevent replay attacks)
     */
    uint8_t nonce[ENCRYPTED_EVENTLOG_NONCE_LEN];
    /**
     * @brief Very short auth tag (to authenticate message). This is too short for
     * real authentication. In our case this should be thought of more as a CRC on
     * the message and authenticated header fields.
     * @note remaining fields in the struct are authenticated
     */
    uint8_t auth_tag[ENCRYPTED_EVENTLOG_AUTH_LEN];
    /**
     * @brief Encrypted Eventlog header version. This can be bumped in the future to
     * identify changes in encryption method or changes in eventlog binary representation.
     * @note: Should be ENCRYPTED_EVENTLOG_HDR_VERSION
     */
    uint32_t hdr_version;
    /**
     * @brief on-device timestamp at time of sending events
     */
    uint32_t send_timestamp;
    /**
     * @brief Identify what keys we're using to support rotation
     */
    uint16_t key_id;
    /**
     * @brief length of event data buffer (payload)
     */
    uint16_t buffer_length;
} encrypted_eventlog_hdr_t;
_Static_assert(sizeof(encrypted_eventlog_hdr_t) == 60, "incorrect size for encrypted_eventlog_hdr_t");

/**
 * @brief Offset to authenticated part of the header
 */
#define ENCRYPTED_EVENTLOG_HDR_AUTH_OFFSET offsetof(encrypted_eventlog_hdr_t, hdr_version)
_Static_assert(ENCRYPTED_EVENTLOG_HDR_AUTH_OFFSET == 48, "incorrect offset for authed part of header");

/**
 * @brief Size of authenticated part of the header
 */
#define ENCRYPTED_EVENTLOG_HDR_AUTH_LEN (sizeof(encrypted_eventlog_hdr_t) - ENCRYPTED_EVENTLOG_HDR_AUTH_OFFSET)
_Static_assert(ENCRYPTED_EVENTLOG_HDR_AUTH_LEN == 12, "incorrect size for authed part of header");

/**
 * @brief Initialize the Eventlog Encryption system
 */
encrypted_eventlog_result_t encrypted_eventlog_init(void);

/**
 * @brief Encrypt a buffer of eventlog data.
 *
 * @param data Pointer to buffer of eventlog data. The buffer should begin with an
 * initialized encrypted_eventlog_hdr_t header.
 * @brief len Length (including header) of buffer pointed to by data.
 */
encrypted_eventlog_result_t encrypted_eventlog_encrypt(uint8_t *data, size_t len);
