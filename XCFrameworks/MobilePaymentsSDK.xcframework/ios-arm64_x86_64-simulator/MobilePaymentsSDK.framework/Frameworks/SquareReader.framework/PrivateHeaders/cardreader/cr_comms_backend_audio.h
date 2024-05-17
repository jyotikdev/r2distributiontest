// Copyright (c) 2015 Square, Inc. All rights reserved.

#pragma once

#include <cardreader/cr_cardreader.h>
#include <cardreader/cr_comms_backend.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct cr_comms_backend_audio_t cr_comms_backend_audio_t;

typedef void (*cr_comms_backend_audio_send_raw_cb_t)(void * context, cr_cardreader_raw_reader_data_t const * raw_data);
typedef void (*cr_comms_backend_audio_legacy_swipe_cb_t)(void * context, cr_comms_event_t event, cr_comms_event_data_t * event_data);
typedef void (*cr_comms_backend_comms_rate_cb_t)(void * context, cr_cardreader_comms_rate_t in_comms_rate, cr_cardreader_comms_rate_t out_comms_rate);
typedef void (*cr_comms_backend_audio_connection_timeout_cb_t)(void * context);

typedef struct cr_comms_backend_audio_cfg_t
{
    // send_raw is invoked when LCR needs to send data to the reader.
    cr_comms_backend_audio_send_raw_cb_t send_raw_cb;
    // legacy_swipe is invoked when a non R6 audio packet is received
    cr_comms_backend_audio_legacy_swipe_cb_t legacy_swipe_cb;
    // connection_timeout is invoked when connection to an R6 has failed
    cr_comms_backend_audio_connection_timeout_cb_t connection_timeout_cb;
    // Invoked when the phy/link comms rate changes.
    cr_comms_backend_comms_rate_cb_t comms_rate_cb;
    // The input sample rate of the data from the mic.
    uint32_t audio_input_sample_rate_hz;
    // The output sample rate of the data on the L/R channels.
    uint32_t audio_output_sample_rate_hz;
    // Timer api callbacks.
    crs_timer_api_t timer_api;
    // For debug only, prevent rate negotiation.
    bool debug_prevent_rate_negotiation;
    // cb_ctx holds a user specified pointer that will be passed as the first argument to
    // callbacks
    void *cb_ctx;
} cr_comms_backend_audio_cfg_t;


cr_comms_backend_audio_t *cr_comms_backend_audio_alloc(void);
cr_comms_backend_result_t cr_comms_backend_audio_free(cr_comms_backend_audio_t *backend);

cr_comms_backend_result_t cr_comms_backend_audio_init(cr_comms_backend_audio_t *backend, cr_comms_backend_audio_cfg_t *cfg);
void cr_comms_backend_audio_shutdown(cr_comms_backend_audio_t *backend);

void cr_comms_backend_audio_build_api(cr_comms_backend_audio_t* backend, cr_comms_backend_api_t *api);

// Notify LCR that sending of data requested by send_raw_cb has completed
void cr_comms_backend_audio_notify_phy_tx_complete(cr_comms_backend_audio_t *backend);

// Sets the legacy reader type
void cr_comms_backend_audio_set_legacy_reader_type(cr_comms_backend_audio_t * backend,
                                                   cr_cardreader_reader_type_t reader_type);

// Process data sent by the reader to LCR.
cr_comms_backend_result_t cr_comms_backend_audio_recv_raw_from_reader(cr_comms_backend_audio_t *backend,
                                                                      cr_cardreader_raw_reader_data_t const * raw_data);

// Wake up reader by playing wakeup tone. NOTE: This is a temporary API allowing for manual
// triggering of the wakeup tone for testing purposes only.
void cr_comms_backend_audio_wake_reader(cr_comms_backend_audio_t *backend);

// Enable LCR to send data over audio for the current connection (until the reader is unplugged).
void cr_comms_backend_audio_enable_tx_for_connection(cr_comms_backend_audio_t *backend);
