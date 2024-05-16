// Copyright 2017 Square, Inc.

#pragma once

#include <cardreader/cr_comms_backend_api.h>
#include <cardreader_rpc_server.h>

void cardreader_rpc_server_backend_set_server(cardreader_rpc_server_backend_t *backend, cardreader_rpc_server_t *server);
void cardreader_rpc_server_backend_destroy(cardreader_rpc_server_backend_t *backend);
cr_comms_backend_api_t cardreader_rpc_server_backend_get_comms_backend_api(cardreader_rpc_server_backend_t *backend);
bool cardreader_rpc_server_backend_connect(cardreader_rpc_server_backend_t *backend);
void cardreader_rpc_server_backend_disconnect(cardreader_rpc_server_backend_t *backend);
