// Copyright (c) 2015 Square, Inc. All rights reserved.

#pragma once

#include <cardreader/cr_cardreader.h>
#include <cardreader/cr_comms_backend.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct cr_comms_backend_uart_t cr_comms_backend_uart_t;

typedef void (*cr_comms_backend_uart_send_raw_cb_t)(void * context, cr_cardreader_raw_reader_data_t const * raw_data);

typedef struct cr_comms_backend_uart_cfg_t
{
    // send_raw is invoked when LCR needs to send data to the reader.
    cr_comms_backend_uart_send_raw_cb_t send_raw_cb;
    // Timer api callbacks.
    crs_timer_api_t timer_api;
    // cb_ctx holds a user specified pointer that will be passed as the first argument to
    // callbacks
    void *cb_ctx;
} cr_comms_backend_uart_cfg_t;


cr_comms_backend_uart_t *cr_comms_backend_uart_alloc(void);
cr_comms_backend_result_t cr_comms_backend_uart_free(cr_comms_backend_uart_t *backend);

cr_comms_backend_result_t cr_comms_backend_uart_init(cr_comms_backend_uart_t *backend, cr_comms_backend_uart_cfg_t *cfg);
void cr_comms_backend_uart_shutdown(cr_comms_backend_uart_t *backend);

void cr_comms_backend_uart_build_api(cr_comms_backend_uart_t* backend, cr_comms_backend_api_t *api);

// Notify LCR that sending of data requested by send_raw_cb has completed
void cr_comms_backend_uart_notify_phy_tx_complete(cr_comms_backend_uart_t *backend);

// Process data sent by the reader to LCR.
cr_comms_backend_result_t cr_comms_backend_uart_recv_raw_from_reader(cr_comms_backend_uart_t *backend,
                                                                     cr_cardreader_raw_reader_data_t const * raw_data);
