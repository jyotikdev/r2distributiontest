// Copyright (c) 2023 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief TMN Utility
 *
 * @details This provides TMN utility function(s).
 * It allows the callers to extract audio id without initializing cardreader or any feature of LibCardReader.
 *
 * @author Gokhan Aydeniz <gokhan@squareup.com>
 */

#pragma once

#include <cardreader_shared/crs_tmn_msgs.h>

#include <stddef.h>

#define BYTEARRAY2LONG(x) ((((const uint8_t*)(x))[0] << 24) | (((const uint8_t*)(x))[1] << 16) | (((const uint8_t*)(x))[2] << 8) | (((const uint8_t*)(x))[3] << 0))
#define LONG2BYTEARRAY(v, ba) {ba[0] = (v >> 24); ba[1] = (uint8_t)(v >> 16); ba[2] = (uint8_t)(v >> 8); ba[3] = (uint8_t)v;}

static const uint8_t TMN_PROTOCOL_VERSION[] = {0x10, 0x00};
static const uint8_t TMN_PACKET_TYPE_REQUEST[] = {0x00, 0x11};
static const uint8_t TMN_PACKET_TYPE_DEVICE_PROCESSING_REQUEST[] = {0x00, 0x30};
static const uint8_t TMN_PACKET_TYPE_DEVICE_PROCESSING_RESPONSE[] = {0x00, 0x31};
#define TMN_PACKET_LENGTH_LENGTH 4 // Linux build fails when this is a static const size_t.
static const size_t TMN_PACKET_HEADER_LENGTH = sizeof(TMN_PROTOCOL_VERSION) + sizeof(TMN_PACKET_TYPE_REQUEST) +
                                               TMN_PACKET_LENGTH_LENGTH;
static const size_t TMN_PACKET_MESSAGE_COUNT_LENGTH = 4;
static const uint8_t TMN_PROTOCOL_INLINE_PAYMENT_RESULT[] = {0x80, 0x01};
static const size_t TMN_ERROR_GROUPING_CODE_LENGTH = 4;
static const uint8_t TMN_DEVICE_CONTROL_CODE_FELICA_COMMAND[] = {0x00, 0x21};
static const uint8_t TMN_DEVICE_CONTROL_CODE_DEVICE_COMMAND[] = {0x00, 0x23};
static const uint8_t TMN_DEVICE_COMMAND_CODE_LED_LIT[] = {0x00, 0x11};
static const uint8_t TMN_DEVICE_COMMAND_CODE_LED_BLINK[] = {0x01, 0x11};
static const uint8_t TMN_DEVICE_COMMAND_CODE_SOUND[] = {0x01, 0x01};
static const uint8_t TMN_DEVICE_COMMAND_CODE_SOUND_LOOP[] = {0x10, 0x00};
static const uint8_t TMN_DEVICE_COMMAND_SOUND_VOLUME_OFFSET = 16;
static const uint8_t TMN_DEVICE_COMMAND_SOUND_AUDIO_NUMBER_OFFSET = 17;
static const size_t TMN_DEVICE_CONTROL_COMMAND_LENGTH_OFFSET = 16;
static const size_t TMN_DEVICE_COMMAND_CODE_OFFSET = 20;
static const size_t TMN_DEVICE_COMMAND_OFFSET = 24;
static const size_t TMN_FELICA_COMMAND_LENGTH_OFFSET = 20;
static const size_t TMN_FELICA_COMMAND_OFFSET = 21;
static const size_t TMN_DEVICE_PROCESSING_RESPONSE_HEADER_LENGTH_OFFSET = 4;
static const size_t TMN_DEVICE_PROCESSING_RESPONSE_DATA_LENGTH_OFFSET = 8;
static const size_t TMN_DEVICE_PROCESSING_RESPONSE_EMPTY_LENGTH = 8;
static const size_t TMN_REQUEST_HEADER_LENGTH = 16;
static const size_t TMN_REQUEST_TYPE_OFFSET = 10;
static const uint8_t TMN_REQUEST_TYPE_TRANSACTION[] = {0x00, 0x01};
static const uint8_t TMN_REQUEST_TYPE_CHECK_BALANCE[] = {0x00, 0x03};

/**
 * @brief Extracts the audio id.
 *
 * @param data TMN message.
 * @param data_len TMN message length.
 *
 * @return TMN audio id, TMN_AUDIO_NONE if none exists.
 */
crs_tmn_audio_t cr_tmn_util_extract_audio_id(uint8_t const *data, size_t data_len);
