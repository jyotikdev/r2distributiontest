// Copyright (c) 2014 Square, Inc. All rights reserved.

#pragma once

#include <cardreader_shared/crs_log.h>

#include <cardreader_shared/crs_pack.h>
#include <cardreader_shared/crs_coredump.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Possible result codes for cr_coredump_* functions.
typedef enum
{
    CR_COREDUMP_RESULT_SUCCESS,
    CR_COREDUMP_RESULT_ERROR0,
    CR_COREDUMP_RESULT_INVALID_PARAMETER,
    CR_COREDUMP_RESULT_SESSION_ERROR,
    CR_COREDUMP_RESULT_NOT_INITIALIZED,
    CR_COREDUMP_RESULT_ALREADY_TERMINATED,
    CR_COREDUMP_RESULT_NOT_TERMINATED,
    CR_COREDUMP_RESULT_CALL_UNEXPECTED,
} cr_coredump_result_t;

struct cr_coredump_t;
struct cr_cardreader_t;

// received_coredump_info is invoked when LCR receives coredump information from the reader in
// response to cr_coredump_get_info.
typedef void (*cr_coredump_received_coredump_info_cb_t)(void *context,
                                                        struct cr_coredump_t *coredump,
                                                        bool coredump_available);

// received_coredump_data is invoked when LCR receives coredump data from the reader in
// response to cr_coredump_get_data. The key and data must both be stored in order to interpret
// the coredump.
typedef void (*cr_coredump_received_coredump_data_cb_t)(void *context,
                                                        struct cr_coredump_t *coredump,
                                                        uint8_t *const key_buf,
                                                        size_t key_buf_size,
                                                        uint8_t *const data_buf,
                                                        size_t data_buf_size);

// received_coredump_erased is invoked when LCR receives notification that the coredump has
// been erased in response to cr_coredump_erase.
typedef void (*cr_coredump_received_coredump_erased_cb_t)(void *context, struct cr_coredump_t *coredump);

// received_coredump_triggered is invoked when LCR receives notification that a request for
// a coredump trigger was received by the reader. `was_triggered` indicates if the coredump
// trigger was successful (it will fail in the case of a production reader)
typedef void (*cr_coredump_received_coredump_triggered_cb_t)(void *context,
                                                             struct cr_coredump_t *coredump,
                                                             crs_coredump_trigger_result_t was_triggered);

// receive_data_progress is invoked 1 or more times when LCR is receiving coredump data in
// response to cr_coredump_get_data.
typedef void (*cr_coredump_receive_data_progress_cb_t)(void *context,
                                                       struct cr_coredump_t *coredump,
                                                       uint32_t bytes_received,
                                                       uint32_t total_received,
                                                       uint32_t bytes_total);

typedef struct cr_coredump_event_api_t
{
    cr_coredump_received_coredump_info_cb_t received_coredump_info;
    cr_coredump_received_coredump_data_cb_t received_coredump_data;
    cr_coredump_received_coredump_erased_cb_t received_coredump_erased;
    cr_coredump_received_coredump_triggered_cb_t received_coredump_triggered;
    cr_coredump_receive_data_progress_cb_t receive_data_progress;
    // context holds a user specified pointer that will be passed as the first argument to the
    // above callbacks.
    void *context;
} cr_coredump_event_api_t;

// Allocates the memory for a cr_coredump_t.
struct cr_coredump_t *cr_coredump_alloc(void);

// Initializes a cr_cordump_t with the specified callbacks and registers the feature with the
// specified cardreader.
cr_coredump_result_t cr_coredump_init(struct cr_coredump_t *coredump,
                                      struct cr_cardreader_t *cardreader,
                                      cr_coredump_event_api_t const *api);

// Terminates a cr_coredump_t, deregistering the feature.
cr_coredump_result_t cr_coredump_term(struct cr_coredump_t *coredump);

// Frees the memory for a cr_coredump_t. The cr_coredump_t must have been previously terminated.
cr_coredump_result_t cr_coredump_free(struct cr_coredump_t *coredump);

// Retrieve information about coredump status of a reader. A call will be made (asynchronously) to
// cr_coredump_event_api_t::received_coredump_info indicating whether the R6 has a coredump
// available.
cr_coredump_result_t cr_coredump_get_info(struct cr_coredump_t *coredump);

// Retrieve the coredump from an R6. Retrieval progress is reported via (asynchronous) calls to
// cr_coredump_event_api_t::receve_data_progress which will be called 1 or more times, after which
// a call to cr_coredump_event_api_t::received_coredump_data with filled key and data buffers.
// Returns CR_COREDUMP_RESULT_CALL_UNEXPECTED if cr_coredump_get_info has not been called.
cr_coredump_result_t cr_coredump_get_data(struct cr_coredump_t *coredump);

// Erase the coredump on the R6, if it exists. A call will be made (asynchronously) to
// cr_coredump_event_api_t::received_coredump_erase.
cr_coredump_result_t cr_coredump_erase(struct cr_coredump_t *coredump);

// Testing support functionality to force coredump creation on the R6. A call will be made
// (asynchronously) to cr_coredump_event_api_t::received_coredump_triggered.
cr_coredump_result_t cr_coredump_trigger_dump(struct cr_coredump_t *coredump, crs_coredump_trigger_target_t target);

// Retrieves the identifier for the coredump subsystem that can be used for setting a subsystem
// specific log threshold.
crs_log_subsystem_t cr_coredump_get_log_subsystem(void);
