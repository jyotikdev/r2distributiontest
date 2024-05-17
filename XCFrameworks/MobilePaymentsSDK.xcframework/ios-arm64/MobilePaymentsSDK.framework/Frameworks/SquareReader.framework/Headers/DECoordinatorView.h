//
//  DECoordinatorView.h
//  SquareReader
//
//

@class CRPaymentCard;
@class CROmnichannelCommsFeature;
@class DEDigitEntryTheme;
@class DEDigitEntryStrings;
@class DEDigitEntryGlyphs;

@protocol DEDigitEntryCoordinatorSubview;
@protocol DEDigitEntryViewDelegate;


@interface DECoordinatorView : UIView

// Don't expose any initializers to POS
- (nonnull instancetype)init NS_UNAVAILABLE;
+ (nonnull instancetype) new NS_UNAVAILABLE;
- (nonnull instancetype)initWithCoder:(nonnull NSCoder *)coder NS_UNAVAILABLE;
- (nonnull instancetype)initWithFrame:(CGRect)frame NS_UNAVAILABLE;

@property (nonatomic, copy, nonnull, readwrite) DEDigitEntryGlyphs *glyphs;
@property (nonatomic, copy, nonnull, readwrite) DEDigitEntryStrings *strings;
@property (nonatomic, copy, nonnull, readwrite) DEDigitEntryTheme *theme;
@property (nonatomic, readwrite) BOOL isRetry;
@property (nonatomic, readwrite) BOOL isPhone;

@end
