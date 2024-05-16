// Copyright 2017 Square, Inc.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum {
    CRP_USB_DEFAULT_TIMEOUT_SECONDS = 60,
    CRP_USB_MAX_STR_LEN = 255,
};

typedef enum {
    CRP_USB_STATUS_SUCCESS = 0,
    CRP_USB_STATUS_ERROR_DEVICE_NOT_FOUND,
    CRP_USB_STATUS_ERROR_TIMEOUT,
    CRP_USB_STATUS_ERROR_DEVICE_NOT_SUPPORTED,
    CRP_USB_STATUS_ERROR_GENERAL
} crp_usb_status_t;

typedef struct crp_usb_priv_t crp_usb_priv_t;

typedef struct crp_usb_context_t {
    bool quit;
    bool read_failed;
    crp_usb_priv_t *priv;
} crp_usb_context_t;

crp_usb_context_t *crp_usb_alloc(void);
void crp_usb_free(crp_usb_context_t *context);
crp_usb_status_t crp_usb_init(crp_usb_context_t *context, uint16_t vid, const uint16_t *pids, int pids_len, char const *serial_number, uint32_t timeout);
bool crp_usb_send(crp_usb_context_t *context, uint8_t const *send_buf, size_t len);
bool crp_usb_recv(crp_usb_context_t *context, uint8_t *recv_buf, size_t len);
void crp_usb_quit(crp_usb_context_t *context);
