// Copyright 2014 Square, Inc.

/**
 * @file
 *
 * @brief Cardreader Securesession Feature
 *
 * @details Square’s EMV solution integrates dedicated payment acceptance
 * hardware, a mobile application and trusted software module, and a backend
 * payment processing environment. Securesession defines the protocol used for
 * cryptographic key exchange and subsequent encryption of payment flow control
 * and customer PINs between the secure card reader and backend payment
 * processing environment through a mobile application.
 *
 * @author Nick Hebner <hebner@squareup.com>
 *
 * @see https://docs.google.com/document/d/1rK2YGQzA2FtsodpCUyyAJBF97h-y49IVJl85BMk3G5M
 *
 * @see https://docs.google.com/document/d/1i--Q2NWBQ2nploSPgoPzDcXzDC22jRkN3h9DWlOJkxU
 *
 * @copyright 2014 Square, Inc.
 */

/**
 * @addtogroup lcr_securesession_feature Securesession Feature
 * @ingroup lcr
 * @{
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Forward declaration of the Securesession Feature to hide away the
 * private implementation.
 */
struct cr_securesession_feature_t;

/**
 * @brief Forward declaration for the cardreader instance.
 */
struct cr_cardreader_t;

/**
 * @brief Forward declaration for card information.
 */
struct cr_card_info_t;

/**
 * @brief Enumeration of the possible result codes for calling the
 * `cr_securesession_feature_` functions.
 *
 * @note Do not change the order of these enumerations.
 */
