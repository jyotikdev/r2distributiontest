// Copyright 2017 Square, Inc.

#pragma once

#include <securesession_host_config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool SECURESESSION_HOST_WRAPPER_sha256_hash(uint8_t const *data, size_t len, uint8_t *digest);
bool SECURESESSION_HOST_WRAPPER_wbsha256_hash_redM_greenD(uint8_t const *data, uint32_t len, uint8_t *digest);
bool SECURESESSION_HOST_WRAPPER_sha256_compare(uint8_t const *hash_one, uint8_t const *hash_two);
bool SECURESESSION_HOST_WRAPPER_rand_fill_buf(uint8_t *buf, size_t len);
void *SECURESESSION_HOST_WRAPPER_memset_s(void *ptr, int value, size_t num);
void *SECURESESSION_HOST_WRAPPER_memcpy(void *s1, const void *s2, size_t n);
bool SECURESESSION_HOST_WRAPPER_curve25519_generate_private_key(securesession_host_curve_context_t *context);
bool SECURESESSION_HOST_WRAPPER_curve25519_get_public_key(securesession_host_curve_context_t *context, curve_public_t pk);
bool SECURESESSION_HOST_WRAPPER_curve25519_get_shared_secret(
    securesession_host_curve_context_t *context,
    curve_private_t shared,
    const curve_public_t their_pk);

bool SECURESESSION_HOST_WRAPPER_curve25519_cleanup(securesession_host_curve_context_t *context);

bool SECURESESSION_HOST_WRAPPER_tfit_slice(
    unsigned char const *in,
    unsigned int full_input_len,
    unsigned int input_len,
    unsigned int byte_number,
    unsigned int length,
    void const *slice_table,        // const wbslice_table_t *
    void const *input_byte_order,   // const wbslice_byte_order_t *
    unsigned char *out,
    unsigned int out_offset);

bool SECURESESSION_HOST_WRAPPER_wb_hmac_sha256(
    uint8_t const *key,
    uint32_t const key_length,
    uint8_t const *message,
    uint32_t const message_length,
    uint8_t *result);

bool SECURESESSION_HOST_WRAPPER_wbaes_gcm_encrypt_redK(
    unsigned char *data,
    uint16_t data_len,
    uint8_t const *key,
    int const key_bits,
    uint8_t const *nonce,
    uint8_t const nonce_len,
    uint8_t const *auth_cleartext,
    uint16_t const auth_cleartext_len,
    uint8_t *tag,
    uint8_t const tag_len);

bool SECURESESSION_HOST_WRAPPER_wbaes_gcm_decrypt_redK(
    unsigned char *data,
    uint16_t data_len,
    uint8_t const *key,
    int const key_bits,
    uint8_t const *nonce,
    uint8_t const nonce_len,
    uint8_t const *auth_cleartext,
    uint16_t const auth_cleartext_len,
    uint8_t *tag,
    uint8_t const tag_len);

bool SECURESESSION_HOST_WRAPPER_kdf_nist108_greenK_redOK(
    uint8_t const *key,
    uint16_t const key_len_in_bits,
    uint8_t const *app_label,
    uint16_t app_label_len,
    uint8_t *output_key,
    uint16_t output_key_len_in_bits);

bool SECURESESSION_HOST_WRAPPER_wbtdes_encrypt(
        unsigned char const *in,
        unsigned char *out,
        unsigned char const *key_buf,
        size_t const key_buf_len);

bool SECURESESSION_HOST_WRAPPER_wbtdes_decrypt(
        unsigned char const *in,
        unsigned char *out,
        unsigned char const *key_buf,
        size_t const key_buf_len);

bool SECURESESSION_HOST_WRAPPER_wbtaes128ecb_encrypt(
        unsigned char const *in,
        unsigned char *out,
        unsigned char const *key_buf,
        size_t const key_buf_len);
