// Copyright (c) 2014 Square, Inc. All rights reserved.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <cardreader_shared/crs_hardware_platform_feature.h>
#include <cardreader_shared/crs_feature_flags.h>
#include <cardreader_shared/crs_system_msgs.h>

// Possible result codes for cr_system_* functions.
typedef enum
{
    CR_SYSTEM_RESULT_SUCCESS,
    CR_SYSTEM_RESULT_INVALID_PARAMETER = 1,
    CR_SYSTEM_RESULT_NOT_INITIALIZED = 2,
    CR_SYSTEM_RESULT_ALREADY_INITIALIZED = 3,
    CR_SYSTEM_RESULT_NOT_TERMINATED = 4,
    CR_SYSTEM_RESULT_ALREADY_TERMINATED = 5,
    CR_SYSTEM_RESULT_SESSION_ERROR = 7,
    CR_SYSTEM_RESULT_CALL_UNEXPECTED = 8,
    CR_SYSTEM_RESULT_FATAL = 9,
} cr_system_result_t;

struct cr_system_t;
struct cr_cardreader_t;

// Callback invoked when the reader provides the hardware serial number. The supplied serial number
// is a NUL terminated string.
typedef void (*cr_system_on_reader_receive_hw_serial_number_cb_t)(void *impl,
                                                                  struct cr_system_t *system,
                                                                  char const *sn);

// Callback invoked when the reader provides the firmware version number. The supplied version
// number is a NUL terminated string.
typedef void (*cr_system_on_reader_receive_fw_version_cb_t)(void *impl,
                                                            struct cr_system_t *system,
                                                            char const *version);

// Callback invoked when the reader provides the charge cycle count.
typedef void (*cr_system_on_reader_receive_charge_cycle_count_cb_t)(void *impl,
                                                                    struct cr_system_t *system,
                                                                    uint32_t charge_count);

// Callback invoked when the reader provides the capabilities
typedef void (*cr_system_on_reader_receive_capabilities_cb_t)(void *impl,
                                                              struct cr_system_t *system,
                                                              uint8_t const *capabilities,
                                                              size_t capabilities_len,
                                                              bool capabilities_supported);

// Callback invoked when the reader sends notification that the K400 has reset
// (if the system has a K400). It returns the number of resets it has performed
// since the last time it has received a successful response
typedef void (*cr_system_on_reader_receive_k400_has_reset_cb_t)(void *impl,
                                                                struct cr_system_t *system,
                                                                uint32_t number_of_resets);

typedef void (*cr_system_on_reader_receive_reader_error_cb_t)(void *impl,
                                                              struct cr_system_t *system,
                                                              crs_system_reader_error_t reader_error);

typedef struct cr_system_event_api_t
{
    // on_reader_receive_hw_serial_number is invoked when LCR receives the hardware serial number
    // from the reader in response to cr_system_read_system_info.
    cr_system_on_reader_receive_hw_serial_number_cb_t on_reader_receive_hw_serial_number;

    // on_reader_receive_fw_version is invoked when LCR receives the firmware version from the
    // reader in response to cr_system_read_system_info.
    cr_system_on_reader_receive_fw_version_cb_t on_reader_receive_fw_version;

    // on_reader_receive_charge_cycle_count is invoked when LCR receives the charge cycle count from the
    // reader in response to cr_system_read_system_info
    cr_system_on_reader_receive_charge_cycle_count_cb_t on_reader_receive_charge_cycle_count;

    // on_reader_receive_capabilities is invoked when LCR receives the capabilities from the
    // reader in response to a CR_CARDREADER_EVENT_SYSTEM_CAN_QUERY_CAPABILITIES event
    cr_system_on_reader_receive_capabilities_cb_t on_reader_receive_capabilities;

    // on_reader_receive_k400_reset is invoked when LCR receives notification that K400 has reset
    cr_system_on_reader_receive_k400_has_reset_cb_t on_reader_receive_k400_has_reset;

    // Indicates an unrecoverable error from reader. These are error types in
    // which the reader can boot and talk with LCR but is damaged or not usable.
    cr_system_on_reader_receive_reader_error_cb_t on_reader_receive_reader_error;

    // context holds a user specified pointer that will be passed as the first argument to the
    // above callbacks.
    void *context;
} cr_system_event_api_t;

// Allocates the memory for a cr_system_t.
struct cr_system_t *cr_system_alloc(void);

// Initializes a cr_system_t with the specified callbacks and registers the feature with the
// specified cardreader.
cr_system_result_t cr_system_init(struct cr_system_t *system,
                                  struct cr_cardreader_t *cardreader,
                                  cr_system_event_api_t const *event_api);

// Terminates a cr_system_t, deregistering the feature.
cr_system_result_t cr_system_term(struct cr_system_t *system);

// Frees the memory for a cr_system_t. The cr_system_t must have been previously terminated.
cr_system_result_t cr_system_free(struct cr_system_t *system);

// Retrieves system info from the reader. System info includes hardware serial number, charge cycle count,
// and firmware version number.
//
// A call will be made (asynchronously) to cr_system_event_api_t::on_reader_receive_hw_serial_number,
// cr_system_event_api_t::on_reader_receive_charge_cycle_count, and cr_system_event_api_t::on_reader_receive_fw_version.
cr_system_result_t cr_system_read_system_info(struct cr_system_t *system);

// Set the hardware platform feature.
cr_system_result_t cr_system_set_hardware_platform_feature(
    struct cr_system_t *system,
    crs_hardware_platform_feature_t const hardware_platform_feature);

cr_system_result_t cr_system_send_keepalive(struct cr_system_t *system);
cr_system_result_t cr_system_send_keepalive_halt(struct cr_system_t *system);
bool cr_system_is_keepalive_supported(struct cr_system_t *system);
bool cr_system_usb_data_is_keepalive_msg(struct cr_system_t *system, uint8_t *data, size_t data_len);
bool cr_system_usb_data_is_keepalive_halt_msg(struct cr_system_t *system, uint8_t *data, size_t data_len);
cr_system_result_t cr_system_send_external_charging_state(struct cr_system_t *system, bool const is_charging);

/**
 * @brief Send a disconnection notification and wait for the response
 *
 * @param system      Pointer to the cr_system_t instance for this feature.
 * @param timeout_ms  Maximum amount of time to wait for SPE to confirm disconnect. This should be
 *                    at least a conservatively sufficient time for a round-trip message to the SPE.
 *
 * @return cr_system_result_t result code (CR_SYSTEM_RESULT_SUCCESS on success or timeout)
 */
cr_system_result_t cr_system_send_disconnect_notification_and_pend_response(struct cr_system_t *system, uint32_t timeout_ms);

// Set one reader feature flag value (but don't yet send to reader)
cr_system_result_t cr_system_set_reader_feature_flag(struct cr_system_t *system,
                                                     const char *feature_name,
                                                     uint8_t value);

// Send all feature flags to reader if and when requirements (firmware version and capabilities)
// are met. For this to happen system_info must be requested (using cr_system_read_system_info).
cr_system_result_t cr_system_mark_feature_flags_ready_to_send(struct cr_system_t *system);

/**
 * @brief Send a request for the reader to send a bad data packet
 *
 * @param system      Pointer to the cr_system_t instance for this feature.
 *
 * @return cr_system_result_t result code (CR_SYSTEM_RESULT_SUCCESS on success or timeout)
 */
cr_system_result_t cr_system_send_bad_data_req(struct cr_system_t *system);
