//
//  DEDigitEntryCoordinatorSubview.h
//  SquareReader
//
//

@class DECoordinatorView;
@class DEDigitEntryTheme;
@class DEDigitEntryGlyphs;

@protocol DEDigitEntryScreenStrings;
@protocol DEDigitEntryViewDelegate;


@protocol DEDigitEntryCoordinatorSubview

@property (nonatomic, copy, nonnull, readwrite) DEDigitEntryGlyphs *glyphs;
@property (nonatomic, copy, nonnull, readwrite) DEDigitEntryTheme *theme;
@property (nonatomic, readwrite) BOOL isRetry;
@property (nonatomic, readwrite) BOOL isPhone;

@property (nonatomic, weak, nullable, readwrite) id<DEDigitEntryViewDelegate> delegate;
@property (nonatomic, weak, nullable, readwrite) DECoordinatorView *coordinator;

- (void)setStrings:(id<DEDigitEntryScreenStrings>_Nonnull)strings;

@end
