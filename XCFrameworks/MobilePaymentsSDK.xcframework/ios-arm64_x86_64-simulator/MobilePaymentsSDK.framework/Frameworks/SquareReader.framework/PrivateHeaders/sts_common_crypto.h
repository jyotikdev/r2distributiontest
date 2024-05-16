// Copyright 2023 Square, Inc.

/**
 * @file
 * @defgroup sts_common_crypto STS Common Crypto Operations
 *
 * @brief STS common cryptographic operations.
 *
 * @ingroup sts_common
 * @{
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include <curve25519.h>

/// Index of the SPE certificate in the auth_state proto
#define STS_SPE_CERT_INDEX 0

/// Index of the POS/host certificate in the auth_state proto
#define STS_POS_CERT_INDEX 1

#define STS_LABEL_LEN (32)      //< All labels are the same length
#define STS_KEY_LEN (128 / 8)   //< Common key derivation output length (128-bit)
#define STS_HMAC_LEN (256 / 8)  //< Common HMAC output length
#define STS_KDK_LEN (256 / 8)   //< Key derivation key length

// Common shared secret generation and validation
extern const uint8_t STS_EXTRACTION_LABEL[STS_LABEL_LEN];
extern const uint8_t STS_SESSION_LABEL[STS_LABEL_LEN];
extern const uint8_t STS_SPE_MAC_LABEL[STS_LABEL_LEN];

// SPE generation constants
extern const uint8_t STS_SECRET_KEY_LABEL[STS_LABEL_LEN];
extern const uint8_t STS_SESSION_ID_LABEL[STS_LABEL_LEN];
extern const uint8_t STS_COOKIE_KEY_LABEL[STS_LABEL_LEN];

// EMV
extern const uint8_t STS_PIN_KEY_LABEL[STS_LABEL_LEN];

/**
 * @brief Convert a uint64 (e.g. session ID) into a serialized byte array.
 *
 * @param value The value to convert.
 * @param[out] output Output buffer. Must be 8 bytes in length.
 */
void sts_crypto_uint64_to_byte_array(uint64_t value, uint8_t *output);

/**
 * @brief Convert a uint32 (e.g. a length or counter) to a serialized byte array.
 *
 * @param value The value to convert.
 * @param[out] output Output buffer. Must be 4 bytes in length.
 */
void sts_crypto_uint32_to_byte_array(uint32_t value, uint8_t *output);

/**
 * @brief KDF used by STS.
 *
 * The output buffer must be at most STS_HMAC_LEN in length, but may be smaller.
 *
 * @param kdk The Key derivation key for the session.
 * @param kdk_len Length of the KDK. (Must be STS_KDK_LEN)
 * @param label HMAC label.
 * @param label_len Length of the label. (Must be STS_LABEL_LEN)
 * @param context Addional bytes for input into the KDF
 * @param context_len Length of context (Must be less than or equal to STS_KDK_CONTEXT_MAX_LEN)
 * @param[out] output Output buffer to place the HMAC digest.
 * @param output_len Number of bytes to place in the output buffer. Must be between STS_HMAC_LEN and STS_KEY_LEN.
 */
void sts_crypto_kdf(uint8_t const *kdk, size_t kdk_len,
                    uint8_t const *label, size_t label_len,
                    uint8_t const *context, size_t context_len,
                    uint8_t *output, size_t output_len);

/**
 * @brief Generic key derivation, using HMAC-SHA256 under the hood.
 *
 * The output buffer must be at most STS_HMAC_LEN in length, but may be smaller.
 *
 * @param kdk The Key derivation key for the session.
 * @param kdk_len Length of the KDK. (Must be STS_KDK_LEN)
 * @param label HMAC label.
 * @param label_len Length of the label. (Must be STS_LABEL_LEN)
 * @param session_id The session ID.
 * @param[out] output Output buffer to place the HMAC digest.
 * @param output_len Number of bytes to place in the output buffer. Must be between STS_HMAC_LEN and STS_KEY_LEN.
 */
void sts_crypto_derive_key(uint8_t const *kdk, size_t kdk_len,
                           uint8_t const *label, size_t label_len,
                           uint64_t session_id,
                           uint8_t *output, size_t output_len);

