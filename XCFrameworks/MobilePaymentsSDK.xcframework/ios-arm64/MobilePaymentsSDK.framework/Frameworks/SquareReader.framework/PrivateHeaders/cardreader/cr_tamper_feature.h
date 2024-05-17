// Copyright (c) 2014 Square, Inc. All rights reserved.

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Possible result codes for cr_tamper_* functions.
typedef enum
{
    CR_TAMPER_RESULT_SUCCESS,
    CR_TAMPER_RESULT_INVALID_PARAMETER = 1,
    CR_TAMPER_RESULT_NOT_INITIALIZED = 2,
    CR_TAMPER_RESULT_ALREADY_INITIALIZED = 3,
    CR_TAMPER_RESULT_NOT_TERMINATED = 4,
    CR_TAMPER_RESULT_ALREADY_TERMINATED = 5,
    CR_TAMPER_RESULT_SESSION_ERROR = 7,
    CR_TAMPER_RESULT_CALL_UNEXPECTED = 8,
    CR_TAMPER_RESULT_FATAL = 9,
} cr_tamper_result_t;

typedef enum
{
    /// Tamper result not known.
    CR_TAMPER_STATUS_UNKNOWN,
    /// Device is not tampered.
    CR_TAMPER_STATUS_NORMAL,
    /// Device is tampered and should not be used.
    CR_TAMPER_STATUS_TAMPERED,
    /// Device is non-lethally tampered and may still be used.
    CR_TAMPER_STATUS_FLAGGED,
} cr_tamper_status_t;

struct cr_tamper_t;
struct cr_cardreader_t;

// Callback invoked when the reader provides tamper data. The tamper_status parameter is a pointer
// to a crs_tamper_data_t struct. sizeof(crs_tamper_data_t) == len.
typedef void (*cr_tamper_data_cb_t)(void *context,
                                    struct cr_tamper_t *tamper,
                                    void const *tamper_status,
                                    size_t tamper_status_len);

// Callback invoked when the reader provides tamper status.
// See the tamper.proto definitions for the meaning of each value unit_status can take on
typedef void (*cr_tamper_status_cb_t)(void *context, struct cr_tamper_t *tamper, cr_tamper_status_t const unit_status);

typedef void (*cr_tamper_start_tamperscope_ack_cb_t)(void *context);
typedef void (*cr_tamper_tamperscope_data_cb_t)(void *context, void const *buf, size_t buf_len);

typedef void (*cr_tamper_num_events_cb_t)(void *context, uint32_t count);

typedef void (*cr_tamper_tag_reset_cb_t)(void *context);

typedef void (*cr_tamper_cumulative_reset_cb_t)(void *context);

typedef struct cr_tamper_event_api_t
{
    // on_tamper_status_response is invoked when LCR receives tamper status in response to a call
    // to cr_tamper_get_tamper_status.
    cr_tamper_status_cb_t on_tamper_status_response;
    // on_tamper_data_response is invoked when LCR receives tamper data in response to a call to
    // cr_tamper_get_tamper_data.
    cr_tamper_data_cb_t on_tamper_data_response;
    cr_tamper_start_tamperscope_ack_cb_t on_tamper_start_tamperscope_ack;
    cr_tamper_tamperscope_data_cb_t on_tamperscope_data_cb;
    cr_tamper_num_events_cb_t on_num_events_cb;
    cr_tamper_tag_reset_cb_t on_tamper_tag_reset_cb;
    cr_tamper_cumulative_reset_cb_t on_tamper_cumulative_reset_cb;
    // context holds a user specified pointer that will be passed as the first argument to the
    // above callbacks.
    void *context;
} cr_tamper_event_api_t;

// Allocates the memory for a cr_tamper_t.
struct cr_tamper_t *cr_tamper_alloc(void);

// Initializes a cr_tamper_t with the specified callbacks and registers the feature with the
// specified cardreader.
cr_tamper_result_t cr_tamper_init(struct cr_tamper_t *tamper,
                                  struct cr_cardreader_t *cardreader,
                                  cr_tamper_event_api_t const *event_api);

// Terminates a cr_tamper_t, deregistering the feature.
cr_tamper_result_t cr_tamper_term(struct cr_tamper_t *tamper);

// Frees the memory for a cr_tamper_t. The cr_tamper_t must have been previously terminated.
cr_tamper_result_t cr_tamper_free(struct cr_tamper_t *tamper);

// A status request returns only whether or not the reader is "damaged"
// A request for "tamper data" returns full logging information that can be
// used to diagnose the cause, time, and other information about the tamper
//
// The distinction is made in order to limit data traffic on initial plugin

// Retrieves the reader tamper status. A call will be made (asynchronously) to
// cr_tamper_event_api_t::on_tamper_status_response. The tamper status is smaller and faster to
// retrieve than the tamper data.
cr_tamper_result_t cr_tamper_get_tamper_status(struct cr_tamper_t *tamper);

// Retrieves the reader tamper data. A call will be made (asynchronously) to
// cr_tamper_event_api_t::on_tamper_data_response. The tamper data is larger and slower to retrieve
// than the tamper status and more complex to interpret.
cr_tamper_result_t cr_tamper_get_tamper_data(struct cr_tamper_t *tamper);

// This is exposed so that the host can acknowledge events reported by "FLAGGED" readers
//
// The effect of this command on FLAGGED readers is to clear the stored "first" tamper
// event.  A persistently tampered reader may continue to report events until it reaches
// its maximum tamper events counter.
//
// This command allows us to limit the noise and the number of rows in Tarkin's database.
cr_tamper_result_t cr_tamper_reset_tag(struct cr_tamper_t *tamper);
cr_tamper_result_t cr_tamper_reset_cumulative(struct cr_tamper_t *tamper);
cr_tamper_result_t cr_tamper_start_tamperscope(struct cr_tamper_t *tamper);
cr_tamper_result_t cr_tamper_clear_tamperscope(struct cr_tamper_t *tamper);
cr_tamper_result_t cr_tamper_fetch_tamperscope(struct cr_tamper_t *tamper);

cr_tamper_result_t cr_tamper_get_num_events(struct cr_tamper_t *tamper);
