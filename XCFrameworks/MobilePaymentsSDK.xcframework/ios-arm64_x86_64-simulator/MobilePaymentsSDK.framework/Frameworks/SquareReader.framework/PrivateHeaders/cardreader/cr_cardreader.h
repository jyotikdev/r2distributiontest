// Copyright (c) 2014 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief Library CardReader (LCR)
 *
 * @details LCR is  the library that provides high level APIs for performing
 * functions with a Square hardware device, such as starting a payment, or
 * reading a badge. These API calls translate into low level messages that are
 * sent to the SPE hardware over a defined communication channel provided by
 * the higher level host.
 *
 * @author Rodion Steshenko <rodion@squareup.com>
 *
 * @copyright 2014 Square, Inc.
 */

/**
 * @defgroup lcr LibCardReader
 *
 * @brief LCR is an API for interfacing with a Square Secure Payment (SPE)
 * hardware.
 *
 * Overview
 * ========
 * LCR (LibCardReader) is a C library which implements the logic for
 * interacting with a Square device. Internally, LCR is a state machine, and is
 * agnostic as to how communication with the downstream device occurs.
 *
 * Design Patterns
 * ===============
 *
 * - LCR functions do not grab locks, make blocking system calls or otherwise
 *   block the calling thread with the exception of invoking callbacks which
 *   also should not block.
 * - LCR is organized around a set of features with each feature defining
 *   functions to manipulate the reader or request data from the reader and a
 *   set of callbacks that are invoked when requested data arrives or a
 *   requested operation completes.
 *   - LCR features provide an object oriented C interface. The callback structs
 *     (e.g. `cr_*_api_t`) are essentially a class with virtual functions.
 *
 * Thread Safety
 * =============
 * LCR is *NOT* thread safe and all calls to LCR functions must be externally
 * synchronized. If a mutex is used for external synchronization LCR callbacks
 * will be invoked with that mutex held. If a serialized dispatch queue is used
 * for synchronization LCR callbacks will be invoked on that dispatch queue.
 *
 * Reentrancy
 * ==========
 * LCR is *NOT* reentrant. It is an error to invoke an LCR function from an LCR
 * callback. Doing so will sometimes result in an assert and could potentally
 * cause a crash or other bad behavior.
 *
 * @{
 */

#pragma once

#include <cardreader/cr_comms_backend_api.h>
#include <cardreader/cr_comms_driver_api.h>
#include <cardreader/cr_comms_types.h>
#include <cardreader/cr_payment_defines.h>
#include <cardreader_shared/crs_capability.h>
#include <cardreader_shared/crs_feature_flags.h>
#include <cardreader_shared/crs_log.h>
#include <cardreader_shared/crs_securesession_utils.h>
#include <cardreader_shared/crs_spe_device.h>
#include <cardreader_shared/crs_timer.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Forward declaration for the Cardreader instance.
 */
struct cr_cardreader_t;

/**
 * @brief Possible result codes for `cr_cardreader_*()` functions.
 */
typedef enum {
    /**
     * @brief API call was successful.
     */
    CR_CARDREADER_RESULT_SUCCESS,

    /**
     * @brief Invalid parameter passed to API call.
     */
    CR_CARDREADER_RESULT_INVALID_PARAMETER,

    /**
     * @brief Memory allocation failed.
     */
    CR_CARDREADER_RESULT_OUT_OF_MEMORY,

    /**
     * @brief Cardreader instance has not been initialized.
     */
    CR_CARDREADER_RESULT_NOT_INITIALIZED,

    /**
     * @brief Cardreader instance has not been terminated.
     */
    CR_CARDREADER_RESULT_NOT_TERMINATED,

    /**
     * @brief Cardreader instance is already initialized.
     */
    CR_CARDREADER_RESULT_ALREADY_INITIALIZED,

    /**
     * @brief Endpoint has not been registered with Cardreader.
     */
    CR_CARDREADER_RESULT_ENDPOINT_NOT_REGISTERED,

    /**
     * @brief Endpoint is already registered with Cardreader.
     */
    CR_CARDREADER_RESULT_ENDPOINT_ALREADY_REGISTERED,

    /**
     * @brief Cardreader feature is not enabled.
     */
    CR_CARDREADER_RESULT_FEATURE_NOT_ENABLED,

    /**
     * @brief Cardreader feature was already enabled.
     */
    CR_CARDREADER_RESULT_FEATURE_ALREADY_ENABLED,

    /**
     * @brief Message could not be sent; throttling in effect.
     */
    CR_CARDREADER_RESULT_SEND_MSG_ENDPOINT_BACKPRESSURE,

    /**
     * @brief Attempt to send a message to a reader when there is no connection
     * present with a reader.
     */
    CR_CARDREADER_RESULT_SEND_MSG_NOT_CONNECTED,

    /**
     * @brief Un-expected error received from the communication driver.
     */
    CR_CARDREADER_RESULT_COMMS_ERROR,

    /**
     * @brief Decode of an audio packet failed.
     */
    CR_CARDREADER_RESULT_DECODE_FAILED,

    /**
     * @brief Communication protocol incompatibility.
     */
    CR_CARDREADER_RESULT_PROTOCOL_INCOMPATIBLE
} cr_cardreader_result_t;

