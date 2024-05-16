
//
//  DEDigitEntryView_Internal.h
//  SquareReader
//
//  Created by Andrew Klinker on 3/2/21.
//

@class CRPaymentCard;
@class CROmnichannelCommsFeature;

@protocol DEDigitEntryScreen;

#import "DEDigitEntryView.h"


@interface DEDigitEntryView ()

@property (nonatomic, nonnull, readwrite) CRPaymentCard *card;
@property (nonatomic, readwrite) BOOL bypassAllowed;
@property (nonatomic, readwrite) BOOL lastAttempt;
@property (nonatomic, nonnull, readwrite) CROmnichannelCommsFeature *omnichannelCommsFeature;

@property (nonatomic, nonnull, readwrite) id<DEDigitEntryScreen> currentScreen;

- (nonnull instancetype)initWithCard:(nonnull CRPaymentCard *)card
                       bypassAllowed:(BOOL)bypassAllowed
                         lastAttempt:(BOOL)lastAttempt
             omnichannelCommsFeature:(nonnull CROmnichannelCommsFeature *)omnichannelCommsFeature;

@end
