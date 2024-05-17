// Copyright 2017 Square, Inc.

#pragma once

#include <securesession_utils.h>

#include <squareup/spe/k21/ritual.pb.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Include shared type definitions from R6 firmware
#include <securesession_host_config.h>

#if READER_FRAMEWORK
#include <pinblock.h>
#elif COCOAPODS
#include <cardreader/pinblock.h>
#else
#include <pinblock.h>
#endif

// These calls were previously an API exported by Minesweeper, but now internally
// referenced directly by cardreader code

typedef intptr_t securesession_host_context_handle_t;
typedef struct securesession_host_context_t securesession_host_context_t;

typedef void(*securesession_host_send_to_server_cb_t)(void *securesession, uint8_t *crypto_frame, size_t crypto_frame_len);

enum {
    SECURESESSION_HOST_CONTEXT_HANDLE_MAX = 8,
    SECURESESSION_HOST_CONTEXT_BAD_HANDLE = 0,
};

securesession_result_t securesession_host_get_valid_session_id(
    securesession_host_context_handle_t context_handle,
    uint8_t** session_id_ptr,
    size_t* outlen);

securesession_result_t securesession_host_initialize(
    securesession_host_context_handle_t context_handle,
    uint8_t *whitebox,
    securesession_init_t *session_init);

securesession_result_t securesession_host_validate(
    securesession_host_context_handle_t context_handle,
    squareup_firmware_ritual_SecureSessionInitData const * init_data,
    uint8_t** ms_frame_ptr,
    size_t* outlen,
    bool *is_ticket_request_async,
    securesession_host_send_to_server_cb_t send_to_server_cb,
    void *send_to_server_context);

securesession_result_t securesession_host_validate_v4(
    securesession_host_context_handle_t context_handle,
    squareup_firmware_ritual_SecureSessionInitDataV4 const * init_data,
    uint8_t** ms_frame_ptr,
    size_t* outlen,
    bool *is_ticket_request_async,
    securesession_host_send_to_server_cb_t send_to_server_cb,
    void *send_to_server_context);

securesession_result_t securesession_host_approve_v4(
    securesession_host_context_handle_t context_handle,
    uint8_t* ms_frame_response,
    size_t inlen,
    uint8_t** securesession_response_ptr,
    size_t* outlen,
    uint32_t *protocol_version);

securesession_result_t securesession_host_approve_v5(
    securesession_host_context_handle_t context_handle,
    uint8_t* ms_frame_response,
    size_t inlen,
    uint8_t** securesession_response_ptr,
    size_t* outlen,
    uint32_t *protocol_version);

securesession_result_t securesession_host_terminate(securesession_host_context_handle_t context_handle);
securesession_result_t securesession_host_handle_update_session_secret_v4(securesession_host_context_handle_t context_handle, squareup_firmware_ritual_SecureSessionKeyUpdate const *key_update_msg);
securesession_result_t securesession_host_handle_update_session_secret_v5(securesession_host_context_handle_t context_handle, squareup_firmware_ritual_SecureSessionKeyUpdate const *key_update_msg);
securesession_result_t securesession_host_handle_mix_seed_into_secrets(securesession_host_context_handle_t context_handle, uint8_t const *seed, size_t seed_len);

securesession_result_t securesession_host_context_alloc(securesession_host_context_handle_t *context_handle_out);
securesession_result_t securesession_host_context_free(securesession_host_context_handle_t context_handle);
securesession_result_t securesession_host_get_context(securesession_host_context_handle_t context_handle, securesession_host_context_t **context_out);

//
//  PIN Encryption API for "old" format PIN blocks, where "old" means ISO format 1.
//  See as well the "new" pinblock format below.
//
//  Intended usage:
//      - pin_reset
//      - some calls to add_digit
//      - pin_finalize
//          - returns a securesession message of type pin_response_t
//
//  In addition to function-specific reasons why they might return error (e.g. already
//  too many pin digits), these functions return false if no active session was open
//
securesession_result_t securesession_host_pin_reset(securesession_host_context_handle_t context_handle);
securesession_result_t securesession_host_pin_add_digit(securesession_host_context_handle_t context_handle, uint8_t digit);
securesession_result_t securesession_host_pin_finalize(securesession_host_context_handle_t context_handle, uint8_t *pin_response_buf, size_t pin_response_buf_len);

