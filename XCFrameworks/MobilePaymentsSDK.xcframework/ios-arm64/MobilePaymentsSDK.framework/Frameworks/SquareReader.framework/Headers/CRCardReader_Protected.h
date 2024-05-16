//
//  CRCardReader_Protected.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRDispatch.h>
#import <SquareReader/CRCardReader.h>


@interface CRCardReader ()

+ (dispatch_queue_t)timerQueue;
+ (BOOL)isListeningToCardReaderDriverLogs;

@property (nonatomic, strong, readwrite) CRCoredumpFeature *coredumpFeature;
@property (nonatomic, strong, readwrite) CREventlogFeature *eventlogFeature;
@property (nonatomic, strong, readwrite) CRFirmwareUpdateFeature *firmwareUpdateFeature;
@property (nonatomic, strong, readwrite) CRPaymentFeature *paymentFeature;
@property (nonatomic, strong, readwrite) CRPowerFeature *powerFeature;
@property (nonatomic, strong, readwrite) CRReaderFeature *readerFeature;
@property (nonatomic, strong, readwrite) CROmnichannelCommsFeature *secureSessionFeature;
@property (nonatomic, strong, readwrite) CRSystemFeature *systemFeature;
@property (nonatomic, strong, readwrite) CRTamperFeature *tamperFeature;
@property (nonatomic, strong, readwrite) CRUserInteractionFeature *userInteractionFeature;

- (instancetype)init NS_UNAVAILABLE;

/// Initialize a CRCardReader using the delegate dispatch queue.
- (instancetype)initWithDelegateQueue:(NSOperationQueue *)delegateQueue NS_DESIGNATED_INITIALIZER;

@property (nonatomic, assign, readwrite, getter=isAttachedToInterface) BOOL attachedToInterface;
@property (nonatomic, assign, readwrite) BOOL cannotConnectDueToMaxReadersConnected;

/// Call whenever the underlying cardreader drivers wish to log a message.
+ (void)logMessage:(NSString *)message;

@end // CRCardReader ()
