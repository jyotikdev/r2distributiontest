//
//  DEDigitEntryAccessibilitySelectionStrings.h
//  SquareReader
//
//

#import "DEDigitEntryScreenStrings.h"


@interface DEDigitEntryAccessibilitySelectionStrings : NSObject <DEDigitEntryScreenStrings>

- (nonnull instancetype)initWithText:(nonnull NSString *)selectionScreenText
         accessibilityModeButtonText:(nonnull NSString *)accessibilityModeButtonText
          highContrastModeButtonText:(nonnull NSString *)highContrastModeButtonText;

@property (nonatomic, copy, nonnull, readonly) NSString *selectionScreenText;
@property (nonatomic, copy, nonnull, readonly) NSString *accessibilityModeButtonText;
@property (nonatomic, copy, nonnull, readonly) NSString *highContrastModeButtonText;

@end