//
//  "New" PIN Encryption API, where "new" means ISO format 4.
//  See RFC for context: https://docs.google.com/document/d/1JgO4YE4-wRbxBBRZ7V3tLlZ3Ceo7ATKwdzRrzGqvhOw/edit
//
//  Intended usage:
//      - pin_reset
//      - some calls to add_digit
//      - pin_finalize
//          - returns a securesession message of type pin_response_t
//
//  In addition to function-specific reasons why they might return error (e.g. already
//  too many pin digits), these functions return false if no active session was open
//
securesession_result_t securesession_host_pinv4_reset(securesession_host_context_handle_t context_handle);
securesession_result_t securesession_host_pinv4_add_digit(securesession_host_context_handle_t context_handle, uint8_t digit);
securesession_result_t securesession_host_pinv4_finalize(securesession_host_context_handle_t context_handle, uint8_t *pin_response_buf, size_t pin_response_buf_len, uint8_t *pseudo_panblock);


//
// These calls are used directly by the comms driver for encryption/decryption of messages
//
uint32_t securesession_host_comms_get_crypto_hdr_size(void);
securesession_result_t securesession_host_comms_decrypt(securesession_host_context_handle_t context_handle, uint8_t *data, uint16_t data_len, const uint8_t *auth_cleartext, const uint16_t auth_cleartext_len);
securesession_result_t securesession_host_comms_encrypt(securesession_host_context_handle_t context_handle, uint8_t *data, uint16_t data_len, const uint8_t *auth_cleartext, const uint16_t auth_cleartext_len);

//
// A callback type for retrieving the minesweeper/flipper ticket to include in securesession validation requests.
// A minesweeper ticket may be stored or retrieved from flipper. it has an expiration after which a stored one should be replaced.
// This callback should be set by the host application.
//
typedef bool(*securesession_host_ms_ticket_callback_t)(uint8_t **ticket_out, size_t *ticket_out_len, void *user_ref);

//
// Set an minesweeper ticket callback that will perform a synchronous action to fetch a ticket.
//
void securesession_host_set_ms_ticket_callback(securesession_host_context_handle_t context_handle, securesession_host_ms_ticket_callback_t callback, void *callback_ref);

//
// Return function arguments passed back in the return function when the ms_ticket_async_callback finishes its asynchronous actions.
// See securesession_host_ms_ticket_async_return_func_t.
//
typedef struct {
    securesession_host_send_to_server_cb_t send_to_server_cb;
    void *send_to_server_context;

    uint32_t session_id;
    void *securesession_validate;
    size_t securesession_validate_len;
    securesession_host_context_handle_t context_handle;
} securesession_host_ms_ticket_async_return_func_args_t;

//
// Return function callback type that should be called by the ms_ticket_async_callback when it finishes its asynchronous actions.
// Signals to securesession_host that it should resume initialization and construction of the securesession validation message.
// Can return a success boolean. If this function returns false then construction of the validate message has failed.
//
typedef bool(*securesession_host_ms_ticket_async_return_func_t)(uint8_t **ticket_data, size_t *ticket_len, securesession_host_ms_ticket_async_return_func_args_t *return_func_args, bool has_ticket);

//
// A callback type for retrieving the minesweeper/flipper ticket to include in securesession validation requests.
// Different from `securesession_host_ms_ticket_callback_t` in that the actions performed in the callback to retrieve a ticket are assumed
// to be asynchronous. As a result, securesession validation message construction will halt/pause until `return_func` is called to resume
// message construction.
// A minesweeper ticket may be stored or retrieved from flipper. it has an expiration after which a stored one should be replaced.
// This callback should be set by the host application.
//
typedef void(*securesession_host_ms_ticket_async_callback_t)(void *callback_ref, securesession_host_ms_ticket_async_return_func_t return_func, securesession_host_ms_ticket_async_return_func_args_t *return_func_args);

//
// Set an minesweeper ticket callback that will perform an asynchronous action to fetch a ticket.
// The securesession implementation will wait to validate a session until the asynchronous action has finished and called the associated return function.
// See `securesession_host_set_ms_ticket_callback`.
//
void securesession_host_set_ms_ticket_async_callback(securesession_host_context_handle_t context_handle, securesession_host_ms_ticket_async_callback_t callback, void *callback_ref);

