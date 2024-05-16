// Copyright 2020 Square, Inc.

#pragma once

#include <cardreader/cr_comms_driver_api.h>
#include <cardreader_shared/crs_key_injector_defs.h>
#include <cardreader_shared/crs_factory_msgs.h>

typedef struct cardreader_ecr_t cardreader_ecr_t;

typedef void (*cardreader_ecr_on_callback_recvd_cb_t)(void *context);

typedef struct {
    // Called from the RECEIVE thread when a callback message has been received
    // from the server. The host should signal the LCR MAIN thread to call
    // cardreader_ecr_process_callbacks() as soon as possible.
    cardreader_ecr_on_callback_recvd_cb_t on_ecr_callback_recvd;
    const uint8_t *tms_data;
    size_t tms_data_len;
    const char* tms_name;
    uint32_t firmware_version;
    uint8_t hwid[HWID_LEN];
    char hwsn[CRS_FACTORY_MAX_SYSCFG_STRING_SIZE + 1]; // plus null
    void *context;
} cardreader_ecr_cfg_t;

// Create a new cardreader ecr
cardreader_ecr_t *cardreader_ecr_create(cardreader_ecr_cfg_t const *cfg);

// Builds a cr_comms_driver API structure to be passed into a cr_cardreader instance
cr_comms_driver_api_t cardreader_ecr_build_comms_driver_api(cardreader_ecr_t *ecr);

// Connect to a cardreader ecr
bool cardreader_ecr_connect(cardreader_ecr_t *ecr);

// Called to process queued callback messages
// Do not call from the ecr receive thread - may cause deadlock.
void cardreader_ecr_process_callbacks(cardreader_ecr_t *ecr);
