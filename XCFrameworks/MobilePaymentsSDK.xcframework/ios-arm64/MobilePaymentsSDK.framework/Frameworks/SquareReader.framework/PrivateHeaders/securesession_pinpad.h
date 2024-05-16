//  Copyright (c) 2015 Square Inc. All rights reserved.

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Note, this type is duplicated in (ios_)minesweeper.h
// which is legal under C-11, but some xcode configurations still emit
// the following warning, breaking -Werror.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtypedef-redefinition"
typedef struct securesession_pinpad_app_handler securesession_pinpad_app_handler_t;
#pragma clang diagnostic pop

typedef enum {
    SECURESESSION_PINPAD_APP_ERROR_NONE,
    SECURESESSION_PINPAD_APP_ERROR_TOO_MANY_DIGITS,
    SECURESESSION_PINPAD_APP_ERROR_TOO_FEW_DIGITS,
    SECURESESSION_PINPAD_APP_ERROR_TAMPER_DETECTED,
    SECURESESSION_PINPAD_APP_ERROR_UNSPECIFIED,
} securesession_pinpad_app_error_t;

struct securesession_pinpad_app_handler {
    // Opqaue pointer reference to an object which is passed to all callbacks.
    // This pointer may be NULL.
    void *ctx;

    // Handle for the host context used with this pinpad.
    void *context_handle;

    // digit_count_callback:
    // Receives updates to the count of digits entered.
    // This can be NULL, to indicate not to send callbacks of this type.
    void (*digit_count_callback)(void* ctx, size_t digits);

    // error_callback:
    // Receives a callback to convey error conditions
    // This can be NULL, to indicate not to send callbacks of this type.
    void (*error_callback)(void* ctx, securesession_pinpad_app_error_t error);

    // submit_button_callback:
    // Receives notice and (optionally) an encrypted pin_block when submit
    // button is pressed.  If securesession_pinblock encryption fails,
    // pinblock will be NULL and pinblock_len will be set to 0.
    // This can be NULL, to indicate not to send callbacks of this type.
    void (*submit_button_callback)(void* ctx, uint8_t *pinblock, size_t pinblock_len);

    // clear_button_callback:
    // Received notice when clear button is pressed.
    // This can be NULL, to indicate not to send callbacks of this type.
    void (*clear_button_callback)(void* ctx);
};


// Opaque type for a securesession pinpad context used by the secure touch
// implementation.
typedef struct securesession_pinpad securesession_pinpad_t;


// Creates a new pinpad context with the specified coordinates and
// callback handlers set.
securesession_pinpad_t *securesession_pinpad_create(double x, double y, double width, double height, securesession_pinpad_app_handler_t *app_handler, uint32_t debounce_timeout_ms);

// Destroys and free's a pinpad context. If a NULL pinpad is passed
// in, this function does nothing.
void securesession_pinpad_free(securesession_pinpad_t *pinpad);

// Used to allow the application to convey new pinpad coordinates such
// as when orientation changes occur. If a NULL pinpad is passed in,
// this function does nothing.
void securesession_pinpad_update_coordinates(securesession_pinpad_t *pinpad, double x, double y, double width, double height);


// Result codes returned by securesession_pinpad_handle_touch_event
// to convey basic status and what actionx to take on a touch event.
// These values are used by the secure touch integration and are not
// generally intended for use in application code.
typedef enum {
    // Indicates an unrecoverable error with the pinpad context.
    // Secure-touch should tear itself down and pass up any touch events.
    SECURESESSION_PINPAD_TOUCH_RESULT_FATAL,

    // Indicates a recoverable error with the pinpad context.
    // Secure-touch should _not_ tear itself down and should not pass up touch events.
    SECURESESSION_PINPAD_TOUCH_RESULT_ERROR,

    // Indicates the touch event was ignored by the handler.
    // For secure-touch, this means the original touch event should be
    // passed up to the application.
    SECURESESSION_PINPAD_TOUCH_RESULT_EVENT_IGNORED,

    // Indicates that the touch event was handled.
    // For secure-touch, this means the original touch event should be
    // passed up to the application.
    SECURESESSION_PINPAD_TOUCH_RESULT_EVENT_HANDLED,

    // Indicates that the touch event was handled, and that it resulted in the
    // pinpad session being ended (such as when 'submit' is pressed).
    // The caller should finalize and free the pinpad context and return
    // control back to the application.
    SECURESESSION_PINPAD_TOUCH_RESULT_FINISHED,
} securesession_pinpad_touch_result_t;


// This function is designed to be called with all touch events intercepted
// during secure touch UI interaction. It performs processing on the touch
// event and pin entry and simply indicates back to the caller whether the
// event was handled or ignored, or if an error occurred.
securesession_pinpad_touch_result_t securesession_pinpad_handle_touch_event(securesession_pinpad_t *pinpad, double x, double y, bool touch_down);