/**
 * @brief Communication protocol version check result.
 */
typedef enum {
    /**
     * @brief Communication protocol version matches between LCR, and the
     * reader.
     */
    CR_CARDREADER_COMMS_VERSION_RESULT_OK,

    /**
     * @brief Mismatch on communication protocol versions between LCR, and the
     * reader. An update of LCR is required.
     */
    CR_CARDREADER_COMMS_VERSION_RESULT_CARDREADER_UPDATE_REQUIRED,

    /**
     * @brief Mismatch on communication protocol versions between LCR, and the
     * reader. A firmware update of the reader is required.
     */
    CR_CARDREADER_COMMS_VERSION_RESULT_FIRMWARE_UPDATE_REQUIRED,
} cr_cardreader_comms_version_result_t;

/**
 * @brief Raw audio data representation.
 */
typedef struct cr_cardreader_raw_audio_data_t {
    /**
     * @brief Number of channels used to record the audio data.
     */
    uint8_t num_channels;

    /**
     * @brief Array of recorded samples.
     */
    int16_t const *samples;

    /**
     * @brief Length of the #cr_cardreader_raw_audio_data_t::samples array.
     */
    size_t num_samples;

    /**
     * @brief Boolean indicating if samples are looped or not.
     */
    bool loop;
} cr_cardreader_raw_audio_data_t;

/**
 * @brief Raw UART data representation.
 */
typedef struct cr_cardreader_raw_uart_data_t {
    /**
     * @brief Array of bytes received over UART.
     */
    uint8_t const *data;

    /**
     * @brief Length of the #cr_cardreader_raw_uart_data_t::data array in
     * bytes.
     */
    size_t data_len;
} cr_cardreader_raw_uart_data_t;

/**
 * @brief Union for encapsulating raw data received over a defined
 * communication channel.
 */
typedef union {
    /**
     * @brief Received raw audio data.
     */
    cr_cardreader_raw_audio_data_t audio;

    /**
     * @brief Received raw UART data.
     */
    cr_cardreader_raw_uart_data_t uart;
} cr_cardreader_raw_reader_data_t;

/**
 * @brief Different communication rates for each INPUT / OUTPUT directions
 * can operate at.
 */
typedef enum {
    /**
     * @brief Unknown communication rate.
     */
    CR_CARDREADER_COMMS_RATE_UNKNOWN,

    /**
     * @brief Mono audio with Linear Feedback Shift Register (LFSR) encoding.
     */
    CR_CARDREADER_COMMS_RATE_AUDIO_LFSR_AND_HAMMING_5_BYTE_BLOCKS_MONO,

    /**
     * @brief Mono audio with manchester encoding.
     */
    CR_CARDREADER_COMMS_RATE_AUDIO_MANCHESTER_AND_HAMMING_5_BYTE_BLOCKS_MONO,

    /**
     * @brief Stereo audio with Linear Feedback Shift Register (LFSR) encoding.
     */
    CR_CARDREADER_COMMS_RATE_AUDIO_LFSR_AND_HAMMING_5_BYTE_BLOCKS_STEREO,

    /**
     * @brief Stereo audio with manchester encoding.
     */
    CR_CARDREADER_COMMS_RATE_AUDIO_MANCHESTER_AND_HAMMING_5_BYTE_BLOCKS_STEREO,

    /**
     * @brief Audio baud rate of 1500 Hz.
     */
    CR_CARDREADER_COMMS_RATE_AUDIO_1500_BAUD,

    /**
     * @brief Audio baud rate of 6000 Hz.
     */
    CR_CARDREADER_COMMS_RATE_AUDIO_6000_BAUD,
} cr_cardreader_comms_rate_t;

