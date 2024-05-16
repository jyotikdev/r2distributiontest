// Copyright (c) 2023 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief The SPE Transport Security (STS) LCR host feature.
 */

/**
 * @defgroup lcr_sts_feature STS Feature
 *
 * @brief The SPE Transport Security (STS) LCR host feature.
 *
 * This feature is used to handle sending and receiving STS messages and state. Related crypto and authentication
 * operations must be implemented by the supporting APIs.
 *
 * The architecture can generally be viewed as:
 * ```
 *  -------------------
 *  | Event Callbacks |
 *  -------------------
 *          /|\
 *           |
 *          \|/
 *  -------------------       -------------------
 *  |   STS Feature   | <---> |   Crypto API    |
 *  -------------------       -------------------
 *          /|\                      /:\
 *           |                        :
 *          \|/                       :
 *  -------------------               :
 *  |   Card Reader   | <- - - - - - -
 *  -------------------
 * ```
 *
 * Event callbacks are used to communicate general state transitions and requests for authentication. The event handler
 * may be required to forward authentication requests to an external server, and then provide the server response to
 * this feature. The final authenticated state may also be cached.
 *
 * The Crypto API handles key management and any shared secret derivation. The crypto API is left as a runtime binding
 * since different use cases call for different cryptographic implementations.
 *
 * Notably, this feature does NOT implement cardreader transport security. If the intended use of STS is for transport
 * security, then the crypto API must provide the necessary function and the caller must setup the card reader via
 * cr_cardreader_set_crypto_api() manually. The Crypto API must follow the STS design doc for its intended use case.
 *
 * @see STS design doc: https://docs.google.com/document/d/1PnI3QxmDIMmELh7vLTEt7huWnGKVrGvRElY4tcymvjA/edit
 *
 * @ingroup lcr
 * @{
 */

#pragma once
#include <cardreader_shared/crs_system_msgs.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct cr_sts_feature_t;
struct cr_cardreader_t;
struct cr_card_info_t;

/// Current STS protocol version
#define CR_STS_FEATURE_PROTOCOL_VERSION (1)

/// Maximum length of the encoded auth state
#define CR_STS_FEATURE_ENCODED_AUTH_STATE_MAX_LEN (2048)

/**
 * @brief Supported usages for STS. This dictates the behavior of the reader and host.
 */
typedef enum
{
    /// Default value if unset
    CR_STS_FEATURE_USAGE_UNSET,

    /// Basic transport security. No external authenticator.
    CR_STS_FEATURE_USAGE_BASIC,

    /// SQUID Secure Session. TOFU with External authenticator for key reset only.
    CR_STS_FEATURE_USAGE_SQUID_SS,
} cr_sts_feature_usage_t;

/**
 * @brief Session information passed to callbacks.
 */
typedef struct cr_sts_feature_session_info_t
{
    /// ID of the current session state. May be zero.
    uint64_t session_id;

    /// Protocol version expected.
    uint32_t protocol_version;

    /// Indicated usage of this session.
    cr_sts_feature_usage_t usage;

    /// Encoded squareup.spe.sts.AuthState proto
    uint8_t auth_state[CR_STS_FEATURE_ENCODED_AUTH_STATE_MAX_LEN];
    size_t auth_state_len;

    /// Cached reader serial number
    char reader_serial[CRS_FACTORY_MAX_SYSCFG_STRING_SIZE + 1];
    size_t reader_serial_len;
} cr_sts_feature_session_info_t;

/**
 * @brief Possible result codes for cr_sts_* functions.
 */
typedef enum
{
    CR_STS_FEATURE_RESULT_SUCCESS,
    CR_STS_FEATURE_RESULT_INVALID_PARAMETER,
    CR_STS_FEATURE_RESULT_NOT_INITIALIZED,
    CR_STS_FEATURE_RESULT_ALREADY_INITIALIZED,
    CR_STS_FEATURE_RESULT_NOT_TERMINATED,
    CR_STS_FEATURE_RESULT_ALREADY_TERMINATED,

    // "Session" has nothing to do with STS. this is a normal LCR
    // feature lifecycle term used in most other features
    CR_STS_FEATURE_RESULT_SESSION_ERROR,

    CR_STS_FEATURE_RESULT_CALL_UNEXPECTED,
    CR_STS_FEATURE_RESULT_AUTH_ERROR,
    CR_STS_FEATURE_RESULT_PROTOCOL_ERROR,
    CR_STS_FEATURE_RESULT_GENERIC_ERROR,
    CR_STS_FEATURE_RESULT_NO_READER,
    CR_STS_FEATURE_RESULT_SESSION_EXPIRED,
} cr_sts_feature_result_t;

