// Copyright 2020 Square, Inc.

/**
 * @file
 *
 * @brief Eventlog Parser Module
 *
 * @details The eventlog_parser module provides an interface for parsing
 * eventlog byte-array data from devices into human readable strings.
 *
 * @author Eldon Rivers <eldon@squareup.com>
 *
 * @see https://docs.google.com/document/d/12fxJ9BIiJIHo3vGNjsEsK1gMhWo_EQVPgSjKcQOOGOI
 */

/**
 * @addtogroup eventlog_parser Event Log Parser
 * @ingroup lib
 * @{
 */

#pragma once

#include <LCRCore/squareup/spe/eventlog_definition.pb.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Maximum string length of a parsed eventlog
 */
#define EVENTLOG_MAX_STRING_LENGTH        (256)

/**
 * @brief Resulting struct of a parsed eventlog event
 */
typedef struct eventlog_parsed_event_t {
    /**
     * @brief Timestamp (in ms) of the parsed event
     */
    uint64_t              timestamp;
    /**
     * @brief unique event id of this parsed event
     */
    uint32_t              event;
    /**
     * @brief source id of this parsed event
     */
    uint32_t              source;
    /**
     * @brief parsed string output of this parsed event
     */
    char                  string[EVENTLOG_MAX_STRING_LENGTH];
} eventlog_parsed_event_t;

struct eventlog_parser_t;

/**
 * @brief Definition for the handle to an eventlog parser instance.
 */
typedef struct eventlog_parser_t * eventlog_parser_handle_t;

/**
 * @brief Allocates an eventlog parser instance.
 *
 * @return Handle to the allocated eventlog parser instance.
 */
eventlog_parser_handle_t eventlog_parser_alloc(void);

/**
 * @brief Frees a previously allocated eventlog parser instance specified by
 * the given handle.
 *
 * @param parser Handle to the eventlog parser instance.
 *
 * @note Handle must correspond to a valid handle returned by
 * `eventlog_parser_alloc()` that has not already been fried.
 */

void eventlog_parser_free(eventlog_parser_handle_t parser);

/**
 * @brief Initializes the eventlog parser module with default eventlog definitions.
 *
 * @param parser Handle to the allocated eventlog parser instance.
 */
void eventlog_parser_init(eventlog_parser_handle_t parser);

/**
 * @brief Initializes the eventlog parser module with provided eventlog definitions.
 *
 * @param parser    Handle to the allocated eventlog parser instance.
 * @param bytes     byte array of encoded eventlog definition proto.
 * @param num_bytes size in bytes of encoded eventlog definition proto.
 *
 * @note Event definitions can be updated by calling this function after
 * first time initialization.
 */
void eventlog_parser_init_with_definitions(eventlog_parser_handle_t parser,
                                           const uint8_t *bytes,
                                           size_t num_bytes);

/**
 * @brief Generates parsed events from an eventlog data buffer.
 *
 * @param parser                 Handle to the allocated eventlog parser instance.
 * @param[out] parsed_events     Output parameter array of parsed events of type
 #                               eventlog_parsed_event_t
 * @param buffer byte-array      input eventlog data buffer to be parsed
 * @param buffer_length          length of input eventlog data buffer in bytes
 * @param timestamp_now host     timestamp in ms at the time eventlogs were drained
 *                               from the source device.
 * @param use_num_param_encoding allows support for older reader eventlog encoding
 *                               when false. This can be assumed to be true for Stand.
 *                               This value comes from reader capability
 *                               CRS_CAPABILITY_EVENTLOG_NUM_PARAMS_ENCODING.
 *
 * @return                       number of parsed events in parsed_events output param.
 */
uint32_t eventlog_parser_stringify_events(eventlog_parser_handle_t parser,
                                          eventlog_parsed_event_t **parsed_events,
                                          const uint8_t *buffer,
                                          size_t buffer_length,
                                          uint64_t timestamp_now,
                                          bool use_num_param_encoding);

/**
 * @brief Determine if an eventlog buffer is complete and ready to be parsed.
 *
 * @param parser                 Handle to the allocated eventlog parser instance.
 * @param buffer                 byte-array input eventlog data buffer to be parsed
 * @param buffer_length          length of input eventlog data buffer in bytes
 * @param use_num_param_encoding allows support for older reader eventlog encoding
 *                               when false. This can be assumed to be true for Stand.
 *                               This value comes from reader capability
 *                               CRS_CAPABILITY_EVENTLOG_NUM_PARAMS_ENCODING.
 *
 * @return                       true if the provided data represents a complete
 *                               draining of events. To ensure valid event timestamps
 *                               this should be checked (and return true) before
 *                               calling eventlog_parser_stringify_events.
 */
bool eventlog_parser_is_buffer_complete(eventlog_parser_handle_t parser,
                                        const uint8_t *buffer,
                                        size_t buffer_length,
                                        bool use_num_param_encoding);

/** @} */
