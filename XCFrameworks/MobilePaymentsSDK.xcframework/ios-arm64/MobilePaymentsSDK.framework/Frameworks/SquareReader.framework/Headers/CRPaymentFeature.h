//
//  CRPaymentFeature.h
//  cardreader
//
//  Created by Tamir Duberstein on 11/13/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>
#import <SquareReader/CRNDEFCard.h>
#import <SquareReader/CRPaymentCard.h>
#import <SquareReader/CRReaderFeature.h>


@class CRPaymentAccount;
@class CRPaymentApplication;
@class CRPaymentCapability;
@class CRPaymentFeature;
@class CRPaymentTiming;


typedef NS_CLOSED_ENUM(NSUInteger, CRPaymentFeatureSwipeRequestReason) {
    CRPaymentFeatureSwipeRequestReasonUnknown = 0,
    CRPaymentFeatureSwipeRequestReasonTechnicalFallback,
    CRPaymentFeatureSwipeRequestReasonSchemeFallback,
};

typedef NS_ENUM(NSUInteger, CRPaymentFeatureCardEntryType) {
    CRPaymentFeatureCardEntryTypeUnknown = 0,
    CRPaymentFeatureCardEntryTypeContact = 1,
    CRPaymentFeatureCardEntryTypeContactless = 2,
    CRPaymentFeatureCardEntryTypeSwipe = 3,
};

typedef NS_ENUM(NSUInteger, CRPaymentFeatureStandardMessage) {
    // These are from EMV Book 4.3 Ch 11.12 page 89.
    CRPaymentFeatureStandardMessageNone = 0x00,
    CRPaymentFeatureStandardMessageAmount = 0x01,
    CRPaymentFeatureStandardMessageAmountOK = 0x02,
    CRPaymentFeatureStandardMessageApproved = 0x03,
    CRPaymentFeatureStandardMessageCallYourBank = 0x04,
    CRPaymentFeatureStandardMessageCancelOrEnter = 0x05,
    CRPaymentFeatureStandardMessageCardError = 0x06,
    CRPaymentFeatureStandardMessageDeclined = 0x07,
    CRPaymentFeatureStandardMessageEnterAmount = 0x08,
    CRPaymentFeatureStandardMessageEnterPIN = 0x09,
    CRPaymentFeatureStandardMessageIncorrectPIN = 0x0A,
    CRPaymentFeatureStandardMessageInsertCard = 0x0B,
    CRPaymentFeatureStandardMessageNotAccepted = 0x0C,
    CRPaymentFeatureStandardMessagePINOK = 0x0D,
    CRPaymentFeatureStandardMessagePleaseWait = 0x0E,
    CRPaymentFeatureStandardMessageProcessingError = 0x0F,
    CRPaymentFeatureStandardMessageRemoveCard = 0x10,
    CRPaymentFeatureStandardMessageUseChipReader = 0x11,
    CRPaymentFeatureStandardMessageUseMagStrip = 0x12,
    CRPaymentFeatureStandardMessageTryAgain = 0x13,
    // These are from EMV Contactless Book A Ch 9.4 page 88.
    CRPaymentFeatureStandardMessageWelcome = 0x14,
    CRPaymentFeatureStandardMessagePresentCard = 0x15,
    CRPaymentFeatureStandardMessageProcessing = 0x16,
    CRPaymentFeatureStandardMessageCardReadOKPleaseRemoveCard = 0x17,
    CRPaymentFeatureStandardMessagePleaseInsertOrSwipeCard = 0x18,
    CRPaymentFeatureStandardMessagePleasePresentOneCard = 0x19,
    CRPaymentFeatureStandardMessageApprovedPleaseSign = 0x1A,
    CRPaymentFeatureStandardMessageAuthorizingPleaseWait = 0x1B,
    CRPaymentFeatureStandardMessageInsertSwipeOrTryAnotherCard = 0x1C,
    CRPaymentFeatureStandardMessagePleaseInsertCard = 0x1D,
    CRPaymentFeatureStandardMessageNoMessage = 0x1E,
    CRPaymentFeatureStandardMessageSeePhoneForInstructions = 0x20,
    CRPaymentFeatureStandardMessagePresentCardAgain = 0x21,
    CRPaymentFeatureStandardMessageUnlockPhoneToPay = 0x22,
    CRPaymentFeatureStandardMessageTooManyTaps = 0x23,
    CRPaymentFeatureStandardMessageNotAuthorizedInOfflineMode = 0x24,
};