/**
 * @brief Events that the STS feature can indicate.
 */
typedef enum
{
    CR_STS_FEATURE_EVENT_TYPE_SESSION_VALID,
    CR_STS_FEATURE_EVENT_TYPE_SESSION_INVALID,
} cr_sts_feature_event_type_t;

/**
 * @brief Callback for when an event occurs such as the session becoming valid or invalid.
 *
 * @param context Pointer to the context provided in the event_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session.
 * @param event_type The event that triggered this call.
 */
typedef void (*cr_sts_feature_event_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info,
        cr_sts_feature_event_type_t event_type);

/**
 * @brief Callback to authenticate certificates, provided in the session_info structure.
 *
 * If this callback is provided (non-NULL in the event_api_t), then
 * cr_sts_feature_auth_update() must be called with the auth result.
 *
 * @param context Pointer to the context provided in the event_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session, including encoded certificates.
 */
typedef void (*cr_sts_feature_auth_request_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info);

/**
 * @brief Callback for when the SPE auth update is complete.
 *
 * The session_info will contain certs and an optional SPE cookie. This
 * information may be cached and used at a later time to resume sessions.
 *
 * @param context Pointer to the context provided in the event_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session, including encoded certificates and the SPE cookie.
 */
typedef void (*cr_sts_feature_auth_complete_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info);

/**
 * @brief Callback for when the SPE claims it can resume a cached session.
 *
 * The host must determine if it has the ability to resume the session. One of two outcomes must occur:
 *   1. Cached session available: The host must call cr_sts_feature_auth_update() with the cached auth state.
 *   2. No cached session: The host must call cr_sts_feature_reinitialize().
 *
 * If this callback is not provided in the API, the SPE will be reinitialized automatically.
 *
 * This may be called under different scenarios:
 *  - The host has no session state, but the SPE does.
 *  - The SPE has no current session state, but indicates an old session may be resumed.
 *
 * The session information at this point may be fairly sparse and only contain the reader serial number.
 *
 * @param context Pointer to the context provided in the event_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Current session state information.
 */
typedef void (*cr_sts_feature_resume_session_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info);

/**
 * @brief Event API. Events may be set to NULL.
 */
typedef struct cr_sts_feature_event_api_t
{
    void *context; ///< Context passed to all callback functions.
    cr_sts_feature_event_cb_t on_sts_event;
    cr_sts_feature_auth_request_cb_t on_auth_request;
    cr_sts_feature_auth_complete_cb_t on_auth_complete;
    cr_sts_feature_resume_session_cb_t on_resume_session;
} cr_sts_feature_event_api_t;

/**
 * @brief Callback to request a new host cert for a session.
 *
 * This certificate is optionally signed by the host. Passed as an encoded Signed protobuf message.
 *
 * @param context Pointer to the context provided in the crypto_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session.
 * @param [out] signed_cert Buffer to place an encoded Signed certficiate
 * @param [in,out] signed_cert_len Size of the (in) buffer and (out) encoded Signed certificate.
 */
typedef cr_sts_feature_result_t (*cr_sts_feature_gen_host_cert_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info,
        uint8_t *signed_cert,
        size_t *signed_cert_len);

/**
 * @brief Callback to request a new ephemeral public key for secret key agreement.
 *
 * The key type and size is dictated by the session usage. In practice: curve25519.
 *
 * @param context Pointer to the context provided in the crypto_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session.
 * @param [out] ephemeral_public_key Buffer to place the encoded host PublicKey message.
 * @param [in,out] ephemeral_public_key_len Size of the (in) buffer and (out) PublicKey.
 */
typedef cr_sts_feature_result_t (*cr_sts_feature_gen_ephemeral_key_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info,
        uint8_t *ephemeral_public_key,
        size_t *ephemeral_public_key_len);

/**
 * @brief Callback to validate the authenticated auth state.
 *
 * This is called during an auth update, where the approved auth state is passed back to STS.
 *
 * The crypto library is responsible for ensuring the auth state is signed by the appropriate entities, if needed.
 *
 * @param context Pointer to the context provided in the crypto_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session, including the approved auth state.
 */
typedef cr_sts_feature_result_t (*cr_sts_feature_validate_auth_state_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info);

/**
 * @brief Callback to pass the SPE's ephemeral public key and validate its key generation response.
 *
 * The key types involved and confirmation algorithm is dictated by the session usage.
 *
 * @param context Pointer to the context provided in the crypto_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session.
 * @param spe_public_key Encoded SPE PublicKey message.
 * @param spe_public_key_len Size of the SPE PublicKey.
 * @param spe_confirmation SPE confirmation bytes.
 * @param spe_confirmation_len Size of the SPE confirmation.
 */
