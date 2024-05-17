// Copyright 2018 Square, Inc.

#pragma once

#include <cardreader_shared/crs_stm_msgs.h>

#include <cardreader/cr_cardreader.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Possible result codes for cr_secure_touch_mode_* functions.
typedef enum
{
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_SUCCESS,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_INVALID_PARAMETER,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_NOT_INITIALIZED,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_ALREADY_INITIALIZED,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_NOT_TERMINATED,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_ALREADY_TERMINATED,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_SESSION_ERROR,
    CR_SECURE_TOUCH_MODE_FEATURE_RESULT_CALL_UNEXPECTED,
} cr_secure_touch_mode_feature_result_t;

typedef enum
{
    CR_SECURE_TOUCH_MODE_PIN_TRY_NONE,
    CR_SECURE_TOUCH_MODE_PIN_TRY_FIRST,
    CR_SECURE_TOUCH_MODE_PIN_TRY_RETRY,
    CR_SECURE_TOUCH_MODE_PIN_TRY_FINAL,
} cr_secure_touch_mode_feature_pin_try_t;

typedef enum
{
    CR_SECURE_TOUCH_KEEPALIVE_ERROR_EXPIRED,
    CR_SECURE_TOUCH_KEEPALIVE_ERROR_BAD_NONCE,
    CR_SECURE_TOUCH_KEEPALIVE_ERROR_SUCCESS,
} cr_secure_touch_mode_feature_keepalive_error_t;

typedef struct cr_secure_touch_mode_feature_t cr_secure_touch_mode_feature_t;

// Called when SPE initializes the pin entry flow. This is the first
// message to POS that informs POS that we will start the secure pin entry flow.
// POS should then inform SQUID to disable touch reporting to POS.
typedef void (*cr_secure_touch_mode_feature_disable_squid_touch_driver_request_cb_t)(
    void *context, // Java handler - jObject
    struct cr_secure_touch_mode_feature_t *secure_touch_mode);

// Called when SPE is requesting POS to hide the pin pad after detecting that
// secure touch flow needs to end. This can happen either when a user presses 'enter' after
// entering pin digits, 'cancel' or 'accessibility' buttons.
typedef void (*cr_secure_touch_mode_feature_hide_pin_pad_request_cb_t)(
    void *context, // Java handler - jObject
    struct cr_secure_touch_mode_feature_t *secure_touch_mode);

// Called when SPE exits the pin entry flow. This is the last
// message to POS that informs POS that we will end the secure pin entry flow.
// POS should then inform SQUID to enable touch reporting to POS.
typedef void (*cr_secure_touch_mode_feature_enable_squid_touch_driver_request_cb_t)(
    void *context,
    struct cr_secure_touch_mode_feature_t *secure_touch_mode);

// Called after SPE has switched the mux to direct touch data to K21, and in subsequent
// retries of pin entry. The API's params are:
//  card_info: card info that will be displayed on POS.
//  pin_try: an enum of type cr_secure_touch_mode_feature_pin_try_t that indicates whether
//     this is the first try, a retry, or final try.
// After this API call, the pin-pad should be shown in POS's UI.
typedef void (*cr_secure_touch_mode_feature_show_pin_pad_request_cb_t)(
    void *context,
    cr_card_info_t const *card_info,
    cr_secure_touch_mode_feature_pin_try_t pin_try,
    bool pin_bypass_enabled,
    struct cr_secure_touch_mode_feature_t *secure_touch_mode);

// Called when the keepalive mechanism fails. Two scenarios lead to failure:
//    1) SPE doesn't respond to the keepalive_request in time.
//    2) SPE's keepalive_response contains a nonce that LCR did not expect.
// Both of the above scenarios are reported using the "error_code" parameter.
// PoS should crash after this callback. The reason LCR delegates the
// crashing to PoS is two-folds:
// 1) Because we want to be able to test the mechanism in python testing.
// 2) Allow POS to log these events to eventstream.
typedef void (*cr_secure_touch_mode_feature_keepalive_failed_cb_t)(
    void *context,
    struct cr_secure_touch_mode_feature_t *secure_touch_mode,
    cr_secure_touch_mode_feature_keepalive_error_t keepalive_error_code);

// Called when SPE has touch events to send to POS. Informs POS of which event
// has happened. An event could be a button-press or another notification.
// This message should not contain the enum value for the pin pad numbers,
// instead it will pass up a generic value when a digit is pressed. This is to
// prevent the pin data from being transmitted over an unsecured bus.
typedef void (*cr_secure_touch_mode_feature_pin_pad_event_cb_t)(
    void *context,
    struct cr_secure_touch_mode_feature_t *secure_touch_mode,
    crs_stm_pin_pad_event_id_t event_id);

// Called when SPE has pin pad center coordinates to send to POS along with
// the type of pin pad to display.
typedef void (*cr_secure_touch_mode_feature_pin_pad_center_point_cb_t)(
    void *context,
    struct cr_secure_touch_mode_feature_t *secure_touch_mode,
    crs_stm_pin_pad_center_point_t *pin_pad_center_point);