typedef NS_ENUM(NSUInteger, CRPaymentFeatureCardPresence) {
    CRPaymentFeatureCardPresenceUnknown = 0,
    CRPaymentFeatureCardPresenceNotPresent,
    CRPaymentFeatureCardPresencePresent,
};

typedef NS_ENUM(NSUInteger, CRPaymentCardInfoRequestErrorCode) {
    /// No error occurred.
    CRPaymentCardInfoRequestErrorCodeUnknown = 0,

    /// A generic error occurred during the card info request.
    CRPaymentCardInfoRequestErrorCodeGeneric,

    /// The card was removed before the card info request was completed.
    CRPaymentCardInfoRequestErrorCodeCardRemoved,

    /// The card info request timed out.
    CRPaymentCardInfoRequestErrorCodeTimeout
};

typedef NS_ENUM(NSUInteger, CRTMNBrand) {
    CRTMNBrandUnknown,
    CRTMNBrandCommon,
    CRTMNBrandQuicPay, // no 'k' in 'Quic'
    CRTMNBrandID,
    CRTMNBrandSuica,
    CRTMNBrandRakuten,
    CRTMNBrandWaon,
    CRTMNBrandNanaco,
    CRTMNBrandPiTaPa
};

typedef NS_ENUM(NSUInteger, CRTMNUIMessage) {
    CRTMNUIMessageUnknown,

    // payment-related from TMN
    CRTMNUIMessageWaitingForTap,              // Tap card
    CRTMNUIMessageProcessing,                 // Don't remove card until success tone
    CRTMNUIMessagePaymentComplete,
    CRTMNUIMessageInsufficientBalance,
    CRTMNUIMessageMultipleSuicaCardsTapped,   // Check inside case
    CRTMNUIMessageWaitingForRetap,            // Tap card again
    CRTMNUIMessageOnlineProcessing,
    CRTMNUIMessageCheckBalanceComplete,
    CRTMNUIMessageCardReadError,
    CRTMNUIMessageCardWriteError,
    CRTMNUIMessageInvalidCard,
    CRTMNUIMessageCancelPayment,              // Payment was interrupted
    CRTMNUIMessageCardChanged,                // Tapped different card from original. Tap card again.
    CRTMNUIMessageWrongCardError,             // Tapped wrong card. No longer polling.
    CRTMNUIMessageGenericError,               // Payment failed
    CRTMNUIMessageAmountExceedsLimit,         // iD and QUICPay only
    CRTMNUIMessageExpiredCard,                // iD and QUICPay only
    CRTMNUIMessageLockedMobileService,        // iD only. Can’t use on mobile device.

    // payment failure-related from R12
    CRTMNUIMessagePollingTimeout,
    CRTMNUIMessageMiryoSuccess,
    CRTMNUIMessageMiryoFailure,
    CRTMNUIMessageMiryoUnknown,

    // payment failure-related from SquareReader
    CRTMNUIMessageNetworkFailure,
    CRTMNUIMessageMissingAuth
};

typedef NS_ENUM(NSUInteger, CRAudioMessage) {
    CRAudioMessageUnknown,
    CRAudioMessageSuccess,
    CRAudioMessageSuccessAlternate,   // E-money: Suica-only. EMV: Buzzer
    CRAudioMessageFailure,
    CRAudioMessageRetouch,
    CRAudioMessageNone
};

typedef NS_ENUM(NSUInteger, CRNDEFResult) {
    CRNDEFResultUnknown,
    CRNDEFResultSuccess,
    CRNDEFResultFailed,
    CRNDEFResultCanceled,
    CRNDEFResultCardNotActivated,
    CRNDEFResultIncorrectMerchantID,
    CRNDEFResultUnknownError,

    // Record specific errors
    CRNDEFResultNA,
    CRNDEFResultCardFull,
    CRNDEFResultCryptoError,
    CRNDEFResultIncorrectApplicationType,
    CRNDEFResultRecordExists,
    CRNDEFResultRecordNotFound,
    CRNDEFResultTimeout,
    CRNDEFResultUIDMismatch,
    CRNDEFResultUnsupportedCard,
    CRNDEFResultDefectiveCard
};

