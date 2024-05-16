// Copyright 2022 Square, Inc.

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define LSPE_FINGERPRINT_SIZE_BYTES 32

// Represents a fingerprint of the current LSPE instance.
typedef struct lspe_fingerprint_t {
    uint8_t bytes[LSPE_FINGERPRINT_SIZE_BYTES];
} lspe_fingerprint_t;

// Initialize the LSPE policy. It does nothing if the policy has already been 
// initialized. It must be called at least once prior to calling other 
// functions (e.g., lspe_get_fingerprint).
// 
// Returns true if successful, or false if any error occurs.
bool lspe_initialize_policy(void);

// Gets the LSPE fingerprint.
// lspe_initialize_policy should be called at least once prior to calling this function.
// Returns true if successful, or false if unable to get the fingerprint.
bool lspe_get_fingerprint(lspe_fingerprint_t *fp);
