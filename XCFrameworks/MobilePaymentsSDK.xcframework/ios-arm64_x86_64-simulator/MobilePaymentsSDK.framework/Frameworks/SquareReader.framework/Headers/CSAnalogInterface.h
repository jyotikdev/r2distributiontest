//
//  CSAnalogInterface.h
//  CoreSwipe
//
//  Created by Sam Wen on 1/13/10.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

@class CSAnalogInterface;
@class CSAnalogSignalDecoderResult;
@class CSSwipedPaymentCard;


extern NSUInteger const CSAnalogInterfaceMaximumSamplesPerBuffer;

/// Notification posted when this analog interface starts or stops listening.
/// User info keys are below.
extern NSString *const CSAnalogInterfaceListeningDidChangeNotification;

/// Only present when the value of listening has changed to YES (when starting listening).
/// A BOOL indicating if this analog interface started listening due to a recent attachment of a device to the audio jack,
/// or for some other reason.
extern NSString *const CSAnalogInterfaceListeningDidChangeNotificationUserInfoStartListeningWasRecentlyAttachedKey;

/// A BOOL indicating whether the current CSAnalogInterfaceListeningDidChangeNotification was posted because of starting or
/// stopping listening.
extern NSString *const CSAnalogInterfaceListeningDidChangeNotificationUserInfoIsListeningKey;


@protocol CSAnalogInterfaceDelegate <NSObject>

- (void)analogInterface:(CSAnalogInterface *)analogInterface didAttemptToDecodeSignal:(CSAnalogSignalDecoderResult *)result;
- (void)analogInterface:(CSAnalogInterface *)analogInterface initializationStateDidChange:(BOOL)initialized;

@end


@interface CSAnalogInterface : NSObject

@property (nonatomic, weak) id<CSAnalogInterfaceDelegate> delegate;

/// Notify the analog interface that it is now listening.
/// @param wasRecentlyAttached Pass YES if we are starting to listen because a device was just attached to the audio jack.
/// Pass NO otherwise (e.g. if the app was launched with a device already connected).
- (void)didStartListeningDueToRecentAttachment:(BOOL)wasRecentlyAttached;

// Notify the analog interface that it is no longer listening.
- (void)didStopListening;

@property (nonatomic, assign, getter=isListening, readonly) BOOL listening;


@property (nonatomic, assign, getter=isInitialized) BOOL initialized;

/// These methods are thread-safe.

/**
 Send raw audio samples.
 @param data The interleaved 44.1kHz signed linear PCM stereo audio data to be provided to CoreAudio.
 @param loop Loop the samples until the next call to sendRawAudioData.
*/
- (void)sendRawAudioData:(NSData *)data loop:(BOOL)loop;

/** 
 Stop sending and discards previously enqueued raw audio data. This is required in order to generate a
 CSCardReaderDidSendCardReaderDataNotification for raw audio data sent with the loop flag set to YES.
 */
- (void)stopSendingAudioData;

/** 
 Fills the supplied buffer with audio data, returning the amount of data (in bytes) that was
 written to the buffer.
 */
- (size_t)fillOutgoingAudioBuffer:(void *const)buffer size:(size_t)size;

@end
