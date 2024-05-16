//
//  CSAnalogSignalDecoderResult.h
//  CoreSwipe
//
//  Created by Martin Mroz on 7/19/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "CSAnalogSignalDecoderResultCarrierDetectStatistics.h"


extern NSString *const CSHardwareDeviceModelGen2;
extern NSString *const CSHardwareDeviceModelOersted;
extern NSString *const CSHardwareDeviceModelR4;
extern NSString *const CSHardwareDeviceModelR6;

extern NSString *const CSAnalogSignalDecoderResultKey;

extern NSString *const CSR4HardwareIDKey;


@class CSSwipedPaymentCard;


typedef enum : NSUInteger {
    CSAnalogSignalDecoderResultTypeUnknown,
    CSAnalogSignalDecoderResultTypeSwipeMessage,
    CSAnalogSignalDecoderResultTypeReaderAwakeMessage,
    CSAnalogSignalDecoderResultTypeReaderDeadMessage,
    CSAnalogSignalDecoderResultTypeReaderBlankMessage,
    CSAnalogSignalDecoderResultTypeRawDataMessage,
    CSAnalogSignalDecoderResultTypeControlMessage,
} CSAnalogSignalDecoderResultType;


NSString *NSStringFromCSAnalogSignalDecoderResultType(CSAnalogSignalDecoderResultType type);


typedef enum : NSUInteger {
    CSAnalogSignalDecoderResultSwipeDirectionUnknown = 0,
    CSAnalogSignalDecoderResultSwipeDirectionForwards,
    CSAnalogSignalDecoderResultSwipeDirectionBackwards
} CSAnalogSignalDecoderResultSwipeDirection;

typedef enum : NSUInteger {
    CSAnalogSignalDecoderResultErrorUnknown = 0,
    CSAnalogSignalDecoderResultErrorTrackNotPresent,
    CSAnalogSignalDecoderResultErrorNoZeroesDetected,
    CSAnalogSignalDecoderResultErrorCardDataTooLong,
    CSAnalogSignalDecoderResultErrorCardDataTooShort,
    CSAnalogSignalDecoderResultErrorMissingStartSentinel,
    CSAnalogSignalDecoderResultErrorMissingEndSentinel,
    CSAnalogSignalDecoderResultErrorParityCheckFailed,
    CSAnalogSignalDecoderResultErrorLRCFailed,
    CSAnalogSignalDecoderResultErrorInvalidCharacter,
    CSAnalogSignalDecoderResultErrorCounterFailed
} CSAnalogSignalDecoderResultError;

typedef enum : NSUInteger {
    CSAnalogSignalDecoderResultLinkUnknown = 0,
    CSAnalogSignalDecoderResultLinkNoise,
    CSAnalogSignalDecoderResultLinkGen2,
    CSAnalogSignalDecoderResultLinkOersted,
    CSAnalogSignalDecoderResultLinkSqLinkFast,
    CSAnalogSignalDecoderResultLinkSqLinkSlow
} CSAnalogSignalDecoderResultLinkType;

// Values defined by SignalFound_Decision in swipe_experience.proto.
typedef enum : NSUInteger {
    CSSwipeEventFilterDecisionSwipeSuccess = 0,
    CSSwipeEventFilterDecisionSwipeFailed = 1,
    CSSwipeEventFilterDecisionDelayedFailed = 2,
    CSSwipeEventFilterDecisionIgnoredNoise = 3,
    CSSwipeEventFilterDecisionIgnoredNoAction = 4,
    CSSwipeEventFilterDecisionDeadReader = 5
} CSSwipeEventFilterDecision;


NSString *NSStringFromCSAnalogSignalDecoderResultLinkType(CSAnalogSignalDecoderResultLinkType type);


static inline uint64_t CSMicrosecondsFromNSTimeInterval(NSTimeInterval interval)
{
    return (uint64_t)(interval * 1e6);
}


@interface CSAnalogSignalDecoderResultHardwareInfo : NSObject

@property (nonatomic, copy) NSString *model;
@property (nonatomic, copy) NSString *readerID;
@property (nonatomic, assign) uint8_t hardwareMajorRevision;
@property (nonatomic, assign) uint8_t hardwareMinorRevision;

@end


@interface CSAnalogSignalDecoderResult : NSObject

//
// These values originate in the Analog Interface and Card Reader Controller.
// They are always available, even if the decode failed. They are used to support event
// coalescing and performance metrics.
//
@property (nonatomic, assign, getter=isEarlyPacket) BOOL earlyPacket;
@property (nonatomic, strong) NSDate *swipeEventDate;
@property (nonatomic, assign) CSAnalogSignalDecoderResultLinkType classifiedLinkType;
@property (nonatomic, assign) CSAnalogSignalDecoderResultLinkType actualLinkType;
@property (nonatomic, assign) NSTimeInterval decodeDuration;
@property (nonatomic, assign) NSTimeInterval signalStartToDecisionDuration;
@property (nonatomic, assign) NSUInteger numberOfSamples;
@property (nonatomic, assign, readonly) BOOL decodeSucceeded;
@property (nonatomic, copy) NSString *expectedReaderType;

@property (nonatomic, strong) CSAnalogSignalDecoderResultCarrierDetectStatistics *carrierDetectStatistics;

//
// The following values originate directly from Analog Signal Decoders.
//
@property (nonatomic, assign) CSAnalogSignalDecoderResultType type;
@property (nonatomic, strong) CSAnalogSignalDecoderResultHardwareInfo *readerInfo;
@property (nonatomic, strong) CSSwipedPaymentCard *swipedPaymentCard;
@property (nonatomic, assign) uint32_t track2Length;
@property (nonatomic, assign) CSAnalogSignalDecoderResultSwipeDirection swipeDirection;

//
// The following value originates from the swipe event filter.
//
@property (nonatomic, assign) CSSwipeEventFilterDecision decision;

- (NSDictionary *)readerAttributesForSwipeNotification;

@end