typedef enum {
    /**
     * @brief API call was successful.
     */
    CR_SECURESESSION_FEATURE_RESULT_SUCCESS,

    /**
     * @brief Invalid parameter was passed to the API function.
     */
    CR_SECURESESSION_FEATURE_RESULT_INVALID_PARAMETER,

    /**
     * @brief Securesession Feature was not initialized before an API function
     * call was attempted; #cr_securesession_feature_init() must be called
     * befor eany other API functions are called.
     */
    CR_SECURESESSION_FEATURE_RESULT_NOT_INITIALIZED,

    /**
     * @brief Securesession Feature initialization was called when the feature
     * was already initialized.
     */
    CR_SECURESESSION_FEATURE_RESULT_ALREADY_INITIALIZED,

    /**
     * @brief Freeing of the Securesession Feature was attempted when the
     * feature has not yet been terminated.
     */
    CR_SECURESESSION_FEATURE_RESULT_NOT_TERMINATED,

    /**
     * @brief Termination of the Securesession Feature was attempted when the
     * feature has already been terminated.
     */
    CR_SECURESESSION_FEATURE_RESULT_ALREADY_TERMINATED,

    /**
     * @brief Error encountered when sending a request ot the device, or if
     * there is an issue with the Cardreader instance.
     *
     * @note The "session" in this enumeration has nothing to do with
     * Securesession; this is a normal LCR (LibCardReader) feature lifecycle
     * term used amongst other features.
     */
    CR_SECURESESSION_FEATURE_RESULT_SESSION_ERROR,

    /**
     * @brief Request made during an invalid state of the Securesession
     * Feature.
     */
    CR_SECURESESSION_FEATURE_RESULT_CALL_UNEXPECTED,

    /**
     * @brief Generic catch-all error code. If you see this, it is quite
     * unfortunate for you.
     */
    CR_SECURESESSION_FEATURE_RESULT_GENERIC_ERROR,

    /**
     * @brief Failed to send a message to the reader; it is possible that the
     * reader is not connected.
     */
    CR_SECURESESSION_FEATURE_RESULT_NO_READER,

    /**
     * @brief Server returned a securesession denial response, failure
     * response, or malformed response.
     *
     * @note This error is LCR's (LibCardReader's) way of surfacing errors from
     * the backend server; this means that the server is sending us an error,
     * *not* the reader or LCR. Receiving this error does *not* indicate a
     * failure from LCR, or the reader. The server is denying us!
     */
    CR_SECURESESSION_FEATURE_RESULT_SERVER_DENY_ERROR,

    /**
     * @brief Generic `securesession` library error.
     *
     * @details This error code is funneled up from the `securesession` library
     * that LCR uses. It is a generic catch-all for errors from
     * the `securesession` library, *not* the Securesession Feature.
     *
     * @note This error is returned in cases where
     * #CR_SECURESESSION_FEATURE_RESULT_SERVER_DENY_ERROR is not returned, and
     * there is an error returned from the `securesession` library.
     */
    CR_SECURESESSION_FEATURE_RESULT_MODULE_GENERIC_ERROR,

    /**
     * @brief Maximum number of allowable readers have been connected, and
     * secured with a given Securesession Feature instance.
     */
    CR_SECURESESSION_FEATURE_RESULT_MAX_READERS_CONNECTED,

    /**
     * @brief Invalid argument passed to API function in the `securesession`
     * library.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_ARG.
     */
    CR_SECURESESSION_FEATURE_RESULT_ARG,

    /**
     * @brief State machine's session state for the `securesession` library is
     * invalid for the given API call being attempted.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_SESSION_STATE.
     */
    CR_SECURESESSION_FEATURE_RESULT_SESSION_STATE,

    /**
     * @brief Input length passed to `securesession` library API method is too
     * large.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_INPUT_SIZE.
     */
    CR_SECURESESSION_FEATURE_RESULT_INPUT_SIZE,

    /**
     * @brief Output buffer size passed to `securesession` library API method
     * is too small to fit requested data.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_OUTPUT_SIZE.
     */
    CR_SECURESESSION_FEATURE_RESULT_OUTPUT_SIZE,

    /**
     * @brief Invalid message received from server and passed to the
     * `securesession` library.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_MSG_TYPE.
     */
    CR_SECURESESSION_FEATURE_RESULT_MSG_TYPE,

    /**
     * @brief Session ID of approval, or key update message does not match the
     * `securesession` library's session state session ID.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_SESSION_ID.
     */
    CR_SECURESESSION_FEATURE_RESULT_SESSION_ID,

    /**
     * @brief `securesession` library failed to generate Curve25519 private, or
     * public key.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_CURVE.
     */
    CR_SECURESESSION_FEATURE_RESULT_CURVE,

    /**
     * @brief HKDF (Hash-Based Key Derivation Function) derivation failed in
     * `securesession` library.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_HKDF.
     */
    CR_SECURESESSION_FEATURE_RESULT_HKDF,

    /**
     * @brief Server denial message passed to `securesession` library.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_DENIED.
     */
    CR_SECURESESSION_FEATURE_RESULT_DENIED,

    /**
     * @brief Invalid pin digit passed to `securesession` library.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_BAD_DIGIT.
     */
    CR_SECURESESSION_FEATURE_RESULT_BAD_DIGIT,

    /**
     * @brief Attempt to add an additional digit for a PIN when current pin is
     * already sufficiently long.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_PIN_FULL.
     */
    CR_SECURESESSION_FEATURE_RESULT_PIN_FULL,

    /**
     * @brief PIN finalization attempt made when the PIN is too short.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_PIN_TOO_SHORT.
     */
    CR_SECURESESSION_FEATURE_RESULT_PIN_TOO_SHORT,

    /**
     * @brief Unused.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_INVALID_PIN_REQUEST.
     */
    CR_SECURESESSION_FEATURE_RESULT_INVALID_PIN_REQUEST,

    /**
     * @brief Invalid hash in key update message.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_INVALID_KEY_UPDATE_MSG.
     */
    CR_SECURESESSION_FEATURE_RESULT_INVALID_KEY_UPDATE_MSG,

    /**
     * @brief Unused.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_AES.
     */
    CR_SECURESESSION_FEATURE_RESULT_AES,

    /**
     * @brief Invalid protocol version in server response, or approval.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_PROTOCOL_VERSION.
     */
    CR_SECURESESSION_FEATURE_RESULT_PROTOCOL_VERSION,

    /**
     * @brief Unused.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_APPROVAL_MISMATCH.
     */
    CR_SECURESESSION_FEATURE_RESULT_APPROVAL_MISMATCH,

    /**
     * @brief Unused.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_APPROVAL_EXPIRED.
     */
    CR_SECURESESSION_FEATURE_RESULT_APPROVAL_EXPIRED,

    /**
     * @brief No more transactions can be completed on the current
     * Securesession session.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_NO_TXN_LEFT.
     */
    CR_SECURESESSION_FEATURE_RESULT_NO_TXN_LEFT,

    /**
     * @brief Generic `securesession` library API call failure.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_API_CALL.
     */
    CR_SECURESESSION_FEATURE_RESULT_API_CALL,

    /**
     * @brief Error calling into `minesweeper` from the `securesession`
     * library.
     *
     * @details Minesweeper is an external (non-ESW owned, not a part of LCR,
     * not under the purview of the hardware) library that is a part of the
     * mobile Point-of-Sale (POS) applications and called into by the
     * `securesession` library for ticket validation. If the mobile application
     * does not have a valid ticket, then Securesession API calls will fail.
     * Tickets are granted by the server (read: Flipper) as a result of the
     * Device Attestation process. Device Attestation is a process that happens
     * on the mobile application, and is un-related to LCR or firmware
     * operations (read: Device Attestation is not tied to the reader,
     * firmware, or LCR). The process involves the mobile device using
     * Minesweeper to compute a hash of its application space, including RAM,
     * and send that up with relevant device information to the server (read:
     * Flipper) for validation; upon validation, Flipper will grant it a
     * ticket; if validation fails, Flipper will not grant it a ticket.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @note *PLEASE READ*: If you are seeing this error, there is an issue in
     * the POS application preventing Securesession, or potentially an issue
     * with the backend server, but there is *not* an issue in the reader,
     * firmware, or LCR.
     *
     * @see SECURESESSION_ERROR_MINESWEEPER_CALL.
     */
    CR_SECURESESSION_FEATURE_RESULT_MINESWEEPER_CALL,

    /**
     * @brief SHA256 hash of Minesweeper frame failed.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_SHA256.
     */
    CR_SECURESESSION_FEATURE_RESULT_SHA256,

    /**
     * @brief HMAC verification of Securesession approval message failed.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_BAD_HMAC.
     */
    CR_SECURESESSION_FEATURE_RESULT_BAD_HMAC,

    /**
     * @brief TDES encryption of pinblock data failed.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_TDES.
     */
    CR_SECURESESSION_FEATURE_RESULT_TDES,

    /**
     * @brief Unused.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_ENCODE_FAILURE.
     */
    CR_SECURESESSION_FEATURE_RESULT_ENCODE_FAILURE,

    /**
     * @brief No `securesession` host context is allocated.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_CONTEXT.
     */
    CR_SECURESESSION_FEATURE_RESULT_CONTEXT,

    /**
     * @brief Out of memory for allocating a `securesession` host context.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_OUT_OF_CONTEXTS.
     */
    CR_SECURESESSION_FEATURE_RESULT_OUT_OF_CONTEXTS,

    /**
     * @brief Invalid parameters in Securesession initialization message, or
     * approval message.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_BAD_FIELD.
     */
    CR_SECURESESSION_FEATURE_RESULT_BAD_FIELD,

    /**
     * @brief Failure to deserialize whitebox keys.
     *
     * @note This bubbles up the error from the `securesession` library thorugh
     * the Securesession Feature; this is an error from the library, *not* the
     * feature.
     *
     * @see SECURESESSION_ERROR_WHITEBOX_KEY_DESERIALIZE.
     */
    CR_SECURESESSION_FEATURE_RESULT_WHITEBOX_KEY_DESERIALIZE,

    /**
     * @brief Unknown error message from `securesession` library.
     */
    CR_SECURESESSION_FEATURE_RESULT_UNKNOWN
} cr_securesession_feature_result_t;

