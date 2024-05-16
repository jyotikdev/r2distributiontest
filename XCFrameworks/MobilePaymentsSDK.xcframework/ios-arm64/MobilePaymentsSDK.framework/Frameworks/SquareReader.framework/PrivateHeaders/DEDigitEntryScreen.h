//
//  DEDigitEntryScreen.h
//  SquareReader
//
//  Created by Andrew Klinker on 5/7/21.
//

@class DEDigitEntryView;


@protocol DEDigitEntryScreen

@property (nonatomic, weak, nullable, readwrite) DEDigitEntryView *digitEntryView;

- (nonnull instancetype)initWithDigitEntryView:(nonnull DEDigitEntryView *)digitEntryView;

- (CGRect)frame;
- (void)draw;
- (void)handleTap:(CGPoint)tapCoordinates;

@end
