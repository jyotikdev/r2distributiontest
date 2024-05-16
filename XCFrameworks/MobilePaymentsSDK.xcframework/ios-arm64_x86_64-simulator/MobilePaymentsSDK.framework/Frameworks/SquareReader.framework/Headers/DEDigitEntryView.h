//
//  DEDigitEntryView.h
//  SquareReader
//
//  Created by Andrew Klinker on 3/2/21.
//

@class DEDigitEntryTheme;
@class DEDigitEntryStrings;
@class DEDigitEntryGlyphs;

@protocol DEDigitEntryViewDelegate;

#import "DEDigitEntryCoordinatorSubview.h"


@interface DEDigitEntryView : UIView <DEDigitEntryCoordinatorSubview>

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

@property (nonatomic, weak, nullable, readwrite) id<DEDigitEntryViewDelegate> delegate;

@end
