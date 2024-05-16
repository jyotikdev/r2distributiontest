// Copyright (c) 2014 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief Cardreader Firmware Update Feature
 *
 * @details This file defines the APIs for performing a firmware update of a
 * Square hardware device.
 *
 * @author Rodion Steshenko <rodion@squareup.com>
 *
 * @copyright 2014 Square, Inc.
 */

/**
 * @defgroup lcr_fwup Cardreader Firmware Update
 *
 * @ingroup lcr
 *
 * @brief Cardreader Firmware Update Feature
 *
 * Overview
 * ========
 *
 * Firmware update is the process by which the running firmware on a reader is
 * updated to a new version of the firmware. It is similar to the process of
 * software deployment for mobile or web applications, but for firmware, which
 * is the low level software that runs on a hardware device. This process
 * allows for hardware devices that have been shipped to customers to
 * continually receive changes to fix issues, introduce new features, and patch
 * security vulnerabilities.
 *
 * As the hardware devices do not have network capabilities of their own, they
 * rely on a connected host to retrieve and send them the update images. LCR
 * acts as the bridge for sending the assets down to the connected Square
 * hardware device.
 *
 * Tarkin
 * ======
 *
 * Once units are in the field, to determine if a new firmware update is
 * necessary for the device, a connected host communicates with the Tarkin
 * server. Tarkin is the service that manages the distribution of firmware to
 * devices in the field. On receipt of a firmware manifest, the host must send
 * the manifest up to Tarkin to check if there is an update. If there is an
 * update, it must then attempt to send the update to the reader. Updates
 * should be continually checked for, either via some polling scheme, or push
 * notifications.
 *
 * @{
 */

#pragma once

#include <cardreader_shared/crs_fwup_msgs.h>
#include <cardreader_shared/crs_pack.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Forward declaration for the firmware update feature.
 */
struct cr_firmware_update_feature_t;

/**
 * @brief Forward declaration for the Cardreader instance.
 */
struct cr_cardreader_t;

/**
 * @brief Enumeration of the possible result codes from the
 * `cr_firmware_update_*()` API functions.
 */
typedef enum {
    /**
     * @brief API call was successful.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS,

    /**
     * @brief Generic catch-all error code. If you see this, it is quite
     * unfortunate for you.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_ERROR,

    /**
     * @brief Invalid parameter was passed to the API function call.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_INVALID_PARAMETER,

    /**
     * @brief Cannot perform firmware update due to missing device manifest.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_MISSING_MANIFEST,

    /**
     * @brief Error encountered when sending request to the device, or if there
     * is an issue with the CardReader instance.
     *
     * @note The "session" in this context corresponds to the feature lifecycle
     * session.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_SESSION_ERROR,

    /**
     * @brief Firmware Update Feature was not initialized before an API
     * function callw as attempted; #cr_firmware_update_feature_init() must be
     * called before any other API functions are called.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_NOT_INITIALIZED,

    /**
     * @brief Freeing of the Firmware Update Feature was attempted when the
     * feature has not yet been terminated.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_ALREADY_TERMINATED,

    /**
     * @brief Request made during an invalid state of the Firmware Update
     * Feature.
     */
    CR_FIRMWARE_UPDATE_FEATURE_RESULT_CALL_UNEXPECTED,
} cr_firmware_update_feature_result_t;

/**
 * @brief Identifier for the asset whose version number is being reported.
 */
