// Copyright 2017 Square, Inc.

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

int ecc_curve_compute_y_from_x(uint8_t *w_x_in, size_t inlen, uint8_t *w_y_out);

// Warning: all kinds of hardcoded things -- holdover solution you probably shouldn't use!
// And intentionally restrictive becuase limited time to test, so limited functionality..
bool ecc_point_multiply_by_eight(uint8_t * const x, uint8_t * const y, uint8_t * x_out, uint8_t * y_out);