typedef NS_ENUM(NSInteger, CRTMNPaymentFailure) {
    /// .None will never be passed back to clients and is defined for internal use / as a default value
    CRTMNPaymentFailureNone, // never used

    /// .Unknown represents a case where something went wrong at a lower level but SquareReader doesn't know how to expose it at a higher level yet
    CRTMNPaymentFailureUnknown, // never used

    /// Something is wrong with the data we attempted to send to the reader, e.g. the payment handle or tmn data are NULL
    CRTMNPaymentFailureInvalidParameter,

    /// We were unable to send bytes to the reader for some reason, e.g.: the reader connection was lost or the data couldn't fit into a ritual proto
    CRTMNPaymentFailureSessionError,

    /// We received a failure at some point while trying to send data to / receive a response from TMN through felica-proxy
    CRTMNPaymentFailureMessageProxy
};

typedef NS_ENUM(NSUInteger, CRTMNPaymentResult) {
    CRTMNPaymentResultUnknown,

    // TMN-provided payment results
    CRTMNPaymentResultSuccess,
    CRTMNPaymentResultCardReadError,              // Retry operation by terminal is recommended
    CRTMNPaymentResultDisabledCard,               // e.g. Invalid or expired cards
    CRTMNPaymentResultInvalidBrand,
    CRTMNPaymentResultCancellation,
    CRTMNPaymentResultInsufficientBalance,
    CRTMNPaymentResultWaitingForRetapError,       // Suspicion of incomplete
    CRTMNPaymentResultTMNCenterError,
    CRTMNPaymentResultPollingTimeout,
    CRTMNPaymentResultImpossibleOperation,        // e.g. Cancel event not found, invalid value, unauthorized access
    CRTMNPaymentResultMultipleCardsDetected,      // Suica only
    CRTMNPaymentResultAmountExceedsThreshold,
    CRTMNPaymentResultCenterOperationFailed,      // e.g. Online authorization failure, initial communication of edy failure, charge authentication failure
    CRTMNPaymentResultInvalidParameter,
    CRTMNPaymentResultSummaryError,
    CRTMNPaymentResultDisabledTerminal,
    CRTMNPaymentResultOnlineProcessingFailure,
    CRTMNPaymentResultOtherError,

    // Square-specific payment results
    CRTMNPaymentResultMiryoResolutionFailure,
    CRTMNPaymentResultMiryoResultFailure
};

typedef NS_CLOSED_ENUM(NSUInteger, CRPaymentFeatureLoyaltyResult) {
    CRPaymentFeatureLoyaltyResultSuccess,
    CRPaymentFeatureLoyaltyResultVASDataNotFound,
    CRPaymentFeatureLoyaltyResultCanceled,
    CRPaymentFeatureLoyaltyResultTerminated,
    CRPaymentFeatureLoyaltyResultSkipped,
    CRPaymentFeatureLoyaltyResultURLPushFailed
};


@protocol CRPaymentDelegate <NSObject>

- (void)selectApplication:(NSArray<CRPaymentApplication *> *)applications;
- (void)selectAccountFromAccounts:(NSArray<CRPaymentAccount *> *)accounts language:(NSString *)language selectedApplicationIdentifier:(uint8_t)applicationIdentifier;

/// The reader has received the cardholder name as soon as possible and is surfacing it for utility purposes.
- (void)cardholderNameReceived:(CRPaymentCard *)paymentCard;

/** The reader is requesting that we authorize the payment, using the provided requestData and keyData.
    cardEntryType indicates the entry method by which we received this authorization data (and should be considered the authoratative entry method for this transaction).
    isContinuedCardPresenceRequired indicates if the card must remain present for the remainder of the payment (e.g. this is true for normal dip, false for quick chip dip, false for tap). */
- (void)authorizeWithRequestData:(NSData *)requestData keyData:(NSData *)keyData cardEntryType:(CRPaymentFeatureCardEntryType)cardEntryType isContinuedCardPresenceRequired:(BOOL)isContinuedCardPresenceRequired;

/**
 Indicates the payment was canceled by either the reader or the cardholder.
 */
- (void)paymentCanceledWithMessage:(CRPaymentFeatureStandardMessage)message;

/// @param paymentTimings An array of CRPaymentTiming objects. May be nil.
- (void)paymentApprovedWithPaymentCard:(CRPaymentCard *)paymentCard
                   clearingRequestData:(NSData *)requestData
                       approvedOffline:(BOOL)approvedOffline
                        paymentTimings:(NSArray <CRPaymentTiming *> *)paymentTimings;

/// @param paymentTimings An array of CRPaymentTiming objects. May be nil.
- (void)paymentDeclinedWithPaymentCard:(CRPaymentCard *)paymentCard
                   reversalRequestData:(NSData *)requestData
                        paymentTimings:(NSArray <CRPaymentTiming *> *)paymentTimings;

