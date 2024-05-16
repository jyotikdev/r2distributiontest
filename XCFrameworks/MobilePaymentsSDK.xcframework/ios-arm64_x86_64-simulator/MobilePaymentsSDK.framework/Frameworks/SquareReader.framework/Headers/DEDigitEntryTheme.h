//
//  DEDigitEntryTheme.h
//  SquareReader
//
//  Created by Andrew Klinker on 3/2/21.
//


@interface DEDigitEntryTheme : NSObject

- (nonnull instancetype)initWithBackgroundColor:(nonnull UIColor *)backgroundColor
                   accessibilityBackgroundColor:(nonnull UIColor *)accessibilityBackgroundColor
                             neutralButtonColor:(nonnull UIColor *)neutralButtonColor
                        highContrastButtonColor:(nonnull UIColor *)highContrastButtonColor
                              cancelButtonColor:(nonnull UIColor *)cancelButtonColor
                               clearButtonColor:(nonnull UIColor *)clearButtonColor
                              submitButtonColor:(nonnull UIColor *)submitButtonColor
                            emphasisButtonColor:(nonnull UIColor *)emphasisButtonColor
                               defaultTextColor:(nonnull UIColor *)defaultTextColor
                              cardInfoTextColor:(nonnull UIColor *)cardInfoTextColor
                            backButtonTextColor:(nonnull UIColor *)backButtonTextColor
                               inverseTextColor:(nonnull UIColor *)inverseTextColor
                                    defaultFont:(nonnull UIFont *)defaultFont
                                      titleFont:(nonnull UIFont *)titleFont
                          highContrastTitleFont:(nonnull UIFont *)highContrastTitleFont
                               instructionsFont:(nonnull UIFont *)instructionsFont
                                      largeFont:(nonnull UIFont *)largeFont
                    highContrastDigitButtonFont:(nonnull UIFont *)highContrastDigitButtonFont
                    highContrastClearButtonFont:(nonnull UIFont *)highContrastClearButtonFont
                                lastAttemptFont:(nonnull UIFont *)lastAttemptFont;

// Element Colors
@property (nonatomic, copy, nonnull, readonly) UIColor *backgroundColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *accessibilityBackgroundColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *neutralButtonColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *highContrastButtonColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *cancelButtonColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *clearButtonColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *submitButtonColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *emphasisButtonColor;

// Text Colors
@property (nonatomic, copy, nonnull, readonly) UIColor *defaultTextColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *cardInfoTextColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *backButtonTextColor;
@property (nonatomic, copy, nonnull, readonly) UIColor *inverseTextColor;

// Fonts
@property (nonatomic, copy, nonnull, readonly) UIFont *defaultFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *titleFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *highContrastTitleFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *instructionsFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *largeFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *highContrastDigitButtonFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *highContrastClearButtonFont;
@property (nonatomic, copy, nonnull, readonly) UIFont *lastAttemptFont;

@end
