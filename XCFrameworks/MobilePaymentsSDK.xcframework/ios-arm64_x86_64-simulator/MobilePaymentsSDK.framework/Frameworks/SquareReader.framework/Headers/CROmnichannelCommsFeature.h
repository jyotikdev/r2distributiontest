//
//  CROmnichannelCommsFeature.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//
// CROmnichannelComms is generically named on purpose. It was renamed from
// CRSecureSession to resolve RI-27624 (which is a subtask in resolving SECENG-3533).
//

#import <UIKit/UIKit.h>

#import <SquareReader/CRFeature.h>
#import <SquareReader/CRPaymentCard.h>

@class CRCardReader;
@class CROmnichannelCommsFeature;
@class DEDigitEntryView;

@protocol CROmnichannelCommsDelegate <NSObject>

// TODO(pmattis): Perhaps move corroborateSession: to CRTransport. As the code is currently
// organized, there is a race where creation of a CRCardReader will start to initialize the reader
// before we've set the CROmnichannelCommsFeature delegate which can allow corroborateSession: to be
// invoked a nil reader. This doesn't crash (an objective-c feature), but silently ignores the
// method call but causes us to get into a state where secure sessions are not initialized.
//
// An alternative solution would be to always start CRCardReader paused and require an explicit
// [CRCardReader resume] operation.

- (void)corroborateSession:(NSData *)data;
- (void)corroborated;
- (void)contradicted;

/// Called when a secure session feature becomes enabled.
/// The feature will either start enabled, or start 'disabledDueToMaxReadersConnected`.
/// The feature may later become enabled if the user disconnects enough other readers and retries.
- (void)featureDidBecomeEnabled;

- (void)featureDidBecomeDisabledByTsm:(NSData *)explanation;

- (void)feature:(CROmnichannelCommsFeature *)feature saveSessionData:(NSData *)sessionData forKey:(NSString *)key;
- (void)feature:(CROmnichannelCommsFeature *)feature lookupSessionDataForKey:(NSString *)key completionHandler:(void(^)(NSData *))completionHandler;
- (void)feature:(CROmnichannelCommsFeature *)feature determinedSessionExpirationTime:(NSDate *)expirationTime;

@end


@protocol CROmnichannelCommsPaymentsDelegate <NSObject>

/**
 This delegate method is invoked when a PIN-enabled reader requests that the cardholder
 enter their PIN.

 @param feature The secure session feature from which the PIN request was received.
 @param digitEntryView The view containing the screen for entering a PIN.
 */
- (void)omnichannelCommsFeature:(CROmnichannelCommsFeature *)feature didReceiveInputRequestWithPaymentCard:(CRPaymentCard *)paymentCard digitEntryView:(DEDigitEntryView *)digitEntryView;

@end

typedef NS_ENUM(NSUInteger, CROmnichannelCommsFeatureServerUserExperienceHint) {
    CROmnichannelCommsFeatureServerUserExperienceHintDoNotUse = 0,
    CROmnichannelCommsFeatureServerUserExperienceHintNoSuggestedAction,
    CROmnichannelCommsFeatureServerUserExperienceHintSuggestRetry,
    CROmnichannelCommsFeatureServerUserExperienceHintSuggestActivation,
    CROmnichannelCommsFeatureServerUserExperienceHintSuggestContactSupport
};

typedef NS_ENUM(NSUInteger, CROmnichannelCommsFeatureServerError) {
    // The default error
    CROmnichannelCommsFeatureServerErrorGenericError = 0,

    // LCR errors
    CROmnichannelCommsFeatureServerErrorParseError,

    // Server Errors
    CROmnichannelCommsFeatureServerErrorUnknownDenyReason,
    CROmnichannelCommsFeatureServerErrorNotDenied,
    CROmnichannelCommsFeatureServerErrorSellerSuspended,
    CROmnichannelCommsFeatureServerErrorSellerNotEligible,
    CROmnichannelCommsFeatureServerErrorDeviceHardwareNotSupported,
    CROmnichannelCommsFeatureServerErrorDeviceFirmwareTooOld,
    CROmnichannelCommsFeatureServerErrorDeviceFirmwareNotSupported,
    CROmnichannelCommsFeatureServerErrorDeviceFirmwareModified,
    CROmnichannelCommsFeatureServerErrorDeviceFirmwareDamaged,
    CROmnichannelCommsFeatureServerErrorDeviceInDeveloperMode,
    CROmnichannelCommsFeatureServerErrorAppTooOld,
    CROmnichannelCommsFeatureServerErrorAppDamaged,
    CROmnichannelCommsFeatureServerErrorInvalidRequest,
    CROmnichannelCommsFeatureServerErrorDeviceInAccessibilityMode,
    CROmnichannelCommsFeatureServerErrorSellerNotActivated,

    // No errors
    CROmnichannelCommsFeatureServerErrorNone,
};

