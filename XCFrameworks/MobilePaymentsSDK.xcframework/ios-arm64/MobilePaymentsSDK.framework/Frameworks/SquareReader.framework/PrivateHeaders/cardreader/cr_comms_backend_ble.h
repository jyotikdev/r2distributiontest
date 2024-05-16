// Copyright (c) 2015 Square, Inc. All rights reserved.

#pragma once

#include <cardreader/cr_cardreader.h>
#include <cardreader/cr_comms_backend.h>

#include <stdint.h>
#include <stddef.h>

// 128bit UUIDs for our service and read/write characteristics
// Of the form 0000-XXXX-0000-1000-8000-0080-5F9B-34FB
typedef uint8_t cr_comms_backend_ble_uuid_t[16];

extern const cr_comms_backend_ble_uuid_t cr_comms_backend_ble_uuid_service_cardreader;
extern const cr_comms_backend_ble_uuid_t cr_comms_backend_ble_uuid_characteristic_write;
extern const cr_comms_backend_ble_uuid_t cr_comms_backend_ble_uuid_characteristic_read;
extern const cr_comms_backend_ble_uuid_t cr_comms_backend_ble_uuid_characteristic_ack_vector;
extern const cr_comms_backend_ble_uuid_t cr_comms_backend_ble_uuid_characteristic_mtu;

typedef struct cr_comms_backend_ble_t cr_comms_backend_ble_t;

typedef void (*cr_comms_backend_ble_write_to_characteristic_cb_t)(void *context,
                                                                  const cr_comms_backend_ble_uuid_t characteristic,
                                                                  void const *data,
                                                                  size_t length);

typedef void (*cr_comms_backend_ble_read_from_characteristic_cb_t)(void *context,
                                                                   const cr_comms_backend_ble_uuid_t characteristic);

typedef struct cr_comms_backend_ble_cfg_t
{
    // Send data to reader via a BLE service/characteristic
    cr_comms_backend_ble_write_to_characteristic_cb_t write_to_characteristic_cb;
    // Read data from a characteristic
    cr_comms_backend_ble_read_from_characteristic_cb_t read_from_characteristic_cb;
    // cb_ctx holds a user specified pointer that will be passed as the first argument to callbacks
    void *cb_ctx;
} cr_comms_backend_ble_cfg_t;

cr_comms_backend_ble_t *cr_comms_backend_ble_alloc(void);
cr_comms_backend_result_t cr_comms_backend_ble_free(cr_comms_backend_ble_t *backend);

// Initialize a ble backend.
// `comms_version_bytes` Bytes received from comms_version characterisitic.
cr_comms_backend_result_t cr_comms_backend_ble_init(cr_comms_backend_ble_t *backend,
                                                    cr_comms_backend_ble_cfg_t *cfg,
                                                    uint8_t const *comms_version_bytes,
                                                    size_t const comms_version_bytes_size);

void cr_comms_backend_ble_shutdown(cr_comms_backend_ble_t *backend);

void cr_comms_backend_ble_build_api(cr_comms_backend_ble_t* backend, cr_comms_backend_api_t *api);

// Notify the backend of data received from the reader (via the a service/characteristic)
cr_comms_backend_result_t cr_comms_backend_ble_received_data_from_characteristic(cr_comms_backend_ble_t *backend,
                                                                                 const cr_comms_backend_ble_uuid_t characteristic,
                                                                                 void const *data,
                                                                                 size_t length);
