//
//  CSAnalogInterfaceController.h
//  CoreSwipe
//
//  Created by Sam Wen on 7/15/10.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <TargetConditionals.h>
#import <Foundation/Foundation.h>


@class CSAnalogInterface;
@class CSAnalogSignalDecoderResult;
@class CSSwipedPaymentCard;
@protocol CSAnalogInterfaceControllerDelegate;


extern NSUInteger const CSAnalogInterfaceBitsPerSample;
extern NSUInteger const CSAnalogInterfaceBytesPerSample;


typedef int16_t CSAnalogInterfaceSample;


@interface CSAnalogInterfaceController : NSObject

@property (nonatomic, weak) id<CSAnalogInterfaceControllerDelegate> delegate;
@property (nonatomic, readonly, getter=isAttached) BOOL attached;
@property (nonatomic, strong, readonly) CSAnalogInterface *analogInterface;

/// Once the audio input queue is running, audio output can be enabled. This is a separate step because it should not
/// be done until the attached reader has identified itself as in need of bi-directional communications.
/// This property is thread-safe.
@property (atomic, assign) BOOL playbackEnabled;

/// This is only required if a Square Stand S3 is attached, otherwise it should be kept off
/// to avoid interfering with other audio devices the merchant may have running
@property (atomic, assign) BOOL USBAudioEnabled;


+ (CSAnalogInterfaceController *)sharedAnalogInterfaceController;

/// Initializes and configures the audio session, and begins recording audio if a suitable input device is connected.
/// In addition, the controller starts or stops recording audio as appropriate when devices are connected and disconnected.
/// This method is thread-safe.
- (void)start;

/// Stops recording and playing audio and listening for audio related events. This method is thread-safe.
- (void)stop;

@end


@protocol CSAnalogInterfaceControllerDelegate

- (void)analogInterfaceController:(CSAnalogInterfaceController *)analogInterfaceController attachedDidChange:(BOOL)attached;
- (void)analogInterfaceController:(CSAnalogInterfaceController *)analogInterfaceController initializationStateDidChange:(BOOL)initialized;
- (void)analogInterfaceController:(CSAnalogInterfaceController *)analogInterfaceController decodeEvent:(CSAnalogSignalDecoderResult *)result;
- (void)analogInterfaceController:(CSAnalogInterfaceController *)analogInterfaceController consumeSamples:(CSAnalogInterfaceSample *)inputSamples numberOfSamples:(NSUInteger)count channels:(uint8_t)channels;

@end
