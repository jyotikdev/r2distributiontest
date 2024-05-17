// Copyright 2017 Square, Inc.

#pragma once

// Squid keyagree feature currently only works on platforms that use the LCR
// "RPC" client/server
#include <cardreader_rpc_client.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <squareup/spe/k21/ritual.pb.h>

struct cr_squid_key_agree_feature_t;
struct cr_cardreader_t;

typedef enum
{
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_SUCCESS,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_ERROR,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_INVALID_PARAMETER,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_SESSION_ERROR,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_NOT_INITIALIZED,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_ALREADY_TERMINATED,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_CALL_UNEXPECTED,
    CR_SQUID_KEY_AGREE_FEATURE_RESULT_NO_CAPABILITY,
} cr_squid_key_agree_feature_result_t;

typedef void (*cr_squid_key_agree_key_status_cb_t)(void *context, bool has_key);
typedef void (*cr_squid_key_agree_key_established_cb_t)(void *context);
typedef void (*cr_squid_key_agree_key_master_key_reset_request_cb_t)(void *context,
                                                                     const uint8_t *master_key_reset_request,
                                                                     const pb_size_t request_len);
typedef void (*cr_squid_key_agree_key_do_master_key_reset_cb_t)(void *context, bool status);
typedef void (*cr_squid_key_agree_master_key_error_cb_t)(void *context, uint8_t error);
typedef void (*cr_squid_key_agree_key_recovery_failed_cb_t)(void *context);

typedef struct
{
    // on_peripheral_master_key_status is invoked when LCR receives a squid key status
    // from the SPE in response to cr_squid_key_agree_peripheral_master_key_get_status.
    cr_squid_key_agree_key_status_cb_t on_peripheral_master_key_status;

    // on_peripheral_master_key_error is invoked when LCR receives a squid master
    // key error from the SPE in response to any request.
    cr_squid_key_agree_master_key_error_cb_t on_peripheral_master_key_error;

    // on_host_master_key_status is invoked when LCR receives a squid key status
    // from TrustZone in response to cr_squid_key_agree_host_master_key_get_status.
    cr_squid_key_agree_key_status_cb_t on_host_master_key_status;

    // on_peripheral_session_key_status is invoked when LCR receives a squid
    // session key status from the SPE in response to
    // cr_squid_key_agree_master_session_key_get_status
    cr_squid_key_agree_key_status_cb_t on_peripheral_session_key_status;

    // on_host_session_key_status is invoked when LCR receives a squid
    // session key status from the host (RPC server) in response to
    // cr_squid_key_agree_host_session_key_get_status
    cr_squid_key_agree_key_status_cb_t on_host_session_key_status;

    // on_master_key_established is invoked after both the host and peripheral
    // confirm master key establishment.
    cr_squid_key_agree_key_established_cb_t on_master_key_established;

    // on_session_key_established is invoked after both the host and peripheral
    // confirm session key establishment.
    cr_squid_key_agree_key_established_cb_t on_session_key_established;

    // on_master_key_reset_request is invoked after a `prepare_peripheral_master_key_reset_request`
    // is processed by the SPE, and the resulting reset request is sent back.
    cr_squid_key_agree_key_master_key_reset_request_cb_t on_master_key_reset_request;

    // on_do_master_key_reset is invoked after the SPE has handled a call to
    // cr_squid_key_agree_do_master_key_reset and returned the response.
    cr_squid_key_agree_key_do_master_key_reset_cb_t on_do_master_key_reset;

    // on_master_key_recovery_failed is invoked after TZ sent failure status response
    // to cr_squid_key_agree_begin_master_key_recovery.
    cr_squid_key_agree_key_recovery_failed_cb_t on_master_key_recovery_failed;

    // context holds a user specified pointer that will be passed as the first argument to the
    // above callbacks.
    void *context;
} cr_squid_key_agree_feature_event_api_t;

typedef struct
{
    bool establish_session_key_on_spe_connection;
} cr_squid_key_agree_cfg_t;

// Allocates the memory for a cr_squid_key_agree_feature_t.
struct cr_squid_key_agree_feature_t *cr_squid_key_agree_feature_alloc(void);

