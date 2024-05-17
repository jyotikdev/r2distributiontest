// Copyright (c) 2021 Square, Inc. All rights reserved.

#pragma once

#include "pb_decode.h"

typedef bool (*pb_decoder_t)(pb_istream_t *stream, const pb_field_t *field, void *dest) checkreturn;

bool checkreturn buf_read(pb_istream_t *stream, pb_byte_t *buf, size_t count);
bool checkreturn pb_decode_varint32(pb_istream_t *stream, uint32_t *dest);
bool checkreturn read_raw_value(pb_istream_t *stream, pb_wire_type_t wire_type, pb_byte_t *buf, size_t *size);

bool checkreturn pb_dec_varint(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_uvarint(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_svarint(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_fixed32(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_fixed64(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_bytes(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_bytes_to_ritual(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_string(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_submessage(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_dec_submessage_to_ritual(pb_istream_t *stream, const pb_field_t *field, void *dest);
bool checkreturn pb_skip_varint(pb_istream_t *stream);
bool checkreturn pb_skip_string(pb_istream_t *stream);
bool checkreturn pb_dec_bytes_to_ritual_and_get_size(pb_istream_t *stream, const pb_field_t *field, void *dest, pb_size_t *bytes_used);

bool checkreturn pb_decode_tag(pb_istream_t *stream, pb_wire_type_t *wire_type, uint32_t *tag, bool *eof);
bool checkreturn pb_skip_field(pb_istream_t *stream, pb_wire_type_t wire_type);
bool checkreturn pb_make_string_substream(pb_istream_t *stream, pb_istream_t *substream);
void pb_close_string_substream(pb_istream_t *stream, pb_istream_t *substream);
