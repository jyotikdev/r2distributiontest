// Copyright 2017 Square, Inc.

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Safe Buffer type
 *
 * sbuf is a small buffer type that provides safe (bounds-checked) access to an
 * underlying memory buffer. The structure itself stores a pointer, size and
 * position and allows byte and buffer access to the memory.
 *
 * sbuf also provides a file-like streaming interface which provides safe typed
 * reads and writes to the sbuf.
 *
 * Examples:
 *
 * // core
 * uint8_t buffer[8];
 * sbuf_t sbuf = sbuf_from_buf(buffer, sizeof(buffer));
 *
 * sbuf_set(&sbuf, 0, 42); // equivalent to buf[0] = 42
 * uint8_t x = sbuf_get(&sbuf, 0); // equivalent to x = buf[0]
 *
 * sbuf_set(&sbuf, 9, 42); // asserts
 *
 * // copy buffer2 into the sbuf (at offset 2)
 * uint8_t buffer2[] = {1,2,3,4}
 * sbuf_copy_from_buf(&sbuf, 2, sizeof(buffer2), buffer2);
 *
 * // streaming
 * sbuf_stream_position(&sbuf); // returns 0
 * sbuf_stream_remaining(&sbuf); // returns 8
 *
 * sbuf_stream_read_u8(&sbuf); // returns byte at pos 0, pos now 1
 * sbuf_stream_read_u32(&sbuf); // returns word at pos 1, pos now 4
 *
 * sbuf_stream_rewind(&sbuf); // set pos to 0
 *
 * sbuf_stream_write_bytes(&sbuf, 4, buffer2); // writes 4 bytes from buffer2
 *                                             // into sbuf, pos now 4.
 */

typedef struct {
    uint8_t *ptr;
    size_t size;
    size_t pos;
} sbuf_t;

// Core API - Provides byte & buffer read/write calls.

// Create a new sbuf that wraps the given buffer (and size).
sbuf_t sbuf_from_buf(uint8_t *buf, size_t buf_size);
// Read from the sbuf, returning the byte at index.
uint8_t sbuf_get(sbuf_t *sbuf, size_t index);
// Write to the sbuf, setting the byte at index.
void sbuf_set(sbuf_t *sbuf, size_t index, uint8_t value);
// Copy len bytes from raw src buffer into the sbuf at offset
void sbuf_copy_from_buf(sbuf_t *sbuf, size_t offset, size_t len, const uint8_t *src);
// Copy len bytes from the sbuf at offset into raw dest buffer
void sbuf_copy_to_buf(sbuf_t *sbuf, size_t offset, size_t len, uint8_t *dest);
// Copy len bytes starting at src_offset to dest_offset within the sbuf.
// Copied regions may overlap.
void sbuf_move(sbuf_t *sbuf, size_t dest_offset, size_t src_offset, size_t len);

// Stream API - Provides a file like (with implicit position) read/write calls.

// Return the current position in the stream
size_t sbuf_stream_position(sbuf_t *sbuf);
// Move the current position in the stream forward by a relative offset.
// Return false (and don't update position) if new position is out of bounds.
bool sbuf_stream_seek(sbuf_t *sbuf, size_t ofs);
// Return the bytes remaining in the stream (length - position)
size_t sbuf_stream_remaining(sbuf_t *sbuf);
// Set the stream position to 0
void sbuf_stream_rewind(sbuf_t *sbuf);
// Return pointer to current position in buffer
uint8_t *sbuf_stream_pointer(sbuf_t *sbuf);

// Peek
uint8_t sbuf_stream_peek_u8(sbuf_t *sbuf);

// Read a specified type (native byteorder) from the stream and increment position
uint8_t sbuf_stream_read_u8(sbuf_t *sbuf);
void sbuf_stream_read_bytes(sbuf_t *sbuf, size_t len, void *dest);

// Write a specified type (native byteorder) from the stream and increment position
void sbuf_stream_write_u8(sbuf_t *sbuf, uint8_t val);
void sbuf_stream_write_u16(sbuf_t *sbuf, uint16_t val);
void sbuf_stream_write_u32(sbuf_t *sbuf, uint32_t val);
void sbuf_stream_write_bytes(sbuf_t *sbuf, size_t len, const void *src);
