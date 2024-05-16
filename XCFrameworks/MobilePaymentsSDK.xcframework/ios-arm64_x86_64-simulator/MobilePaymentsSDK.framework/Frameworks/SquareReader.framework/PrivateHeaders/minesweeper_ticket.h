// Copyright 2018 Square, Inc.
#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool minesweeper_get_ticket(uint8_t **ticket_ptr, size_t *ticket_len);

//  Used to inject entropy into Minesweeper's PRNG for SPOC
bool ms_crypto_rand_inject_entropy(const uint8_t *entropy, size_t n_bytes);