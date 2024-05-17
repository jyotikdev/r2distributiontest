/*
 * Square helpers and extensions for dealing with nanopb.
 */

#pragma once

#include "pb.h"
#include <string.h>
#include <util.h>

// helper to set optional values and has_<field>
#define PB_SET(PB, FIELD, VALUE) \
    do { \
        (PB)->FIELD = (VALUE); \
        (PB)->has_##FIELD = true; \
    } while (0)

#define PB_SET_BYTES(PB, FIELD, DATA, SIZE) \
      do { \
          SQ_ASSERT(SIZE <= sizeof((PB)->FIELD.bytes)); \
          const void *_pb_set_data = (DATA); \
          if (_pb_set_data != NULL) { \
              memmove((PB)->FIELD.bytes, _pb_set_data, (SIZE)); \
          } \
          (PB)->FIELD.size = (pb_size_t)(SIZE); \
          (PB)->has_##FIELD = true; \
      } while (0)

#define PB_SET_BYTES_REQUIRED(PB, FIELD, DATA, SIZE) \
      do { \
          SQ_ASSERT(SIZE <= sizeof((PB)->FIELD.bytes)); \
          const void *_pb_set_data = (DATA); \
          if (_pb_set_data != NULL) { \
              memmove((PB)->FIELD.bytes, _pb_set_data, (SIZE)); \
          } \
          (PB)->FIELD.size = (pb_size_t)(SIZE); \
      } while (0)

// Note: silent truncation of src string is possible
#define PB_SET_STRING(PB, FIELD, DATA, SIZE) \
    do { \
        SQ_ASSERT((SIZE) <= sizeof((PB)->FIELD)); \
        SQ_ASSERT((DATA) != NULL); \
        (void)sq_strcpy_s((PB)->FIELD, (DATA), (SIZE)); \
        (PB)->has_##FIELD = true; \
    } while (0)

// Helper to get max size of a BYTES_ARRAY field's .bytes member (not including .size)
#define PB_FIELD_BYTES_ARRAY_LEN(PB_TYPE, FIELD) sizeof(((PB_TYPE*)0)->FIELD.bytes)

typedef struct pb_raw_bytes_t
{
    uint8_t *bytes;
    size_t len;
    bool was_called;
} pb_raw_bytes_t;

/*
 Helper function to setup the encode callback of a pb_callback_t field.
 Should not be called directly, pb_encode() will call this function when encoding the corresponding field.

 stream: the output stream which contains the buffer which will store the proto.
 field: the field being encoded. We do not use this parameter in our implementation.
 arg: the data that will be encoded in a pb_raw_bytes_t struct.
 */
bool sq_pb_encode_callback(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);

/*
 Helper function to setup the decode callback of a pb_callback_t field.
 Should not be called directly, pb_decode() will call this function when decoding the corresponding field.

 stream: the output stream which contains the buffer which will store the proto.
 field: the field being decoded. We do not use this parameter in our implementation.
 arg: the buffer that will be store the field in a pb_raw_bytes_t struct.
      The size of the buffer should be set before calling pb_decode().
      The size of the written buffer is available after calling pb_decode().
 */
bool sq_pb_decode_callback(pb_istream_t *stream, const pb_field_t *field, void **arg);

/*
 Helper function to calculate the size used for a protobuf struct. This is useful for
 determining actual number of used bytes for a oneof or struct with sparse data.

 fields: fields for the message type of the structure
 src_struct: pointer to the structure to calculate used size of
 returns: size used for the protobuf struct (not the encoded size, but the struct size)
 */
size_t sq_pb_get_proto_struct_used_size(const pb_msgdesc_t fields[], const void *src_struct);

/*
 Helper function to calculate the size for a protobuf struct.

 fields: fields for the message type of the structure
 returns: size for the protobuf struct (not the encoded size, but the struct size)
 */
size_t sq_pb_get_proto_struct_max_size(const pb_msgdesc_t fields[]);

/*
 Helper function to return fields for a sub-field matching a specified tag

 fields: fields for parent message
 tag: the specific tag we'd like to get fields for
 returns: pb_msgdesc_t for the submessage matching 'tag'
 */
const pb_msgdesc_t *sq_pb_get_fields_from_tag(const pb_msgdesc_t fields[], pb_size_t tag);

/*
 Helper function to calculate the size of a sub-field matching a specified tag

 fields: fields for the message type of the structure
 tag: the specific tag we'd like to know the size of
 returns: size fo the field matching tag or zero if not found
 */
size_t sq_pb_get_field_size_from_tag(const pb_msgdesc_t fields[], pb_size_t tag);

/*
 Helper function to decode the buffer size of a delimited encoded message (including size prefix)

 data: buffer containing at least the first varint of the encoded message
 returns: size of a delimited encoded message (including size prefix)
 */
size_t sq_pb_decode_delimited_msg_size(const void *data);