/**
 * @brief Enumeration of response codes to send to the Flipper endpoint.
 *
 * @note These enumerations are compatible with Flipper API v1.0 session
 * validation endpoint: `/1.0/flipper/session/validate`.
 */
typedef enum {
    /**
     * @brief Message for validation of a securesession request.
     */
    CR_SECURESESSION_FEATURE_SERVER_MSG_VALIDATE,
} cr_securesession_feature_server_msg_type_t;

/**
 * @brief Enumeration of the different error response codes from Flipper.
 *
 * @details This enumeration covers the different server error codes that
 * Flipper may send in response to a Securesession establishment attempt.
 */
typedef enum {
    /**
     * @brief Generic catch-all error code.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_GENERIC_ERROR = 0,

    /**
     * @brief Failure to parse server's response.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_PARSE_ERROR,

    /**
     * @brief Server is denying the session, but does not have a specific
     * reason, as of yet, as to why.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_UNKNOWN_DENY_REASON,

    /**
     * @brief Session was not denied.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_NOT_DENIED,

    /**
     * @brief Session was denied because the seller's account is suspended.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_SELLER_SUSPENDED,

    /**
     * @brief Session was denied because the seller is not allowed to acquire
     * a securesession.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_SELLER_NOT_ELIGIBLE,

    /**
     * @brief Session was denied because the seller's hardware is not
     * supported by Flipper.
     *
     * @note This can happen with random unknown Android devices running some
     * weird version of Android, for example.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_HARDWARE_NOT_SUPPORTED,

    /**
     * @brief Session was denied because the reader's firwmare is too old.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_FIRMWARE_TOO_OLD,

    /**
     * @brief Session was denied because the reader's firmware is not
     * supported.
     *
     * @note An example of this can be attempting to use firmware with US
     * TMS in Canada.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_FIRMWARE_NOT_SUPPORTED,

    /**
     * @brief Session was denied because the reader's firmware was modified.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_FIRMWARE_MODIFIED,

    /**
     * @brief Session was denied because the reader is tampered.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_FIRMWARE_DAMAGED,

    /**
     * @brief Session was denied because the mobile device has developer mode
     * enabled.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_IN_DEVELOPER_MODE,

    /**
     * @brief Session was denied because the mobile application is too old.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_APP_TOO_OLD,

    /**
     * @brief Session was denied because the mobile application has been
     * tampered with.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_APP_DAMAGED,

    /**
     * @brief Session was denied because an invalid request was made.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_INVALID_REQUEST,

    /**
     * @brief Session was denied because the mobile device is in accessibility
     * mode.
     *
     * @note One might ask why this is an issue, but for countries where SPoC
     * (Secure Pin on COTS) is enabled, this allows an attacker to intercept
     * PINs.
     *
     * @note COTS stands for Commercial Off-The-Shelf Devices.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_DEVICE_IN_ACCESSIBILITY_MODE,

    /**
     * @brief Session was denied because the seller's account is not yet
     * active.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_SELLER_NOT_ACTIVATED,

    /**
     * @brief No server error.
     */
    CR_SECURESESSION_FEATURE_SERVER_ERROR_NONE,
} cr_securesession_feature_server_error_t;

/**
 * @brief Enumeration of the hints sent from the server (read: Flipper) that
 * are bubbled up through LCR to the Mobile Application in order to guide the
 * visbile UX or UI.
 *
 * @note These values do *not* come from LCR or the reader, they are sent down
 * from the server to be bubbled up to POS.
 */
