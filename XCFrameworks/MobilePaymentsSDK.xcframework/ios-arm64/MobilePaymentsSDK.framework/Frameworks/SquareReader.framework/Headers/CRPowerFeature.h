//
//  CRPowerFeature.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>

@class CRCardReader;

@protocol CRPowerDelegate <NSObject>

/// Called when we receive a power update. Not all fields are gauranteed to be valid for all readers.
- (void)powerReceivedVoltage:(NSUInteger)voltage voltagePercentage:(NSUInteger)voltagePercentage current:(NSInteger)current temperature:(NSInteger)temperature critical:(BOOL)critical charging:(BOOL)charging;

- (void)powerResetAck;
- (void)powerOffAck;

@end // CRPowerDelegate


@interface CRPowerFeature : CRFeature

@property (nonatomic, weak) id<CRPowerDelegate> delegate;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

/// Request the reader battery's current state. [CRPowerDelegate powerBatteryVoltageReceived:] and [CRPowerDelegate chargingStatusReceived:]
/// will be invoked on the delegate queue if applicable.
- (BOOL)requestChargeStatus;

/// Ask for the reader power to be reset. [CRPowerDelegate powerResetAck] will be invoked on the
/// delegate queue.
- (BOOL)powerReset;

/// Ask for the reader power to be turned off. [CRPowerDelegate powerOffAck] will be invoked on the
/// delegate queue.
- (BOOL)powerOff;

@end // CRPowerFeature