typedef enum {
    /**
     * @brief K21 firmware asset.
     *
     * @note The K21 firmware asset is slotted between slot A and slot B. This
     * does not indicate the slot to which the asset belongs, but whichever
     * version is reported corresponds to the running slot.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_K21 = 0,

    /**
     * @brief K400 CPU0 firmware asset.
     *
     * @note There is no slot information for this asset.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_K400_CPU0,

    /**
     * @brief K21 CPU1 firmware asset.
     *
     * @note There is no slot information for this asset.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_K400_CPU1,

    /**
     * @brief Version information for the BLE firmware image.
     *
     * @note For the EFR32 BLE IC, see
     * #CR_FIRMWARE_UPDATE_VERSION_INFO_EFR32_FIRMWARE, and
     * #CR_FIRMWARE_UPDATE_VERSION_INFO_EFR32_BOOTLOADER.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_BLE,

    /**
     * @brief TMS (Terminal Management Software) assest.
     *
     * @note This asset is not slotted.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_TMS,

    /**
     * @brief Certificate Authority Public Keys (CAPKS) asset.
     *
     * @note This asset is not slotted.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_CAPKS,

    /**
     * @brief TMS (Terminal Management Software) + CAPKS (Certificate Authority
     * Public Keys) asset.
     *
     * @note The TMS+CAPKS asset is slotted between slot A and slot B. This
     * does not indicate the slot to which the asset belongs, but whichever
     * version is reported corresponds to the running slot.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_TMS_CAPKS,

    /**
     * @brief FPGA (Field Programmable Gate Array) asset.
     *
     * @note This asset is not slotted.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_FPGA,

    /**
     * @brief K450 CPU0 firmware asset.
     *
     * @note The K450 CPU0 firmware asset is slotted between slot A and slot B.
     * This does not indicate the slot to which the asset belongs, but
     * whichever version is reported corresponds to the running slot.
     *
     * @note This is also reported for K480 CPU0.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_K450_CPU0,

    /**
     * @brief K450 CPU1 firmware asset.
     *
     * @note The K450 CPU1 firmware asset is slotted between slot A and slot B.
     * This does not indicate the slot to which the asset belongs, but
     * whichever version is reported corresponds to the running slot.
     *
     * @note This is also reported for K480 CPU1.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_K450_CPU1,

    /**
     * @brief Firmware asset for the EFR32.
     *
     * @note This asset is not slotted.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_EFR32_FIRMWARE,

    /**
     * @brief Bootloader asset for the EFR32.
     *
     * @note This asset is not slotted.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_EFR32_BOOTLOADER,

    /**
     * @brief NFC configuration asset.
     *
     * @note This asset is not slotted.
     */
    CR_FIRMWARE_UPDATE_VERSION_INFO_NFC_CONFIG
} cr_firmware_update_version_info_t;

/**
 * @brief Structure representing a version info pair.
 *
 * @details A version info pair contains an asset identifier, and associated
 * version number for a reader asset.
 *
 * ```
 * {
 *     .name = CR_FIRMWARE_UPDATE_VERSION_INFO_K21,
 *     .version = "1.2.3"
 * }
 * ```
 */
typedef struct {
    /**
     * @brief Identifier for the firmware asset.
     */
    cr_firmware_update_version_info_t name;

    /**
     * @brief Version number string.
     *
     * @details Version numbers are encoded as a string composed of three
     * components: `major`, `minor`, and `revision`. The string representation
     * appears as `"<major>.<minor>.<revision>"`. For example, with major as
     * `3`, minor as `61`, and minor as `100`, the string representation would
     * be `"3.61.100"`. A revision number is up to three digits, and a minor
     * number is up to two digits; leading zeros are omitted. The numeric
     * representation for `"3.61.100"` is `361100`.
     */
    char version[16];
} cr_firmware_update_version_info_pair_t;

/**
 * @brief Callback invoked by LCR once a firmware manifest has been received
 * from the reader.
 *
 * @details This callback is invoked asynchronously by LCR in response to a
 * #cr_firmware_update_feature_get_manifest() API call. The received manifest
 * may be either a signed (V2) manifest, or un-signed (V1) manifest.
 *
 * @param context         User-supplied context pointer.
 * @param firmware_update Pointer to the Firmware Update feature.
 * @param manifest        Firmware manifest from the reader.
 * @param manifest_len    Length of the @p manifest in bytes.
 * @param required_update If `true`, then the reader requires an update in
 *                        order to proceed communication with the host.
 * @param result          Result of the manifest request:
 *   - #CR_FIRMWARE_UPDATE_FEATURE_RESULT_ERROR if manifest
 *     was retrieved sucessfully.
 *   - #CR_FIRMWARE_UPDATE_FEATURE_RESULT_ERROR if an error
 *     occurred in the API call.
 *   - #CR_FIRMWARE_UPDATE_FEATURE_RESULT_MISSING_MANIFEST
 *     if manifest was empty.
 */