// Initializes a cr_squid_key_agree_feature_t with the specified callbacks and registers the
// feature with the specified cardreader.
cr_squid_key_agree_feature_result_t cr_squid_key_agree_feature_init(
    struct cr_squid_key_agree_feature_t *squid_key_agree,
    struct cr_cardreader_t *cardreader,
    struct cardreader_rpc_client_t *rpc_client,
    cr_squid_key_agree_feature_event_api_t const *api,
    cr_squid_key_agree_cfg_t const *cfg);

// Terminates a cr_squid_key_agree_feature_t, deregistering the feature.
cr_squid_key_agree_feature_result_t cr_squid_key_agree_feature_term(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Frees the memory for a cr_squid_key_agree_feature_t. The cr_squid_key_agree_feature_t must have
// been previously terminated.
cr_squid_key_agree_feature_result_t cr_squid_key_agree_feature_free(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Request the master key status from the reader. A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t::on_peripheral_master_key_status
// Assumes that caller has already requested and received reader capabilities.
cr_squid_key_agree_feature_result_t cr_squid_key_agree_peripheral_master_key_get_status(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Request the master key status from the host (Trustzone). A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t::on_host_master_key_status
cr_squid_key_agree_feature_result_t cr_squid_key_agree_host_master_key_get_status(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Request the session key status from the reader. A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t::on_peripheral_session_key_status
cr_squid_key_agree_feature_result_t cr_squid_key_agree_peripheral_session_key_get_status(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Request the session key status from the host (RPC server). A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t::on_host_session_key_status
cr_squid_key_agree_feature_result_t cr_squid_key_agree_host_session_key_get_status(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Initiates master key establishment with the reader. A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t::on_master_key_established
// if `establish_session_key_after` is set to `true`, LCR will establish the
// session key when master key establishment is complete. `establish_session_key_after`
// should only be set to `true` when called internally from an LCR message
// handler, as it assumes the cardreader lock is held by the calling thread.
cr_squid_key_agree_feature_result_t cr_squid_key_agree_begin_master_key_establishment(
    struct cr_squid_key_agree_feature_t *squid_key_agree,
    bool establish_session_key_after);

// Initiates session key establishment with the reader. A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t::on_session_key_established
cr_squid_key_agree_feature_result_t cr_squid_key_agree_begin_session_key_establishment(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Asks the reader the prepare a request to reset the master key. A call will be made (asynchronously) to
// cr_squid_key_agree_feature_event_api_t:on_master_key_reset_request
cr_squid_key_agree_feature_result_t cr_squid_key_agree_prepare_peripheral_master_key_reset_request(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// Asks the reader to perform an actual reset of the master key, given authorization from factory-server in
// `reset_response`. A call will be made (asynchronously) cr_squid_key_agree_feature_event_api_t:on_do_master_key_reset
cr_squid_key_agree_feature_result_t cr_squid_key_agree_do_master_key_reset(
    struct cr_squid_key_agree_feature_t *squid_key_agree,
    uint8_t *reset_response,
    size_t reset_response_len);

// Begins the master key recovery state machine. This function will be called by the key-agree arbiter (`spe_info`
// today), when it detects that the SPE has stored a master key, but the host has not. A call will be made
// (asynchronously) to cr_squid_key_agree_feature_event_api_t::on_session_key_established or
// cr_squid_key_agree_feature_event_api_t::on_master_key_recovery_failed
cr_squid_key_agree_feature_result_t cr_squid_key_agree_begin_master_key_recovery(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

// NOTE: This function should only be called for testing purposes.
// Attempts to clear the master key stored on the host. The host software (TrustZone) will not accept this command on
// production-fused devices.
cr_squid_key_agree_feature_result_t cr_squid_key_agree_host_master_key_reset(
    struct cr_squid_key_agree_feature_t *squid_key_agree);

cr_squid_key_agree_feature_result_t cr_squid_key_agree_send_session_keys(
    struct cr_squid_key_agree_feature_t *squid_key_agree,
    const uint8_t *session_key_sending,
    size_t session_key_sending_length,
    const uint8_t *session_key_receiving,
    size_t session_key_receiving_length);
