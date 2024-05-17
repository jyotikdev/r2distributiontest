//
//  CRTransport.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <SquareReader/CRDefines.h>


@protocol CRBTLETransport;


/// Delegate of CRAudioTransport. Should handle passing audio data back to LCR.
@protocol CRAudioTransportDelegate <NSObject>

/// consumeReceiveAudioData is invoked when data is received from the hardware.
/// @return YES if the audio data was successfully consumed, NO otherwise.
- (BOOL)consumeReceivedAudioData:(NSData *)data channels:(uint8_t)channels numberOfSamples:(size_t)sampleCount;

/// Set the type of audio reader associated with this transport.
- (void)setCurrentReaderType:(CRCardReaderReaderType)readerType;

/// transportNotifySendComplete is invoked when a send data operation completes.
- (void)transportNotifySendComplete;

@end


/// Delegate of CRUSBTransport. Should handle passing USB data back to LCR.
@protocol CRUSBTransportDelegate <NSObject>

/// consumeReceivedUSBData is invoked when data is received from the hardware.
- (BOOL)consumeReceivedUSBData:(NSData *)data;

@end


/// Delegate of CRBTLETransport. Should handle passing data back to LCR.
@protocol CRBTLETransportDelegate <NSObject>

/// Invoked when comms data is received from the hardware.
- (BOOL)transport:(id<CRBTLETransport>)transport consumeReceivedBluetoothLEData:(NSData *)data;

/// Invoked when a new ack vector is recieved from the hardware.
- (BOOL)transport:(id<CRBTLETransport>)transport consumeAckVector:(NSNumber *)ackVector;

// Invoked when a new MTU is received from the hardware.
- (BOOL)transport:(id<CRBTLETransport>)transport consumeNewMTU:(NSNumber *)newMTU;

@end


/// DEPRECATED: Contains all methods required for the audio and bluetooth delegates.
@protocol CRTransportDelegate <NSObject, CRAudioTransportDelegate>
@end


/// Base methods required for all transport types.
@protocol CRTransportBase <NSObject>

/// Log an error message from LCR.
- (void)logErrorMessage:(NSString *)errorMessage forEndpointWithID:(int)endpointID;

@end


/// Transport for readers using Audio comms. Communicates with a CRAudioTransportDelegate.
@protocol CRAudioTransport <CRTransportBase>

@property (nonatomic, weak) id<CRAudioTransportDelegate> delegate;

/**
 Send data to the underlying hardware. The implementation needs to take care of invoking
 [CRTransportDelegate transportNotifySendComplete] when the data has been sent.
 @param data The raw data to send to the audio interface. Should stop sending looped data if nil.
 @param loop Continuously resend these audio samples until called again with NO or nil data.
 */
- (void)sendAudioData:(NSData *)data loop:(BOOL)loop;

/// Called once LCR has given up broadcasting for a potential R6 connection.
- (void)didStopBroadcastingForR6DueToConnectionTimeout;

/// Process received audio data.
/// @return YES if the receiver successfully handled the data.
- (BOOL)didReceiveAudioData:(NSData *)data channels:(uint8_t)channels numberOfSamples:(size_t)sampleCount;

/// Set the type of audio reader associated with this transport.
- (void)setCurrentReaderType:(CRCardReaderReaderType)readerType;

@end


/// Transport for readers using USB comms. Communicates with a CRUSBTransportDelegate.
@protocol CRUSBTransport <CRTransportBase>

@property (nonatomic, weak) id<CRUSBTransportDelegate> delegate;

/**
 Send bluetooth data to the underlying hardware.
 @param data The raw data to send to the USB interface.
 */
- (void)sendUSBData:(NSData *)data;

@end


/// Transport for readers using BluetoothLE. Communicates with a CRBTLETransportDelegate
@protocol CRBTLETransport <CRTransportBase>

@property (nonatomic, weak) id<CRBTLETransportDelegate> delegate;

@property (nonatomic, strong, readonly) NSNumber *MTU;

@property (nonatomic, strong, readonly) NSData *commsVersionData;

/**
 Send bluetooth LE data to the underlying hardware.
 The implementation should *not* call transportNotifySendComplete.
 If there is an error sending to the hardware, the CRCardReader instance should be torn down and recreated
 */
- (void)sendBluetoothLEData:(NSData *)data;

- (void)readAckVector;

/**
 Called to initiate a read of the MTU characteristic from the underlying BLE peripheral.
 */
- (void)readMTU;

@end


/// CRTransport defines the interface between CRCardReader and the underlying hardware as well as
/// providing the various hooks to cryptographic functions.
/// DEPRECATED: Should use CRTransport{Audio,Bluetooth} instead.
@protocol CRTransport <CRAudioTransport>

@property (nonatomic, weak) id<CRTransportDelegate> delegate;

@end // CRTransport
