// Copyright 2017 Square, Inc.

#pragma once

#include <squareup/spe/securesession.pb.h>
#include <square.h>
#include <string.h>
#include <stdbool.h>

typedef bool (*hmac_func_t)(uint8_t const *,
                            uint32_t const,
                            uint8_t const *,
                            uint32_t const,
                            uint8_t *);
typedef bool (*sha256_compare_t)(uint8_t const *hash_one,
                                 uint8_t const *hash_two);

typedef struct {
    hmac_func_t hmac_sha256;
    sha256_compare_t sha256_compare;
} hmac_proto_context_t;

/*
Set up the hmac library with the relevant function pointers to be used in
encoding and decoding a HMAC message.

See mock-flipper.c for an example for how to use this library.

Arguments
    ctx: A hmac_proto_context_t to hold the function pointers.
    hmac_sha256: Function pointer to the desired hmac function.
    sha256_compare: (optional) Function pointer to the desired hmac compare
                    function - should be a constant time compare to prevent side
                    channel attacks.

Return
    bool indicating if the init succeeded
*/
bool hmac_proto_init(hmac_proto_context_t *ctx,
                     hmac_func_t hmac_sha256,
                     sha256_compare_t sha256_compare);

/*
This function converts a protobuf message, that is passed in as a
byte buffer, into a HMACMessage protobuf with a hmac sha. The protobuf will
be saved into the body field of the HMACMessage protobuf.

Specifically, it encodes a given unencoded proto struct (and its type in the form of a
pb_fields_t), encode into a buf (of max size buf_len).

The encoded proto is of type HMACMessage where the body field is the
encoded body proto, over which the HMAC is calculated

Warning
    This function allocates a 512 byte array. Make sure the process you are
    calling this function from has access to adequate space.

Arguments
    hmac_sha256: Function pointer to the hmac function. For example, on the
                 reader side, this should be passed in from
                 &(ctx->crypto_api.hmac_sha256).
    proto: (Src) Pointer to the unencoded proto struct.
    proto_fields: The fields of the proto message.
    hmac_buf: (Dest) The byte buffer that the resulting hmac proto will be
              written to. Should be passed in as an empty array.
    hmac_buf_len: (In) The max length of the resulting HMACMessage.
                  (Out) The length of the resulting hmac message.
    hmac_key: Key used to calculate the hmac.
    hmac_key_size: Size of the hmac key in bytes.

Return
    bool indicating in the encode succeeded
*/
bool hmac_proto_encode(hmac_proto_context_t *ctx,
                       void *proto, const pb_msgdesc_t *proto_fields,
                       uint8_t *hmac_buf, size_t *hmac_buf_len,
                       const uint8_t *hmac_key, size_t hmac_key_size);

/*
This function decodes a given buffer as an HMACMessage and extracts the
message body. It authenticates the message body by rederiving the HMAC
and checking that it matches the HMAC that is received.

Arguments
    hmac_sha256: Function pointer to the hmac function. For example, on the
                 reader side, this should be passed in from
                 &(ctx->crypto_api.hmac_sha256).
    proto: (Dest) Should point to an protobuf struct that matches the type passed
          in the message body of the HMAC message. Should be passed in as a
          (void *).
    proto_fields: The fields of the protobuf struct that is passed in to the
                  message body of the HMAC message.
    hmac_buf: (Src) The HMACMessage passed in as a byte array.
    hmac_buf_len: The actual length of the hmac message.
    hmac_key: Key used to calculate the hmac.
    hmac_key_size: Size of the hmac key in bytes.

Return
    bool indicating in the decode succeeded
*/
bool hmac_proto_decode(hmac_proto_context_t *ctx,
                       void *proto, const pb_msgdesc_t *proto_fields,
                       uint8_t *hmac_buf, size_t hmac_buf_len,
                       const uint8_t *hmac_key, size_t hmac_key_size);

/*
Converts any protobuf into an HMACMessage. The protobuf is encoded into a
buffer, which is set as the body field of a HMACMessage proto (hmac_proto).
The HMACMessage's hmac is also calculated over the encoded body buffer.

Arguments
    ctx: A hmac_proto_context_t that holds the function pointers.
         It should have been set up with a call to hmac_proto_init();
    proto: (Src) Pointer to an unencoded proto struct - can be any proto.
    proto_fields: The fields of the proto message.
    hmac_proto: (Dest) The resulting HMACMessage that will be written to.
    hmac_key: Key used to calculate the hmac.
    hmac_key_size: Size of the hmac key in bytes.

Return
    bool indicating if the HMACMessage was successfully populated
*/
bool hmac_proto_body_proto_to_hmac_message(hmac_proto_context_t *ctx,
                                           void *proto,
                                           const pb_msgdesc_t *proto_fields,
                                           squareup_spe_securesession_HMACMessage *hmac_message_proto,
                                           const uint8_t *hmac_key,
                                           size_t hmac_key_size);

