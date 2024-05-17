// Copyright 2023 Square, Inc.

/**
 * @file
 * @defgroup sts_host_squid_crypto STS Host SQUID Crypto API
 *
 * @brief STS host Squid Secure Session crypto functionality.
 *
 * For Squid SS the static session keys are held in trustzone.
 * This module implements the STS host crypto API by communicating with Trustzone as needed for operations.
 *
 * @ingroup sts_common
 * @{
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <sts_host_common.h>

struct sts_host_squid_context_t;

/**
 * @brief Function used to send/receive commands to/from sqsee.
 *
 * @param context Context used by the function. Whatever context is given to sts_host_squid_create will be passed every time the function is called.
 * @param serialized_command Serialized command to be sent to sqsee.
 * @param[out] serialized_response Response from sqsee.
 *
 */
typedef bool (*sqsee_send_cmd_fn)(void * context, const uint8_t *serialized_command, size_t serialized_command_len,
                                  uint8_t *serialized_response_out, size_t *serialized_response_len_out);

/**
 * @brief Allocate and initialize a basic STS host
 *
 * @param host_type The STS host type.
 * @param serial Host serial number (may be NULL).
 * @param serial_len Host serial number length.
 * @param software_version Software version encoded as a 32-bit unsigned integer.
 * @param sqsee_send Function for comunicating with TZ.
 * @param sqsee_send_context Context to be passed to the sqsee_send function.
 * @returns A new sts_host_squid_context_t structure.
 */
struct sts_host_squid_context_t *sts_host_squid_create(sts_host_type_e host_type,
                                                       uint8_t const *serial, size_t serial_len,
                                                       uint32_t software_version,
                                                       sqsee_send_cmd_fn sqsee_send, void* sqsee_send_context);

/**
 * @brief Free a basic STS host context
 *
 * @param host_context The allocated sts host structure.
 */
void sts_host_squid_free(struct sts_host_squid_context_t *host_context);

/**
 * @brief Populate a STS Crypto API for the given sts host context.
 *
 * @param host_context The created sts host structure.
 * @param [out] crypto_api An output Crypto API structure.
 */
void sts_host_squid_setup_crypto_api(struct sts_host_squid_context_t *host_context, struct cr_sts_feature_crypto_api_t *crypto_api);

/**
 * @brief Populate a comms API to register with the cardreader for encrypting/decrypting traffic
 *
 * @param host_context The created sts host structure.
 * @param [out] comms_api An output comms API structure.
 */
void sts_host_squid_setup_comms_api(struct sts_host_squid_context_t *host_context, struct crs_securesession_comms_api_t *comms_api);

/** @} */
