// Copyright (c) 2021 Square, Inc. All rights reserved.

/* pb_decode_to_ritual.h: Functions to decode encoded protocol buffers into
 * struct-based ritual message. The main function is pb_decode_to_ritual.
 * You also need an input stream, and the field descriptions created by
 * nanopb_generator.py.
 */
#pragma once

#include "pb_decode.h"
#include "pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * Main decoding functions *
 ***************************/
 
/* Decode a single protocol buffers message from input stream into a C structure.
 * Returns true on success, false on any failure.
 * The actual struct pointed to by dest must match the description in fields.
 * Callback fields of the destination structure must be initialized by caller.
 * All other fields will be initialized by this function.
 *
 * Example usage:
 *    crs_mfgtest_tap_msg_t taptest_msg;
 *    pb_decode_to_ritual(&pb_stream, squareup_firmware_ritual_MfgTestTapTest_fields, taptest_msg);
 */
bool pb_decode_to_ritual(pb_istream_t *stream, const pb_msgdesc_t fields[], void *dest_struct, size_t *dest_struct_len_out);

/* Same as pb_decode_to_ritual, except does not initialize the destination
 * structure to default values. This is slightly faster if you need no default
 * values and just do memset(struct, 0, sizeof(struct)) yourself.
 *
 * This can also be used for 'merging' two messages, i.e. update only the
 * fields that exist in the new message.
 */
bool pb_decode_to_ritual_noinit(pb_istream_t *stream, const pb_msgdesc_t fields[], void *dest_struct, size_t *dest_struct_len_out);

/* Decode the tag and submessage size from the stream.
 * Output the tag and max size of the ritual struct for the submessage.
 * Returns true on success, false on any failure.
 */
bool pb_decode_to_ritual_get_tag_and_size(pb_istream_t *stream, const pb_msgdesc_t fields[], pb_size_t *tag_out, size_t *size_out);

#ifdef __cplusplus
} /* extern "C" */
#endif
