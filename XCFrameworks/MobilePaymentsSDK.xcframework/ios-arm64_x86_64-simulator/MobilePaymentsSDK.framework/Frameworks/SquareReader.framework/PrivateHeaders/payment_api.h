// Copyright 2019 Square, Inc.

#pragma once

#define PAYMENT_BUFFER_SIZE 40000


#include <payment_platform_api.h>

void* payment_init(const payment_platform_api_t* api, void* buf, size_t sz);
void payment_process_msg(void* context, void* msg);