typedef void (*cr_firmware_update_receive_manifest_cb_t)(void *context,
                                                         struct cr_firmware_update_feature_t *firmware_update,
                                                         void const *manifest,
                                                         size_t manifest_len,
                                                         bool required_update,
                                                         cr_firmware_update_feature_result_t result);

/**
 * @brief Callback invoked by LCR during the process of a firmware update.
 *
 * @details This callback is invoked 1 or more times by LCR during the process
 * of a firmware update to notify the host of the progress of the update. This
 * callback is invoked asynchronously in response to a
 * #cr_firmware_update_feature_send_data() before the completion of the FWUP.
 * Once the FWUP completes, either successfully or not, then
 * #cr_firmware_update_result_cb_t() is invoked.
 *
 * @param context               User-supplied context pointer.
 * @param firmware_update       Pointer to the Firmware Update feature instance.
 * @param completion_percentage Percentage of the firmware update, in the range
 *                              `(0 - 100)` as a whole integer.
 */
typedef void (*cr_firmware_update_progress_cb_t)(void *context,
                                                 struct cr_firmware_update_feature_t *firmware_update,
                                                 int completion_percentage);

/**
 * @brief Callback invoked once LCR has completed sending a firmware update
 * image to a reader.
 *
 * @details This callback is invoked asynchronously in response to a
 * #cr_firmware_update_feature_send_data().
 *
 * @param context         User-supplied context pointer.
 * @param firmware_update Pointer to the Firmware Update feature instance.
 * @param result          Result of the firmware update.
 */
typedef void (*cr_firmware_update_result_cb_t)(void *context,
                                               struct cr_firmware_update_feature_t *firmware_update,
                                               crs_fwup_result_t result);

/**
 * @brief Callback invoked by LCR when a SPE huamn readable version number has
 * been extracted from a received firmware manifest.
 *
 * @details The version returned by this callback is indicative of the main
 * CPU's main firmware image that is reported for PTS / PCI compliance. This
 * is a string of the format `SCRP.<pts_version>` or `PTS.<pts_version>` where
 * the PTS version is of the format `<pts>.<major>.<minor>.<revision>`. For
 * deatils around `<major>.<minor>.<revision>`, see
 * #cr_firmware_update_version_info_pair_t.
 *
 * @param context         User-supplied context pointer.
 * @param firmware_update Pointer to the Firmware Update feature instance.
 * @param version         PTS version string.
 */
typedef void (*cr_firmware_update_spe_version_cb_t)(void *context,
                                                    struct cr_firmware_update_feature_t *firmware_update,
                                                    char *version);

/**
 * @brief Callback invoked by LCR when per-asset version numbers are extracted
 * from a received firmware manifest.
 *
 * @details The firmware version pair array contains an array of asset
 * identifiers, and their associated version number in string representation.
 * See #cr_firmware_update_version_info_pair_t for more information.
 *
 * @param context         User-supplied context pointer.
 * @param firmware_update Pointer to the Firmware Update feature instance.
 * @param infos           Array of asset - version identifier pairs.
 * @param infos_count     Number of entries in the @p infos array.
 */
typedef void (*cr_firmware_update_version_info_cb_t)(void *context,
                                                     struct cr_firmware_update_feature_t *firmware_update,
                                                     cr_firmware_update_version_info_pair_t *infos,
                                                     size_t infos_count);

/**
 * @brief Callback invoked once the country of a device is determined.
 *
 * @details This callback is invoked in response to LCR receiving a TMS
 * (Terminal Management Software) country code.
 *
 * @param context            User-supplied context pointer.
 * @param firmware_update    Pointer to the Firmware Update feature instance.
 * @param[out] country_code  Output pointer to store the country code string.
 *
 * @note Country codes are 2 character strings, such as `"ca"` for `Canada`,
 * and `"us"` for `United States`. The full list of country codes is specified
 * by ISO 3166-1 Alpha-2.
 */