/// The reader has failed to power the card, request the user to reinsert their card.
- (void)requestCardReinsertion;

/// Request the user to swipe instead of insert their card for a `CRPaymentFeatureSwipeRequestReason`.
- (void)requestCardSwipeWithReason:(CRPaymentFeatureSwipeRequestReason)reason;

/// Request that the user insert their card instead of tapping due to contactless transaction failure.
- (void)requestCardInsertionFromContactless;

/// Multiple contactless cards were presented. Request the buyer to present only one card.
- (void)requestPresentOnlyOneContactlessCard;

/// The limit for the presented contactless card was exceeded, another card must be used.
- (void)requestTryAnotherCardContactlessLimitExceeded;

/// The limit for the presented contactless card was exceeded. The card may be dipped, or another card presented.
- (void)requestInsertCardContactlessLimitExceeded;

/// The reader has explicitly requested that the dipped card be tapped instead. (i.e. Interac supported only for contactless)
- (void)requestContactlessCardPresentationFromInsertion;

/// The reader has explicitly requested that the card be presented again after receiving an auth response from the issuer
- (void)requestCardPresentationFromIssuer;

/**
 A contactless card was presented and that card is unable to be used due to an error. The buyer should present
 another card, which may or may not be contactless, to use to complete this transaction.
 */
- (void)requestPresentAnotherCardDueToContactlessError;

/**
 A contactless card was presented but was unable to be processed due to an error that may be transient. The buyer
 should present their contactless card again.
 */
- (void)requestPresentContactlessCardAgainDueToError;

/**
 A phone was presented as a contactless payment instrument, and the buyer must see their phone for instructions in order
 to proceed with the transaction.
 */
- (void)requestSeePhoneForContactlessInstructions;

/**
 A phone was presented as a contactless payment instrument, but was unable to be processed due to the phone being locked.
 The buyer must unlock their phone and present their phone again.
 */
- (void)requestUnlockPhoneToPay;

/**
 Message sent when the current payment is continuing due to a card insertion.
 (e.g. A contactless transaction is not canceled due to an insert, but the payment continues).
 */
- (void)paymentContinuingDueToCardPresence;

/**
 Message sent when the current payment is canceled due to the current payment timer expiring.
 (e.g. A10's contactless field expires 30 seconds after payment start with current configurations.)
 */
- (void)paymentCanceledDueToTimeout;

/// Message sent when the reader first comes up and says whether a card is already dipped or not.
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didReceiveInitialCardPresence:(BOOL)cardPresent;

/// Message sent when the reader first comes up and says whether a card is already dipped or not.
/// EMV brand is included because callback to play sound comes in before on_payment_complete with the final card info comes in
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didReceiveAudioMessage:(CRAudioMessage)message EMVBrand:(CRPaymentCardBrand)brand;

- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didReadNDEFCard:(CRNDEFCard *)card withResult:(CRNDEFResult)result;

- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didWriteNDEFCardWithResult:(CRNDEFResult)result;

- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didEraseNDEFCardWithResult:(CRNDEFResult)result;

/// Message sent when the reader receives Apple VAS data from the device, indicating an existing loyalty account in the Apple Wallet.
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature
didReadVASPassWithData:(NSData *)data
     merchantLoyaltyID:(NSString *)merchantLoyaltyID;

/// Message sent when the reader writes Apple VAS data to the device, the first step in linking a loyalty account in the Apple Wallet.
- (void)paymentFeatureDidWriteVASPass:(CRPaymentFeature *)paymentFeature;

/// Message sent when the reader fails to either read or write Apple VAS data, including the specific failure.
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didVASFailureWithResult:(CRPaymentFeatureLoyaltyResult)result;

@end


@protocol CRTMNPaymentDelegate <NSObject>

/// Send TMN some data to continue with the transaction
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature sendTMNData:(NSData *)data miryoTransactionId:(NSString *)miryoTransactionId withSuccessBlock:(void (^)(NSData * /* response */))successBlock failureBlock:(dispatch_block_t)failureBlock;

/// TMN requested that POS display a message on screen as directions for the merchant or customer
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didReceiveUIMessage:(CRTMNUIMessage)message amount:(NSString *)amount balance:(NSString *)balance;

/// TMN requested that POS play a tone to aid the merchant or customer
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didReceiveAudioMessage:(CRAudioMessage)message;

