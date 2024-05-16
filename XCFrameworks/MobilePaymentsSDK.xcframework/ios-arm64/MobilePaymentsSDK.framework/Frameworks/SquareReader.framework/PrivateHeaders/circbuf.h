// Copyright 2017 Square, Inc.

/**
 * @file
 *
 * @brief Circbuf Library
 *
 * @ingroup circbuf_lib
 */

/**
 * @defgroup circbuf_lib Circular Buffer Library
 *
 * @brief Circular buffer library.
 *
 * @ingroup lib
 *
 * @{
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Struct to hold statistics for the circular buffer.
 */
typedef struct circbuf_stats_t {
    int overflows; /** Number of times the buffer has overflowed. */
    size_t max_length; /** Maximum length the buffer has reached. */
    int pushes; /** Number of times data has been pushed into the buffer. */
    int pops; /** Number of times data has been popped from the buffer. */
} circbuf_stats_t;

/**
 * @brief Struct to hold metadata of the circular buffer.
 */
typedef struct circbuf_info_t {
    size_t size; /** Current size of the buffer. */
    uint32_t head; /** Index of the head (least recent element) of the buffer. */
    uint32_t tail; /** Index of the tail (most recent element) of the buffer. */
    size_t capacity; /** Maximum capacity of the buffer. */
    uint8_t *data; /** Pointer to the data buffer. */
    bool replace_when_full; /** Flag to indicate if data should be replaced when the buffer is full. */
    circbuf_stats_t stats; /** Statistics for the buffer. */
} circbuf_info_t;

/**
 * @brief Initialize a circular buffer.
 *
 * @note @p data and @p circbuf must have the same lifetime.
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @param capacity Maximum capacity of the circular buffer.
 * @param data Pointer to the data buffer.
 * @param replace_when_full Flag to indicate if data should be replaced when the buffer is full.
 */
void circbuf_init(circbuf_info_t *circbuf, size_t capacity, uint8_t *data, bool replace_when_full);

/**
 * @brief Get the current length of the circular buffer.
 *        Answers "How much data is in the buffer?".
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @return Current size of the circular buffer.
 */
size_t circbuf_get_length(circbuf_info_t *circbuf);

/**
 * @brief Get the available space in the circular buffer.
 *        Answers "How much data can we still put in the buffer?".
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @return Available space in the circular buffer.
 */
size_t circbuf_get_available(circbuf_info_t *circbuf);

/**
 * @brief Push a value into the circular buffer.
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @param value Value to be pushed into the buffer.
 * @return True if the buffer was full and a value was replaced (an overflow), false otherwise.
 */
bool circbuf_push(circbuf_info_t *circbuf, uint8_t value);

/**
 * @brief Pop a value from the circular buffer.
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @return Value popped from the buffer.
 */
uint8_t circbuf_pop(circbuf_info_t *circbuf);

/**
 * @brief Peek into the circular buffer. Copies the data from the head of the
 *        buffer into the destination buffer without removing it from the
 *        circbuf.
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @param dest Destination buffer to copy the peeked data.
 * @param num_bytes Number of bytes to peek.
 * @return Number of bytes that were available.
 */
uint32_t circbuf_peek(circbuf_info_t *circbuf, uint8_t *dest, size_t num_bytes);

/**
 * @brief Insert data into the circular buffer.
 *
 * @param circbuf Pointer to the circular buffer structure.
 * @param src Source buffer from which data is to be copied. Cannot be the same buffer as circbuf->data.
 * @param num_bytes Number of bytes to insert.
 * @return Number of bytes actually inserted.
 */
size_t circbuf_insert(circbuf_info_t *circbuf, void const *src, size_t num_bytes);


/**
 * @brief Remove data from the head of the circular buffer.
 *
 * @details Only removes #circbuf_get_length() bytes if @p num_bytes is greater than length.
 * @param circbuf Pointer to the circular buffer structure.
 * @param num_bytes Number of bytes to remove.
 * @return Number of bytes actually removed.
 */
size_t circbuf_remove(circbuf_info_t *circbuf, size_t num_bytes);

/**
 * @brief Clear the circular buffer. Removes all data from the buffer.
 *
 * @param circbuf Pointer to the circular buffer structure.
 */
void circbuf_clear(circbuf_info_t *circbuf);

/**
 * @brief Rotate the circular buffer so that the head is at the beginning.
 *
 * @param circbuf Pointer to the circular buffer structure.
 */
void circbuf_rotate(circbuf_info_t *circbuf);

/**
 * @brief Get a pointer to the data in the circular buffer.
 *
 * @details Returns a pointer to head, followed by num_bytes of contiguous data,
 *          rotating the buffer if necessary to ensure the data is linear within
 *          the buffer.
 * @param circbuf Pointer to the circular buffer structure.
 * @param num_bytes Number of bytes to access.
 * @return Pointer to the data in the buffer.
 */
uint8_t *circbuf_data(circbuf_info_t *circbuf, size_t num_bytes);

/** @} */
