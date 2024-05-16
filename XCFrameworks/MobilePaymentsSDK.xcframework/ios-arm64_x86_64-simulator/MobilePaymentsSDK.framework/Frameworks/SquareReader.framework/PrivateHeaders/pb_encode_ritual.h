// Copyright (c) 2019 Square, Inc. All rights reserved.

/* pb_encode_ritual.h: Functions to encode protocol buffers as struct-based
 * ritual messages. Depends on pb_encode_ritual.c. The main function is
 * pb_encode_as_ritual. You also need an output stream, and the field
 * descriptions created by nanopb_generator.py.
 */

#ifndef PB_ENCODE_RITUAL_H_INCLUDED
#define PB_ENCODE_RITUAL_H_INCLUDED

#include "pb.h"
#include "pb_encode.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * Main encoding functions *
 ***************************/

/* Encode a single protocol buffers message from C structure into a stream.
 * Returns true on success, false on any failure.
 * The actual struct pointed to by src_struct must match the description in fields.
 * All required fields in the struct are assumed to have been filled in.
 *
 * Example usage:
 *    MyMessage msg = {};
 *    uint8_t buffer[64];
 *    pb_ostream_t stream;
 *
 *    msg.field1 = 42;
 *    stream = pb_ostream_ritual_from_buffer(buffer, sizeof(buffer));
 *    pb_encode(&stream, MyMessage_fields, &msg);
 */
bool pb_encode_as_ritual(pb_ostream_t *stream, const pb_msgdesc_t fields[], const void *src_struct);

/* Encode the message to get the size of the encoded data, but do not store
 * the data. */
bool pb_get_encoded_ritual_size(size_t *size, const pb_msgdesc_t fields[], const void *src_struct);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