typedef void (*cr_firmware_update_tms_country_cb_t)(void *context,
                                                    struct cr_firmware_update_feature_t *firmware_update,
                                                    char *country_code);

/**
 * @brief Callback API structure for the Firmware Update feature.
 */
typedef struct cr_firmware_update_feature_event_api_t {
    /**
     * @brief Callback invoked by the Firmware Update feature once a device
     * manifest has been received.
     */
    cr_firmware_update_receive_manifest_cb_t on_receive_manifest;

    /**
     * @brief Callback invoked by the Firmware Update feature to update the
     * host on the progress of a firmware update before completion.
     */
    cr_firmware_update_progress_cb_t on_progress;

    /**
     * @brief Callback invoked by the Firmware Update feature once a firmware
     * update has completed.
     *
     * @note This is invoked in *both* success and failure cases.
     */
    cr_firmware_update_result_cb_t on_result;

    /**
     * @brief Callback invoked by the Firmware Update feature once a manifest
     * has been received, and per-asset version numbers decoded.
     */
    cr_firmware_update_version_info_cb_t on_version_info;

    /**
     * @brief Callback invoked by the Firmware Update feature once the country
     * of a device has been determined.
     */
    cr_firmware_update_tms_country_cb_t on_tms_country;

    /**
     * @brief Callback invoked by the Firmware Update feature once a manifest
     * has been received and a human readable PTS version string is extracted.
     */
    cr_firmware_update_spe_version_cb_t on_spe_version;

    /**
     * @brief User-specified context pointer to pass to registered event API
     * callbacks.
     */
    void *context;
} cr_firmware_update_feature_event_api_t;

/**
 * @brief Allocates memory for a Firmware Update feature instance.
 *
 * @return Pointer to memory for a Firmware Update feature instance
 * (`cr_firmware_update_feature_t`).
 *
 * @note The returned feature instance should be initialized by a call to
 * #cr_firmware_update_feature_init() before attempting to use it with any
 * other Firmware Update API methods.
 */
struct cr_firmware_update_feature_t *cr_firmware_update_feature_alloc(void);