/// Indicates a failure in communication either with the reader or felica-proxy. SquareReader will cancel the payment with the reader after this is called.
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature sendTMNData:(NSData *)data miryoTransactionId:(NSString *)miryoTransactionId didFailWithResult:(CRTMNPaymentFailure)result;

/// Indicates a failure in communication either with the reader or felica-proxy. SquareReader will cancel the payment with the reader after this is called.
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature sendTMNDataDidSucceed:(NSData *)data miryoTransactionId:(NSString *)miryoTransactionId;

/// Indicates that a card reader is about to enter its final phase of a TMN payment and is writing the updated balance to a card.
- (void)paymentFeatureWillWriteUpdatedBalanceToCard:(CRPaymentFeature *)paymentFeature priorBalanceCents:(NSNumber *)priorBalanceCents amountChargedCents:(NSNumber *)amountChargedCents miryoData:(NSData *)miryoData;

/// TMN transaction completed with a success or error. Includes a list of timing events about the payment for logging purposes.
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature paymentDidCompleteWithResult:(CRTMNPaymentResult)result paymentTimings:(NSArray<CRPaymentTiming *> *)paymentTimings;

@end


@protocol CRPaymentCardInfoRequestDelegate <NSObject>

- (void)paymentFeature:(CRPaymentFeature *)paymentFeature didReceiveRequestedPaymentCard:(CRPaymentCard *)paymentCard entryMethod:(CRPaymentFeatureCardEntryType)entryMethod readerData:(NSData *)readerData;

- (void)paymentFeatureContinuingRequestWithInsertedCard:(CRPaymentFeature *)paymentFeature;

- (void)paymentFeatureCardInfoRequestDidFail:(CRPaymentFeature *)paymentFeature errorCode:(CRPaymentCardInfoRequestErrorCode)errorCode;

@end


@protocol CRPaymentSwipePassthroughDelegate <NSObject>

/**
 The delegate method invoked when a payment card is swiped through the card reader that would normally have been
 rejected due to an ICC requirement. This allows the card data to be utilized for non-payments features such
 as searching transactions in the back of house, and for offline payments.

 @param paymentFeature The payment feature from which the passthrough swipe as received.
 @param paymentCard Payment card metadata.
 @param requestData M1 Encrypted Track 1 and Track 2 Data.
 @param keyData The KSN key data if applicable.
 */
- (void)paymentFeature:(CRPaymentFeature *)paymentFeature passthroughDidSwipePaymentCard:(CRPaymentCard *)paymentCard authorizationRequestData:(NSData *)requestData keyData:(NSData *)keyData;

@end // CRPaymentDelegate


@protocol CRPaymentInteraction

@property (nonatomic, readonly) NSDate *startedAt;
@property (nonatomic, copy, readonly) NSUUID *UUID;
@property (nonatomic, assign, readonly) NSUInteger authorizationAmountCents;

@end

@interface CRPaymentTiming : NSObject

- (instancetype)initWithLabel:(NSString *)label deltaMS:(NSUInteger)deltaMS;

@property (nonatomic, copy, readonly) NSString *label;
@property (nonatomic, assign, readonly) NSUInteger deltaMS;

@end


@interface CREMVPaymentInteraction : NSObject <CRPaymentInteraction>

+ (instancetype)paymentByAuthorizatingAmountInCents:(NSUInteger)authorizationAmountCents
                                          startedAt:(NSDate *)startedAt
                                  merchantLoyaltyID:(NSData *)merchantLoyaltyID
                                 merchantLoyaltyURL:(NSData *)merchantLoyaltyURL;

+ (instancetype)refundByAuthorizatingAmountInCents:(NSUInteger)authorizationAmountCents
                                         startedAt:(NSDate *)startedAt;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype) new NS_UNAVAILABLE;

@property (nonatomic, copy, readonly) NSData *merchantLoyaltyID;
@property (nonatomic, copy, readonly) NSData *merchantLoyaltyURL;

@end


@interface CRTMNPaymentInteraction : NSObject <CRPaymentInteraction>

@property (nonatomic, assign) BOOL shouldForceCancel;

+ (instancetype)paymentByAuthorizatingAmountInCents:(NSUInteger)authorizationAmountCents
                                          startedAt:(NSDate *)startedAt
                                              brand:(CRTMNBrand)brand;
+ (instancetype)refundByAuthorizatingAmountInCents:(NSUInteger)authorizationAmountCents
                                         startedAt:(NSDate *)startedAt
                                             brand:(CRTMNBrand)brand;
