// Copyright 2016 Square, Inc.

#pragma once

#include <cardreader_shared/crs_capability.h>
#include <cardreader_shared/crs_feature_flags.h>
#include <cardreader_shared/crs_securesession_utils.h>

#include <stdint.h>
#include <stddef.h>

typedef uint16_t pb_size_t;

typedef enum {
    CR_COMMS_DRIVER_RESULT_SUCCESS,
    CR_COMMS_DRIVER_RESULT_NO_CALLBACKS,
    CR_COMMS_DRIVER_RESULT_INVALID_PARAMETER,
    CR_COMMS_DRIVER_RESULT_ENDPOINT_BACKPRESSURE,
    CR_COMMS_DRIVER_RESULT_NOT_CONNECTED,
    CR_COMMS_DRIVER_RESULT_MISSING_PROTOCOL_VERSION,
    CR_COMMS_DRIVER_RESULT_UNKNOWN_ERROR,
    CR_COMMS_DRIVER_RESULT_PROTOCOL_INCOMPATIBLE,
} cr_comms_driver_result_t;

typedef enum {
    CR_COMMS_DRIVER_DATA_MOVEMENT_PHY_TO_LINK,
    CR_COMMS_DRIVER_DATA_MOVEMENT_LINK_TO_TRANSPORT,
    CR_COMMS_DRIVER_DATA_MOVEMENT_TRANSPORT_TO_APP,
    CR_COMMS_DRIVER_DATA_MOVEMENT_APP_TO_TRANSPORT,
    CR_COMMS_DRIVER_DATA_MOVEMENT_TRANSPORT_TO_LINK,
    CR_COMMS_DRIVER_DATA_MOVEMENT_LINK_TO_PHY,
} cr_comms_driver_data_movement_t;

typedef enum {
    CR_COMMS_DRIVER_RITUAL_LCR_TO_READER,
    CR_COMMS_DRIVER_RITUAL_READER_TO_LCR
} cr_comms_driver_ritual_t;

// Identifies the interface over which a cardreader is connected
// New values MUST be added to the end of the enum to maintain
// compatibility with remote comms drivers.
typedef enum {
    CR_COMMS_DRIVER_CONNECTION_TYPE_AUDIO,
    CR_COMMS_DRIVER_CONNECTION_TYPE_BLE,
    CR_COMMS_DRIVER_CONNECTION_TYPE_UART,
    CR_COMMS_DRIVER_CONNECTION_TYPE_USB,
    CR_COMMS_DRIVER_CONNECTION_TYPE_ARQ,
} cr_comms_driver_connection_type_t;

// Internal compatibility values.
//
// This should be removed from the public API.
typedef enum
{
    CR_CARDREADER_COMPATIBILITY_UNKNOWN,
    CR_CARDREADER_COMPATIBILITY_READER_UPDATE_REQUIRED,
    CR_CARDREADER_COMPATIBILITY_HOST_UPDATE_REQUIRED,
    CR_CARDREADER_COMPATIBILITY_OLDER_PROTOCOL_COMPATIBLE,
    CR_CARDREADER_COMPATIBILITY_OLDER_PROTOCOL_INCOMPATIBLE,
    CR_CARDREADER_COMPATIBILITY_FULLY_COMPATIBLE,
} cr_cardreader_compatibility_t;

typedef void (*cr_comms_driver_recv_msg_cb_t)(void *context, pb_size_t which_message, void *proto_struct, size_t proto_struct_len);
typedef void (*cr_comms_driver_on_connection_cb_t)(void *context, cr_comms_driver_connection_type_t connection_type);
typedef void (*cr_comms_driver_protocol_version_cb_t)(void *context, uint8_t transport_version, uint8_t app_version, uint8_t ep_version, cr_cardreader_compatibility_t compatibility);
typedef void (*cr_comms_driver_enter_cb_t)(void *context, char const *file, int line);
typedef int (*cr_comms_driver_enter_nonblocking_cb_t)(void *context, char const *file, int line);
typedef void (*cr_comms_driver_exit_cb_t)(void *context);
typedef void (*cr_comms_driver_data_movement_cb_t)(void *context, cr_comms_driver_data_movement_t movement, uint8_t const *data, size_t data_len);
typedef void (*cr_comms_driver_ritual_movement_cb_t)(void *param, cr_comms_driver_ritual_t ritual_movement, pb_size_t which_message, void *proto_struct);
// This is function is called from cr_comms_driver_check_flags_field to pass the msg header
// flags field up to cr_cardreader.
typedef void (*cr_comms_driver_pass_msg_hdr_flags_cb_t)(void *context, uint16_t msg_hdr_flags);
// This function provides access to libcardreader feature flags
typedef uint8_t (*cr_comms_driver_get_feature_flag_cb_t)(void *context, crs_lcr_feature_flags_t flag);
// Provides access to capabilities
typedef bool (*cr_comms_driver_get_capability_cb_t)(void *context, crs_capability_t capability);

/**
 * @brief Callback to report fatal comms errors.
 */
typedef void (*cr_comms_driver_report_error_cb_t)(void *context, char const *msg);

typedef struct {
    cr_comms_driver_recv_msg_cb_t recv_msg_cb;
    cr_comms_driver_on_connection_cb_t on_connection_cb;
    cr_comms_driver_protocol_version_cb_t protocol_version_cb;
    cr_comms_driver_enter_cb_t enter_cb;
    cr_comms_driver_enter_nonblocking_cb_t enter_nonblocking_cb;
    cr_comms_driver_exit_cb_t exit_cb;
    cr_comms_driver_pass_msg_hdr_flags_cb_t pass_msg_hdr_flags_cb;
    cr_comms_driver_get_feature_flag_cb_t get_feature_flag_cb;
    cr_comms_driver_get_capability_cb_t get_capability_cb;
    cr_comms_driver_report_error_cb_t report_comms_error_cb;

    void *context;
} cr_comms_driver_callback_api_t;

typedef void (*cr_comms_driver_set_callback_api_t)(void *comms_driver,
                                                   cr_comms_driver_callback_api_t const *callback_api);

typedef void (*cr_comms_driver_set_crypto_api_t)(void *comms_driver,
                                                 crs_securesession_comms_api_t const *crypto_api);

typedef void (*cr_comms_driver_destroy_t)(void *comms_driver);

typedef cr_comms_driver_result_t (*cr_comms_driver_send_msg_to_reader_t)(void *comms_driver,
                                                                         pb_size_t which_message,
                                                                         void *proto_struct,
                                                                         size_t proto_len);

typedef void (*cr_comms_driver_notify_reader_plugged_t)(void *comms_driver);

typedef void (*cr_comms_driver_notify_reader_unplugged_t)(void *comms_driver);

typedef void (*cr_comms_driver_enable_ritual_v2_t)(void *comms_driver);

typedef struct {
    cr_comms_driver_set_callback_api_t set_callback_api;
    cr_comms_driver_set_crypto_api_t set_crypto_api;
    cr_comms_driver_send_msg_to_reader_t send_msg_to_reader;
    cr_comms_driver_notify_reader_plugged_t notify_reader_plugged;
    cr_comms_driver_notify_reader_unplugged_t notify_reader_unplugged;
    cr_comms_driver_destroy_t destroy;
    cr_comms_driver_enable_ritual_v2_t enable_ritual_v2_support;
    void *impl;
} cr_comms_driver_api_t;
