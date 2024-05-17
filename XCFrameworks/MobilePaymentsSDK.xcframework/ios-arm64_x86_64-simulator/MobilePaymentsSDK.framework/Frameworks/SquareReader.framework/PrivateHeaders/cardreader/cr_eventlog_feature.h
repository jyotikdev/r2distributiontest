// Copyright (c) 2014 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief Cardreader Eventlog Feature
 */

/**
 * @defgroup lcr_eventlog_feature Eventlog Feature
 *
 * @brief Cardreader Eventlog Feature
 *
 * @details This file defines the API for the Cardreader Eventlog Feature. The
 * eventlog feature is responsible for providing an API for receiving, and
 * parsing event logs from the reader. Due to limited storage space and runtime
 * requirements, the firmware is unable to log strings as one might do on a
 * mobile device; in place of this, the firmware uses event logs, which are
 * events logged with zero or more non-zero numerical values.
 *
 * @author Rodion Steshenko <rodion@squareup.com>
 *
 * @copyright 2014 Square, Inc.
 *
 * @ingroup lcr
 * @{
 */

#pragma once

#include <cardreader_shared/crs_eventlog_utils.h>
#include <cardreader_shared/crs_log.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Enumeration of the possible result codes for the `cr_eventlog_*`
 * functions.
 */
typedef enum {
    /**
     * @brief API call was successful.
     */
    CR_EVENTLOG_RESULT_SUCCESS,

    /**
     * @brief Invalid parameter was passed to the API function.
     */
    CR_EVENTLOG_RESULT_INVALID_PARAMETER,

    /**
     * @brief Eventlog feature was not initialized before an API call was
     * attempted; #cr_eventlog_init() must be called before any other API
     * functions are called.
     */
    CR_EVENTLOG_RESULT_NOT_INITIALIZED,

    /**
     * @brief Eventlog feature was attempted to be initialized while already
     * initialized.
     */
    CR_EVENTLOG_RESULT_ALREADY_INITIALIZED,

    /**
     * @brief Freeing of the Eventlog Feature was attempted when the feature
     * has not yet been terminted; #cr_eventlog_term() must be called.
     */
    CR_EVENTLOG_RESULT_NOT_TERMINATED,

    /**
     * @brief Termination of the Eventlog Feature was attempted when the
     * feature has already been terminated.
     */
    CR_EVENTLOG_RESULT_ALREADY_TERMINATED,

    /**
     * @brief Error encountered when sending a request to the device, or if
     * there is an issue with the Cardreader instance.
     */
    CR_EVENTLOG_RESULT_SESSION_ERROR,

    /**
     * @brief Requested made during a state when the specified request is
     * invalid for the Eventlog Feature.
     */
    CR_EVENTLOG_RESULT_CALL_UNEXPECTED,
} cr_eventlog_result_t;

/**
 * @brief Forward declaration for the cardreader instance.
 */
struct cr_cardreader_t;

/**
 * @brief Forward declaration for the Eventlog Feature.
 */
struct cr_eventlog_t;

/**
 * @brief Callback invoked when events are parsed as a result of
 * #cr_eventlog_feature_get_strings_from_data().
 *
 * @details This callback is invoked when an eventlog buffer is successfully
 * decoded, and events are parsed from the buffer. Those events are passed in
 * an array of #crs_eventlog_utils_parsed_event_t instances.
 *
 * @param context     User-supplied context pointer.
 * @param events      Array of the parsed eventlog events.
 * @param num_events  Length of the @p events arrays.
 */
typedef void (*cr_eventlog_feature_parsed_events_cb_t)(void *context,
                                                       crs_eventlog_utils_parsed_event_t const *events,
                                                       size_t num_events);

/**
 * @brief Callback invoked when the reader provides eventlog data.
 *
 * @details The events encoded in the data can be decoded using the
 * #cr_eventlog_feature_get_strings_from_data() function. To ensure that the
 * buffer is complete, one should call #cr_eventlog_feature_is_valid_event().
 *
 * @param context   User-supplied context pointer.
 * @param eventlog  Pointer to the Eventlog Feature instance.
 * @param data      Pointer to the received eventlog data.
 * @param data_len  Length of the received eventlog data.
 */
typedef void (*cr_eventlog_data_cb_t)(void *context,
                                      struct cr_eventlog_t *eventlog,
                                      uint8_t const *data,
                                      size_t data_len);

/**
 * @brief Callback invoked when the reader provides a complete set of profile
 * data.
 *
 * @details Eventlog profiles provide an avenue for the firmware to profile the
 * timings of different tasks, such as the duration of a tap payment. This
 * callback is invoked when the reader is providing the host with a complete
 * set of profile data. The received profiles may be from multiple chips.
 *
 * @param context   User-supplied context pointer.
 * @param eventlog  Pointer to the Eventlog Feature instance.
 * @param data      Pointer to the received profile data.
 * @param data_len  Length of the received profile data.
 */
typedef void (*cr_eventlog_profile_cb_t)(void *context,
                                         struct cr_eventlog_t *eventlog,
                                         uint8_t const *data,
                                         size_t data_len);

/**
 * @brief Type definition for the event API outlining the callbacks that are
 * invoked by the Eventlog Feature.
 */
typedef struct cr_eventlog_event_api_t {
    /**
     * @brief Invoked when LCR receives eventlog data. This data is sent back
     * periodically by the reader and not in response to any specific public
     * LCR function.
     */
    cr_eventlog_data_cb_t on_data_received;

    /**
     * @brief Invoked when LCR receives profile data.
     */
    cr_eventlog_profile_cb_t on_profile_received;

    /**
     * @brief User specified pointer that will be passed as the first argument
     * to the registered callbacks.
     */
    void *context;
} cr_eventlog_event_api_t;

