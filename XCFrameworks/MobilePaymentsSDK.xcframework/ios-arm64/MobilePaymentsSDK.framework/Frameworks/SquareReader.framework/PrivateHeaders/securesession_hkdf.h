#pragma once

// Per http://tools.ietf.org/html/rfc5869
//
// Only using SHA256, though

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define HKDF_HASHLEN (32) // NOT CONFIGURABLE FOR NOW (TODO)

// RFC has application-specific information as configurable length, capping it for our purposes
#define HKDF_MAX_APP_INFO_LEN HKDF_HASHLEN

typedef int (*hkdf_hmac_fn_t)(const uint8_t *key, const uint32_t keyLength, const uint8_t *message, const uint32_t messageLength, uint8_t *result);

// If app_info is not provided, app_info_len should be set to 0 (same for salt + salt_len)
// If app_info_len is > HKDF_HASHLEN, the extra bytes are ignored
bool hkdf(const uint8_t   *salt,
          uint16_t        salt_len,
          const uint8_t   *initial_key,
          uint16_t        initial_key_len,
          const uint8_t   *app_info,
          uint16_t        app_info_len,
          uint8_t         *output_key,
          uint16_t        output_key_len,
          hkdf_hmac_fn_t  hmac_fn);