/**
 * @brief Callback invoked when an error occurs in LCR.
 *
 * @details The given @p endpoint_id corresponds to the `EP_*` defines for a
 * specific application endpoint as defined in `crs_app_msg_hdr_common.h`.
 *
 * @param context     User-supplied context pointer for error handling.
 * @param cardreader  Pointer to the Cardreader instance.
 * @param endpoint_id Identifier for the endpoint the error occurred on.
 * @param msg         Error message to be logged.
 */
typedef void (*cr_cardreader_error_cb_t)(void *context,
                                         struct cr_cardreader_t *cardreader,
                                         int endpoint_id,
                                         char const *msg);

/**
 * @brief Callback invoked when initial communication setup has taken place,
 * and the reader is ready for operation.
 *
 * @details The host should wait for this callback before attempting to use any
 * of the LCR features with the reader. It is an error to perform reader
 * operations before #cr_cardreader_feature_api_t::on_reader_ready() is
 * invoked. The @p reader_type indicates what type of device was connected to,
 * and should be the source of truth for device identification; the transport
 * protocol in use should *NOT* be used for device identification.
 *
 * @param impl        Pointer to the user-supplied context pointer.
 * @param cardreader  Pointer to the Cardreader instance.
 * @param reader_type Identified reader type.
 */
typedef void (*cr_cardreader_reader_ready_cb_t)(void *impl,
                                                struct cr_cardreader_t *cardreader,
                                                cr_cardreader_reader_type_t reader_type);

/**
 * @brief Callback invoked when a reader error occurs, such as an unexpected message received
 * from the reader. For example, receiving a payment result when not expected, or data
 * packets that do not look like a valid message.
 *
 * @details This will cause all features to receive an "on_reader_error" notification allowing
 * features a chance to respond (ex: invalidating securesession). The Application
 * will also receive a callback that can be used to log the error and initiate a new
 * reader connection.
 *
 * @param impl       Pointer to the user-supplied context pointer.
 * @param cardreader Pointer to the cardreader instance.
 */
typedef void (*cr_cardreader_reader_error_cb_t)(void *impl, struct cr_cardreader_t *cardreader);

/**
 * @brief Callback invoked when LCR and the reader have exchanged transport,
 * application, and endpoint version numbers.
 *
 * @details Once endpoint version numbers have been exchanged, LCR will
 * determine if a host update or firmware update is required before any more
 * meaningful communication can occur. If this is the case, then the host will
 * either need to initiate a firmware update, or prompt the user to update the
 * host application in order to pull in a newer version of LCR.
 *
 * @param impl              Pointer to the user-supplied context pointer.
 * @param cardreader        Pointer to the cardreader instance.
 * @param comms_result      Result indicating if full communication has been
 *                          established.
 * @param transport_version Transport protocol version number; see
 *                          `crs_transport.h`.
 * @param app_version       Application protocol version number; see
 *                          `crs_app_msg_hdr_common.h`.
 * @param endpoint_version  Endpoint protocol version number; see `crs_ep.h`.
 *
 * @return Enumeration value indicating next steps required:
 *  - #CR_CARDREADER_COMMS_VERSION_RESULT_OK: Full communication is established.
 *  - #CR_CARDREADER_COMMS_VERSION_RESULT_CARDREADER_UPDATE_REQUIRED: Host
 *    update is required in order to communicate with the reader (LCR is too
 *    old).
 *  - #CR_CARDREADER_COMMS_VERSION_RESULT_FIRMWARE_UPDATE_REQUIRED: Host should
 *    initiate a firmware update of the reader before attempting any other
 *    communication.
 */