/**
 * @brief Allocates the memory for a cr_eventlog_t.
 *
 * @details The internals of the Eventlog Feature are private to the library.
 * In order to get an instance of the Eventlog Feature with the associated
 * necessary memory, this method must be called.
 *
 * @return Pointer to an allocated Eventlog Feature.
 */
struct cr_eventlog_t *cr_eventlog_alloc(void);

/**
 * @brief Initializes an Eventlog Feature instance (cr_eventlog_t) with the
 * specified callbacks, and registers the feature with the specified Cardreader
 * instance.
 *
 * @details This method initializes an instance of the Eventlog Feature with
 * the specified callbacks defined in the @p api, and registers the feature
 * with the specified @p cardreader.
 *
 * @param eventlog   Pointer to the Eventlog Feature instance.
 * @param cardreader Pointer to the Cardreader instance.
 * @param api        Pointer to the callback API.
 *
 * @return #CR_EVENTLOG_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_eventlog_result_t.
 *
 * @note The Eventlog Feature instance must have been allocated by a call to
 * #cr_eventlog_alloc().
 *
 * @note This method must be called before calling other API functions on an
 * Eventlog Feature.
 */
cr_eventlog_result_t cr_eventlog_init(struct cr_eventlog_t *eventlog,
                                      struct cr_cardreader_t *cardreader,
                                      cr_eventlog_event_api_t const *api);

/**
 * @brief Terminates an Eventlog Feature instance instantiated by a call to
 * #cr_eventlog_init(), and de-registers the feature from Cardreader.
 *
 * @param eventlog Pointer to the Eventlog Feature instance.
 *
 * @return #CR_EVENTLOG_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_eventlog_result_t.
 *
 * @note The Eventlog Feature instance must have been initialized by a call to
 * #cr_eventlog_init().
 *
 * @note The Eventlog Feature instance must be terminated before freeing the
 * associated memory by calling #cr_eventlog_free().
 */
cr_eventlog_result_t cr_eventlog_term(struct cr_eventlog_t *eventlog);

/**
 * @brief Frees an instanc eof a cr_eventlog_t that was allocated by a call to
 * #cr_eventlog_alloc().
 *
 * @param eventlog Pointer to the Eventlog Feature instance.
 *
 * @return #CR_EVENTLOG_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_eventlog_result_t.
 *
 * @note The Eventlog Feature instance must have been previously terminated by
 * a call to #cr_eventlog_term().
 */
cr_eventlog_result_t cr_eventlog_free(struct cr_eventlog_t *eventlog);

/**
 * @brief Retrieves the identifier for the eventlog subsystem that can be used
 * for setting a subsystem specific log threshold.
 *
 * @return crs_log_subsystem_t identifier.
 */
crs_log_subsystem_t cr_eventlog_get_log_subsystem(void);

/**
 * @brief Returns a boolean indicating whether the connected SPE device encodes
 * the number of parameters in its event logs.
 *
 * @param eventlog Pointer to the Eventlog Feature instance.
 *
 * @return `true` if the connected SPE device uses `num_param` eventlog
 * encoding.
 */
bool cr_eventlog_uses_num_param_encoding(struct cr_eventlog_t *eventlog);

/**
 * @brief Parses event log data from an eventlog data buffer, generating an
 * array of parsed events, #crs_eventlog_utils_parsed_event_t, which is passed
 * to the passed callback.
 *
 * @details This method is used to parse event log data in order to invoke a
 * callback with the stringified events for logging. In order to determine
 * whether the buffer is complete, one should call
 * #cr_eventlog_feature_is_valid_event() before invoking this method. If the
 * buffer is complete, then one can call this method, otherwise, one should
 * wait for the remaining data before invoking this method.
 *
 * @param context                   User-supplied context pointer.
 * @param eventlog                  Pointer to the Eventlog Feature instance.
 * @param eventlog_parsed_events_cb Callback to invoke with the parsed events.
 * @param parse_buf                 Buffer of received eventlog data.
 * @param buf_len                   Length of the @p parse_buf in bytes.
 *
 * @return Length of the passed @p parse_buf in bytes.
 */
uint32_t cr_eventlog_feature_get_strings_from_data(void *context,
                                                   struct cr_eventlog_t *eventlog,
                                                   cr_eventlog_feature_parsed_events_cb_t eventlog_parsed_events_cb,
                                                   uint8_t const *parse_buf,
                                                   size_t buf_len);

/**
 * @brief This function is used to determine whether a buffer has a complete
 * set of eventlog data.
 *
 * @details If the buffer contains a complete set of eventlog data, then
 * #cr_eventlog_feature_get_strings_from_data() can be called with the buffer
 * in order to parse the events for stringification.
 *
 * @param eventlog Pointer to the Eventlog Feature instance.
 * @param data     Pointer to the eventlog data buffer.
 * @param data_len Length of the @p data buffer in bytes.
 *
 * @return #CRS_EVENTLOG_UTILS_BUFFER_STATE_VALID if the buffer contains
 * complete eventlog data; otherwise
 * #CRS_EVENTLOG_UTILS_BUFFER_STATE_NEED_MORE_DATA if more data is required to
 * complete the buffer for parsing.
 */
crs_eventlog_utils_buffer_state_t cr_eventlog_feature_is_valid_event(struct cr_eventlog_t *eventlog,
                                                                     uint8_t const *data,
                                                                     int data_len);

/** @} */
