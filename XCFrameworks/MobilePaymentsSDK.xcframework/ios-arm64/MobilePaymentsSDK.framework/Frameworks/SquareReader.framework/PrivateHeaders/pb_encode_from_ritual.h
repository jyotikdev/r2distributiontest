// Copyright (c) 2021 Square, Inc. All rights reserved.

/* pb_encode_from_ritual.h: Functions to encode ritual struct data into
 * encoded protobuf data. The main function is pb_encode_from_ritual.
 * You also need an output stream, and the field descriptions created by
 * nanopb_generator.py.
 */
#pragma once

#include "pb_encode.h"
#include "pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * Main encoding function *
 ***************************/
 
/* Encode a single protocol buffers message from C ritual structure into a stream.
 * Returns true on success, false on any failure.
 * The actual struct pointed to by src_struct must match the description in fields.
 * All required fields in the struct are assumed to have been filled in.
 *
 * Example usage:
 *    crs_mfgtest_tap_msg_t taptest_msg = {...};
 *    uint8_t buffer[64];
 *    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
 *    pb_encode_from_ritual(&stream, squareup_firmware_ritual_MfgTestTapTest_fields, &msg, sizeof(msg));
 */
bool pb_encode_from_ritual(pb_ostream_t *stream, const pb_msgdesc_t fields[], const void *src_struct, size_t src_size);

/* Encode the message to get the size of the encoded data, but do not store the data. */
bool pb_get_encoded_from_ritual_size(size_t *size, const pb_msgdesc_t fields[], const void *src_struct, size_t src_size);

/**
 * @brief Encode ritual data as a submessage indicated by the tag parameter
 *
 * @param stream Input stream
 * @param fields Pointer to the fields of the parent message (eg: squareup_firmware_ritual_SpeMessage_fields)
 * @param tag Tag of the encoded submessage (eg: squareup_firmware_ritual_SpeMessage_coredump_data_tag)
 * @param src_struct Pointer to the ritual struct to encode (eg: a coredump_data_t)
 * @param src_size Size of the ritual struct to encode
 */
bool pb_encode_from_ritual_by_tag(pb_ostream_t *stream, const pb_msgdesc_t fields[], pb_size_t tag, const void *src_struct, size_t src_size);

/* Encode the message to get the size of the encoded data, but do not store the data. */
bool pb_get_encode_from_ritual_by_tag_size(size_t *size, const pb_msgdesc_t fields[], pb_size_t tag, const void *src_struct, size_t src_size);

#ifdef __cplusplus
} /* extern "C" */
#endif
