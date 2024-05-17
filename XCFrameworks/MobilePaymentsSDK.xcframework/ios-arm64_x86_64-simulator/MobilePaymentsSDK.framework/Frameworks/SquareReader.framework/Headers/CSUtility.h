//
//  CSUtility.h
//  CoreSwipe
//
//  Created by Sam Wen on 7/13/10.
//  Copyright 2010 Square, Inc. All rights reserved.
//
//  This file contains data structures, functions, and constants related to the
//  magstripe.
//

#import <Foundation/Foundation.h>


#define CSFree(x)     \
    do {              \
        if (x) {      \
            free(x);  \
            x = NULL; \
        }             \
    } while (0)

#define CSFreeAndZeroizeNullTerminatedString(x)     \
    do {                                            \
        if (x) {                                    \
            memset(x, 0, strlen(x) * sizeof(char)); \
            CSFree(x);                              \
        }                                           \
    } while (0)

#define CSFreeAndZeroizeBytesWithLength(x, len) \
    do {                                        \
        if (x) {                                \
            memset(x, 0, len);                  \
            CSFree(x);                          \
        }                                       \
    } while (0)

#define CSBitsPerByte 8

extern const long CSRawSignalMaximumLength;
extern const unsigned long CSBitArraySubarrayNotFound;

typedef enum {
    CSTrackType1,
    CSTrackType2,
    CSTrackType3, // Unused
    CSTrackTypeEncryptedO1Data,
    CSTrackTypeEncryptedR4Track1Data,
    CSTrackTypeEncryptedR4Track2Data,
    CSTrackTypeEncryptedR4Track1AndTrack2Data,
    CSTrackTypeEncryptedA10Track1Data,
    CSTrackTypeEncryptedA10Track2Data,
    CSTrackTypeEncryptedA10Track1AndTrack2Data,
} CSTrackType;

typedef struct {
    short *rawSignal;
    long rawSignalLength;
} CSRawSignal;

typedef CSRawSignal *CSRawSignalRef;

typedef struct {
    char *track;
    size_t trackLength;
    CSTrackType trackType;
} CSTrack;

typedef CSTrack *CSTrackRef;

typedef struct {
    unsigned int *bits;
    unsigned long bitCount;
} CSBitArray;

typedef CSBitArray *CSBitArrayRef;

CSRawSignalRef CSRawSignalCreate(long rawSignalSize);
void CSRawSignalRelease(CSRawSignalRef rawSignal);

CSTrackRef CSTrackCreate(CSTrackType trackType, int trackSize);
void CSTrackRelease(CSTrackRef track);

CSBitArrayRef CSBitArrayCreate(unsigned long bitCount);
void CSBitArraySetOne(CSBitArrayRef bitArray, unsigned long index);
void CSBitArraySetZero(CSBitArrayRef bitArray, unsigned long index);
void CSBitArrayZeroOut(CSBitArrayRef bitArray);
void CSBitArrayOneOut(CSBitArrayRef bitArray);
unsigned int CSBitArrayGetBit(CSBitArrayRef bitArray, unsigned long index);
unsigned int CSBitArrayGetBits(CSBitArrayRef bitArray, unsigned long index, unsigned long length);
unsigned int CSBitArrayGetBitsReverse(CSBitArrayRef bitArray, unsigned long index, unsigned long length);
unsigned long CSBitArrayGetBitCount(CSBitArrayRef bitArray);
unsigned long CSBitArrayIndexOfSubarray(CSBitArrayRef bitArray, CSBitArrayRef subArray);
void CSBitArrayRelease(CSBitArrayRef bitArray);

// Utility Methods
unsigned char CSByteBySwitchingEndianness(unsigned char inByte);
unsigned char CSASCIIByteToANSIISOAlphaByte(unsigned char inByte);
void CSStripWhiteSpaceFromString(char *string);
