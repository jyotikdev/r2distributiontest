// Copyright 2017 Square, Inc.

#pragma once

#include <cardreader_shared/crs_link.h>

#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

enum {
    CRP_AUDIO_NUM_OUTPUT_CHANNELS = 2,
    CRP_AUDIO_NUM_INPUT_CHANNELS = 1,
    CRP_AUDIO_DEFAULT_SAMPLE_RATE = 44100,
};

typedef void (*audio_samples_cb_t)(int16_t const * const data, uint32_t const num_samples, double const sample_start_time);

void crp_audio_set_sample_cbs(audio_samples_cb_t inbound_cb, audio_samples_cb_t outbound_cb);

void crp_audio_clear_sample_cbs(void);

void crp_audio_init(int sample_rate);

int crp_audio_get_buf_size(void);

bool crp_audio_send(int16_t const * samples, size_t num_samples, uint8_t num_channels, bool loop);

void crp_audio_stop_playback(void);

bool crp_audio_send_data_raw_msg(void const * bytes, size_t num_bytes, bool left_channel, bool duplicate_channels);

size_t crp_audio_recv(int16_t * samples, size_t max_samples);

void crp_audio_set_input_channel(int input_channel);

void crp_audio_set_left_output_channel(int output_channel);

void crp_audio_set_right_output_channel(int output_channel);

void crp_audio_set_inverted(bool invert);

void crp_audio_quit(void);

void crp_audio_set_signal_strength(double signal_strength);

void crp_audio_start_tone(void);

void crp_audio_stop_tone(void);

bool crp_audio_set_tx_encoding_params(crs_link_encoding_t encoding,
                                      crs_link_encoding_params_t *encoding_params);

bool crp_audio_set_phy_in_rate(crs_phy_in_rate_t phy_in_rate);

void crp_audio_pause(void);

void crp_audio_resume(void);
