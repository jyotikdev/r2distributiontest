// Copyright 2017 Square, Inc.

#pragma once

#define CRC32_INITIAL (0)            /* RFC 1952 uses 0 as init value */

#ifndef BYTE2SHORT
#define BYTE2SHORT(x) ((((uint8_t*)(x))[0]) | (((uint8_t*)(x))[1] << 8))
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

typedef struct util_mem_block_t {
    uint8_t const *bytes;
    int length;
    int full_length;
    int offset;
    struct util_mem_block_t *next;
} util_mem_block_t;

#if !TARGET_EMBEDDED
bool write_file(const char *dst_file, const uint8_t *data, const uint32_t len);
size_t read_file(const char *in_file, uint8_t *in_data, int in_data_max_len);
#endif

void bytes_to_hex_string(uint8_t const *bytes, char *str, size_t str_size);
bool ascii2byte(char const *ascii, uint8_t *binary);
bool parse_hex_string(char const *input_str, uint8_t *output, size_t max_strlen);

void *sq_memset_s(void *ptr, int value, size_t num);
void *sq_memset_32bit_s(void *ptr, uint32_t value, size_t len);
int sq_memcmp_with_value_s(uint8_t const *buf1, uint8_t value, size_t len);
int sq_memcmp_s(uint8_t const *buf1, uint8_t const *buf2, size_t len);
int sq_memcmp_with_value_32bit_s(uint32_t const *buf1, uint32_t value, size_t len);
void *sq_memcpy(void *s1, const void *s2, size_t n);
void sq_reverse_array(uint8_t *buf, size_t len);


/**
 * @brief Safe (length limited) string length
 *
 * @details This function computes the length of the string input_str, not
 * including the NUL-terminator, and at most len.
 *
 * @param input_str    Pointer to the string.
 * @param len          Maximum length of the string to check.
 *
 * @return             Number of bytes to the NUL or len, whichever is less.
 */
size_t sq_strlen_s(char const *input_str, size_t len);

/**
 * @brief Safe (length limited) string copy
 *
 * @details This function copies up to max_length - 1 characters from the
 * NUL-terminated string source_str to dest_str, NUL-terminating the result.
 *
 * @param dest_str     Pointer to the destination string.
 * @param source_str   Pointer to the source string.
 * @param max_length   Maximum number of bytes to copy.
 *
 * @return             Number of bytes copied from source_str
 */
size_t sq_strcpy_s(char *dest_str, char const *source_str, size_t max_length);

void sq_crc_32_init(void);
uint16_t sq_crc_16_calc(uint16_t crc, uint8_t const *data, size_t data_len, size_t zero_pad_len);
uint32_t sq_crc_32_calc(const uint8_t *data, uint32_t num_bytes);
uint32_t sq_crc_32_intermediate (uint32_t crc, const uint8_t *data, uint32_t len);
uint32_t sq_crc_32_final (uint32_t crc);
uint16_t sq_ascii_to_packed_bin(const uint8_t *ascii_array, uint16_t ascii_array_len, uint8_t *binary_array, uint16_t binary_array_len);

uint32_t divide_round_up(uint32_t dividend, uint32_t divisor);

/**
 * @brief Populates a buffer with the decimal character version of a value
 *
 * @param str     A precreated buffer to hold the string.
 * @param str_len The max length of the string.
 * @param val     The value to convert to a string.
 */
char *sq_dec2str(char* str, size_t str_len, uint32_t val);

void sq_big_endian_memcpy(uint8_t *dst, uint8_t *src, size_t len);
uint32_t sq_flip_bytes_in_word(uint32_t in);
