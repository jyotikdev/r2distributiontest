//
//  CSAnalogSignalDecoderResultCarrierDetectStatistics.h
//  CoreSwipe
//
//  Created by Scott Perry on 9/16/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef NS_ENUM(uint8_t, CRCommsEventCarrierDetect) {
    CRCommsEventCarrierDetectUnknown = 0,
    CRCommsEventCarrierDetectSuccess,
    CRCommsEventCarrierDetectRestart,
    CRCommsEventCarrierDetectDiscard,
    CRCommsEventCarrierDetectTooShort,
    CRCommsEventCarrierDetectTooLong,
    CRCommsEventCarrierDetectReady,
    CRCommsEventCarrierDetectNotReady,
};

typedef NS_ENUM(uint8_t, CRCommsLinkType) {
    CRCommsLinkTypeUnknown = 0,
    CRCommsLinkTypeNoise,
    CRCommsLinkTypeO1,
    CRCommsLinkTypeGen2,
    CRCommsLinkTypeSQLinkFast,
    CRCommsLinkTypeSQLinkSlow,
};


@interface CSAnalogSignalDecoderResultClassifyStatistics : NSObject

@property (nonatomic, assign, readonly) CRCommsLinkType commsLinkType;
@property (nonatomic, assign, readonly) int link;
@property (nonatomic, assign, readonly) NSTimeInterval runtime;
@property (nonatomic, assign, readonly) NSTimeInterval lowPassFilterRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval squareCalculationAndMeanRemovalRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval fftRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval fftAnalysisRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval gen2LowPassFilterRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval normalizeAndCenterAroundMeanRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval findPeaksRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval calculateSpacingsAndVariabilityRuntime;
@property (nonatomic, assign, readonly) float o1Score;
@property (nonatomic, assign, readonly) float m1FastScore;
@property (nonatomic, assign, readonly) float m1SlowScore;
@property (nonatomic, assign, readonly) float gen2Score;
@property (nonatomic, assign, readonly) int peakCount;
@property (nonatomic, assign, readonly) float peakVar;

@end


@interface CSAnalogSignalDecoderResultCarrierDetectStatistics : NSObject

@property (nonatomic, assign, readonly) CRCommsEventCarrierDetect carrierDetectEvent;
@property (nonatomic, assign, readonly) int event;
@property (nonatomic, assign, readonly) uint64_t startSample;
@property (nonatomic, assign, readonly) uint32_t samples;
@property (nonatomic, assign, readonly) NSTimeInterval totalRuntime;
@property (nonatomic, assign, readonly) NSTimeInterval totalTime;
@property (nonatomic, assign, readonly) uint16_t startThreshold;
@property (nonatomic, assign, readonly) uint16_t startAverage;
@property (nonatomic, assign, readonly) uint16_t endThreshold;
@property (nonatomic, assign, readonly) uint16_t endAverage;
@property (nonatomic, assign, readonly) BOOL allowRestart;
@property (nonatomic, strong, readonly) CSAnalogSignalDecoderResultClassifyStatistics *classify1Statistics;
@property (nonatomic, strong, readonly) CSAnalogSignalDecoderResultClassifyStatistics *classify2Statistics;
@property (nonatomic, assign, readonly) NSTimeInterval inPacketRuntime;

- (NSDictionary *)loggingDictionary;

@end