/**
 * @brief Perform NIST 800-56A C(2e, 2s) using Curve25519 plus randomness extraction to create the KDK (key derivation key).
 *
 * The output buffer must be STS_KDK_LEN in length.
 *
 * @param public_s "Their" public static key (from the certificate).
 * @param private_s "My" private static key (from the certificate).
 * @param public_e "Their" ephemeral public key.
 * @param private_e "My" ephemeral private key.
 * @param[out] kdk_out Output buffer to place the KDK.
 * @param kdk_out_len Number of bytes to place in the output buffer. (Must be STS_KDK_LEN)
 * @returns True if successful, false if there's an error.
 */
bool sts_crypto_key_exchange(const curve_public_t public_s, const curve_private_t private_s,
                             const curve_public_t public_e, const curve_private_t private_e,
                             uint8_t *kdk_out, size_t kdk_out_len);

/**
 * @brief Generate the STS key confirmation MAC.
 *
 * This does NOT perform comparison, but only generation of the resultant HMAC. The caller must compare the output with
 * an expected value and zeroize immediately after use.
 *
 * The output buffer length must be STS_HMAC_LEN.
 * The KDK length must be STS_KDK_LEN.
 *
 * @param session_id Session ID
 * @param kdk Key derivation key. Output of sts_crypto_key_exchange().
 * @param kdk_len Length of the KDK. (Must be STS_KDK_LEN)
 * @param pub_key_spe The SPE's ephemeral public key.
 * @param pub_key_pos The POS's ephemeral public key.
 * @param cert_id_spe The SPE's certificate ID.
 * @param cert_id_spe_len The length of the SPE's certificate ID.
 * @param cert_id_pos The POS's certificate ID.
 * @param cert_id_pos_len The length of the POS's certificate ID.
 * @param[out] output Output buffer to place the key confirmation.
 * @param output_len Number of bytes to place in the output buffer.
 */
void sts_crypto_gen_key_confirm(uint64_t session_id,
                                uint8_t const *kdk, size_t kdk_len,
                                const curve_public_t pub_key_spe, const curve_public_t pub_key_pos,
                                uint8_t const *cert_id_spe, size_t cert_id_spe_len,
                                uint8_t const *cert_id_pos, size_t cert_id_pos_len,
                                uint8_t *output, size_t output_len);
/**
 * @brief Authenticated encryption using STS. Message can be decrypted using sts_crypto_decrypt_comms().
 *
 * @note Assumes the caller provided a buffer that looks like:
 *       auth_cleartext bytes + blank bytes + data bytes
 *       where the sections are auth_cleartext_len, crypto_hdr_size, and data_len bytes in
 *       length, respectively
 *
 * @param kdk Key derivation key which is used to derive the session key.
 * @param kdk_len
 * @param session_id Current session ID. Used along with the KDK to derive the session key.
 * @param data Data to be encrypted.
 * @param data_len
 * @param auth_cleartext Additonal unencrypted authenticated data.
 * @param auth_cleartext_len
 * @return true Encryption was successful.
 * @return false Encryption failed.
 */
bool sts_crypto_encrypt_comms(uint8_t const *kdk, size_t kdk_len,
                              uint64_t session_id,
                              uint8_t *data, uint16_t data_len,
                              const uint8_t *auth_cleartext, const uint16_t auth_cleartext_len);

/**
 * @brief Decrypts and authenticates a message encrypted with sts_crypto_encrypt_comms().
 *
 * @note Assumes the caller provided a buffer that looks like:
 *       auth_cleartext bytes + blank bytes + data bytes
 *       where the sections are auth_cleartext_len, crypto_hdr_size, and data_len bytes in
 *       length, respectively
 *
 * @param kdk Key derivation key which is used to derive the session key.
 * @param kdk_len
 * @param session_id Current session ID. Used along with the KDK to derive the session key.
 * @param data Data to be encrypted.
 * @param data_len
 * @param auth_cleartext Additonal unencrypted authenticated data.
 * @param auth_cleartext_len
 * @return true Encryption was successful.
 * @return false Decryption or authentication failed.
 */
bool sts_crypto_decrypt_comms(uint8_t const *kdk, size_t kdk_len,
                              uint64_t session_id,
                              uint8_t *data, uint16_t data_len,
                              const uint8_t *auth_cleartext, const uint16_t auth_cleartext_len);

/** @} */
