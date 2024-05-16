// Copyright (c) 2021 Square, Inc. All rights reserved.

#pragma once

#include "pb_encode.h"

bool buf_write(pb_ostream_t *stream, const pb_byte_t *buf, size_t count);
bool pb_enc_uvarint(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_svarint(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_fixed32(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_fixed64(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_bytes(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_bytes_from_ritual(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_string(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_submessage(pb_ostream_t *stream, const pb_field_t *field, const void *src);
bool pb_enc_submessage_from_ritual(pb_ostream_t *stream, const pb_field_t *field, const void *src);