typedef enum {
    /**
     * @brief *PLEASE READ*: DO NOT USE!
     */
    CR_SECURESESSION_FEATURE_SERVER_UX_HINT_DO_NOT_USE = 0,

    /**
     * @brief No suggested action to be taken.
     */
    CR_SECURESESSION_FEATURE_SERVER_UX_HINT_NO_SUGGESTED_ACTION,

    /**
     * @brief Suggest to the merchant to retry the operation.
     */
    CR_SECURESESSION_FEATURE_SERVER_UX_HINT_SUGGEST_RETRY,

    /**
     * @brief Suggest to the merchant to activate their account.
     */
    CR_SECURESESSION_FEATURE_SERVER_UX_HINT_SUGGEST_ACTIVATION,

    /**
     * @brief Suggest to the merchant to contact support for assistance.
     */
    CR_SECURESESSION_FEATURE_SERVER_UX_HINT_SUGGEST_CONTACT_SUPPORT,

    /**
     * @brief Enumeration of the total number of possible UX hints.
     *
     * @note *PLEASE READ*: This should always be the last element of this
     * enumeration.
     */
    CR_SECURESESSION_FEATURE_SERVER_UX_HINT_COUNT
} cr_securesession_feature_server_ux_hint_t;

/**
 * @brief Event codes emitted by the Securesession Feature.
 */
typedef enum {
    /**
     * @brief Valid securesession.
     */
    CR_SECURESESSION_FEATURE_EVENT_TYPE_SESSION_VALID,

    /**
     * @brief Invalid securesession.
     */
    CR_SECURESESSION_FEATURE_EVENT_TYPE_SESSION_INVALID,
} cr_securesession_feature_event_type_t;

/**
 * @brief Enumeration of the supported offline modes for the Securesession
 * feature.
 *
 * @see https://docs.google.com/document/d/1xJkub9vPjDCZxKoNUu_23pI4O9Cl0dGFL-wRXAigKy8/edit
 */
typedef enum {
    /**
     * @brief Offline mode is not supported.
     */
    CR_SECURESESSION_FEATURE_OFFLINE_MODE_NONE,

    /**
     * @brief Currently offline. Offline mode is enabled, and supported.
     */
    CR_SECURESESSION_FEATURE_OFFLINE_MODE_SUPPORTED,

    /**
     * @brief Currently offline. In `Unplanned Offline` mode.
     */
    CR_SECURESESSION_FEATURE_OFFLINE_MODE_UNPLANNED_OFFLINE,

    /**
     * @brief Currently offline. In `Planned Offline` mode.
     */
    CR_SECURESESSION_FEATURE_OFFLINE_MODE_PLANNED_OFFLINE,
} cr_securesession_feature_offline_mode_t;

/**
 * @brief Information data recorded about a Securesession session.
 */
typedef struct cr_securesession_feature_session_data_t {
    /**
     * @brief Session ID for the reader's current secure session.
     */
    uint32_t session_id;

    /**
     * @brief Remaining amount of transactions for the reader's current
     * secure session.
     */
    uint32_t reader_transaction_count;

    /**
     * @brief Reader's RTC time since epoch.
     */
    uint32_t reader_utc_epoch_time;
} cr_securesession_feature_session_data_t;

/**
 * @brief Definition for the array containing the UX hints as received from
 * the server.
 */
typedef struct cr_securesession_feature_server_ux_hint_array_t {
    /**
     * @brief Count of the number of received UX hints.
     */
    uint32_t count;

    /**
     * @brief Array of the received UX hints.
     */
    cr_securesession_feature_server_ux_hint_t *ux_hints;
} cr_securesession_feature_server_ux_hint_array_t;

/**
 * @brief Callback invoked by LCR when LCR needs to send data upstream to the
 * server.
 *
 * @details LCR relies on the mobile POS applications in order to send data to
 * Square servers, or receive data from Square servers. It is the
 * responsibility of the mobile application to provide the function that can be
 * called by LCR to do so.
 *
 * @param context        User-supplied context pointer.
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param msg_type       Type of message being sent; should be used to dispatch
 *                       to the correct server endpoint.
 * @param buf            Raw byte buffer; may need to be `base64` encoded,
 *                       depending on the message type.
 * @param buf_len        Length of the passed @p buf in bytes.
 *
 * @note This callback may be invoked at arbitrary times.
 */
typedef void (*cr_securesession_feature_send_to_server_cb_t)(void *context,
                                                             struct cr_securesession_feature_t *securesession,
                                                             cr_securesession_feature_server_msg_type_t msg_type,
                                                             void const *buf,
                                                             size_t buf_len);

/**
 * @brief Callback invoked by LCR when a securesession occurs event occurs,
 * such as the session becoming valid or invalid.
 *
 * @param context        User-supplied context pointer.
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param session_data   Pointer to the current session data.
 * @param event_type     Type of event that occurred.
 *
 * @note A valid securesession is required for chip card payment functionality.
 */
typedef void (*cr_securesession_feature_session_event_cb_t)(void *context,
                                                            struct cr_securesession_feature_t *securesession,
                                                            cr_securesession_feature_session_data_t *session_data,
                                                            cr_securesession_feature_event_type_t event_type);

/**
 * @brief Callback invoked by LCR when LCR requires a PIN in order to complete
 * a payment.
 *
 * @details This callback is a part of the Securesession API as the PIN input
 * needs to be implemented by obfuscated code, such as a Minesweeper.
 *
 * @param context            User-supplied context pointer.
 * @param securesession      Pointer to the Securesession Feature instance.
 * @param card_info          Pointer to the information for the card on which
 *                           the transaction is being performed.
 * @param can_bypass_pin     Boolean indicating whether PIN can be skipped for
 *                           the transaction.
 * @param final_pin_attempt  Boolean indicating whether this is the final PIN
 *                           attempt or not.
 */