typedef void (*cr_cardreader_comms_version_acquired_cb_t)(void *impl,
                                                          struct cr_cardreader_t *cardreader,
                                                          cr_cardreader_comms_version_result_t comms_result,
                                                          uint32_t transport_version,
                                                          uint32_t app_version,
                                                          uint32_t endpoint_version);

/**
 * @brief LCR feature API.
 */
typedef struct cr_cardreader_feature_api_t {
    /**
     * @brief Callback to be invoked on reader ready.
     *
     * @note This *MUST* be defined by the caller.
     */
    cr_cardreader_reader_ready_cb_t on_reader_ready;

    /**
     * @brief Callback to be invoked when there is an error in LCR.
     *
     * @note This *MUST* be defined by the caller.
     */
    cr_cardreader_reader_error_cb_t on_reader_error;

    /**
     * @brief Callback to be invoked when LCR has received the communication
     * protocol tuple from the device indicating application, endpoint, and
     * trasnport versions.
     *
     * @note This *MUST* be defined by the caller.
     */
    cr_cardreader_comms_version_acquired_cb_t on_comms_version_acquired;

    /**
     * @brief User-specified context pointer that will be passed to the feature
     * API's callbacks.
     */
    void *impl;
} cr_cardreader_feature_api_t;

/**
 * @brief Configuration definition for the Cardreader instance.
 */
typedef struct cr_cardreader_cfg_t {
    /**
     * @brief Communication driver API structure.
     *
     * @note This must have been constructed by using a Cardreader Comms Driver
     * build API; `cr_comms_driver_XXX_build_api()`.
     */
    cr_comms_driver_api_t comms_driver_api;

    /**
     * @brief Timer API callbacks.
     *
     * @note The caller must provide their own APIs for constructing and using
     * timers as LCR does not have timer capability in order to be platform
     * aganostic.
     */
    crs_timer_api_t timer_api;

    /**
     * @brief Error callback to be invoked on an error within LCR.
     *
     * @details This callback is invoked in the same circumstances as the
     * feature-specific #cr_cardreader_feature_api_t::on_reader_error(), but
     * provides more details about which feature experienced the error, and the
     * error message that occurred.
     */
    cr_cardreader_error_cb_t report_error;

    /**
     * @brief User-supplied context pointer.
     */
    void *context;

    /**
     * @brief Boolean indicating if host feature is present.
     *
     * @details If this field is set, then #cr_cardreader_cfg_t::host_feature
     * must be defined.
     */
    bool host_feature_valid;

    /**
     * @brief Optional host feature.
     */
    cr_cardreader_feature_api_t host_feature;
} cr_cardreader_cfg_t;

/**
 * @brief Allocates memory for a Cardreader instance.
 *
 * @returns Pointer to the allocated Cardreader instance.
 */
struct cr_cardreader_t *cr_cardreader_alloc(void);

/**
 * @brief Initializes a Cardreader instance with the specified configuration.
 *
 * @details The @p cardreader must have been allocated by a call to
 * #cr_cardreader_alloc().
 *
 * @param cardreader Pointer to the Cardreader instance.
 * @param cfg        Pointer to the configuration to use to instantiate the
 *                   instance.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise a result code as
 * defined in #cr_cardreader_result_t.
 *
 * @note The initialization of the instance may invoke timer callbacks, so it
 * is imperative that this function is synchronized.
 *
 * @note This function should be called before invoking other methods on the
 * Cardreader instance.
 */
cr_cardreader_result_t cr_cardreader_init(struct cr_cardreader_t *cardreader, cr_cardreader_cfg_t const *cfg);

/**
 * @brief Terminates a Cardreader instance, stopping any associated timers.
 *
 * @param cardreader Pointer to the Cardreader instance.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise a result code as
 * defined in #cr_cardreader_result_t.
 *
 * @note The instance must have been initialized by a call to
 * #cr_cardreader_init(), and not been terminated or freed since then.
 */
cr_cardreader_result_t cr_cardreader_term(struct cr_cardreader_t *cardreader);

