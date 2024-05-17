// Copyright (c) 2019 Square, Inc. All rights reserved.

/* pb_decode_ritual.h: Functions to decode protocol buffers from a struct-
 * based ritual message. Depends on pb_decode_ritual.c. The main function is
 * pb_decode_from_ritual. You also need an input stream, and the field
 * descriptions created by nanopb_generator.py.
 */

#ifndef PB_DECODE_RITUAL_H_INCLUDED
#define PB_DECODE_RITUAL_H_INCLUDED

#include "pb.h"
#include "pb_decode.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************
 * Main decoding functions *
 ***************************/

/* Decode a single protocol buffers message from input stream into a C structure.
 * Returns true on success, false on any failure.
 * The actual struct pointed to by dest must match the description in fields.
 *
 * Input data should be a struct generated from ritual.yaml. If input data does
 * not contain all fields remaining fields will be left empty/unused. Ritual struct
 * should match the c proto struct and shoudl not skip fields.
 */
bool pb_decode_from_ritual(pb_istream_t *stream, const pb_msgdesc_t fields[], void *dest_struct);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