+ (instancetype)checkBalanceForBrand:(CRTMNBrand)brand
                           startedAt:(NSDate *)startedAt;
+ (instancetype)onlineTestForBrand:(CRTMNBrand)brand
                         startedAt:(NSDate *)startedAt;
+ (instancetype)miryoRecoveryFlowForBrand:(CRTMNBrand)brand
                                     data:(NSData *)miryoData
                                     UUID:(NSUUID *)UUID;
+ (instancetype)voidForBrand:(CRTMNBrand)brand
               transactionId:(NSString *)transactionId;


- (instancetype)init NS_UNAVAILABLE;
+ (instancetype) new NS_UNAVAILABLE;

@end


@interface CRPaymentFeature : CRFeature

@property (nonatomic, weak) CRReaderFeature *readerFeature;

@property (nonatomic, weak) id<CRPaymentDelegate> delegate;
@property (nonatomic, weak) id<CRTMNPaymentDelegate> TMNDelegate;
@property (nonatomic, weak) id<CRPaymentSwipePassthroughDelegate> passthroughDelegate;
@property (nonatomic, weak) id<CRPaymentCardInfoRequestDelegate> cardInfoRequestDelegate;
@property (nonatomic, assign, readonly) uint16_t ISOCurrencyCode;

/// Initializers
- (instancetype)initWithCardReader:(CRCardReader *)cardReader NS_UNAVAILABLE;
- (instancetype)initWithCardReader:(CRCardReader *)cardReader
              merchantCategoryCode:(uint16_t)mcc
                      currencyCode:(uint16_t)currencyCode NS_DESIGNATED_INITIALIZER;

/// Card Reader Interactions
- (BOOL)startPaymentInteraction:(NSObject<CRPaymentInteraction>*)context;
- (BOOL)cancelPaymentInteraction:(NSObject<CRPaymentInteraction>*)context;
- (BOOL)requestCardInfoWithEntryType:(CRPaymentFeatureCardEntryType)card_entry_type startedAt:(NSDate *)startedAt;

/// Responding to CRPaymentDelegate Requests
- (BOOL)selectAccount:(CRPaymentAccount *)account;
- (BOOL)selectApplication:(CRPaymentApplication *)application;
- (BOOL)authorizeWithResponseData:(NSData *)responseData;

/// Responding to CRTMNPaymentDelegate Requests
- (BOOL)cardWriteWillOccurAcknowledgement;

/// Card Reader State
- (BOOL)requestCardPresenceState;
- (BOOL)getLastKnownCardPresenceState:(CRPaymentFeatureCardPresence *)returnLastKnownCardPresenceState;

/**
 Indicate that swipe events should be propagated to the Passthrough delegate even if the card contains an ICC.
 This is necessary for searching in the back-of-house and for payments in Offline mode.

 @param enabled Whether or not the passthrough should be enabled.
 @return YES if setting the passthrough was successful.
 */
- (BOOL)setSwipePassthroughEnabled:(BOOL)enabled;

/**
 *  Tell the reader that a payment is likely to be started soon.
 *  Allows the reader to optimize for transaction speed and response time.
 * 
 *  @param powerOnDuration The amount of time that the reader should remain powered up
 *  and expecting to start a payment.
 */
- (BOOL)sendPowerUpHintWithPoweredOnDuration:(NSTimeInterval)powerOnDuration;

- (BOOL)readNDEF:(CRNDEFCardApplicationType)applicationType
  withMerchantID:(NSData *)merchantID;

- (BOOL)writeNDEFCard:(CRNDEFCard *)card
             override:(BOOL)override;


/// Read NDEF card using record API.
/// @param applicationType Application requesting to read.
/// @param merchantID The merchant token.
- (BOOL)readNDEFRecord:(CRNDEFCardApplicationType)applicationType
        withMerchantID:(NSData *)merchantID;


/// Writes to NDEF card using record API.
- (BOOL)writeNDEFRecordCard:(CRNDEFCard *)card
                   override:(BOOL)override
           useSecureStorage:(BOOL)useSecureStorage;

/// Erase NDEF records on card, after erasing, the card will be like new.
/// @param applicationType Application requesting to read.
/// @param merchantID The merchant token.
- (BOOL)eraseNDEFData:(CRNDEFCardApplicationType)applicationType
           merchantID:(NSData *)merchantID;


/// Toggle if a custom sound will play when a payment is declined
/// @param enable Whether or not the sound will be enabled.
- (BOOL)enablePaymentDeclinedSound:(BOOL)enable;

@end // CRPaymentFeature