typedef void (*cr_securesession_feature_pin_requested_cb_t)(void *context,
                                                            struct cr_securesession_feature_t *securesession,
                                                            struct cr_card_info_t const *card_info,
                                                            bool can_bypass_pin,
                                                            bool final_pin_attempt);

/**
 * @brief Callback invoked by LCR in order to cache a securesession as part of
 * the Offline Mode feature.
 *
 * @details This callback is a part of the Securesession API to support the
 * Offline Mode feature, which allows a merchant to take EMV payments while
 * offline. This callback is invoked when a Securesession status message is
 * received, and offline mode is enabled.
 *
 * @param context           User-supplied context pointer.
 * @param securesession     Pointer to the Securesession Feature instance.
 * @param reader_id         Unique identifier for the reader with which the
 *                          securesession was established.
 * @param reader_id_len     Length of the reader's identifier.
 * @param session_data      Pointer to the securesession data blob.
 * @param session_data_len  Length of the passed session data in bytes.
 *
 * @note The invoked callback should securely cache the session data using the
 * given @p reader_id as a unique key for looking up session data on the
 * subsequent #cr_securesession_feature_read_cache_cb_t() invocation.
 *
 * @see https://docs.google.com/document/d/1qWWcv0YeejpcfpoaD22iG6eXnwG9kWZ2uLb0qEEHqKc
 */
typedef void (*cr_securesession_feature_write_cache_cb_t)(void *context,
                                                          struct cr_securesession_feature_t *securesession,
                                                          char const *reader_id,
                                                          size_t reader_id_len,
                                                          void const *session_data,
                                                          size_t session_data_len);

/**
 * @brief Callback invoked by LCR in order to read securesession data from the
 * cache as a part of the Offline Mode feature.
 *
 * @details The Application handler that is invoked by this callback should
 * call #cr_securesession_feature_apply_cached_session() with the matching
 * data for the given @p reader_id after reading the data from the cache; if
 * no session data is present in the cache for the given @p reader_id, then
 * it should call #cr_securesession_feature_apply_cached_session() with `NULL`
 * as the session data.
 *
 * @param context           User-supplied context pointer.
 * @param securesession     Pointer to the Securesession Feature instance.
 * @param reader_id         Unique identifier for the reader with which the
 *                          securesession was established; should be used as
 *                          the lookup key in the cache.
 * @param reader_id_len     Length of the reader's identifier.
 *
 * @see https://docs.google.com/document/d/1qWWcv0YeejpcfpoaD22iG6eXnwG9kWZ2uLb0qEEHqKc
 */
typedef void (*cr_securesession_feature_read_cache_cb_t)(void *context,
                                                         struct cr_securesession_feature_t *securesession,
                                                         char const *reader_id,
                                                         size_t reader_id_len);

/**
 * @brief Callback invoked by LCR in order to clear securesession data from the
 * cache as part of the Offline Mode feature.
 *
 * @details This callback is invoked as part of the Offline Mode feature, and
 * requests / signals to the application handler to delete the securesession
 * data from its cache for the reader specified by the given @p reader_id. This
 * may be invoked when a merchant disables Offline Mode, or when the
 * securesession data for a given reader is no longer valid.
 *
 * @param context           User-supplied context pointer.
 * @param securesession     Pointer to the Securesession Feature instance.
 * @param reader_id         Unique identifier for the reader with which the
 *                          securesession was established; should be used as
 *                          the lookup key in the cache.
 * @param reader_id_len     Length of the reader's identifier.
 *
 * @see https://docs.google.com/document/d/1qWWcv0YeejpcfpoaD22iG6eXnwG9kWZ2uLb0qEEHqKc
 */
typedef void (*cr_securesession_feature_clear_cache_cb_t)(void *context,
                                                          struct cr_securesession_feature_t *securesession,
                                                          char const *reader_id,
                                                          size_t reader_id_len);

/**
 * @brief Callback invoked by LCR to signal to the application how much time
 * is remaining on the host's securesession.
 *
 * @details This callback is invoked when securesession is restored or acquired
 * in order to inform the application about the amount of seconds before said
 * session will expire. It is the responsibility of the application to then use
 * this information to know when a future request will be delayed due to
 * acquiring a securesession, or to inform decisions around pro-actively
 * acquiring a securesession.
 *
 * @param context                 User-supplied context pointer.
 * @param securesession           Pointer to the Securesession Feature instance.
 * @param remaining_time_seconds  Remaining securesession time in seconds.
 *
 * @note *PLEASE READ*: When the reader is inactive, it will go into a lower
 * power state in order to conserve power. Repeatedly acquiring a
 * securesession, or sending messages to the reader repeatedly will prevent
 * the device from conserving power, and should be avoided. Instead, if the
 * application developer wishes to prevent delays due to securesession
 * acquisition, they should attempt it just _before_ doing something that
 * would require a securesession, such as when a merchant foregrounds the
 * mobile application, or adds an item to their checkout cart.
 */
typedef void (*cr_securesession_expiration_time_cb_t)(void *context,
                                                      struct cr_securesession_feature_t *securesession,
                                                      uint32_t remaining_time_seconds);

/**
 * @brief Type definition for the Securesession Feature API.
 *
 * @details This API should be defined by the application layer, and passed in
 * an invocation of #cr_securesession_feature_init(). Callbacks defined in the
 * event API will be invoked by LCR when the designated events occur.
 */