// POS defines these functions. And LCR calls them as appropriate to "message" POS.
typedef struct cr_secure_touch_mode_feature_event_api_t
{
    cr_secure_touch_mode_feature_disable_squid_touch_driver_request_cb_t disable_squid_touch_driver_request;
    cr_secure_touch_mode_feature_enable_squid_touch_driver_request_cb_t enable_squid_touch_driver_request;
    cr_secure_touch_mode_feature_show_pin_pad_request_cb_t show_pin_pad_request;
    cr_secure_touch_mode_feature_keepalive_failed_cb_t on_keepalive_failed;
    cr_secure_touch_mode_feature_pin_pad_event_cb_t on_pin_pad_event;
    cr_secure_touch_mode_feature_hide_pin_pad_request_cb_t hide_pin_pad_request;
    cr_secure_touch_mode_feature_pin_pad_center_point_cb_t on_pin_pad_center_point;
    void *context; // JNI magic
} cr_secure_touch_mode_feature_event_api_t;

// POS to LCR API -- For POS to call into LCR to inform LCR of something.

// Called by POS with the result of disabling the touch driver.
void cr_secure_touch_mode_feature_disable_squid_touch_driver_result(cr_secure_touch_mode_feature_t *secure_touch_mode,
                                                                    crs_stm_disable_squid_touch_driver_res_t *result);

// Called by POS as a response to the "show_pin_pad_request". This will be called once for every button.
// Note that the function is named `regular`, but should also be called during the accessibility flow.
// `regular` refers to the fact that this is a call to be made to set buttons that
// are shared between regular and accessibility mode.
void cr_secure_touch_mode_feature_regular_set_button_location(cr_secure_touch_mode_feature_t *secure_touch_mode,
                                                              crs_stm_pin_pad_button_info_t *info);

// Called by POS to flag that button configs are complete. This API will send the configs to SPE, and clear the
// configs from secure_touch_mode memory.
void cr_secure_touch_mode_feature_sent_pinpad_configs(cr_secure_touch_mode_feature_t *secure_touch_mode,
                                                      crs_stm_pinpad_config_type_t crs_stm_pinpad_config_type,
                                                      bool activate_pin_bypass);

// This is called either to re-start the regular secure touch flow after
// anceling out of the enable accessibility dialog or to initiate the
// accessibility secure touch flow after the tutorial.
// Called by POS to indicate that SPE needs to initiate the secure touch flow.
// Ref: Secure touch Design doc: https://docs.google.com/document/d/1Gdi8VnqJoGLsOa3Z08L9z-82YS77GNLXLXWTBMqy11A/edit#
void cr_secure_touch_mode_feature_start_secure_touch(cr_secure_touch_mode_feature_t *secure_touch_mode);

// Called by POS to inform SPE that it needs to stop the secure touch flow.
// Indicates a user-event requiring termination of STM (e.g long press on the power button of T2 to reboot it)
// SPE will respond by asking POS to hide its pin pad and to re-enable SQUID's touch driver.
// This message cannot be called immediately after calling `cr_secure_touch_mode_feature_start_secure_touch`-- SPE
// must be given time to finish processing the start message before POS can issue stop. Else, SPE will abort.
//
//           LCR          FW
//            |           |
//  STM Start | --------> |
//            |           |
//            | <-------- | Disable Squid Touch Reporting
//            |           |
//     Result | --------> |
//            |           |
//            | <-------- | Show Pin Pad Request
//            |           |
//     Result | --------> |
//            |           |
//
//  After this point, it will be safe to call `cr_secure_touch_mode_feature_stop_secure_touch`.
void cr_secure_touch_mode_feature_stop_secure_touch(cr_secure_touch_mode_feature_t *secure_touch_mode);

// Called by POS to set the dimensions of the screen, the border width, and center button radius.
// This call is used during accessibility mode.
// Ref: Secure touch Design doc: https://docs.google.com/document/d/1Gdi8VnqJoGLsOa3Z08L9z-82YS77GNLXLXWTBMqy11A/edit#
void cr_secure_touch_mode_feature_set_accessibility_configs(cr_secure_touch_mode_feature_t *secure_touch_mode,
                                                            crs_stm_access_pin_pad_config_t *access_pin_pad_config);

// Called by POS to set message informing SPE/STM module of the screen's display resolution and current orientation.
// This call is used during STM mode (regular and/or acccessibility)
void cr_secure_touch_mode_feature_set_screen_configs(cr_secure_touch_mode_feature_t *secure_touch_mode,
                                                     crs_stm_screen_config_t *screen_config);

// Called by POS to inform LCR that the pin pad is hidden.
// Important Note: Must be called on POS when the pin pad is hidden!
// The purpose of this API is to disable the keepalives between LCR and SPE. If
// this callback is not called, the keepalives will continue to to be sent even
// when we are not in the pin-flow.
void cr_secure_touch_mode_pin_pad_is_hidden(cr_secure_touch_mode_feature_t *secure_touch_mode,
                                            crs_stm_hide_pin_pad_res_t *result);

cr_secure_touch_mode_feature_t *cr_secure_touch_mode_feature_alloc(void);
cr_secure_touch_mode_feature_result_t cr_secure_touch_mode_feature_free(
    cr_secure_touch_mode_feature_t *secure_touch_mode);
cr_secure_touch_mode_feature_result_t cr_secure_touch_mode_feature_init(
    cr_secure_touch_mode_feature_t *secure_touch_mode,
    struct cr_cardreader_t *cardreader,
    cr_secure_touch_mode_feature_event_api_t const *event_api);
cr_secure_touch_mode_feature_result_t cr_secure_touch_mode_feature_term(
    cr_secure_touch_mode_feature_t *secure_touch_mode);