typedef NS_ENUM(NSUInteger, CROmnichannelCommsFeatureResult) {
    CROmnichannelCommsFeatureResultSuccess = 0,
    CROmnichannelCommsFeatureResultInvalidParameter,
    CROmnichannelCommsFeatureResultNotInitialized,
    CROmnichannelCommsFeatureResultAlreadyInitialized,
    CROmnichannelCommsFeatureResultNotTerminated,
    CROmnichannelCommsFeatureResultAlreadyTerminated,

    // "Session" has nothing to do with securesession.. this is a normal LCR
    // feature lifecycle term used in most other features
    CROmnichannelCommsFeatureResultSessionError,

    CROmnichannelCommsFeatureResultCallUnexpected,
    CROmnichannelCommsFeatureResultGenericError,
    CROmnichannelCommsFeatureResultNoReader,

    // These two error codes are the funneled-up versions of the securesession
    // (used-to-be-submodule) code under the hood.  At this level, we only care
    // about whether or not the server explicitly denied a session request --
    // all other errors are funneled to the MODULE_GENERIC_ERROR
    CROmnichannelCommsFeatureResultServerDenyError,
    CROmnichannelCommsFeatureResultModuleGenericError,
    CROmnichannelCommsFeatureResultMaxReadersConnected,

    // These result codes were added to match `securesession_result_t`.
    CROmnichannelCommsFeatureResultArg,
    CROmnichannelCommsFeatureResultSessionState,
    CROmnichannelCommsFeatureResultInputSize,
    CROmnichannelCommsFeatureResultOutputSize,
    CROmnichannelCommsFeatureResultMsgType,
    CROmnichannelCommsFeatureResultSessionId,
    CROmnichannelCommsFeatureResultCurve,
    CROmnichannelCommsFeatureResultHkdf,
    CROmnichannelCommsFeatureResultDenied,
    CROmnichannelCommsFeatureResultBadDigit,
    CROmnichannelCommsFeatureResultPinFull,
    CROmnichannelCommsFeatureResultPinTooShort,
    CROmnichannelCommsFeatureResultInvalidPinRequest,
    CROmnichannelCommsFeatureResultInvalidKeyUpdateMsg,
    CROmnichannelCommsFeatureResultAes,
    CROmnichannelCommsFeatureResultProtocolVersion,
    CROmnichannelCommsFeatureResultApprovalMismatch,
    CROmnichannelCommsFeatureResultApprovalExpired,
    CROmnichannelCommsFeatureResultNoTxnLeft,
    CROmnichannelCommsFeatureResultApiCall,
    CROmnichannelCommsFeatureResultMinesweeperCall,
    CROmnichannelCommsFeatureResultSha256,
    CROmnichannelCommsFeatureResultBadHmac,
    CROmnichannelCommsFeatureResultTdes,
    CROmnichannelCommsFeatureResultEncodeFailure,
    CROmnichannelCommsFeatureResultContext,
    CROmnichannelCommsFeatureResultOutOfContexts,
    CROmnichannelCommsFeatureResultBadField,
    CROmnichannelCommsFeatureResultWhiteboxKeyDeserialize,
    CROmnichannelCommsFeatureResultUnknown,

    CROmnichannelCommsFeatureResultMapTranslationError = NSUIntegerMax
};

typedef NS_CLOSED_ENUM(NSUInteger, CROfflineMode) {
    CROfflineModeNone = 0,
    CROfflineModeSupported,
    CROfflineModeUnplannedOffline,
    CROfflineModePlannedOffline,
};


///
/// CROmnichannelCommsFeature is responsible for coordinating
/// secure sessions between a reader and the server and processing
/// PIN requests and entry in a secure fashion.
///
/// CROmnichannelCommsFeature is generically and obscurely named on purpose.
/// It was renamed from CROmnichannelCommsFeature to resolve RI-27624
/// (which is a subtask in resolving SECENG-3533).
///
@interface CROmnichannelCommsFeature : CRFeature

@property (nonatomic, weak) id<CROmnichannelCommsDelegate> delegate;
@property (nonatomic, weak) id<CROmnichannelCommsPaymentsDelegate> paymentsDelegate;

@property (nonatomic, assign, getter=isInputHandlerInUse) BOOL inputHandlerInUse;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader offlineMode:(CROfflineMode)offlineMode;

#pragma mark - Session Establishment

/**
 Process a message received from the server.

 @param data The raw data we received from the server.
 @param server_error The error related to processing the secure session data.
 @param userExperienceHint The userExperienceHint sent by flipper on secure session denial
 @param localizedTitle The localized title to display, sent by flipper on secure session denial
 @param localizedDescription The localized description to display, sent by flipper on secure session denial

 @returns The result of processing the data.
 We can either process the data sucessfully, where we return a CROmnichannelCommsFeatureResultSuccess, with CROmnichannelCommsFeatureServerErrorNone on `server_error`,
 or some error, with the specific the reason on `server_error`.
 */
- (CROmnichannelCommsFeatureResult)processServerMessage:(NSData *)data
                                            serverError:(CROmnichannelCommsFeatureServerError *)server_error
                                     userExperienceHint:(CROmnichannelCommsFeatureServerUserExperienceHint *)userExperienceHint
                                         localizedTitle:(NSString **)localizedTitle
                                   localizedDescription:(NSString **)localizedDescription;

/**
 Tell the reader that it should begin attempting to establish a secure session, if needed.
 Does nothing if we already have a valid session.
 */
- (BOOL)establishIfNeeded;

/**
 When YES, this feature cannot be used until some other card readers are disconnected.
 The only method that can be called is `establishIfNeeded`, which will attempt to
 re-acquire a secure session context and become enabled.
 */
@property (nonatomic, assign, readonly) BOOL disabledDueToMaxReadersConnected;

@property (nonatomic, assign, readonly) CROfflineMode offlineMode;

- (BOOL)configureOfflineMode:(CROfflineMode)offlineMode;

/**
 Tell the reader that we were unable to validate the secure sesssion data provided by CROmnichannelCommsDelegate corroborateSession: with the
 server.
 */
- (void)notifyValidationFailedDueToNetworkError;

/// Called once on init to configure the flipper ticket API callback.
- (void)configureTicketEndpointWithContext:(id)context callback:(void *)callback;

#pragma mark - PIN

/**
 This method indicates to the card reader that the cardholder has opted out of PIN verification.
 In certain markets (Austrialia, for instance) it is common practice for the PIN pad to include a
 "Sign Instead" button.
 */
- (BOOL)attemptToBypassInputRequirements;

@end
