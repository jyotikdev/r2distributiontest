// Copyright 2017 Square, Inc.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct crp_ble_t crp_ble_t;

crp_ble_t *crp_ble_alloc(void);
bool crp_ble_init(crp_ble_t *ble);
bool crp_ble_connect_to_peripheral(crp_ble_t *ble, const char serial[4], const char address[18]);
void crp_ble_disconnect(crp_ble_t * ble);
void crp_ble_print_peripherals(crp_ble_t *ble);
uint8_t crp_ble_get_mtu(crp_ble_t *ble);
bool crp_ble_get_comms_version(crp_ble_t *ble, uint8_t *bytes, size_t *size);
bool crp_ble_write_characteristic(crp_ble_t *ble,
                                  const uint8_t service[16],
                                  const uint8_t characteristic[16],
                                  void const *data,
                                  size_t length);
bool crp_ble_read_from_characteristic(crp_ble_t *ble,
                                      const uint8_t service[16],
                                      const uint8_t characteristic[16],
                                      void *data,
                                      size_t *length);
bool crp_ble_read_ack_vector_characteristic(crp_ble_t *ble,
                                            uint16_t *ack_vector);
void crp_ble_trigger_read_ack_vector(crp_ble_t *ble);
bool crp_ble_read_mtu_characteristic(crp_ble_t *ble,
                                     uint8_t *mtu);
void crp_ble_trigger_read_mtu(crp_ble_t *ble);
