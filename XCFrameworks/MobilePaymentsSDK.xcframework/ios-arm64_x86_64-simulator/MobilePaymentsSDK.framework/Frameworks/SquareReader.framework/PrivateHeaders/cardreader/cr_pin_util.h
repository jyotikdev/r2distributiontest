// Copyright (c) 2022 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief CardReader PIN Utility
 *
 * @details This provides PIN utility function(s).
 * It allows the callers to manipulate PIN without initializing cardreader or any feature of LibCardReader.
 *
 * @author Gokhan Aydeniz <gokhan@squareup.com>
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Formats the PIN digits into a Format4 pinblock. It uses uint8_t pointer
 * for pinblock instead of pinblock_format4_t in order to provide SWIG friendly interface.
 *
 * @param pin PIN digits.
 * @param pin_len PIN digit count.
 * @param pinblock Format4 pinblock buffer used for outcome.
 * @param pin_len Size of Format4 pinblock.
 *
 * @return Success or Failure of the operation.
 */
bool cr_pin_util_get_format4_pinblock(uint8_t const *pin, size_t pin_len, uint8_t *pinblock, size_t pinblock_len);