typedef struct cr_securesession_feature_event_api_t {
    /**
     * @brief Callback to invoke to send a securesession request from the
     * reader up to Square's servers for validation.
     *
     * @note This is a required field; it must not be `NULL`.
     */
    cr_securesession_feature_send_to_server_cb_t on_send_to_server;

    /**
     * @brief Callback invoked when a securesession is acquired or denied.
     *
     * @note This is a required field; it must not be `NULL`.
     */
    cr_securesession_feature_session_event_cb_t on_session_event;

    /**
     * @brief Callback invoked when a PIN is requested to complete a payment
     * transaction is a part of SPoC.
     *
     * @note This is a required field; it must not be `NULL`.
     */
    cr_securesession_feature_pin_requested_cb_t on_pin_requested;

    /**
     * @brief Callback invoked in order to write securesession data to the
     * application's cache as a part of the Offline Mode feature.
     *
     * @note This is a required field when Offline Mode is enabled; must not
     * be `NULL` if enabled.
     */
    cr_securesession_feature_write_cache_cb_t on_write_cache;

    /**
     * @brief Callback invoked in order to read securesession data from the
     * application's cache as a part of the Offline Mode feature.
     *
     * @note This is a required field when Offline Mode is enabled; must not
     * be `NULL` if enabled.
     */
    cr_securesession_feature_read_cache_cb_t on_read_cache;

    /**
     * @brief Callback invoked in order to clear securesession data from the
     * application's cache as a part of the Offline Mode feature.
     *
     * @note This is a required field when Offline Mode is enabled; must not
     * be `NULL` if enabled.
     */
    cr_securesession_feature_clear_cache_cb_t on_clear_cache;

    /**
     * @brief Callback invoked when a securesession is acquired or read from
     * the cache in order to indicate to the application layer how much time
     * is remaining for the securesession.
     *
     * @note This field is optional.
     */
    cr_securesession_expiration_time_cb_t on_expiration_time;

    /**
     * @brief User-supplied context pointer; allows the application to pass in
     * a pointer to data that will be passed back to the application when any
     * of the event API callbacks fire.
     *
     * @note This field is optional.
     */
    void *context;
} cr_securesession_feature_event_api_t;

/**
 * @brief Allocates memory for a Securesession Feature instance.
 *
 * @details This API method allows for allocatio of sufficient memory for a
 * Securesession Feature  that will be passed into the
 * `cr_securesession_feature_*` API calls. Should be called before calling any
 * of the API methods in order to get a valid feature instance.
 *
 * @return Pointer to a Securesession Feature instance.
 */
struct cr_securesession_feature_t *cr_securesession_feature_alloc(void);

/**
 * @brief Initializes a Securesession Feature instance
 * (cr_securesession_feature_t) with the specified callbacks, and registers
 * the feature with the specified Cardreader instance.
 *
 * @details This method initializes an instance of the Securesession Feature
 * with the specified callbacks defined in the @p event_api, and registers the
 * feature with the specified @p cardreader instance.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param cardreader     Pointer to the Cardreader instance upon which to bind
 *                       the feature.
 * @param event_api      Pointer to the application's event API.
 * @param offline_mode   What offline mode support to instantiate the feature
 *                       with.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as outlined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been allocated by a call
 * to #cr_securesession_feature_alloc().
 *
 * @note This method must be called before calling other API functions on a
 * Securesession Feature instance.
 */
cr_securesession_feature_result_t cr_securesession_feature_init(
    struct cr_securesession_feature_t *securesession,
    struct cr_cardreader_t *cardreader,
    struct cr_securesession_feature_event_api_t const *event_api,
    cr_securesession_feature_offline_mode_t offline_mode);

/**
 * @brief Terminates a Securesession Feature instance instantiated by a call to
 * #cr_securesession_feature_init(), and de-registers the feature from Cardreader.
 *
 * @param securesession Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as outlined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 *
 * @note The Securesession Feature instance should be terminated before
 * freeing the associated memory by calling #cr_securesession_feature_free().
 */
cr_securesession_feature_result_t cr_securesession_feature_term(struct cr_securesession_feature_t *securesession);

/**
 * @brief Frees an instance of a Securesession Feature that was allocated by a
 * call to #cr_securesession_feature_alloc().
 *
 * @param securesession Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as outlined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been terminated by a
 * call to #cr_securesession_feature_term().
 */
cr_securesession_feature_result_t cr_securesession_feature_free(struct cr_securesession_feature_t *securesession);

