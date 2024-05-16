//
//  DEDigitEntryStrings.h
//  SquareReader
//
//  Created by Andrew Klinker on 3/2/21.
//

#import "DEDigitEntryScreenStrings.h"

@class DECancelTransactionStrings;
@class DEDigitEntryAccessibilityInstructionsStrings;
@class DEDigitEntryAccessibilitySelectionStrings;
@class DEEnterDigitsStrings;
@class DEIncorrectDigitsStrings;

@interface DEDigitEntryStrings : NSObject <DEDigitEntryScreenStrings>

- (nonnull instancetype)initWithEnterDigitsStrings:(nonnull DEEnterDigitsStrings *)enterDigitsStrings
                                     cancelStrings:(nonnull DECancelTransactionStrings *)cancelStrings
                            incorrectDigitsStrings:(nonnull DEIncorrectDigitsStrings *)incorrectDigitsStrings
                     accessibilitySelectionStrings:(nonnull DEDigitEntryAccessibilitySelectionStrings *)accessibilitySelectionStrings
                  accessibilityInstructionsStrings:(nonnull DEDigitEntryAccessibilityInstructionsStrings *)accessibilityInstructionsStrings;

- (instancetype _Nonnull )init NS_UNAVAILABLE;

@property (nonatomic, copy, nonnull, readonly) DEEnterDigitsStrings *enterDigits;
@property (nonatomic, copy, nonnull, readonly) DECancelTransactionStrings *cancel;
@property (nonatomic, copy, nonnull, readonly) DEIncorrectDigitsStrings *incorrectDigits;
@property (nonatomic, copy, nonnull, readonly) DEDigitEntryAccessibilitySelectionStrings *accessibilitySelection;
@property (nonatomic, copy, nonnull, readonly) DEDigitEntryAccessibilityInstructionsStrings *accessibilityInstructions;
@property (nonatomic, copy, nonnull, readonly) NSString *cancelTransactionText;

@end
