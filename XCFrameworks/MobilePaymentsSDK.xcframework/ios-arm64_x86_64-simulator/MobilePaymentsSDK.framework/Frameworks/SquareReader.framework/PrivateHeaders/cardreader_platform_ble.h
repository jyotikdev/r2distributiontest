// Copyright 2017 Square, Inc.

#pragma once
#include <sqble_profile.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct crp_ble_t crp_ble_t;

typedef void (*crp_characteristic_data_callback_t)(void *param, att_uuid_t characteristic, void const *data, size_t length);

crp_ble_t *crp_ble_alloc(void);
bool crp_ble_init(crp_ble_t *ble);
bool crp_ble_supports_async_notify(crp_ble_t *ble);
void crp_ble_register_characteristic_data_callback(crp_ble_t *ble, crp_characteristic_data_callback_t cb, void *param);
bool crp_ble_connect_to_peripheral(crp_ble_t *ble, const char *serial, const char *address);
void crp_ble_disconnect(crp_ble_t *ble);
void crp_ble_print_peripherals(crp_ble_t *ble);
// Return true if the comms_version has been received.
// Will write the comms_version bytes to `bytes` and set `*size` to the length.
bool crp_ble_get_comms_version(crp_ble_t *ble, uint8_t *bytes, size_t *size);
bool crp_ble_write_characteristic(crp_ble_t *ble,
                                  att_uuid_t service,
                                  att_uuid_t characteristic,
                                  void const *data,
                                  size_t length);
// Read upto `length` bytes into data. Will update length to the amount of data read.
bool crp_ble_read_from_characteristic(crp_ble_t *ble,
                                      att_uuid_t service,
                                      att_uuid_t characteristic,
                                      void *data,
                                      size_t *length);
bool crp_ble_read_ack_vector_characteristic(crp_ble_t *ble,
                                            uint16_t *ack_vector);
void crp_ble_trigger_read_ack_vector(crp_ble_t *ble);
bool crp_ble_read_mtu_characteristic(crp_ble_t *ble,
                                     uint8_t *mtu);
void crp_ble_trigger_read_mtu(crp_ble_t *ble);
