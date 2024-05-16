// Copyright 2016 Square, Inc.

#pragma once

#include <cardreader/cr_cardreader.h>
#include <cardreader/cr_comms_backend.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct cr_comms_backend_arq_t cr_comms_backend_arq_t;

typedef void (*cr_comms_backend_arq_send_data_to_reader_cb_t)(void * context, void const *data, size_t length);

typedef struct cr_comms_backend_arq_cfg_t
{
    // send_raw_data_to_reader_cb is invoked when LCR needs to send data to the reader.
    cr_comms_backend_arq_send_data_to_reader_cb_t send_raw_data_to_reader_cb;
    // Timer api callbacks.
    crs_timer_api_t timer_api;
    // cb_ctx holds a user specified pointer that will be passed as the first argument to
    // callbacks
    void *cb_ctx;
} cr_comms_backend_arq_cfg_t;


cr_comms_backend_arq_t *cr_comms_backend_arq_alloc(void);
cr_comms_backend_result_t cr_comms_backend_arq_free(cr_comms_backend_arq_t *backend);

cr_comms_backend_result_t cr_comms_backend_arq_init(cr_comms_backend_arq_t *backend, cr_comms_backend_arq_cfg_t *cfg);
void cr_comms_backend_arq_shutdown(cr_comms_backend_arq_t *backend);

void cr_comms_backend_arq_build_api(cr_comms_backend_arq_t* backend, cr_comms_backend_api_t *api);

// Process data sent by the reader to LCR.
cr_comms_backend_result_t cr_comms_backend_arq_recv_raw_data_from_reader(cr_comms_backend_arq_t *backend,
                                                                         void const *data,
                                                                         size_t length);
