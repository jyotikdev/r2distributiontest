// Copyright 2017 Square, Inc.

#pragma once

#include <cardreader_rpc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define CR_RPC_SERVER_UNINITIALIZED_SOCKET -1

enum {
    CARDREADER_RPC_SERVER_DEFAULT_PORT = 50016
};

typedef struct cardreader_rpc_server_t cardreader_rpc_server_t;
typedef struct cardreader_rpc_server_backend_t cardreader_rpc_server_backend_t;

typedef int (*cardreader_rpc_server_reset_spe_cb_t)(void);

typedef struct {
    cardreader_rpc_server_reset_spe_cb_t reset_spe;
} cardreader_rpc_server_host_api_t;

typedef void (*cardreader_rpc_server_set_recv_resp_cb_t)(void *recv_resp_cb);
typedef bool (*cardreader_rpc_server_send_cmd_t)(void *sq_see_client, const uint8_t *serialized_command, size_t serialized_command_len, uint8_t *serialized_response_out, size_t *serialized_response_len_out);

typedef struct {
    cardreader_rpc_server_send_cmd_t send_cmd;
    void *context;
} cardreader_rpc_server_sq_see_api_t;

// Enable logging.
void cardreader_rpc_server_enable_logging(void);

// Create a USB cardreader backend
cardreader_rpc_server_backend_t *cardreader_rpc_server_backend_usb_create(void);

// Create a UART cardreader backend
cardreader_rpc_server_backend_t *cardreader_rpc_server_backend_uart_create(int fd);

/**
 * @brief Get the current count of clients connected to the RPC server
 *
 * @param server Pointer to the RPC server
 *
 * @return The number of clients connected to the RPC server
 */
size_t cardreader_rpc_server_get_client_count(cardreader_rpc_server_t *server);

/**
 * @brief Copy the array of the RPC server's connected sockets to the given array
 *
 * @param server            Pointer to the RPC server
 * @param socket_array      Array to be modified, caller should ensure that the size of the
 *                          array matches the size passed to cardreader_rpc_server_create
 * @param socket_array_size Size that was allocated for the passed-in socket array, will be
 *                          validated against the size passed to cardreader_rpc_server_create
 */
void cardreader_rpc_server_get_client_list(cardreader_rpc_server_t *server, int socket_array[], size_t socket_array_size);

/**
 * @brief Create and initialize an RPC server
 *
 * Creates an RPC server with the associated backend, host API, and SQ SEE API
 * capable of servicing up to a given maximum number of clients.
 *
 * @param backend The backend structure associated with this frontend RPC server
 * @param host_api The API for host-related commands
 * @param sq_see_api The sqsee API used to access trustzone
 * @param max_clients The maximum number of clients that this RPC server should service
 *
 * @return A pointer to the created RPC server, or NULL on failure
 */
cardreader_rpc_server_t *cardreader_rpc_server_create(cardreader_rpc_server_backend_t *backend, cardreader_rpc_server_host_api_t* host_api, cardreader_rpc_server_sq_see_api_t *sq_see_api, size_t max_clients);

/**
 * @brief Disconnect from the SPE and cleanup RPC server resources
 *
 * @param server The RPC server to cleanup
 */
void cardreader_rpc_server_destroy(cardreader_rpc_server_t *server);

/**
 * @brief Add a client to an RPC server.
 *
 * Traverses the RPC server's client socket array and adds the given client socket
 * to the first unused index in the array. If there are no open slots in the array
 * (the RPC server is at capacity for maximum number of clients) failure is returned.
 *
 * @param server Pointer to the RPC server
 * @param client_socket The client socket to add
 * @param is_ipc_client True if the client connected via a Unix socket
 * @param client_type The client type to register this client as by default
 *
 * @return True if the client socket was successfully added, false otherwise.
 */
bool cardreader_rpc_server_add_client(cardreader_rpc_server_t *server, int client_socket, bool is_ipc_client, cr_rpc_client_registration_type_t client_type);

/**
 * @brief Remove a client from the RPC server.
 *
 * Searches the RPC server's client socket array for the given socket number and, if
 * found, closes the socket and removes that client from the array.
 *
 * @param server Pointer to the RPC server
 * @param client_socket The client socket to remove
 *
 * @return True if the client socket was successfully removed, false otherwise.
 */
bool cardreader_rpc_server_remove_client(cardreader_rpc_server_t *server, int client_socket);

/**
 * @brief Return the number of connected clients using an IPC socket for communication
 *
 * @param server Pointer to the RPC server
 *
 * @return The number of IPC clients
 */
size_t cardreader_rpc_server_get_num_ipc_clients(cardreader_rpc_server_t *server);

/**
 * @brief Return the number of connected clients using a TCP socket for communication
 *
 * @param server Pointer to the RPC server
 *
 * @return The number of TCP clients
 */
size_t cardreader_rpc_server_get_num_tcp_clients(cardreader_rpc_server_t *server);

/**
 * @brief Run the RPC server a single time to address a single request from a client.
 *
 * Instructs the RPC server to listen for and service a request from the given
 * client socket.
 *
 * @param server Pointer to the RPC server
 * @param socket_num The client socket to service
 *
 * @return True if the client socket was successfully serviced, false otherwise.
 */
bool cardreader_rpc_server_run_once(cardreader_rpc_server_t *server, int socket_num);

/**
 * @brief Run the RPC server continually to address requests from a client.
 *
 * Instructs the RPC server to listen for and service requests from the given
 * client socket.  This function will only return if an error occurs during the
 * servicing of this client or if the client disconnects.
 *
 * @param server Pointer to the RPC server
 * @param socket_num The client socket to service
 */
void cardreader_rpc_server_run(cardreader_rpc_server_t *server, int socket_num);