/*
A HMACMessage protobuf is encoded and serialized into a byte buffer.

Arguments
    ctx: A hmac_proto_context_t that holds the function pointers.
         It should have been set up with a call to hmac_proto_init();
    hmac_proto: (Src) The resulting HMACMessage that will be written to.
    hmac_buf: (Dest) The HMACMessage encoded and serialized as a byte array.
    hmac_buf_len: (In) The max length of the resulting HMACMessage.
                  (Out) The length of the resulting hmac message.

Return
    bool indicating if the protobuf encoding succeeded
*/
bool hmac_proto_encode_hmac_message_proto(hmac_proto_context_t *ctx,
                                          squareup_spe_securesession_HMACMessage *hmac_message_proto,
                                          uint8_t *hmac_buf,
                                          size_t *hmac_buf_len);

/*
An encoded byte buffer of a HMACMessage proto (hmac_buf) should be passed in.
Both the message body and the HMACMessage will de decoded.

Arguments
    ctx: A hmac_proto_context_t that holds the function pointers.
         It should have been set up with a call to hmac_proto_init();
    proto: (Dest) Pointer to an unencoded proto struct - can be any proto.
    proto_fields: The fields of the proto message.
    hmac_message: (Dest) The resulting HMACMessage that will be written to.
    hmac_buf: (Src) The HMACMessage body encoded and serialized as a byte array.
    hmac_buf_len: The length of the hmac message proto.

Return
    bool indicating if the protobuf decoding succeeded
*/
bool hmac_proto_hmac_buffer_to_body_proto(hmac_proto_context_t *ctx,
                                          void *proto,
                                          const pb_msgdesc_t *proto_fields,
                                          squareup_spe_securesession_HMACMessage *hmac_message_proto,
                                          uint8_t *hmac_buf,
                                          size_t hmac_buf_len);

/*
A decoded HMACMessage proto (hmac_buf) should be passed in.
The message body will de decoded and the hmac will be validated.

Arguments
    ctx: A hmac_proto_context_t that holds the function pointers.
         It should have been set up with a call to hmac_proto_init();
    proto: (Dest) Pointer to a proto struct - can be any proto.
    proto_fields: The fields of the proto message.
    hmac_message: (Src) The HMACMessage proto with an encoded body.
    hmac_key: Key used to re-calculate the hmac.
    hmac_key_size: Size of the hmac key in bytes.

Return
    bool indicating if the protobuf decoding succeeded and if the message was not corrupted
*/
bool hmac_proto_validate_and_decode_message_body(hmac_proto_context_t *ctx,
                                                 void *proto,
                                                 const pb_msgdesc_t *proto_fields,
                                                 squareup_spe_securesession_HMACMessage *hmac_message_proto,
                                                 const uint8_t *hmac_key,
                                                 size_t hmac_key_size);

/*
A decoded HMACMessage proto (hmac_buf) should be passed in.
The message body will de decoded.

Arguments
    ctx: A hmac_proto_context_t that holds the function pointers.
         It should have been set up with a call to hmac_proto_init();
    proto: (Dest) Pointer to a proto struct - can be any proto.
    proto_fields: The fields of the proto message.
    hmac_message: (Src) The HMACMessage proto with an encoded body.

Return
    bool indicating if the protobuf decoding succeeded
*/
bool hmac_proto_hmac_message_to_body_proto(hmac_proto_context_t *ctx,
                                           void *proto,
                                           const pb_msgdesc_t *proto_fields,
                                           squareup_spe_securesession_HMACMessage *hmac_message_proto);

/*
Validates the hmac of an HMACMessage protobuf.

Arguments
    ctx: A hmac_proto_context_t that holds the function pointers.
         It should have been set up with a call to hmac_proto_init();
    hmac_message: The HMACMessage protobuf that will be validated.
    hmac_key: Key used to re-calculate the hmac.
    hmac_key_size: Size of the hmac key in bytes.

Return
    bool indicating if the message was not corrupted
*/
bool hmac_proto_validate_message(hmac_proto_context_t *ctx,
                                 squareup_spe_securesession_HMACMessage *hmac_message_proto,
                                 const uint8_t *hmac_key,
                                 size_t hmac_key_size);
