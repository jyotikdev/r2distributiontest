// Copyright (c) 2014 Square, Inc. All rights reserved.

#pragma once

#include <cardreader_shared/crs_log.h>
#include <cardreader_shared/crs_stats_msg.h>
#include <squareup/spe/k21/ritual.pb.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Possible result codes for cr_stats_* functions.
typedef enum
{
    CR_STATS_FEATURE_RESULT_SUCCESS,
    CR_STATS_FEATURE_RESULT_INVALID_PARAMETER,
    CR_STATS_FEATURE_RESULT_NOT_INITIALIZED,
    CR_STATS_FEATURE_RESULT_ALREADY_INITIALIZED,
    CR_STATS_FEATURE_RESULT_NOT_TERMINATED,
    CR_STATS_FEATURE_RESULT_ALREADY_TERMINATED,
    CR_STATS_FEATURE_RESULT_SESSION_ERROR,
    CR_STATS_FEATURE_RESULT_CALL_UNEXPECTED,
} cr_stats_feature_result_t;

// Passed to LCR stats feature v2 to allow new block types to be added
// without requiring a new callback type.
typedef enum
{
    CR_STATS_FEATURE_BLOCK_VERSION_R6 = 0,
    CR_STATS_FEATURE_BLOCK_VERSION_R12,
    CR_STATS_FEATURE_BLOCK_VERSION_X2,
} cr_stats_feature_block_version_t;

struct cr_stats_feature_t;
struct cr_cardreader_t;

// Callback invoked when R6 provides stats.
typedef void (*cr_stats_feature_on_block_received_cb_t)(void *context,
                                                        struct cr_stats_feature_t *stats,
                                                        squareup_firmware_ritual_StatsBlock const *block);
// Callback invoked when X2 and newer readers provide stats.
typedef void (*cr_stats_feature_block_v2_received_cb_t)(void *context,
                                                        struct cr_stats_feature_t *stats,
                                                        void const *block,
                                                        size_t block_len,
                                                        cr_stats_feature_block_version_t block_version);

typedef struct
{
    // on_block_received is invoked when LCR receives stats from the reader in response to
    // cr_stats_feature_get_block.
    cr_stats_feature_on_block_received_cb_t on_block_received;
    // on_block_v2_received is invoked when LCR receives stats from X2 and
    // newer readers reader in response to cr_stats_feature_get_block.
    cr_stats_feature_block_v2_received_cb_t on_block_v2_received;
    // context holds a user specified pointer that will be passed as the first argument to the
    // above callback.
    void *context;
} cr_stats_feature_event_api_t;

// Allocates the memory for a cr_stats_feature_t.
struct cr_stats_feature_t *cr_stats_feature_alloc(void);

// Initializes a cr_stats_feature_t with the specified callbacks and registers the feature with the
// specified cardreader.
cr_stats_feature_result_t cr_stats_feature_init(struct cr_stats_feature_t *stats,
                                                struct cr_cardreader_t *cardreader,
                                                cr_stats_feature_event_api_t const *api);

// Terminates a cr_stats_feature_t, deregistering the feature.
cr_stats_feature_result_t cr_stats_feature_term(struct cr_stats_feature_t *stats);

// Frees the memory for a cr_stats_feature_t. The cr_stats_feature_t must have been previously
// terminated.
cr_stats_feature_result_t cr_stats_feature_free(struct cr_stats_feature_t *stats);

// Retrieves the reader stats block. A call will be made (asynchronously) to
// cr_stats_feature_event_api_t::on_block_received or
// cr_stats_feature_event_api_t::on_block_v2_received.
cr_stats_feature_result_t cr_stats_feature_get_block(struct cr_stats_feature_t *stats);

// Retrieves the identifier for the stats feature subsystem that can be used for setting a
// subsystem specific log threshold.
crs_log_subsystem_t cr_stats_feature_get_log_subsystem(void);