/**
 * @brief Configures the Securesession feature instance with the provided
 * whhitebox keys.
 *
 * @details Configures the Securesession feature with the provided whitebox
 * keys, copying and storing them internally.
 *
 * @param securesession     Pointer to the Securesession Feature instance.
 * @param whitebox_key      Whitebox key.
 * @param whitebox_key_len  Length of the provided whitebox key.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note If Whitebox keys are not supported by the application, then `NULL`
 * should be passed as the @p whitebox_key with a @p whitebox_key_len of
 * `0`, as would be the case with builds like Point-of-Sale (POS), which do
 * not use whitebox keys vs. builds like PaySDK, which do.
 *
 * @note The Securesession Feature must have been allocated by a call to
 * #cr_securesession_feature_alloc(), but this method *MUST* be called before
 * the feature is instantiated by a call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_set_whitebox_key(
    struct cr_securesession_feature_t *securesession,
    void *whitebox_key,
    size_t whitebox_key_len);

/**
 * @brief Processes the response sent by the server for a request generated by
 * an invocation of #cr_securesession_feature_event_api_t::on_send_to_server().
 *
 * @param securesession             Pointer to the Securesession Feature instance.
 * @param buf                       Server response.
 * @param buf_len                   The size of the server response in bytes.
 * @param server_error_message_out  If the return value is
 *                                  #CR_SECURESESSION_FEATURE_RESULT_SERVER_DENY_ERROR,
 *                                  this will contain additional information about the
 *                                  server deny. Should be ignored otherwise.
 * @param server_ux_hint_out        If the return value is
 *                                  #CR_SECURESESSION_FEATURE_RESULT_SERVER_DENY_ERROR,
 *                                  this will contain additional information about the
 *                                  user experience for the error screen that will be
 *                                  display. Should be ignored otherwise.
 * @param localized_title_out       If the return value is
 *                                  #CR_SECURESESSION_FEATURE_RESULT_SERVER_DENY_ERROR,
 *                                  this will contain the title string to display. Should
 *                                  be ignored otherwise.
 * @param localized_title_out_len   The length of the @p localized_title_out buffer.
 * @param localized_description_out If the return value is
 *                                  #CR_SECURESESSION_FEATURE_RESULT_SERVER_DENY_ERROR,
 *                                  this will contain the description string to display.
 *                                  Should be ignored otherwise.
 * @param localized_description_out_len The length of the @p localized_description_out buffer.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_process_server_message(
    struct cr_securesession_feature_t *securesession,
    void const *buf,
    size_t buf_len,
    cr_securesession_feature_server_error_t *server_error_message_out,
    cr_securesession_feature_server_ux_hint_t *server_ux_hint_out,
    char *localized_title_out,
    size_t localized_title_out_len,
    char *localized_description_out,
    size_t localized_description_out_len);

/**
 * @brief Notifies LCR that a server request failed.
 *
 * @details This function should be called by the application after a server
 * request is attempted by LCR as generated by an invocation of
 * #cr_securesession_feature_event_api_t::on_send_to_server(). The application
 * calls this to nofiy LCR that the server request failed, as LCR *does not*
 * have any network capability itself, and relies on the application to provide
 * that functionlity. If LCR is waiting for a server response, then its state
 * will be updated by this function call, otherwise this function call is a
 * no-op.
 *
 * @param securesession Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_notify_server_error(
    struct cr_securesession_feature_t *securesession);

/**
 * @brief Initiates the process of establishing a securesession.
 *
 * @details This function is invoked to begin the process of establishing a
 * securesession if a valid one does not already exist; if a valid one exists,
 * or a session is currently in the process of being established, then this
 * function call is a no-op.
 *
 * Establishing a securesession is an involved process, involving
 * communicating between the reader and the application, communication
 * between the application and the server, and communication between the
 * reader and the server, using the application as a proxy as the reader lacks
 * network capability. While securesession secures communication between the
 * reader and the server, as it goes through the application, the application
 * itself must secure itself in order to be allowed to act as the proxy for
 * communication. That process is done by a library called Minesweeper that
 * is built into the application, and the process itself is called Device
 * Attestation. Establishing a securesession itself, when the application is
 * already secure, takes an overage < 1 second. However, if the application
 * is not secure when an attempt is made, then securesession establishment
 * will be blocked waiting for the application to secure itself, which can
 * take between 10-20 seconds on average, or even more than 30 seconds
 * depending on the device the application is running on, and the stability
 * and speed of the network connection.
 *
 * @param securesession Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_establish_session(
    struct cr_securesession_feature_t *securesession);

/**
 * @brief Requests for PIN entry to be by-passed.
 *
 * @details This function is envoked by the application layer in order to
 * bypass PIN entry for a card transaction. This can be invoked by the
 * application in response to a
 * #cr_securesession_feature_event_api_t::on_pin_requested() callback where
 * `can_bypass_pin` is `true`. If PIN entry cannot be by-passed, then PIN
 * entry bypassing will fail.
 *
 * @param securesession Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_pin_bypass(struct cr_securesession_feature_t *securesession);

/**
 * @brief Resets the internal PIN material.
 *
 * @details This function will reset the PIN material that lives within the
 * Securesession Feature instance. The PIN block that is reset depends on which
 * format is being used.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
void cr_securesession_host_pin_reset_dispatch(struct cr_securesession_feature_t *securesession);

/**
 * @brief Encrypts the PIN block, and sends it to the reader.
 *
 * @details This method should be called once the PIN has been fully entered by
 * the customer in order to have the PIN block encrypted, and sent off to the
 * reader. This method will determine which format the PIN block is in based
 * on what the host and reader have agreed upon, choosing the most _newest_
 * format when possible over the older format.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_finalize_and_submit_pinblock_dispatch(
    struct cr_securesession_feature_t *securesession);

/**
 * @brief Adds a single digit of a PIN to a current PIN card transaction.
 *
 * @details This function should be called by the application that implements
 * SPoC each time a digit is entered by the customer as a part of their card
 * transaction. This function will choose to use either the _old_ ISO format
 * 1, or the _new_ ISO format 4 for encoding the PIN data; which format is
 * chosen depends on what is agreed upon between the reader and the host.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param digit          The digit to add for the PIN.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_host_pin_add_digit_dispatch(
    struct cr_securesession_feature_t *securesession,
    uint8_t digit);

/**
 * @brief Submits a Pinblock generated in response to the
 * `cr_securesession_feature_event_api_t::on_pin_requested()` callback.
 *
 * @details This method is invoked by a call to
 * #cr_securesession_feature_submit_pinblock_dispatch(), and should not be
 * called by the application directly.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param pinblock       Encrypted pinblock data (ISO Format 1).
 * @param pinblock_len   Length of the @p pinblock in bytes.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 *
 * @deprecated @p pinblock is in ISO format 1, which is deprecated, and should
 * not be used.
 */
