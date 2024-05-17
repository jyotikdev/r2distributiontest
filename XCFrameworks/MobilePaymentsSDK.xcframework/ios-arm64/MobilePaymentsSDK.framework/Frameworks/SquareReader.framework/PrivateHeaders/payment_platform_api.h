// Copyright 2019 Square, Inc.

#pragma once

#include <squareup/spe/k400/private_message.pb.h>
#include <stdarg.h>

typedef size_t (*payment_platform_get_max_msg_size_t)(void* impl);

typedef size_t (*payment_platform_get_msg_size_t)(void* impl, void* msg);

typedef uint16_t (*payment_platform_get_msg_id_t)(void* impl, void* msg);

typedef uint16_t (*payment_platform_get_gen_id_t)(void* impl, void* msg);

typedef uint16_t (*payment_platform_get_timer_id_t)(void* impl, void* msg);

typedef void (*payment_platform_get_k400_msg_t)(void* impl, void* msg, squareup_spe_k400_K400PrivateMessage* k400_msg);

typedef bool (*payment_platform_send_k400_msg_t)(void* impl, squareup_spe_k400_K400PrivateMessage* k400_msg);

typedef void (*payment_platform_send_l1_config_msg_t)(void* impl,
                                                      squareup_spe_k400_K400PrivateMessage* k400_msg,
                                                      squareup_spe_k400_CIL1ConfigParam param,
                                                      uint32_t val);

typedef void (*payment_platform_send_transaction_notification_t)(void* impl, squareup_spe_k400_NotificationCode code);

typedef void (*payment_platform_set_timer_t)(void* impl, uint16_t timer_id, uint32_t period, uint32_t expiry);

typedef void (*payment_platform_remove_timer_t)(void* impl, uint16_t timer_id);

typedef void (*payment_platform_ignore_msg_t)(void* impl, void* msg);

typedef void (*payment_platform_remove_msg_t)(void* impl, void* msg);

typedef void (*payment_platform_eventlog_log_t)(void* impl, uint8_t event);

typedef void (*payment_platform_eventlog_log_with_one_param_t)(void* impl, uint8_t event, uint32_t param1);

typedef void (*payment_platform_eventlog_log_with_two_params_t)(void* impl, uint8_t event, uint32_t param1, uint32_t param2);

typedef void (*payment_platform_eventlog_log_with_three_params_t)(void* impl, uint8_t event, uint32_t param1, uint32_t param2, uint32_t param3);

typedef void (*payment_platform_eventlog_log_v2_t)(void* impl, uint32_t event);

typedef void (*payment_platform_eventlog_log_with_one_param_v2_t)(void* impl, uint32_t event, uint32_t param1);

typedef void (*payment_platform_eventlog_log_with_two_params_v2_t)(void* impl, uint32_t event, uint32_t param1, uint32_t param2);

typedef void (*payment_platform_eventlog_log_with_three_params_v2_t)(void* impl, uint32_t event, uint32_t param1, uint32_t param2, uint32_t param3);

typedef uint32_t (*payment_platform_system_time_get_msec_t)(void* impl);

typedef uint64_t (*payment_platform_system_time_get_usec_t)(void* impl);

typedef const uint8_t* (*payment_platform_tms_capk_get_data_t)(void* impl, uint32_t* length);

typedef uint32_t (*payment_platform_tms_capk_get_struct_version_t)(void* impl);

typedef bool (*payment_platform_tms_capk_get_prod_id_t)(void* impl, char* prod_id_buf, size_t prod_id_buf_sz);

typedef uint32_t (*payment_platform_crc32_initial_t)(void* impl);

typedef uint32_t (*payment_platform_crc32_intermediate_t)(void* impl, uint32_t crc, const uint8_t* data, uint32_t len);

typedef uint32_t (*payment_platform_crc32_final_t)(void* impl, uint32_t crc);

typedef int (*payment_platform_vsnprintf_t)(void* impl, char* s, size_t n, const char* format, va_list arg);

typedef void (*payment_platform_log_error_t)(void* impl, const char* module_name, const char* proc_name, int line_no, const char* message);

typedef void (*payment_platform_log_info_t) (void* impl, const char* module_name, const char* proc_name, int line_no, size_t data_len, const void* data, const char* message);

typedef void (*payment_platform_log_trace_t)(void* impl, const char* module_name, const char* proc_name, int line_no, size_t data_len, const void* data, const char* message);

typedef void (*payment_platform_log_flush_t)(void* impl);

typedef struct payment_platform_api_t
{
    payment_platform_get_max_msg_size_t get_max_msg_size;

    payment_platform_get_msg_size_t get_msg_size;

    payment_platform_get_msg_id_t get_msg_id;

    payment_platform_get_gen_id_t get_gen_id;

    payment_platform_get_timer_id_t get_timer_id;

    payment_platform_get_k400_msg_t get_k400_msg;

    payment_platform_send_k400_msg_t send_k400_msg;

    payment_platform_send_l1_config_msg_t send_l1_config_msg;

    payment_platform_send_transaction_notification_t send_transaction_notification;

    payment_platform_set_timer_t set_timer;

    payment_platform_remove_timer_t remove_timer;

    payment_platform_ignore_msg_t ignore_msg;

    payment_platform_remove_msg_t remove_msg;

    payment_platform_eventlog_log_t eventlog_log;

    payment_platform_eventlog_log_with_one_param_t eventlog_log_with_one_param;

    payment_platform_eventlog_log_with_two_params_t eventlog_log_with_two_params;

    payment_platform_eventlog_log_with_three_params_t eventlog_log_with_three_params;

    payment_platform_eventlog_log_v2_t eventlog_log_v2;

    payment_platform_eventlog_log_with_one_param_v2_t eventlog_log_with_one_param_v2;

    payment_platform_eventlog_log_with_two_params_v2_t eventlog_log_with_two_params_v2;

    payment_platform_eventlog_log_with_three_params_v2_t eventlog_log_with_three_params_v2;

    payment_platform_system_time_get_msec_t system_time_get_msec;

    payment_platform_system_time_get_usec_t system_time_get_usec;

    payment_platform_tms_capk_get_data_t tms_capk_get_data;

    payment_platform_tms_capk_get_struct_version_t tms_capk_get_struct_version;

    payment_platform_tms_capk_get_prod_id_t tms_capk_get_prod_id;

    payment_platform_crc32_initial_t crc32_initial;

    payment_platform_crc32_intermediate_t crc32_intermediate;

    payment_platform_crc32_final_t crc32_final;

    payment_platform_vsnprintf_t vsn_printf;

    payment_platform_log_error_t log_error;

    payment_platform_log_info_t  log_info;

    payment_platform_log_trace_t log_trace;

    payment_platform_log_flush_t log_flush;

    void* impl;
} payment_platform_api_t;
