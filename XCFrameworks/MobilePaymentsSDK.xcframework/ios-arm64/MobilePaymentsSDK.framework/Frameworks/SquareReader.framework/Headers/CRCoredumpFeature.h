//
//  CRCoredumpFeature.h
//  cardreader
//
//  Created by Tamir Duberstein on 11/20/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>


@class CRCardReader;


@protocol CRCoredumpDelegate <NSObject>

- (void)coredumpDataReceived:(NSData *)coredumpData withKey:(NSData *)coredumpKey;
- (void)coredumpStatusReceived:(BOOL)isCoredumpAvailable;
- (void)coredumpErasedAck;
- (void)coredumpTriggeredAck:(BOOL)wasTriggered;
- (void)coredumpProgressReceived:(float)progress;

@end // CRCoredumpDelegate


@interface CRCoredumpFeature : CRFeature

@property (nonatomic, weak) id<CRCoredumpDelegate> delegate;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

- (BOOL)requestCoredumpData;
- (BOOL)requestCoredumpStatus;
- (BOOL)coredumpErase;
- (BOOL)coredumpTrigger;

@end // CRCoredumpFeature
