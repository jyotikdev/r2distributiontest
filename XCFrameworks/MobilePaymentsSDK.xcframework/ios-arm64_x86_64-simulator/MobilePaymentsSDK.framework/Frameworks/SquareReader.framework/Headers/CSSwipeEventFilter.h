//
//  CSSwipeEventFilter.h
//  CoreSwipe
//
//  Created by Martin Mroz on 8/8/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@class CSAnalogSignalDecoderResult;
@protocol CSSwipeEventFilterDelegate;


//
// The swipe event filter is designed to support the new R4 approach to data transmission which
// is essentially to send data once quickly (the Fast packet) and then again slowly (the Slow packet)
// at approximately 6kHz and 1.5kHz respectively. This means that we only really want to forward part of
// the message to the customer each time.
//
// When you swipe twice in rapid succession, however, a Fast packet is sent followed by an interrupted
// slow packet, followed by another Fast packet, followed by another Slow packet (unless the user
// swipes again, in which case the slow packet may once again be interrupted).
//
// As such the Swipe Event Filter should be used to limit interactions with the user. It is designed
// to support the following cases (at minimum). The forwarded signal is in square brackets.
//
//   [SuccessfulOtherDecode]
//    [SuccessfulSlowDecode]
//    [SuccessfulFastDecode] ->   SucessfulSlowDecode
//             FailedDecode  ->         [FailedDecode]
//             FailedDecode  ->  [SuccessfulAnyDecode]
//    [SuccessfulFastDecode] -> [SuccessfulFastDecode] -> ^StartOver^ (Interrupted Swipe)
//    [SuccessfulFastDecode] ->          FailedDecode  -> ^StartOver^ (Interrupted Swipe)
//             FailedDecode  ->         [FailedDecode] -> ^StartOver^ (Interrupted Swipe)
//


@interface CSSwipeEventFilter : NSObject

@property (nonatomic, weak) id<CSSwipeEventFilterDelegate> delegate;

- (void)filterSwipeDecodeResult:(CSAnalogSignalDecoderResult *)decoderResult;

//
// Use -reset to return the event filter to the initial state. This can be done at any time
// you need to return to a known default state (i.e. when the reader connection state is changed).
// This will cancel any internal state transition timers and return to the intial state
// without posting any messages.
//
- (void)reset;

@end


@protocol CSSwipeEventFilterDelegate <NSObject>

- (void)swipeEventFilter:(CSSwipeEventFilter *)eventFilter didDetectSuccessfulDecodeEvent:(CSAnalogSignalDecoderResult *)event;
- (void)swipeEventFilter:(CSSwipeEventFilter *)eventFilter didDetectFailedDecodeEvent:(CSAnalogSignalDecoderResult *)event;

@end
