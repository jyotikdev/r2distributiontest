// Copyright 2017 Square, Inc.

#pragma once

#include <cardreader_rpc.h>
#include <cardreader/cr_comms_driver_api.h>
#include <squareup/client/mock_trustzone.pb.h> // TODO (ESW-5784): replace proto definition file

typedef enum {
    CARDREADER_RPC_CLIENT_DEFAULT_PORT = 50016
} cardreader_rpc_client_port_t;

typedef struct cardreader_rpc_client_t cardreader_rpc_client_t;

typedef void (*cardreader_rpc_client_on_start_thread_cb_t)(void *context);

typedef void (*cardreader_rpc_client_on_end_thread_cb_t)(void *context);

typedef void (*cardreader_rpc_client_on_callback_recvd_cb_t)(void *context);

typedef void (*cardreader_rpc_client_on_server_disconnected_cb_t)(void *context);

typedef void (*cardreader_rpc_client_recv_registration_req_status_cb_t)(void *context, bool request_status);
typedef struct {
    // Called from the RECEIVE thread when a callback message has been received
    // from the server. The host should signal the LCR MAIN thread to call
    // cardreader_rpc_client_process_callbacks() as soon as possible.
    cardreader_rpc_client_on_callback_recvd_cb_t on_rpc_callback_recvd;
    // Called when server disconnected.
    cardreader_rpc_client_on_server_disconnected_cb_t on_server_disconnected;
    // Called when the rpc client starts receiving from the server.
    cardreader_rpc_client_on_start_thread_cb_t on_start_thread;
    // Called when the rpc client stops receiving from the server.
    cardreader_rpc_client_on_end_thread_cb_t on_end_thread;
    void *context;
} cardreader_rpc_client_cfg_t;

typedef void (*cardreader_rpc_client_recv_trustzone_response_cb_t)(void *context, squareup_client_mock_trustzone_TzToHlosResponse const *rsp);
typedef void (*cardreader_rpc_client_recv_session_key_status_cb_t)(void *context, bool has_session_key);
typedef struct {
    // Called when the rpc client receives a response from Trustzone
    cardreader_rpc_client_recv_trustzone_response_cb_t recv_tz_rsp_cb;
    cardreader_rpc_client_recv_session_key_status_cb_t session_key_status_cb;
    void *context;
} cardreader_rpc_client_tz_callback_api_t;

// Create a new cardreader RPC client
cardreader_rpc_client_t *cardreader_rpc_client_create(cardreader_rpc_client_cfg_t const *cfg);

// Builds a cr_comms_driver API structure to be passed into a cr_cardreader instance
cr_comms_driver_api_t cardreader_rpc_client_build_comms_driver_api(cardreader_rpc_client_t *client);

// Sets the callback for messages received from trustzone.
void cardreader_rpc_client_set_callback_api(cardreader_rpc_client_t *client, cardreader_rpc_client_tz_callback_api_t const *callback_api);

// Send command to KeyAgree Trustzone app (asynchronous response)
bool cardreader_rpc_client_send_cmd_to_trustzone(cardreader_rpc_client_t *client,
                                                 squareup_client_mock_trustzone_HlosToTzCommand *tz_command);

// Ask whether the RPC server has a session key established
bool cardreader_rpc_client_get_session_key_status(cardreader_rpc_client_t *client);

// Send APQ<=>SPE Comms encryption session keys to RPC server for message encryption
bool cardreader_rpc_client_send_session_keys(cardreader_rpc_client_t *client, squareup_client_mock_trustzone_DeriveSharedKeyResponse const *derive_shared_key_rsp);

// Destroy a cardreader RPC client
void cardreader_rpc_client_destroy(cardreader_rpc_client_t *client);

// Connect to a cardreader RPC server via TCP
bool cardreader_rpc_client_connect(cardreader_rpc_client_t *client, char const *hostname, uint16_t port, bool reset_spe);

// Connect to a cardreader RPC server via Unix Domain Socket
bool cardreader_rpc_client_connect_ipc(cardreader_rpc_client_t *client, char const *sockpath, bool reset_spe);

// Called to process queued callback messages
// Do not call from the RPC client receive thread - may cause deadlock.
void cardreader_rpc_client_process_callbacks(cardreader_rpc_client_t *client);

/**
 * @brief Set the callback to be used when receiving the response to a
 *        registration request.
 *
 * @param impl Pointer to the RPC client
 * @param cb   The callback function that will be called when the client
 *             receives a registration request response from the server
 *
 */
void cardreader_rpc_client_set_reg_req_status_recv_cb(void *impl, cardreader_rpc_client_recv_registration_req_status_cb_t cb);

/**
 * @brief Register this client as the given type with the RPC server
 *
 * @param client Pointer to the RPC client
 * @param type The registration type to register the client as
 *
 * @return True if the registration was successful, false otherwise
 */
bool cardreader_rpc_client_set_client_registration_type(cardreader_rpc_client_t *client, cr_rpc_client_registration_type_t type);
