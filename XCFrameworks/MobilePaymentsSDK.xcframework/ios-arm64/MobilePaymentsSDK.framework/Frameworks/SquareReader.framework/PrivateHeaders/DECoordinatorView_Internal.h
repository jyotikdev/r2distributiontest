//
//  DECoordinatorView_Internal.h
//  SquareReader
//
//

@class CRPaymentCard;
@class CROmnichannelCommsFeature;

@protocol DEDigitEntryCoordinatorSubview;
@protocol DEDigitEntryViewDelegate;

#import "DECoordinatorView.h"


@interface DECoordinatorView ()

@property (nonatomic, nonnull, readwrite) UIView<DEDigitEntryCoordinatorSubview> *currentView;

@property (nonatomic, weak, nullable, readwrite) id<DEDigitEntryViewDelegate> delegate;

- (nonnull instancetype)initWithCard:(nonnull CRPaymentCard *)card
                       bypassAllowed:(BOOL)bypassAllowed
                         lastAttempt:(BOOL)lastAttempt
             omnichannelCommsFeature:(nonnull CROmnichannelCommsFeature *)omnichannelCommsFeature;

- (void)returnToDigitEntryScreen;

@end
