// Copyright (c) 2013-2014 Square, Inc. All rights reserved.

#pragma once

#include <cardreader_shared/crs_pack.h>

#define SQ_LINK_PACKET_CRC_LENGTH (4)

#define SQ_LINK_PACKET_PACKET_HEADER_LENGTH (8)

#define SQ_LINK_PACKET_READER_ID_LENGTH (8)

#define SQ_LINK_PACKET_TYPE(SYM, VAL) SQ_LINK_PACKET_TYPE_ ## SYM = VAL,
typedef enum SQ_PACK() {
#include <cardreader/SqLinkPackets.def>
} SqLinkPacketType_e;

typedef struct SQ_PACK() {
    char packetHeader[SQ_LINK_PACKET_PACKET_HEADER_LENGTH]; // ASCII bytes, eg "SQUAREM1" or "SQUARER6"
    uint8_t readerId[SQ_LINK_PACKET_READER_ID_LENGTH]; // UUID programmed into EEPROM
    uint8_t hwMajorRev; // A=0, B=1 ...
    uint8_t hwMinorRev; // 0=0 ...
    SqLinkPacketType_e packetType;
    uint16_t payloadSize;
} SqLinkPacketHeader_t;

#define SQ_LINK_PACKET_PAYLOAD_SIZE(_hdrPtr) ((uint32_t)(_hdrPtr)->payloadSizeLo + ((uint32_t)(_hdrPtr)->payloadSizeHi << 8))
