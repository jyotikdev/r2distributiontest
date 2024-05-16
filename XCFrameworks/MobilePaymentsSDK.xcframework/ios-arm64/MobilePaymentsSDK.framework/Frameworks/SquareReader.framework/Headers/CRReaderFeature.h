//
//  CRReaderFeature.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRDefines.h>
#import <SquareReader/CRFeature.h>
#import <SquareReader/CRPaymentCard.h>


@class CRReaderFeature;


@protocol CRReaderPaymentDelegate <NSObject>

/// The user has swiped a card with an ICC, ask them to insert the card.
- (void)requestInsertionOfSwipedPaymentCard;

- (void)didFailToSwipePaymentCard;
- (void)didSwipePaymentCard:(CRPaymentCard *)paymentCard authorizationRequestData:(NSData *)requestData keyData:(NSData *)keyData;

- (void)cardInserted;
- (void)cardRemoved;

@end


@protocol CRReaderDelegate <NSObject>

/** 
 This callback is made on the earliest indication that an EMV card reader is present. This will
 only happen once for the duration of the reader feature's lifecycle. This is done under the
 assumption the stack is torn down on disconnect and re-created on connect.
 */
- (void)readerFeatureDidDetectCardReader:(CRReaderFeature *)readerFeature;

- (void)readerFeature:(CRReaderFeature *)readerFeature didAcquireTransportVersion:(uint32_t)transport_version appVersion:(uint32_t)app_version endpointVersion:(uint32_t)endpoint_version;

- (void)readerReadyWithReaderType:(CRCardReaderReaderType)readerType subtype:(CRCardReaderReaderSubtype)readerSubtype;
- (void)readerResetReceived;
- (void)readerErrorReceived;
- (void)readerUpdateRequired:(uint32_t)transport_version
                  appVersion:(uint32_t)app_version
             endpointVersion:(uint32_t)endpoint_version;
- (void)firmwareUpdateRequired:(uint32_t)transport_version
                    appVersion:(uint32_t)app_version
               endpointVersion:(uint32_t)endpoint_version;
- (void)readerFullCommsReady;

@end // CRReaderDelegate


@interface CRReaderFeature : CRFeature

@property (nonatomic, weak) id<CRReaderDelegate> delegate;
@property (nonatomic, weak) id<CRReaderPaymentDelegate> paymentDelegate;

@end // CRReaderFeature
