// Copyright (c) 2013-2014 Square, Inc. All rights reserved.

#pragma once

#include <cardreader_shared/crs_pack.h>

#include <stdint.h>

#define M1_PACKET_COUNTER_LENGTH (6)

#define M1_PACKET_ENTROPY_LENGTH (6)

#define M1_PACKET_SWIPE_SPEED_LENGTH (3)

#define M1_PACKET_LAST4_LENGTH (4)

#define M1_PACKET_NAME_LENGTH (26)

#define M1_PACKET_AUTH_TAG_LENGTH (8)

#define M1_PACKET_MAX_TRACK_1_LENGTH (128)

#define M1_PACKET_MAX_TRACK_2_LENGTH (128)

#define M1_PACKET_MAX_TRACK_3_LENGTH (128)

typedef struct SQ_PACK() {
    uint8_t counter[M1_PACKET_COUNTER_LENGTH];
    uint8_t entropy[M1_PACKET_ENTROPY_LENGTH];
    uint8_t swipeDirection;
    uint16_t swipeSpeed[M1_PACKET_SWIPE_SPEED_LENGTH];
} M1PacketCardDataPlainText_t;

/* Track result error codes. */
#define M1_PACKET_TRACK_RESULT_NOT_PRESENT (1 << 0) // Track not present
#define M1_PACKET_TRACK_RESULT_NO_ZEROES   (1 << 1) // No zeros detected
#define M1_PACKET_TRACK_RESULT_TOO_LONG    (1 << 2) // Card data too long
#define M1_PACKET_TRACK_RESULT_TOO_SHORT   (1 << 3) // Card data to short
#define M1_PACKET_TRACK_RESULT_NO_START    (1 << 4) // Missing start sentinel
#define M1_PACKET_TRACK_RESULT_NO_END      (1 << 5) // Missing end sentinel
#define M1_PACKET_TRACK_RESULT_PARITY_ERR  (1 << 6) // Parity error
#define M1_PACKET_TRACK_RESULT_CRC_ERR     (1 << 7) // LRC failure

typedef enum SQ_PACK() {
    M1_PACKET_ISSUER_ID_VISA = 0,
    M1_PACKET_ISSUER_ID_MASTERCARD = 1,
    M1_PACKET_ISSUER_ID_DISCOVER = 2,
    M1_PACKET_ISSUER_ID_AMERICAN_EXPRESS = 3,
    M1_PACKET_ISSUER_ID_JCB = 4,
    M1_PACKET_ISSUER_ID_OTHER = 5
} M1PacketIssuerId_e;

typedef struct SQ_PACK() {
    uint8_t t1Len;
    uint8_t t2Len;
    uint8_t t3Len;
    uint8_t t1Result;
    uint8_t t2Result;
    uint8_t t3Result;
    M1PacketIssuerId_e issuerId;
    uint8_t last4[M1_PACKET_LAST4_LENGTH]; /* ASCII */
    uint8_t nameLen;
    uint8_t name[M1_PACKET_NAME_LENGTH]; /* ASCII */
} M1PacketCardDataAuthenticated_t;

typedef struct SQ_PACK() {
    uint8_t authTag[M1_PACKET_AUTH_TAG_LENGTH];
    uint8_t trackData[M1_PACKET_MAX_TRACK_1_LENGTH + M1_PACKET_MAX_TRACK_2_LENGTH + M1_PACKET_MAX_TRACK_3_LENGTH];
} M1PacketCardDataEncrypted_t;