bool securesession_host_initialize_core_crypto_api(void);

//
// Gets the remaining time.  Returns false on error
//
bool securesession_host_session_time_remaining(securesession_host_context_handle_t context_handle, time_t *time_remaining_out);

typedef struct securesession_host_context_state_t {
    securesession_id_t      session_id;
    securesession_state_t   session_state;

    // Secret key managed in separate curve_context struct, see comments in host crypto API header
    curve_private_t         curve25519_public;
    curve_private_t         curve25519_shared;
    curve_public_t          curve25519_peer;

    uint8_t                 chain_key[SS_SHARED_SECRET_KDF_LEN]; // Not used to encrypt any other data
    uint8_t                 shared_secret[SS_SHARED_SECRET_KDF_LEN];
    uint8_t                 pin_encryption_key[SS_SHARED_SECRET_KDF_LEN]; // Not used to encrypt any other data
    uint8_t                 pin_format4_encryption_key[SS_SHARED_SECRET_KDF_LEN]; // Not used to encrypt any other data
    uint8_t                 scratch_buf[sizeof(curve_private_t)];

    uint32_t                txn_count;
    uint8_t                 last_key_update_nonce[KEY_UPDATE_NONCE_LEN];

    pinblock_format_num_t pinblock_format;
    uint8_t pseudo_panblock[16];
    bool pseudo_panblock_valid;
    securesession_pin_response_t pinblock_response;
    securesession_pinv4_response_t pinv4block_response;

    securesession_host_curve_context_t curve_context;

    uint8_t                 *ms_baseline_frame;
    size_t                  ms_baseline_frame_len;

    time_t                  host_start_time;        // LCR local time, seconds since 1/1/1970
    time_t                  host_expiry_time;         // LCR exipiry time, seconds since 1/1/1970
    uint32_t                reader_start_time;      // Reader current RTC, seconds
    uint32_t                reader_expiry_time;       // Reader expiration RTC of secure session, seconds
} securesession_host_context_state_t;

// state, secret key, public key, shared secret, etc
typedef struct securesession_host_context_t {
    securesession_host_context_state_t state;
    securesession_host_context_state_t next_state;
    bool is_allocated;

    securesession_host_ms_ticket_callback_t get_ms_ticket_callback;
    void *get_ms_ticket_callback_ref;

    securesession_host_ms_ticket_async_callback_t get_ms_ticket_async_callback;
    void *get_ms_ticket_async_callback_ref;
} securesession_host_context_t;

/**
 * @brief Get a copy of the secure session state for a given context.
 *
 * @note This does not copy any associated minesweeper data.
 */
securesession_result_t securesession_host_get_state(securesession_host_context_handle_t context_handle, securesession_host_context_state_t *state);

/**
 * @brief Set the secure session state for a given context.
 */
securesession_result_t securesession_host_restore_state(securesession_host_context_handle_t context_handle, securesession_host_context_state_t const *state);

#define SS_CACHED_KDF_DERIVED_KEY_LEN   (16)
#define SS_CACHED_KDF_NONCE_LEN         (16)
#define SS_CACHED_AES_GCM_NONCE_LEN     (12)
#define SS_CACHED_AES_GCM_AUTH_TAG_LEN  (12)

typedef struct securesession_host_cached_session_t {
    uint8_t kdf_nonce[SS_CACHED_KDF_NONCE_LEN];
    uint8_t aes_gcm_nonce[SS_CACHED_AES_GCM_NONCE_LEN];
    uint8_t auth_tag[SS_CACHED_AES_GCM_AUTH_TAG_LEN];
    uint8_t data[sizeof(securesession_host_context_state_t)];
} securesession_host_cached_session_t;

/**
 * @brief Encrypt the cached secure session data.
 *
 * @note Encrypts the secure session data in-place and sets the KDF nonce, AES-GCM nonce,
 * and auth tag on the provided struct.
 */
securesession_result_t securesession_host_encrypt_cached_session(securesession_host_cached_session_t *session);

/**
 * @brief Decrypt the cached secure session data.
 *
 * @note Decrypts the secure session data in-place using the KDF nonce, AES-GCM nonce,
 * and auth tag on the provided struct.
 */
securesession_result_t securesession_host_decrypt_cached_session(securesession_host_cached_session_t *session);
