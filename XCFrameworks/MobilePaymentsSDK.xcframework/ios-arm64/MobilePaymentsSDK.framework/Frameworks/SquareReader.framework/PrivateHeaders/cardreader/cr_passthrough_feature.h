// Copyright (c) 2016 Square, Inc. All rights reserved.

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum
{
    CR_PASSTHROUGH_RESULT_SUCCESS,
    CR_PASSTHROUGH_RESULT_INVALID_PARAMETER = 1,
    CR_PASSTHROUGH_RESULT_SESSION_ERROR = 2,
    CR_PASSTHROUGH_RESULT_NOT_INITIALIZED = 3,
} cr_passthrough_result_t;

struct cr_passthrough_t;
struct cr_cardreader_t;

typedef void (*cr_passthrough_proto_recv_cb_t)(void *context,
                                               struct cr_passthrough_t *passthrough,
                                               uint8_t const *data,
                                               size_t data_length);

typedef void (*cr_passthrough_proto_send_complete_cb_t)(void *context, struct cr_passthrough_t *passthrough);

typedef struct cr_passthrough_event_api_t
{
    // on_receive_proto is invoked when LCR receives a passed-through protobuf message from
    // K400. `data` contains the encoded protobuf message.
    cr_passthrough_proto_recv_cb_t on_receive_proto;
    // on_send_proto_complete is invoked when LCR has completed sending a protobuf message to
    // K400 (via a `cr_passthrough_send_proto` call).
    cr_passthrough_proto_send_complete_cb_t on_send_proto_complete;
    // context holds a user specified pointer that will be passed as the first argument to the
    // above callbacks.
    void *context;
} cr_passthrough_event_api_t;

// Allocates the memory for a cr_passthrough_t.
struct cr_passthrough_t *cr_passthrough_alloc(void);

// Initializes a cr_passthrough_t with the specified callbacks and registers the feature with the
// specified cardreader.
cr_passthrough_result_t cr_passthrough_init(struct cr_passthrough_t *passthrough,
                                            struct cr_cardreader_t *cardreader,
                                            cr_passthrough_event_api_t const *event_api);

// Terminates a cr_passthrough_t, deregistering the feature.
cr_passthrough_result_t cr_passthrough_term(struct cr_passthrough_t *passthrough);

// Frees the memory for a cr_passthrough_t. The cr_passthrough_t must have been previously terminated.
cr_passthrough_result_t cr_passthrough_free(struct cr_passthrough_t *passthrough);

// Send an encoded protobuf message to the reader passthrough feature on the reader.
// The protobuf will be passed through to the K400
cr_passthrough_result_t cr_passthrough_send_proto(struct cr_passthrough_t *passthrough,
                                                  uint8_t *data,
                                                  size_t data_length);