/**
 * @brief Initializes a Firmware Update feature instance.
 *
 * @details This method is used to initialize the feature instance with the
 * specified callbacks. The feature will be registered with the specified
 * @p cardreader instance.
 *
 * @param firmware_update Pointer to the Firmware Update feature to initialize.
 * @param cardreader      CardReader instance to bind the feature to.
 * @param api             Pointer to the API callbacks to bind to the feature
 *                        instance.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS if the feature was
 * successfully initialized, otherwise an error code as defined in
 * #cr_firmware_update_feature_result_t.
 *
 * @note The @p firmware_update feature must have been allocated by a call to
 * #cr_firmware_update_feature_alloc().
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_init(
    struct cr_firmware_update_feature_t *firmware_update,
    struct cr_cardreader_t *cardreader,
    cr_firmware_update_feature_event_api_t const *api);

/**
 * @brief Terminates a Firmware Update feature instance, de-registering the
 * feature from its bound CardReader instance.
 *
 * @param firmware_update Pointer to the Firmware Update feature instance.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS if the feature was
 * successfully terminated, otherwise an error code as defined in
 * #cr_firmware_update_feature_result_t.
 *
 * @note Feature must have been initialized by a call to
 * #cr_firmware_update_feature_init().
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_term(
    struct cr_firmware_update_feature_t *firmware_update);

/**
 * @brief Frees the memory associated with a Firmware Update feature instance.
 *
 * @param firmware_update Pointer to the Firmware Update feature instance.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS on success, otherwise
 * #CR_FIRMWARE_UPDATE_FEATURE_RESULT_INVALID_PARAMETER if the feature instance
 * pointer is invalid.
 *
 * @note The feature instance must have been previously terminated by a call to
 * #cr_firmware_update_feature_term().
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_free(
    struct cr_firmware_update_feature_t *firmware_update);

/**
 * @brief Retrieves a firmware manifest from the reader.
 *
 * @details This method initiates a read of the firmware manifest from a
 * reader. The result to the API call si received asynchronously via the
 * callback #cr_firmware_update_feature_event_api_t::on_receive_manifest().
 *
 * @param firmware_update Pointer to the Firmware Update feature instance.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS on success, otherwise an
 * error code as defined in #cr_firmware_update_feature_result_t.
 *
 * @note The feature must have been initialized by a call to
 * #cr_firmware_update_feature_init().
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_get_manifest(
    struct cr_firmware_update_feature_t *firmware_update);

/**
 * @brief Retrieves a firmware manifest from the reader.
 *
 * @details This method initiates a read of the firmware manifest from a
 * reader. The result to the API call si received asynchronously via the
 * callback #cr_firmware_update_feature_event_api_t::on_receive_manifest().
 * This method is used specifically to retrieve signed device manifests.
 *
 * @param firmware_update Pointer to the Firmware Update feature instance.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS on success, otherwise an
 * error code as defined in #cr_firmware_update_feature_result_t.
 *
 * @note The feature must have been initialized by a call to
 * #cr_firmware_update_feature_init().
 *
 * @note For older firmwares or devices that do not support signed manifests,
 * LCR will construct a signed manifest proto for the reader's response before
 * passing it to the registered callback. The proto will exclude the signature
 * fields.
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_get_manifest_v2(
    struct cr_firmware_update_feature_t *firmware_update);

/**
 * @brief Initiates a firmware update.
 *
 * @details This method is used to start the sending of a firmware image to a
 * reader. Progress is reported asynchronously via the
 * #cr_firmware_update_feature_event_api_t::on_progress() call, which will be
 * called one or more times. Once the full image has been sent to the reader,
 * the result is reported asynchronously via the call to
 * #cr_firmware_update_feature_event_api_t::on_result().
 *
 * @param firmware_update          Pointer to the Firmware Update feature instance.
 * @param header                   Firmware image header data.
 * @param header_length            Length of the @p header in bytes.
 * @param data                     Plaintext or encrypted firmware image data.
 * @param data_length              Length of the @p data in bytes.
 * @param block_index_table        Table of indices for indicating the offsets
 *                                 of the de-compressed data blocks if the
 *                                 passed @p data is compressed.
 * @param block_index_table_length Length of the @p block_index_table in bytes.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS if the firmware update was
 * started successfully, otherwise an error code as defined in
 * #cr_firmware_update_feature_result_t.
 *
 * @note The feature must have been initialized by a call to
 * #cr_firmware_update_feature_init().
 *
 * @note If the sending of the firmware image is interrupted (e.g. by a process
 * crash), it can be restarted by sending the same data again when the process
 * restarts.
 *
 * @note The contents of the @p header and @p data are opaque to the callers.
 * The header data contains information such as the update type, slot, length,
 * etc., while the @p data is either a plaintext or encrypted data blob.
 *
 * @note Firmware image data is buffered in memory, and trickled out to the
 * reader over serveral calls.
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_send_data(
    struct cr_firmware_update_feature_t *firmware_update,
    uint8_t *header,
    size_t header_length,
    uint8_t *data,
    size_t data_length,
    uint8_t *block_index_table,
    size_t block_index_table_length);

/**
 * @brief Pause the current firmware update.
 *
 * @details This method is used to halt an update in progress. To resume the
 * update from its current point in time, make nother call to
 * #cr_firmware_update_feature_send_data() with the same parameters that were
 * used to start the firmware update.
 *
 * @param firmware_update Pointer to the Firmware Update feature.
 *
 * @return #CR_FIRMWARE_UPDATE_FEATURE_RESULT_SUCCESS on success, otherwise an
 * error code as defined in #cr_firmware_update_feature_result_t.
 *
 * @note The feature must have been initialized by a call to
 * #cr_firmware_update_feature_init().
 */
cr_firmware_update_feature_result_t cr_firmware_update_feature_stop_sending_data(
    struct cr_firmware_update_feature_t *firmware_update);

/** @} */
