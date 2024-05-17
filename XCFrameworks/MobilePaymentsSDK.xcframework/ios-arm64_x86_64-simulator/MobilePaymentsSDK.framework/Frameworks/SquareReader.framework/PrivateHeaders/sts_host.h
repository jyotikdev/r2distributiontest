// Copyright 2023 Square, Inc.

/**
 * @file
 * @defgroup sts_host_crypto STS Host Crypto API
 *
 * @brief Basic STS host crypto functionality.
 *
 * This implements cryptographic operations for the LCR STS feature using in-memory ephemeral keys.
 *
 * A persistent host certificate key can be (re)loaded using `sts_host_set_static_cert_key()`.
 *
 * @ingroup sts_common
 * @{
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#include <sts_host_common.h>

struct sts_host_context_t;
struct cr_sts_feature_crypto_api_t;
struct crs_securesession_comms_api_t;

/**
 * @brief Allocate and initialize a basic STS host
 *
 * @param host_type The STS host type.
 * @param serial Host serial number (may be NULL).
 * @param serial_len Host serial number length.
 * @param software_version Software version encoded as a 32-bit unsigned integer.
 * @returns A new sts_host_context_t structure.
 */
struct sts_host_context_t *sts_host_create(sts_host_type_e host_type,
                                           uint8_t const *serial,
                                           size_t serial_len,
                                           uint32_t software_version);

/**
 * @brief Make the host use the given static certificate key.
 *
 * This allows the STS host to resume operations with a related, cached authentication state.
 *
 * @param host_context The created sts host structure.
 * @param private_key Pointer to a curve25519 private key.
 * @param private_key_len Length of the private key. Must be 32 bytes.
 */
void sts_host_set_static_cert_key(struct sts_host_context_t *host_context,
                                  uint8_t const *private_key,
                                  size_t private_key_len);

/**
 * @brief Free a basic STS host context
 *
 * @param host_context The allocated sts host structure.
 */
void sts_host_free(struct sts_host_context_t *host_context);

/**
 * @brief Populate a STS Crypto API for the given sts host context.
 *
 * @param host_context The created sts host structure.
 * @param [out] crypto_api An output Crypto API structure.
 */
void sts_host_setup_crypto_api(struct sts_host_context_t *host_context, struct cr_sts_feature_crypto_api_t *crypto_api);

/**
 * @brief Populate a comms API to register with the cardreader for encrypting/decrypting traffic
 *
 * @param host_context The created sts host structure.
 * @param [out] comms_api An output comms API structure.
 */
void sts_host_setup_comms_api(struct sts_host_context_t *host_context, struct crs_securesession_comms_api_t *comms_api);

/** @} */
