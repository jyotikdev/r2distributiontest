//
//  CRSystemFeature.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>


typedef NS_ENUM(NSInteger, CRSystemFeatureReaderErrorCode) {
    CRSystemFeatureReaderErrorCodeUnknown = 0,
    CRSystemFeatureReaderErrorFuelGaugeConfiguration,
    CRSystemFeatureReaderErrorK400MissingManifest,
    CRSystemFeatureReaderErrorUSBPlugEventOverflow,
    CRSystemFeatureReaderErrorDisconnectFWUPReset,
    CRSystemFeatureReaderErrorDisconnectDailyReset,
    CRSystemFeatureReaderErrorDisconnectCriticalBatteryPowerOff,
    CRSystemFeatureReaderErrorDisconnectIdleTimeoutPowerOff,
    CRSystemFeatureReaderErrorDisconnectPowerReset,
    CRSystemFeatureReaderErrorDisconnectPowerOff,
    CRSystemFeatureReaderErrorDisconnectBLEPairing,
    CRSystemFeatureReaderErrorDisconnectBLEUnpair,
    CRSystemFeatureReaderErrorThermalFault,
    CRSystemFeatureReaderErrorThermalFaultDisconnectUSB,
    CRSystemFeatureReaderErrorThermalFaultPowerOff,
    CRSystemFeatureReaderErrorDisconnectSwitchToUSB,
};

@class CRSystemFeature;
@class CRCardReader;

extern NSString *const CRSystemFeatureErrorDomain;
extern NSString *const CRSystemFeatureUnderlyingErrorCode;


@protocol CRSystemDelegate <NSObject>

/// Invoked when the hardware serial number is received from the card reader.
- (void)systemFeature:(CRSystemFeature *)feature didReceiveHardwareSerialNumber:(NSString *)serialNumber;

/// Invoked when the firmware version is received from the card reader.
- (void)systemFeature:(CRSystemFeature *)feature didReceiveFirmwareVersion:(NSString *)version;

/// Invoked when the current charge cycle count is recieved from the card reader.
- (void)systemFeature:(CRSystemFeature *)feature didReceiveChargeCycleCount:(uint32_t)chargeCycleCount;

/// Invoked when the current comms rates are received from the card reader.
- (void)systemFeature:(CRSystemFeature *)feature didReceiveInCommsRate:(NSString *)inCommsRate outCommsRate:(NSString *)outCommsRate;

/// Invoked when the errors occur by the reader.
- (void)systemFeature:(CRSystemFeature *)feature systemErrorReceived:(NSError *)readerError;

/// Invoked when the capabilities are received by the reader.
- (void)systemFeatureDidReceiveCapabilities:(CRSystemFeature *)feature;

/// Invoked when feature flag setting receives an error
- (void)systemFeature:(CRSystemFeature *)feature didFailToSetFeatureFlag:(NSString *)featureFlag failureResult:(int)result;

/// invoked when the system feature is told that the SPE has disconnected
- (void)systemFeatureDidConfirmDisconnect:(CRSystemFeature *)feature;

@end


@interface CRSystemFeature : CRFeature

@property (nonatomic, weak) id<CRSystemDelegate> delegate;

/// @returns YES when we have received capabilities from the reader, NO otherwise.
@property (nonatomic, assign, readonly) BOOL hasReceivedCapabilities;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

/**
 Request information about the card reader including the serial number, firmware version
 and the charge cycle count. Once they are received, the appropriate delegate methods
 are invoked on the delegate queue.
 
 @return YES if the request was successfully enqueued for sending to the card reader.
 */
- (BOOL)requestSystemInformation;

/**
 @returns YES if the provided data is a keep-alive message encoded for USB transport, NO otherwise.
 */
- (BOOL)USBPacketDataIsKeepaliveMessage:(NSData *)usbData;

/**
  @returns YES if the provided data is a 'halt keep-alives' message encoded for USB transport, NO otherwise.
 */
- (BOOL)USBPacketDataIsHaltKeepalivesMessage:(NSData *)usbData;

/**
 Send a keep-alive message to the reader. These should be sent at the interval specified by CRS_SYSTEM_KEEPALIVE_MESSAGE_SEND_PERIOD_MS.

 @return YES if the request was successfully enqueued for sending to the card reader.
 */
- (BOOL)sendKeepalive;

/**
 Send a message to the reader instructing it that POS is about to disconnect.

 @return YES if the request was successfully received by the reader, NO otherwise.
 */
- (BOOL)sendDisconnectNotification;

/**
 Send a message to the reader instructing it to no longer expect keep-alive messages. After having sent a keep-alive message, you must not stop sending keep-alive messages until you've sent this halt keep-alive messages.

 @return YES if the request was successfully enqueued for sending to the card reader.
 */
- (BOOL)sendHaltKeepalives;

/**
 @returns YES if the reader corresponding to this feature supports keep-alives, NO otherwise.
 
 You must check this before sending any keep-alive or halt keep-alive packets.
 */
- (BOOL)readerSupportsKeepalive;

/**
 Sends reader-specific feature flags to the reader. Internally calls cr_system_set_reader_feature_flag to set each flag, and cr_system_send_reader_feature_flags to send them.
 
 @return YES if the request was successfully enqueued for sending to the card reader.
 */
- (BOOL)sendReaderFeatureFlags:(NSDictionary<NSString *, NSNumber *> *)featureFlags;


/**
 Ask reader to send an invalid ritual message, and in turn, trigger a crs_assert
 */
- (BOOL)crashPlease;

@end
