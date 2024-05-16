//
//  CRPaymentCard.h
//  cardreader
//
//  Created by Tamir Duberstein on 12/4/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef NS_ENUM(NSUInteger, CRPaymentCardBrand) {
    CRPaymentCardBrandVisa = 0,
    CRPaymentCardBrandMasterCard,
    CRPaymentCardBrandDiscover,
    CRPaymentCardBrandAmex,
    CRPaymentCardBrandJCB,
    CRPaymentCardBrandOther,
    CRPaymentCardBrandDiners,
    CRPaymentCardBrandChinaUnionPay,
    CRPaymentCardBrandSquareGiftCard,
    CRPaymentCardBrandInterac,
    CRPaymentCardBrandEftpos,
};

typedef NS_ENUM(NSUInteger, CRBankBrand) {
    CRBankBrandUnknown = 0,
    CRBankBrandAfterpay = 1,
    CRBankBrandCashApp = 2,
    CRBankBrandSquare = 3,
};

typedef NS_ENUM(NSUInteger, CRPaymentCardVerificationMethod) {
    CRPaymentCardVerificationMethodFailed = 0,
    CRPaymentCardVerificationMethodOfflinePlaintextPIN,
    CRPaymentCardVerificationMethodOnlineEncipheredPIN,
    CRPaymentCardVerificationMethodOfflinePlaintextPINAndSignature,
    CRPaymentCardVerificationMethodOfflineEncipheredPIN,
    CRPaymentCardVerificationMethodOfflineEncipheredPINAndSignature,
    CRPaymentCardVerificationMethodOnDevice,
    CRPaymentCardVerificationMethodSignature,
    CRPaymentCardVerificationMethodNone,
    CRPaymentCardVerificationMethodUnknown
};


@interface CRPaymentCard : NSObject

@property (nonatomic, readonly, copy) NSString *lastFour;
@property (nonatomic, readonly, copy) NSString *cardHolderName;
@property (nonatomic, readonly, copy) NSDateComponents *expiry;

@property (nonatomic, assign, readonly) CRPaymentCardBrand brand;
@property (nonatomic, assign, readonly) CRBankBrand bank;
@property (nonatomic, assign, readonly) CRPaymentCardVerificationMethod verificationMethod;

@property (nonatomic, assign, readonly) BOOL hasValidTrack1;
@property (nonatomic, assign, readonly) BOOL hasValidTrack2;

+ (instancetype) new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithLastFour:(NSString *)lastFour
                  cardHolderName:(NSString *)cardHolderName
                          expiry:(NSDateComponents *)expiry
                    paymentBrand:(CRPaymentCardBrand)brand
                       bankBrand:(CRBankBrand)bank
              verificationMethod:(CRPaymentCardVerificationMethod)verificationMethod
                  hasValidTrack1:(BOOL)hasValidTrack1
                  hasValidTrack2:(BOOL)hasValidTrack2 NS_DESIGNATED_INITIALIZER;

@end
