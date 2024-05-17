//
//  CRTamperFeature.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>

@class CRCardReader;

typedef NS_ENUM(NSUInteger, CRTamperFeatureStatus) {
    /// Tamper result not known.
    CRTamperFeatureStatusUnknown = 0,
    /// Device is not tampered.
    CRTamperFeatureStatusNormal = 1,
    /// Device is tampered and should not be used.
    CRTamperFeatureStatusTampered = 2,
    /// Device is non-lethally tampered and may still be used.
    CRTamperFeatureStatusFlagged = 3,
};


@protocol CRTamperDelegate <NSObject>

- (void)tamperDataReceived:(NSData *)tamperData;
- (void)tamperStatusReceived:(CRTamperFeatureStatus)tamperStatus;
- (void)tamperStatusReset;

@end // CRTamperDelegate


@interface CRTamperFeature : CRFeature

@property (nonatomic, weak) id<CRTamperDelegate> delegate;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

/// Request the reader's tamper status. [CRTamperDelegate tamperStatus] will be invoked on the
/// delegate queue.
- (BOOL)requestTamperData;
- (BOOL)requestTamperStatus;
/// @see cr_tamper_reset_tag.
- (BOOL)resetTamperStatus;

@end // CRTamperFeature
