//
//  CSCardReaderController.h
//  CoreSwipe
//
//  Created by Sam Wen on 1/13/10.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <TargetConditionals.h>
#import <Foundation/Foundation.h>

#import <SquareReader/SquareReader.h>


extern NSString *const CSCardReaderSwipeDidFailNotification;
extern NSString *const CSCardReaderDidSwipePaymentCardNotification;
extern NSString *const CSCardReaderDecodeEventNotification;

extern NSString *const CSCardReaderDidSendCardReaderDataNotification;

extern NSString *const CSCardReaderErrorKey;
extern NSString *const CSCardReaderPaymentCardKey;
extern NSString *const CSCardReaderReaderAttributesKey;
extern NSString *const CSCardReaderReaderDataKey;

extern NSString *const CSLocalizedTitleErrorKey;


@class CSCardReaderController;


/// All delegate messages are delivered on the main queue.

@protocol CSCardReaderControllerDelegate <NSObject>

- (void)cardReaderController:(CSCardReaderController *)cardReaderController cardReaderModelDidChange:(NSString *)model;
- (void)cardReaderController:(CSCardReaderController *)cardReaderController initializedDidChange:(BOOL)initialized;
- (void)cardReaderController:(CSCardReaderController *)cardReaderController attachedDidChange:(BOOL)attached;
- (void)cardReaderController:(CSCardReaderController *)cardReaderController analogInterfaceDidReceiveAudioData:(NSData *)data numberOfSamples:(size_t)numberOfSamples channels:(uint8_t)channels;

@end


@interface CSCardReaderController : NSObject <CSAnalogInterfaceControllerDelegate>

@property (nonatomic, readonly, getter=isAttached) BOOL attached;
@property (nonatomic, readonly, getter=isInitialized) BOOL initialized;
@property (nonatomic, assign, getter=isListening) BOOL listening;

@property (nonatomic, weak) id<CSCardReaderControllerDelegate> delegate;

+ (instancetype)sharedCardReaderController;

/// Invoke this method when the expected reader type is no longer known. For instance,
/// when the current application enters the background.
- (void)clearExpectedReaderType;

/** 
 When NO, posts `CSCardReaderSwipeDidFailNotification` on both demodulation failures
 and successful demodulations lacking valid swipe data. When YES, posts `CSCardReaderSwipeDidFailNotification` 
 only when a successful demodulation indicates the absence of swipe data.
 */
@property (nonatomic, assign) BOOL ignoresDemodulationFailures;

/// Notifies the delegate that the expected reader type changed on the main queue.
@property (nonatomic, strong) NSString *expectedReaderType;

@end
