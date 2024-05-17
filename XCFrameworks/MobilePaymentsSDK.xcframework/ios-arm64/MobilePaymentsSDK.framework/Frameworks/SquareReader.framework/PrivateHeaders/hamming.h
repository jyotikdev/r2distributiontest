// Copyright 2017 Square, Inc.

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define HAMMING_MAX_DATA_TO_ENCODE_LENGTH (30)
#define HAMMING_MAX_ENCODED_DATA_LENGTH (31)

size_t hamming_calc_encoded_length(size_t unencoded_length);

size_t hamming_calc_unencoded_length(size_t encoded_length);

size_t hamming_encode_block(uint8_t const *unencoded_data,
                            size_t unencoded_data_length,
                            uint8_t *encoded_data,
                            size_t encoded_data_length);

size_t hamming_decode_block(uint8_t const *encoded_data,
                            size_t encoded_data_length,
                            uint8_t *decoded_data,
                            size_t decoded_data_length);

bool hamming_generate_parity_bits(uint8_t const *unencoded_data,
                                  size_t unencoded_data_length,
                                  uint8_t *out_parity_bits);

bool hamming_correct_data(uint8_t *unencoded_data,
                          size_t unencoded_data_length,
                          uint8_t parity_bits);