/**
 * @brief Frees the memory for an allocated Cardreader instance.
 *
 * @param cardreader Pointer to the terminated Cardreader instance.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise a result code as
 * defined in #cr_cardreader_result_t.
 *
 * @note The instance must have been terminated by a call to
 * #cr_cardreader_term(), and not yet freed.
 */
cr_cardreader_result_t cr_cardreader_free(struct cr_cardreader_t *cardreader);

/**
 * @brief Retries the identifier for the Cardreader subsystem that can be used
 * for setting a subsystem specific log threshold.
 *
 * @return Identifier for the Cardreader level logs.
 */
crs_log_subsystem_t cr_cardreader_get_log_subsystem(void);

/**
 * @brief Sets the cryptographic API used for encrypting and decrypting
 * messages exchanged with the reader.
 *
 * @param cardreader Pointer to the Cardreader instance.
 * @param crypto_api Pointer to the crytptographic API to use.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 */
void cr_cardreader_set_crypto_api(struct cr_cardreader_t *cardreader, crs_securesession_comms_api_t const *crypto_api);

/**
 * @brief API used to notify LCR that an audio connected reader has been
 * plugged in.
 *
 * @param cardreader Pointer to the Cardreader instance.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise an error code if
 * the Cardreader instance is invalid or not initialized.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 */
cr_cardreader_result_t cr_cardreader_notify_reader_plugged(struct cr_cardreader_t *cardreader);

/**
 * @brief API used to notify LCR that an audio connected reader has been
 * unplugged.
 *
 * @param cardreader Pointer to the Cardreader instance.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise an error code if
 * the Cardreader instance is invalid or not initialized.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 */
cr_cardreader_result_t cr_cardreader_notify_reader_unplugged(struct cr_cardreader_t *cardreader);

/**
 * @brief Attempts to decode an R4 packet.
 *
 * @param      cardreader    Pointer to the Cardreader instance.
 * @param      link          Type of audio link the samples were received on.
 * @param      samples       Pointer to the received audio samples.
 * @param      num_samples   Length of the @p samples array.
 * @param[out] packet        Output pointer for the decoded packet.
 * @param[out] stats         Output pointer for the link statistics.
 * @param[out] decode_result Output pointer for the decode result.
 *
 * @return #cr_cardreader_result_t indicating:
 *  - #CR_CARDREADER_RESULT_SUCCESS on success.
 *  - #CR_CARDREADER_RESULT_DECODE_FAILED if the decode failed.
 *  - #CR_CARDREADER_RESULT_INVALID_PARAMETER if an invalid argument is given.
 *  - #CR_CARDREADER_RESULT_NOT_INITIALIZED if the Cardreader instance hst not
 *    been initialized.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 */
cr_cardreader_result_t cr_cardreader_decode_r4_packet(struct cr_cardreader_t *cardreader,
                                                      cr_comms_link_type_t link,
                                                      int16_t *samples,
                                                      size_t num_samples,
                                                      cr_comms_packet_t *packet,
                                                      cr_comms_sqlink_demod_stats_t *stats,
                                                      cr_comms_decode_result_t *decode_result);

/**
 * @brief Returns a boolean indicating if a given feature is enabled or not.
 *
 * @details This method performs a check against the registered features for
 * Cardreader instance to determine if the passed @p feature API matches any
 * of them.
 *
 * @param      cardreader     Pointer to the Cardreader instance.
 * @param      feature        Pointer to the Feature API for the feature.
 * @param[out] out_is_enabled Output pointer to store if feature is enabled or not.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise an error code if
 * the Cardreader instance is invalid or not initialized.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 *
 * @note This function is only used for testing.
 *
 * @deprecated This will be removed from the public API.
 */
cr_cardreader_result_t cr_cardreader_is_feature_enabled(struct cr_cardreader_t *cardreader,
                                                        cr_cardreader_feature_api_t const *feature,
                                                        bool *out_is_enabled);

/**
 * @brief Returns a boolean indicating if the given capability is supported by
 * the reader or not.
 *
 * @param cardreader Pointer to the Cardreader instance.
 * @param capability Capability bit for the capability being checked.
 *
 * @return `true` if capability is supported, otherwise `false`.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 *
 * @note The caller should wait until reader is ready before querying its
 * capabilities.
 */
