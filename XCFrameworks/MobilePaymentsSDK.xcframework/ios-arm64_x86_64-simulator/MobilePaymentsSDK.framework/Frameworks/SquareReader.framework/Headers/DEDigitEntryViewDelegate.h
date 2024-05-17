//
//  DEDigitEntryViewDelegate.h
//  SquareReader
//
//  Created by Andrew Klinker on 3/2/21.
//

@class DEDigitEntryView;


@protocol DEDigitEntryViewDelegate

- (void)digitEntryViewDidTapCancel:(DEDigitEntryView *)digitEntryView;
- (void)digitEntryViewDidTapSubmit:(DEDigitEntryView *)digitEntryView;
- (void)digitEntryViewDidBypassDigitEntry:(DEDigitEntryView *)digitEntryView;

@end
