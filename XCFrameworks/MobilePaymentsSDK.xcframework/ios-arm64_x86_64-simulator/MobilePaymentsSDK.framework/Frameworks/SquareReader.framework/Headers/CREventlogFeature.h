//
//  CREventlogFeature.h
//  cardreader
//
//  Created by Tamir Duberstein on 11/3/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>


@class CRCardReader;
@class CREventlogFeature;
@class CREventlogEvent;


@protocol CREventlogDelegate <NSObject>

/**
 Called once for each individual event extracted from an event log batch received from a card reader. This can be called
 a number of times in rapid succession, so batching should be used to reduce network overhead if these are being
 uploaded to the server.
 
 @param eventlogFeature The eventlog feature which received the event.
 @param eventName The name of the currently processed event (for instance, "CRS_EVENT_EVENTLOG_INIT").
 @param timestamp The 32-bit integer representing the timestamp as provided by the reader.
 */
- (void)eventlogFeature:(CREventlogFeature *)eventlogFeature didReceiveEventNamed:(NSString *)eventName timestamp:(NSNumber *)timestamp;

/**
 Called when the reader firmware has logged an error.

 @param eventlogFeature The eventlog feature which received the event.
 @param message         The error message being logged.
 */
- (void)eventlogFeature:(CREventlogFeature *)eventlogFeature didReceiveErrorLogWithMessage:(NSString *)message;

/**
 Called when the reader firmware sends reader profile data.
 
 @param eventlogFeature The eventlog feature which received the profile data.
 @param profileData     The profile data.
 */
- (void)eventlogFeature:(CREventlogFeature *)eventlogFeature didReceiveProfileData:(NSData *)profileData;

@optional

- (void)eventlogFeature:(CREventlogFeature *)eventlogFeature didReceiveEvents:(NSArray<CREventlogEvent *> *)events;

@end


@interface CREventlogFeature : CRFeature

@property (nonatomic, weak) id<CREventlogDelegate> delegate;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

@end

@interface CREventlogEvent : NSObject

@property (nonatomic, copy) NSNumber *timestamp;
@property (nonatomic, copy) NSString *eventName;

@end