bool cr_cardreader_capability_supported(struct cr_cardreader_t *cardreader, crs_capability_t capability);

/**
 * @brief Retrieves a list of capabilities supported by the reader.
 *
 * @param      cardreader          Pointer to the Cardreader instance.
 * @param[out] capability_list     Output pointer to store a pointer to the
 *                                 retrieved capabilities.
 * @param[out] capability_list_len Number of capabilities present in the
 *                                 @p capability_list.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 *
 * @note The caller should wait until reader is ready before querying its
 * capabilities.
 *
 * @note If the reader does not support capabilities, then the retrieved list
 * may be empty.
 */
void cr_cardreader_get_capabilities_list(struct cr_cardreader_t *cardreader,
                                         crs_capability_t **capability_list,
                                         size_t *capability_list_len);

/**
 * @brief Gets a string representation for the given communication rate.
 *
 * @param comms_rate The communication rate to retrieve the name for.
 *
 * @return String name for the given communication rate.
 *
 * @note THis method is used for logging purposes on the host.
 */
char const *cr_cardreader_get_string_for_comms_rate(cr_cardreader_comms_rate_t comms_rate);

/**
 * @brief Retrieves LCR's compiled transport, application, and endpoint
 * protocol version numbers.
 *
 * @param[out] transport_version Output pointer for LCR's transport protocol
 *                               version number.
 * @param[out] app_version       Output pointer for LCR's application protocol
 *                               version number.
 * @param[out] ep_version        Output pointer for LCR's endpoint protocol
 *                               version number.
 *
 * @note This *DOES NOT* retrieve the connected reader's protocol version
 * numbers.
 */
void cr_cardreader_get_protocol_versions(uint32_t *transport_version, uint32_t *app_version, uint32_t *ep_version);

/**
 * @brief Returns a boolean indicating if LCR has received the reader's
 * capability list.
 *
 * @param cardreader Pointer to the Cardreader instance.
 *
 * @return `true` if LCR has received the reader's capability list; otherwise
 * `false`. `false` indicates that either the list has not yet been received,
 * or the reader does not support capabilities.
 *
 * @note The Cardreader instance must have been instantiated by a call to
 * #cr_cardreader_init().
 */
bool cr_cardreader_capabilities_complete(struct cr_cardreader_t *cardreader);

/**
 * @brief Sets the value for an LCR feature flag.
 *
 * @param cardreader   Pointer to the cardreader instance.
 * @param feature_name Name of the feature the flag corresponds to.
 * @param value        Value to set for the feature flag.
 *
 * @return #CR_CARDREADER_RESULT_SUCCESS on success, otherwise a value as
 * defined in #cr_cardreader_result_t.
 *
 * @note The @p cardreader instance must have been initialized by a call to
 * #cr_cardreader_init().
 */
cr_cardreader_result_t cr_cardreader_set_lcr_feature_flag(struct cr_cardreader_t *cardreader,
                                                          const char *feature_name,
                                                          uint8_t value);

/**
 * @brief Gets the current value for a specified LCR flag.
 *
 * @param cardreader Pointer to the cardreader instance.
 * @param flag       Identifier for the LCR feature flag.
 *
 * @return Value of the LCR flag. Note that if the flag value has not been
 * set, or if the flag does not exist, then this function will always return
 * `0`.
 *
 * @note The @p cardreader instance must have been initialized by a call to
 * #cr_cardreader_init().
 */
uint8_t cr_cardreader_feature_flag_get_value(struct cr_cardreader_t *cardreader, crs_lcr_feature_flags_t flag);

/**
 * @brief Get the idle time in seconds.
 *
 * @details This is the amount of time since the last message was sent from LCR
 * to reader, excluding SecureSession renewal messages.
 *
 * @note This API is intended to be used by other LCR features and is public
 * for functional testing only.
 *
 * @param cardreader Pointer to cardreader instance.
 *
 * @return Idle time in seconds.
 *
 * @note The @p cardreader instance must have been initialized by a call to
 * #cr_cardreader_init().
 */
uint32_t cr_cardreader_get_idle_time_sec(struct cr_cardreader_t *cardreader);

/** @} */
