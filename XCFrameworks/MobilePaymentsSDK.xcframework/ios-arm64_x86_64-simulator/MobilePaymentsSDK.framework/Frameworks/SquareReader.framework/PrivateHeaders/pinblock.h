// Copyright 2017 Square, Inc.

#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    PINBLOCK_OK = 0,
    PINBLOCK_ERROR,
    PINBLOCK_INVALID_PIN,
    PINBLOCK_INVALID_PAN,
    PINBLOCK_INVALID_FORMAT,
} pinblock_result_t;

#define PIN_MIN_DIGITS              (4)
#define PIN_MAX_DIGITS              (12)
#define PINBLOCK_MIN_PAN_LENGTH     (13)
#define PINBLOCK_MAX_PAN_LENGTH     (19)
#define PINBLOCK_DATA_LENGTH        (7)
#define PINBLOCK_FORMAT4_DATA_LENGTH (15)
#define PINBLOCK_HEADER(_format, _pin_len) (((_format) << 4) | ((_pin_len) & 0x0f))
#define PINBLOCK_HEADER_GET_FORMAT(_pinblock_header) ((uint8_t)(((_pinblock_header) & 0xf0) >> 4))
#define PINBLOCK_HEADER_GET_LENGTH(_pinblock_header) ((_pinblock_header) & 0x0f)
#define PINBLOCK_DIGITS(_header) ((int)((_header) & 0x0f))
#define PIN_FORMAT4_PSEUDOPANBLOCK_LEN (16)
#define PINBLOCK_FORMAT4_FILL_BYTE  (0xAA)

typedef enum {
    PINBLOCK_ISO_FORMAT_0 = 0,
    PINBLOCK_ISO_FORMAT_1 = 1,
    PINBLOCK_ISO_FORMAT_2 = 2,
    PINBLOCK_ISO_FORMAT_3 = 3,
    PINBLOCK_ISO_FORMAT_4 = 4,
} pinblock_format_num_t;

typedef struct pinblock_t {
    uint8_t header; // First nibble is always type number (0,1,2,3), second nibble PIN length
    uint8_t data[PINBLOCK_DATA_LENGTH];
} pinblock_t;
_Static_assert(sizeof(pinblock_t) == 8, "pinblock_t has unexpected size");

typedef struct pinblock_format4_t {
    uint8_t header; // First nibble is fixed to 0x4, second nibble PIN length
    uint8_t data[PINBLOCK_FORMAT4_DATA_LENGTH];
} pinblock_format4_t;
_Static_assert(sizeof(pinblock_format4_t) == 16, "pinblock_format4_t has unexpected size");

pinblock_result_t pinblock_format0_plaintext_pinblock( const uint8_t *pin_digits, uint8_t pin_length, const uint8_t *pan_digits, uint8_t pan_length, uint8_t *pinblock_buf, uint8_t pinblock_length);

pinblock_result_t pinblock_format2_plaintext_pinblock( const uint8_t *pin_digits, uint8_t pin_length, uint8_t *pinblock_buf, uint8_t pinblock_length);

// Convert format 1 or format 2 pinblock to raw binary digits - primarily a helper function for testing
pinblock_result_t pinblock_to_raw(
    pinblock_format_num_t pinblock_format_num,
    void const *pinblock_buf,
    size_t pinblock_buf_len,
    uint8_t *pin_digit_buf,
    size_t *pin_digit_buf_len);

// Convert format 4 to raw pin
pinblock_result_t pinblock_format4_to_raw(
    const pinblock_format4_t *pinblock_buf,
    uint8_t *pin_digit_buf,
    size_t *pin_digit_buf_len);

pinblock_result_t pinblock_format1_to_format2(uint8_t *pinblock);

pinblock_result_t pinblock_format4_add_digit(pinblock_format4_t *pinblock, uint8_t digit);
