//
//  CRCardReader.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRDispatch.h>
#import <SquareReader/CRCoredumpFeature.h>
#import <SquareReader/CREventlogFeature.h>
#import <SquareReader/CRFirmwareUpdateFeature.h>
#import <SquareReader/CRPaymentFeature.h>
#import <SquareReader/CRPowerFeature.h>
#import <SquareReader/CRReaderFeature.h>
#import <SquareReader/CROmnichannelCommsFeature.h>
#import <SquareReader/CRSystemFeature.h>
#import <SquareReader/CRTamperFeature.h>
#import <SquareReader/CRUserInteractionFeature.h>
#import <SquareReader/CRTransport.h>
#import <SquareReader/CRCardReaderBackendARQ.h>
#import <SquareReader/CRCardReaderBackendAudio.h>
#import <SquareReader/CRCardReaderBackendUSB.h>
#import <SquareReader/CRCardReaderBackendBTLE.h>

typedef NSDictionary<NSString *, NSNumber *> CardReaderFeatureFlags;

/// Notification posted whenever the underlying cardreader drivers wish to log a message. The object is the class of the card reader that logged the message.
extern NSString *const CRCardReaderLogMessageWasLoggedNotification;

/// The key in the userInfo dictionary of a `CRCardReaderLogMessageWasLoggedNotification` for the log message itself.
extern NSString *const CRCardReaderLogMessageWasLoggedNotificationLogMessageKey;


/**
 CRCardReader is the abstract class for interaction with cardreader drivers.
 The CRCardReader for the desired reader type should be allocated via the factory methods.

 CRCardReader is a holder for the individual cardreader features. Each feature has a
 corresponding delegate that is used by the feature to communicate with the app.

 All interaction with libcardreader happens on a background thread, and
 libcardreader is itself asynchronous by design. What this means for the user of these classes
 is a slightly inconvenient interaction pattern. For example, retrieving the battery voltage is
 done by a call to [CRPowerFeature requestChargeStatus] which will trigger an invocation of
 [CRPowerDelegate powerBatteryVoltage].

 NOTE: All delegate methods are invoked on the "delegate queue" which defaults to the main
 thread.

 CRCardReader is agnostic to the underlying hardware by utilizing a supplied
 CRTransport. Implementations of CRTransport provide methods to send data to the hardware and
 notify CRCardReader when data is received from the hardware. See Tests/fakeReaderTransport.[hm] for an
 implementation of CRTransport which simulates an R6 instead of talking to actual hardware.
 */
@interface CRCardReader : NSObject

/// Starts listening to logs coming from the underlying card reader drivers and posting notifications based on those logs for all card reader instances created after this method is called.
+ (void)startListeningToCardReaderDriverLogs;

- (instancetype)init NS_UNAVAILABLE;

/// Creates a new instance of CRCardReaderBackendARQ and initializes with it.
- (instancetype)initWithTransportARQ:(id<CRARQTransport>)transport delegateQueue:(NSOperationQueue *)delegateQueue merchantCategoryCode:(uint16_t)mcc currencyCode:(uint16_t)currencyCode offlineMode:(CROfflineMode)offlineMode secureSessionFeatureFactory:(CROmnichannelCommsFeature * (^)(CRCardReader *, CROfflineMode))secureSessionFeatureFactory lcrFeatureFlags:(CardReaderFeatureFlags *)lcrFeatureFlags readerFeatureFlags:(CardReaderFeatureFlags *)readerFeatureFlags;

/// Creates a new instance of CRCardReaderBackendAudio and initializes with it.
- (instancetype)initWithTransportAudio:(id<CRAudioTransport>)transport delegateQueue:(NSOperationQueue *)delegateQueue merchantCategoryCode:(uint16_t)mcc currencyCode:(uint16_t)currencyCode offlineMode:(CROfflineMode)offlineMode secureSessionFeatureFactory:(CROmnichannelCommsFeature * (^)(CRCardReader *, CROfflineMode))secureSessionFeatureFactory lcrFeatureFlags:(CardReaderFeatureFlags *)lcrFeatureFlags readerFeatureFlags:(CardReaderFeatureFlags *)readerFeatureFlags;

/// Creates a new instance of CRCardReaderBackendUSB and initializes with it.
- (instancetype)initWithTransportUSB:(id<CRUSBTransport>)transport delegateQueue:(NSOperationQueue *)delegateQueue merchantCategoryCode:(uint16_t)mcc currencyCode:(uint16_t)currencyCode offlineMode:(CROfflineMode)offlineMode secureSessionFeatureFactory:(CROmnichannelCommsFeature * (^)(CRCardReader *, CROfflineMode))secureSessionFeatureFactory lcrFeatureFlags:(CardReaderFeatureFlags *)lcrFeatureFlags readerFeatureFlags:(CardReaderFeatureFlags *)readerFeatureFlags;

/// Creates a new instance of CRCardReaderBackendBTLE and initializes with it.
- (instancetype)initWithTransportBTLE:(id<CRBTLETransport>)transport delegateQueue:(NSOperationQueue *)delegateQueue merchantCategoryCode:(uint16_t)mcc currencyCode:(uint16_t)currencyCode offlineMode:(CROfflineMode)offlineMode secureSessionFeatureFactory:(CROmnichannelCommsFeature * (^)(CRCardReader *, CROfflineMode))secureSessionFeatureFactory lcrFeatureFlags:(CardReaderFeatureFlags *)lcrFeatureFlags readerFeatureFlags:(CardReaderFeatureFlags *)readerFeatureFlags;

- (void)playCRWakeUpToneOverAnalogInterfaceIfNeeded;

/// Set LCR-specific feature flags using cr_cardreader_set_lcr_feature_flag.
- (BOOL)setCRFeatureFlags:(NSDictionary<NSString *, NSNumber *> *)featureFlags;

/// Retrieves the various protocol versions so caller can ensure they are compatible.
+ (void)getHostAppProtocolVersion:(uint32_t *const)appProtocolVersion endpointProtocolVersion:(uint32_t *const)endpointProtocolVersion transportProtocolVersion:(uint32_t *const)transportProtocolVersion;

@property (nonatomic, strong, readonly) CRCoredumpFeature *coredumpFeature;
@property (nonatomic, strong, readonly) CREventlogFeature *eventlogFeature;
@property (nonatomic, strong, readonly) CRFirmwareUpdateFeature *firmwareUpdateFeature;
@property (nonatomic, strong, readonly) CRPaymentFeature *paymentFeature;
@property (nonatomic, strong, readonly) CRPowerFeature *powerFeature;
@property (nonatomic, strong, readonly) CRReaderFeature *readerFeature;
@property (nonatomic, strong, readonly) CROmnichannelCommsFeature *secureSessionFeature;
@property (nonatomic, strong, readonly) CRSystemFeature *systemFeature;
@property (nonatomic, strong, readonly) CRTamperFeature *tamperFeature;
@property (nonatomic, strong, readonly) CRUserInteractionFeature *userInteractionFeature;

@property (nonatomic, readonly, assign) struct cr_cardreader_t *cardreader;
@property (nonatomic, strong, readonly) id<CRAudioTransport> transport;

- (BOOL)readerDidAttach;

- (BOOL)readerDidDetachFromAnalogInterface;

- (void)unloadFeaturesAndTearDownCardReader;

@property (nonatomic, assign, readonly, getter=isAttachedToInterface) BOOL attachedToInterface;

@property (nonatomic, strong, readonly) CRDispatch *dispatch;

/// Backend handling the transport.
@property (nonatomic, strong, readonly) CRCardReaderBackend *backend;

@end // CRCardReader
