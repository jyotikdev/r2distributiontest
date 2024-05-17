// Copyright (c) 2015 Square, Inc. All rights reserved.

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * The comms backend API is the interface between a backend and the
 * cr_comms_driver_t. The backend initialises this structure which is then
 * used by the comms driver to call into the backend.
 *
 * Once a connection with the reader has been established, the backend should
 * call cr_comms_driver_on_connection.
 *
 * When the backend wishes to process data through the comms stack (in either
 * direction) it should call cr_comms_driver_drain.
 **/

// Initialise the backend
typedef void (*cr_comms_backend_setup_t)(void *context, void *comms_driver);
// Request whether a transmission is currently in progress
typedef bool (*cr_comms_backend_tx_in_progress_t)(void *context);
// Request the amount of free space for enqueueing outgoing messages
typedef size_t (*cr_comms_backend_get_free_tx_space_t)(void *context);
// Notify the backend of new outgoing message data. The comms driver will
// not enqueue more data than the value returned by the
// cr_comms_backend_get_free_tx_space_t callback
typedef void (*cr_comms_backend_send_enqueue_data_t)(void *context,
                                                     void const *buf,
                                                     size_t len);
// Notify the backend that it should drain incoming messages (from the
// reader into the comms driver). The backend should call
// cr_comms_driver_recv_msg_fragment and cr_comms_driver_drain_app_rx when
// it has data to pass up.
typedef bool (*cr_comms_backend_drain_rx_t)(void *context);
// Notify the backend that it should drain outgoing messages (from the
// comms driver to the reader). The backend will be fed messages via the
// cr_comms_backend_send_enqueue_data_t callback.
typedef bool (*cr_comms_backend_drain_tx_t)(void *context);
// Notify the backend that a reader was connected
typedef void (*cr_comms_backend_reader_attach_t)(void *context);
// Notify the backend that a reader was disconnected
typedef void (*cr_comms_backend_reader_detach_t)(void *context);
// Request whether the backend supports a specified transport version
// returns true if the version passed in is supported by this backend
typedef bool (*cr_comms_backend_version_supported_t)(void *context,
                                                     uint8_t version);
// Request that the backend complete the connection process
typedef void (*cr_comms_backend_complete_connection_process_t) (void *context);

typedef struct cr_comms_backend_api_t
{
    cr_comms_backend_setup_t setup;
    cr_comms_backend_tx_in_progress_t tx_in_progress;
    cr_comms_backend_get_free_tx_space_t get_free_tx_space;
    cr_comms_backend_send_enqueue_data_t send_enqueue_data;
    cr_comms_backend_drain_rx_t drain_rx;
    cr_comms_backend_drain_tx_t drain_tx;
    cr_comms_backend_reader_attach_t reader_attach;
    cr_comms_backend_reader_detach_t reader_detach;
    cr_comms_backend_version_supported_t version_supported;
    cr_comms_backend_complete_connection_process_t complete_connection_process;

    void *impl;
} cr_comms_backend_api_t;