typedef cr_sts_feature_result_t (*cr_sts_feature_validate_spe_key_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info,
        uint8_t const *spe_public_key,
        size_t spe_public_key_len,
        uint8_t const *spe_confirmation,
        size_t spe_confirmation_len);

/**
 * @brief Callback to invalidate session key material.
 *
 * @param context Pointer to the context provided in the crypto_api during initialization.
 * @param sts An initialized STS feature.
 * @param session_info Information about the current session.
 */
typedef void (*cr_sts_feature_invalidate_session_cb_t)(
        void *context,
        struct cr_sts_feature_t *sts,
        struct cr_sts_feature_session_info_t const *session_info);

/**
 * @brief Crypto API. The supplied crypto API manages key generation and storage.
 */
typedef struct cr_sts_feature_crypto_api_t
{
    void *context; ///< Context passed to all callbacks.
    cr_sts_feature_gen_host_cert_cb_t gen_host_cert;
    cr_sts_feature_gen_ephemeral_key_cb_t gen_ephemeral_key;
    cr_sts_feature_validate_auth_state_cb_t validate_auth_state;
    cr_sts_feature_validate_spe_key_cb_t validate_spe_key;
    cr_sts_feature_invalidate_session_cb_t invalidate_session;
} cr_sts_feature_crypto_api_t;

/**
 * @brief Initializes a cr_sts_feature_t with the specified APIs.
 *
 * @param sts An allocated cr_sts_feature struct.
 * @param usage The intended use of this session.
 * @param event_api Event API callbacks.
 * @param crypto_api Crypto API callbacks.
 * @param cardreader The cardreader to use.
 */
cr_sts_feature_result_t cr_sts_feature_init(
    struct cr_sts_feature_t *sts,
    cr_sts_feature_usage_t usage,
    struct cr_sts_feature_event_api_t const *event_api,
    struct cr_sts_feature_crypto_api_t const *crypto_api,
    struct cr_cardreader_t *cardreader);

/**
 * @brief Terminates, de-registering the feature.
 *
 * @param sts An initialized STS feature.
 */
cr_sts_feature_result_t cr_sts_feature_term(struct cr_sts_feature_t *sts);

/**
 * @brief Begins the process of establishing an STS session.
 *
 * If a valid session already exists or is in the process of being established,
 * this function does nothing.
 *
 * @param sts An initialized STS feature.
 */
cr_sts_feature_result_t cr_sts_feature_establish_session(struct cr_sts_feature_t *sts);

/**
 * @brief Force a session initialization. This will kick off a fresh authentication phase.
 *
 * The Crypto API will be requested to invalidate its stored keys.
 *
 * @param sts An initialized STS feature.
 */
cr_sts_feature_result_t cr_sts_feature_reinitialize(struct cr_sts_feature_t *sts);

/**
 * @brief Request that STS tears down an existing session, invalidating it on the SPE.
 *
 * The Crypto API will be requested to invalidate its stored keys and the Event
 * API will receive an on_sts_event callback.
 *
 * @param sts An initialized STS feature.
 */
cr_sts_feature_result_t cr_sts_feature_teardown(struct cr_sts_feature_t *sts);

/**
 * @brief Update the auth state with authenticated certificates for both the SPE and POS.
 *
 * This will initiate an AuthUpdate sequence with the SPE. This first validates the auth state with the Crypto API, if
 * provided, and may fail if it's not acceptable.
 *
 * @param sts An initialized STS feature.
 * @param session_id The session ID for the auth state.
 * @param auth_state Encoded AuthState message.
 * @param auth_state_len Length of the encoded AuthState message.
 */
cr_sts_feature_result_t cr_sts_feature_auth_update(
        struct cr_sts_feature_t *sts,
        uint64_t session_id,
        void const *auth_state,
        size_t auth_state_len);

/**
 * @brief Request that STS initiates a new key negotiation sequence.
 *
 * @param sts An initialized STS feature.
 */
cr_sts_feature_result_t cr_sts_feature_refresh_keys(struct cr_sts_feature_t *sts);

/**
 * @brief Allocates the memory for a cr_sts_feature_t.
 *
 * @returns An uninitialized STS feature.
 */
struct cr_sts_feature_t *cr_sts_feature_alloc(void);

/**
 * @brief Frees the memory for a cr_sts_feature_t.
 *
 * The cr_sts_feature_t must have been previously terminated.
 *
 * @param sts An allocated STS feature.
 */
cr_sts_feature_result_t cr_sts_feature_free(struct cr_sts_feature_t *sts);

/** @} */
