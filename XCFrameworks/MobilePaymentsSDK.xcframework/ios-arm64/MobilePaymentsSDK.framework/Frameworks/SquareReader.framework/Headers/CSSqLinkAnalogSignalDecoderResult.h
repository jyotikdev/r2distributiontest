//
//  CSSqLinkAnalogSignalDecoderResult.h
//  CoreSwipe
//
//  Created by Scott Perry on 9/20/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <SquareReader/CSAnalogSignalDecoderResult.h>


typedef NS_ENUM(NSInteger, CSSqLinkAnalogSignalDecoderControlCommand) {
    CSSqLinkAnalogSignalDecoderControlCommandUnknown = -1,
    CSSqLinkAnalogSignalDecoderControlCommandStartTone = 0,
    CSSqLinkAnalogSignalDecoderControlCommandStopTone = 1,
    CSSqLinkAnalogSignalDecoderControlCommandInvert = 2
};

typedef NS_ENUM(uint8_t, CSCommsDecodeResult) {
    CSCommsDecodeResultUnknown = 0,
    CSCommsDecodeResultSuccess,
    CSCommsDecodeResultIncomplete,
    CSCommsDecodeResultBadCRC,
    CSCommsDecodeResultBadSize,
    CSCommsDecodeResultNullValue,
    CSCommsDecodeResultOutOfMemory,
    CSCommsDecodeResultOutOfRange,
    CSCommsDecodeResultFrequencyEstimationError,
    CSCommsDecodeResultSyncGenerationError,
    CSCommsDecodeResultPacketCountError,
    CSCommsDecodeResultPacketInvalid,

};


@interface CSSqLinkAnalogSignalDecoderResult : CSAnalogSignalDecoderResult

@property (nonatomic, assign, readonly) CSCommsDecodeResult commsDecodeResult;
@property (nonatomic, assign) uint8_t decodeResult;
@property (nonatomic, assign) uint16_t packetSize;
@property (nonatomic, strong) NSData *packetHeader;

@property (nonatomic, assign) CSSqLinkAnalogSignalDecoderControlCommand controlCommand;
@property (nonatomic, strong) NSData *packetBody;

@property (nonatomic, assign) uint64_t counter;
@property (nonatomic, assign) uint64_t entropy;

/*
 * Contains floats describing the period (in seconds) of the bits on a decoded track:
 *     * track 2 periodicity is used if it was successfully read
 *     * track 1 periodicity is used if track 2 demodulation failed
 *     * empty array if the swipe failed
 * The tracks have different bit densities, so the calculation from bit period to inches per second differs for each, but the data here is unmodified from gumpack in order to ease data analysis and keep parity with Android reporting.
 */
@property (nonatomic, copy) NSArray *swipeSpeedsBitPeriod;

@property (nonatomic, assign) uint32_t track1Length;
@property (nonatomic, assign) uint32_t track3Length;
@property (nonatomic, copy) NSArray *track1Outcomes;
@property (nonatomic, copy) NSArray *track2Outcomes;
@property (nonatomic, copy) NSArray *track3Outcomes;
@property (nonatomic, assign) uint8_t nameLength;

@property (nonatomic, assign, readonly, getter=isFast) BOOL fast;
@property (nonatomic, assign, readonly) NSTimeInterval runtime;
@property (nonatomic, assign, readonly) NSTimeInterval lowPassFilterRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval findPreambleFreqRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval firstFindSyncRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval restFindSyncRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval demodulatePacketsRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval deconvolveRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval viterbiRuntime;
@property (nonatomic, assign, readonly, getter=isInverted) BOOL inverted;
@property (nonatomic, assign, readonly) float preambleFrequency;
@property (nonatomic, copy, readonly) NSArray *syncIndexes;
@property (nonatomic, copy, readonly) NSArray *packetFrequencies;
@property (nonatomic, assign, readonly) uint32_t deconvolveBitErrors;
@property (nonatomic, assign, readonly) uint32_t deconvolveBitCount;

@end
