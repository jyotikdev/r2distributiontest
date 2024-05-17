// Copyright 2017 Square, Inc.

#pragma once

#include <cardreader_shared/crs_io.h>
#include <cardreader_shared/crs_timer.h>
#include <circbuf.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ARQ Protocol Specification at go/arq_design
// (https://docs.google.com/document/d/1Us3APGeJhBQQ0ybWON-dMym1TV72rbAR0i85_9ArqP4)

struct crs_stats_arq_t;

enum {
    ARQ_PROTOCOL_INITIAL_CRC16_VALUE = 0xffff,
    ARQ_PROTOCOL_PACKET_DATA_MAX_PAYLOAD_LEN = 255,
    ARQ_PROTOCOL_PACKET_HDR_LEN = 2,
    ARQ_PROTOCOL_PACKET_CRC16_LEN = sizeof(uint16_t),
    ARQ_PROTOCOL_PACKET_MAX_LEN = ARQ_PROTOCOL_PACKET_HDR_LEN + ARQ_PROTOCOL_PACKET_DATA_MAX_PAYLOAD_LEN + ARQ_PROTOCOL_PACKET_CRC16_LEN,
    ARQ_PROTOCOL_RECV_BUFFER_SIZE = ARQ_PROTOCOL_PACKET_MAX_LEN * 2,
    ARQ_PROTOCOL_CNTRL_PACKET_SIZE = ARQ_PROTOCOL_PACKET_HDR_LEN + 1 + ARQ_PROTOCOL_PACKET_CRC16_LEN,
};

typedef enum {
    ARQ_PROTOCOL_RESULT_SUCCESS = 0,
    ARQ_PROTOCOL_RESULT_ERROR,
    ARQ_PROTOCOL_RESULT_INVALID_PARAMETER,
    ARQ_PROTOCOL_RESULT_ALREADY_SENDING_DATA,
    ARQ_PROTOCOL_RESULT_NOT_READY,
} arq_protocol_result_t;

typedef void (*arq_protocol_receive_request_t)(void *impl, size_t requested_len);
typedef bool (*arq_protocol_received_data_callback_t)(void *impl, uint8_t const *data, size_t len);
typedef uint16_t (*arq_protocol_compute_crc16_t)(uint16_t crc, uint8_t const *data, size_t data_len);
typedef void (*arq_protocol_send_complete_t)(void *impl, uint8_t const *data, size_t len);
typedef void (*arq_protocol_connection_established_t)(void *impl);

typedef struct arq_protocol_api_t {
    arq_protocol_compute_crc16_t compute_crc16;
    arq_protocol_received_data_callback_t received_data;
    arq_protocol_send_complete_t send_complete;
    arq_protocol_connection_established_t connection_established;
    arq_protocol_receive_request_t recv_request;
    void *impl;
} arq_protocol_api_t;

typedef struct arq_protocol_stats_t {
    unsigned int send_success;
    unsigned int send_already_sending_data;
    unsigned int send_not_ready;
    unsigned int send_length;
    unsigned int send_complete;
    unsigned int send_rst_packet_timer_expired;
    unsigned int start_send_rst_packet_timer;
    unsigned int send_data_packet_timer_expired;
    unsigned int start_data_packet_send_timer;
    unsigned int stop_timer;
    unsigned int connection;
    unsigned int header_valid;
    unsigned int header_invalid;
    unsigned int send_over_io_length;
    unsigned int send_data_packet;
    unsigned int send_control_packet;
    unsigned int process_recv_data_length;
    unsigned int crc_match;
    unsigned int crc_mismatch;
    unsigned int data_packets_processed;
    unsigned int ignored_retransmitted;
    unsigned int control_packets_processed;
    unsigned int control_ack_packets_processed;
    unsigned int control_rst_packets_processed;
    unsigned int control_rst_ack_packets_processed;
    unsigned int control_err_packets_processed;
    unsigned int control_err_ack_packets_processed;
    unsigned int total_packets_acked;
    unsigned int packet_recv_timer_expired;
    unsigned int start_packet_recv_timer;
    unsigned int stop_packet_recv_timer;
} arq_protocol_stats_t;

typedef enum {
    ARQ_PROTOCOL_STATE_UNINITIALIZED,
    ARQ_PROTOCOL_STATE_WAITING_FOR_RST_ACK,
    ARQ_PROTOCOL_STATE_READY,
    ARQ_PROTOCOL_STATE_SENDING_DATA_PACKET,
    ARQ_PROTOCOL_STATE_WAITING_FOR_ACK_OR_TIMEOUT_OF_SENT_DATA,
} arq_protocol_state_t;

typedef struct {
    uint32_t reset_timeout_ms;
    uint32_t data_timeout_ms;
    uint32_t packet_recv_timeout_ms;
    size_t max_payload_length;
    bool start_in_ready_state;
} arq_protocol_cfg_t;

typedef struct arq_protocol_descriptor_t {
    arq_protocol_cfg_t cfg;

    crs_timer_api_t timer_api;
    crs_io_api_t io_api;

    arq_protocol_api_t api;
    arq_protocol_state_t state;
    bool connected;

    // RX
    uint8_t recv_expected_sequence_number;
    circbuf_info_t recv_buffer;
    uint8_t *recv_buffer_data;
    size_t recv_buffer_size;
    bool processing_data_packet;

    bool packet_recv_timer_in_use;
    crs_timer_t packet_recv_timer;

    // TX
    union {
        uint8_t tx_packet[ARQ_PROTOCOL_PACKET_MAX_LEN];
        struct {
            uint8_t tx_hdr[ARQ_PROTOCOL_PACKET_HDR_LEN];
            uint8_t tx_payload[ARQ_PROTOCOL_PACKET_DATA_MAX_PAYLOAD_LEN + ARQ_PROTOCOL_PACKET_CRC16_LEN];
        };
    };
    uint8_t send_sequence_number;
    uint8_t const *enqueued_data;
    size_t enqueued_data_length;

    bool timer_in_use;
    crs_timer_t timer;
    uint8_t timer_expired_count;

    arq_protocol_stats_t stats;
} arq_protocol_descriptor_t;

arq_protocol_result_t arq_protocol_init(arq_protocol_descriptor_t *descriptor,
                                        crs_io_api_t *io_api,
                                        crs_timer_api_t *timer_api,
                                        arq_protocol_api_t *api,
                                        size_t max_payload_length,
                                        bool start_in_ready_state);

arq_protocol_result_t arq_protocol_init_with_cfg(arq_protocol_descriptor_t *descriptor,
                                                 crs_io_api_t *io_api,
                                                 crs_timer_api_t *timer_api,
                                                 arq_protocol_api_t *api,
                                                 arq_protocol_cfg_t *cfg);

arq_protocol_result_t arq_protocol_send(arq_protocol_descriptor_t *descriptor, uint8_t const *data, size_t len);

arq_protocol_stats_t const *arq_protocol_get_stats(arq_protocol_descriptor_t *descriptor);

void arq_get_crs_stats(arq_protocol_descriptor_t *descriptor, struct crs_stats_arq_t *stat_block);
void arq_protocol_start(arq_protocol_descriptor_t *descriptor);
void arq_protocol_data_packet_processed(arq_protocol_descriptor_t *descriptor);
