//
//  CSSwipedPaymentCard.h
//  CoreSwipe
//
//  Created by Michael White on 1/22/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//
//  Payment Card model for *swiped* cards.
//

#import <CorePaymentCard/CorePaymentCard.h>
#import <SquareReader/CSUtility.h>


@interface CSSwipedPaymentCard : CPCPaymentCard

/**
 @param track Unencrypted track data.
 @param trackType The type of the unencrypted track data (i.e. .Track2).
 @return A CSSwipedPaymentCard instance using `track` to represent the unencrypted track data.
 */
+ (instancetype)paymentCardWithTrack:(NSString *)track trackType:(CSTrackType)trackType;

/**
 @param track1 Unencrypted track 1 data.
 @param track2 Unencrypted track 2 data.
 @return A CSSwipedPaymentCard using `track1` and `track2` to represent the unencrypted track data.
 */
+ (instancetype)paymentCardWithTrack1:(NSString *)track1 track2:(NSString *)track2;

/**
 Calls through to paymentCardWithEncryptedData:keyData:trackType:readerType:cardBrand:
 cardNumberLastFour:cardholderName:, passing nil for keyData and CPCPaymentCardReaderTypeUnknown for readerType.
 */
+ (instancetype)paymentCardWithEncryptedData:(NSData *)encryptedData trackType:(CSTrackType)trackType cardBrand:(CPCCardBrand)brand cardNumberLastFour:(NSString *)numberLastFour cardholderName:(NSString *)nameOrNil;

/**
 Calls through to paymentCardWithEncryptedData:keyData:trackType:readerType:cardBrand:
 cardNumberLastFour:cardholderName:, passing CPCPaymentCardReaderTypeUnknown for readerType.
 */
+ (instancetype)paymentCardWithEncryptedData:(NSData *)encryptedData keyData:(NSData *)keyData trackType:(CSTrackType)trackType cardBrand:(CPCCardBrand)brand cardNumberLastFour:(NSString *)numberLastFour cardholderName:(NSString *)nameOrNil;

/**
 Used to create a `CSSwipedPaymentCard` instance using `encryptedData` in addition to `keyData` which will be used
 to decrypt the `encryptedData` at a later time. Will be used by R12.

 @throws An uncaught exception if keyData is required and not present for the specified `trackType`.
 @param encryptedData Encrypted `trackType` card data.
 @param keyData Key data used to decrypt the encrypted data at a later time.
 @param trackType The type of encrypted track data (i.e. Encrypted R4 Track1).
 @param readerType The type of the reader, if known, otherwise CPCPaymentCardReaderTypeUnknown.
 @param brand The brand of the card (i.e. MasterCard or Visa).
 @param numberLastFour The last four of the card's PAN.
 @param nameOrNil The name of the cardholder name or nil if not known.
 @return A CSSwipedPaymentCard using a combination of `encryptedData` and `keyData` to represent the track data.
 */
+ (instancetype)paymentCardWithEncryptedData:(NSData *)encryptedData keyData:(NSData *)keyData trackType:(CSTrackType)trackType readerType:(CPCPaymentCardReaderType)readerType cardBrand:(CPCCardBrand)brand cardNumberLastFour:(NSString *)numberLastFour cardholderName:(NSString *)nameOrNil;

/**
 Used to create a `CSSwipedPaymentCard` instance using the R4 M1 authenticatedData and full data blobs.
 */
+ (instancetype)paymentCardWithM1AuthenticatedData:(void *)authenticatedData fullEncryptedData:(NSData *)encryptedData;


// Deprecated methods that are valid only on CPCPaymentCard.
- (id)initWithCardNumber:(NSString *)cardNumber __attribute__((unavailable("CSSwipedPaymentCard is only for swiped cards.")));
- (id)initWithBrand:(CPCCardBrand)brand cardNumberLastFour:(NSString *)numberLastFour __attribute__((unavailable("CSSwipedPaymentCard is only for swiped cards.")));

@property (nonatomic, strong, readonly) CPCString *track1;
@property (nonatomic, strong, readonly) CPCString *track2;
@property (nonatomic, copy, readonly) NSData *encryptedData;
@property (nonatomic, copy, readonly) NSData *keyData;

@end
