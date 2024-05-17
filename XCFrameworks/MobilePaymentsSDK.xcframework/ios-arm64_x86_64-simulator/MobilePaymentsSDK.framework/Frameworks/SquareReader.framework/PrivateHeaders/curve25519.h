#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CURVE25519_KEY_LEN 32

typedef uint8_t curve_private_t[CURVE25519_KEY_LEN];
typedef uint8_t curve_public_t[CURVE25519_KEY_LEN];

bool curve25519_donna(uint8_t *data_out, const uint8_t *data_in, const uint8_t *basepoint);
bool curve25519_get_public_key(curve_public_t pk, const curve_private_t sk);
bool curve25519_get_shared_secret(
    curve_private_t shared,
    const curve_private_t my_sk,
    const curve_public_t their_pk);

