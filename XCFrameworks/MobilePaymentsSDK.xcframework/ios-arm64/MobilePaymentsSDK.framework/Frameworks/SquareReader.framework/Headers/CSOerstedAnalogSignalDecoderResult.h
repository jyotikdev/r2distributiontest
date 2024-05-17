//
//  CSOerstedAnalogSignalDecoderResult.h
//  CoreSwipe
//
//  Created by Scott Perry on 9/20/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <SquareReader/CSAnalogSignalDecoderResult.h>


//
// The following error codes are listed and explained in Square Reader
// Protocol v1.8 (October 21, 2011). They should have 1:1 mappings to error codes
// specfied within the Analog Signal Decoder Result object.
//

typedef enum : char {
    CSOerstedErrorNone = 0,
    CSOerstedErrorCardDataTooLong = '>',
    CSOerstedErrorCardDataTooShort = '<',
    CSOerstedErrorMissingStartSentinel = '%',
    CSOerstedErrorMissingEndSentinel = '?',
    CSOerstedErrorParityCheckFailed = 'p',
    CSOerstedErrorLRCFailed = '!',
    CSOerstedErrorCounterFailed = '#',
    CSOerstedErrorNoZeroesDetected = 'z',
    CSOerstedErrorUnknown = 'x'
} CSOerstedError;


@interface CSOerstedAnalogSignalDecoderResult : CSAnalogSignalDecoderResult

@property (nonatomic, assign) BOOL decodeSucceeded;
@property (nonatomic, copy) NSData *nonSuccessfulSwipeMessage;
@property (nonatomic, assign) CSOerstedError errorCode;

@property (nonatomic, assign) uint16_t packetSize;
@property (nonatomic, assign) uint8_t authenticatedLength;

@property (nonatomic, assign) uint64_t counter;
@property (nonatomic, assign) uint64_t entropy;
@property (nonatomic, copy) NSArray *swipeSpeedsInchesPerSecond;

@property (nonatomic, copy) NSArray *track2Outcomes;

@property (nonatomic, assign) uint8_t numResets;
@property (nonatomic, assign) uint16_t numWakeups;
@property (nonatomic, assign) uint8_t status;
@property (nonatomic, assign) uint8_t startPeriod;
@property (nonatomic, assign) uint8_t endPeriod;

@end
