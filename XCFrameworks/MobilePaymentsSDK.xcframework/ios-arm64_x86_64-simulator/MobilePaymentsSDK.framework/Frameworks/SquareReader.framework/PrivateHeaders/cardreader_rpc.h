// Copyright 2017 Square, Inc.

#pragma once

#include <pb_encode.h>
#include <pb_decode.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Description of client registration types:
 *  CR_RPC_CLIENT_REGISTRATION_TYPE_INVALID
 *      The default type when creating a client array in the RPC server
 *  CR_RPC_CLIENT_REGISTRATION_TYPE_LIMITED
 *      Used to denote that the client should not be able to service
 *      secure-touch messages
 *  CR_RPC_CLIENT_REGISTRATION_TYPE_SECURE_TOUCH
 *      Used to denote that the client should only be allowed to send/receive
 *      a subset of messages to/from the SPE in order to facilitate the
 *      secure-touch feature on SQUID devices
 *  CR_RPC_CLIENT_REGISTRATION_TYPE_FULL
 *     Used to denote that the client has full functionality in sending
 *      and receiving messages to/from the SPE
 *
 * Note that only local clients (clients that reside on the same physical
 * device as the SPE) are allowed to register as type
 * CR_RPC_CLIENT_REGISTRATION_TYPE_SECURE_TOUCH or CR_RPC_CLIENT_REGISTRATION_TYPE_FULL.
 **/
typedef enum cr_rpc_client_registration_type_t {
    CR_RPC_CLIENT_REGISTRATION_TYPE_INVALID = 0,
    CR_RPC_CLIENT_REGISTRATION_TYPE_LIMITED = 1,
    CR_RPC_CLIENT_REGISTRATION_TYPE_SECURE_TOUCH = 2,
    CR_RPC_CLIENT_REGISTRATION_TYPE_FULL = 3,
} cr_rpc_client_registration_type_t;

const char *cr_rpc_client_registration_type_to_string(cr_rpc_client_registration_type_t type);

bool cardreader_rpc_pb_write(pb_ostream_t *stream, uint8_t const *buf, size_t count);
bool cardreader_rpc_pb_read(pb_istream_t *stream, uint8_t *buf, size_t count);