cr_securesession_feature_result_t cr_securesession_feature_submit_pinv1block(
    struct cr_securesession_feature_t *securesession,
    void const *pinblock,
    size_t pinblock_len);

/**
 * @brief Submits a Pinblock generated in response to the
 * `cr_securesession_feature_event_api_t::on_pin_requested()` callback.
 *
 * @details This method is invoked by a call to
 * #cr_securesession_feature_submit_pinblock_dispatch(), and should not be
 * called by the application directly.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param pinblock       Encrypted pinblock data (ISO Format 4).
 * @param pinblock_len   Length of the @p pinblock in bytes.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_submit_pinv4block(
    struct cr_securesession_feature_t *securesession,
    void const *pinblock,
    size_t pinblock_len);

/**
 * @brief Submits a Pinblock generated in response to the
 * `cr_securesession_feature_event_api_t::on_pin_requested()` callback.
 *
 * @details This method is invoked by a call to
 * #cr_securesession_feature_finalize_and_submit_pinblock_dispatch(), and
 * should not be called directly; instead the application should call
 * #cr_securesession_feature_finalize_and_submit_pinblock_dispatch() when the
 * PIN has been fully entered by the customer. This method determines which
 * pinblock format is being used, and calls the appropriate internal
 * pinblock-specific function, and should be favoured over methods like
 * #cr_securesession_feature_submit_pinv4block().
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param pinblock       Encrypted pinblock data.
 * @param len            Length of the @p pinblock in bytes.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_submit_pinblock_dispatch(
    struct cr_securesession_feature_t *securesession,
    void const *pinblock,
    size_t len);

/**
 * @brief Returns a pointer to the underlying Securesession Feature host
 * context handle.
 *
 * @details This method is used to access the underlying Securesession
 * host context handle from the public API.
 *
 * @param securesession       Pointer to the Securesession Feature instance.
 * @param context_handle_out  Output pointer to store the pointer to the host
 *                            context handle.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_get_context_handle(
    struct cr_securesession_feature_t *securesession,
    void **context_handle_out);

/**
 * @brief Applies a cached securesession as a part of the Offline Mode feature.
 *
 * @details This function is used a part of the Offline Mode feature in order
 * to restore the host's secure session data. The application should invoke
 * this method in order to restore its host session data in response to an
 * invocation of cr_securesession_feature_event_api.on_read_cache.
 *
 * @param securesession     Pointer to the Securesession Feature instance.
 * @param reader_id         Unique identifier for the reader whose session this
 *                          data corresponds to.
 * @param reader_id_len     Length of the @p reader_id in bytes.
 * @param session_data      Session data to be restored.
 * @param session_data_len  Length of the @p session_data in bytes.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 *
 * @see https://docs.google.com/document/d/1qWWcv0YeejpcfpoaD22iG6eXnwG9kWZ2uLb0qEEHqKc
 */
cr_securesession_feature_result_t cr_securesession_feature_apply_cached_session(
    struct cr_securesession_feature_t *securesession,
    char const *reader_id,
    size_t reader_id_len,
    void const *session_data,
    size_t session_data_len);

/**
 * @brief Changes the Offline Mode supported by the Securesession Feature.
 *
 * @details This function is used to set the current offline mode for the
 * Securesession Feature. This function should be used when the offline mode
 * should be changed for the current feature instance, such as when a merchant
 * enables or disables Offline Mode.
 *
 * @param securesession  Pointer to the Securesession Feature instance.
 * @param offline_mode   The new offline mode the feature is operating in.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 *
 * @see https://docs.google.com/document/d/1qWWcv0YeejpcfpoaD22iG6eXnwG9kWZ2uLb0qEEHqKc
 */
cr_securesession_feature_result_t cr_securesession_feature_set_offline_mode(
    struct cr_securesession_feature_t *securesession,
    cr_securesession_feature_offline_mode_t offline_mode);

/**
 * @brief Clears stored secure session state from the host.
 *
 * @details This method will cleaer all securesession state for the given
 * Feature instance from LCR memory, and the application cache.
 *
 * @param securesession Pointer to the Securesession Feature instance.
 *
 * @return #CR_SECURESESSION_FEATURE_RESULT_SUCCESS on success, otherwise a
 * failure code as defined in #cr_securesession_feature_result_t.
 *
 * @note The Securesession Feature instance must have been initialized by a
 * call to #cr_securesession_feature_init().
 */
cr_securesession_feature_result_t cr_securesession_feature_clear_session(
    struct cr_securesession_feature_t *securesession);

/** @} */
