//
//  CROmnichannelCommsFeature_Internal.h
//  cardreader
//
//  Created by Dave Terra on 2/18/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import <cardreader/cr_securesession_feature.h>

#import <SquareReader/CROmnichannelCommsFeature.h>


typedef cr_securesession_feature_result_t (^CROmnichannelCommsFeatureInitUnderlyingFeatureBlock)(struct cr_cardreader_t *underlyingCardReader, struct cr_securesession_feature_t **returnSecuresession);


@interface CROmnichannelCommsFeature ()

- (CROmnichannelCommsFeatureResult)forwardServerMessageToSecureSessionFeature:(NSData *)data
                                                                  serverError:(CROmnichannelCommsFeatureServerError *)server_error
                                                           userExperienceHint:(CROmnichannelCommsFeatureServerUserExperienceHint *)userExperienceHint
                                                               localizedTitle:(NSString **)localizedTitle
                                                         localizedDescription:(NSString **)localizedDescription;

@property (nonatomic, weak, readonly) CRCardReader *cardReader;

// The underlying securesession_feature. May be NULL if we were unable to create one.
@property (nonatomic, assign, readwrite) struct cr_securesession_feature_t *underlyingHandle;

// Whether or not the notification listener is registered with LSPE.
@property (nonatomic, assign, getter=isLspeListenerRegistered) BOOL lspeListenerRegistered;


///
/// A block which creates a cr_securesession_feature_t, returning it to the caller via the `returnSecuresession`
/// parameter if successful. Exists for testing how the feature behaves when it fails to init the underlying
/// feature.
///
/// @returns The result of attempting to init the cr_securesession_feature_t. If this is not `CR_SECURESESSION_FEATURE_RESULT_SUCCESS`, returnSecuresession will not be modified.
///
@property (nonatomic, copy, readonly) CROmnichannelCommsFeatureInitUnderlyingFeatureBlock initUnderlyingFeatureBlock;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader underlyingFeatureFactoryBlock:(CROmnichannelCommsFeatureInitUnderlyingFeatureBlock)underlyingFeatureFactoryBlock NS_DESIGNATED_INITIALIZER;

@end
