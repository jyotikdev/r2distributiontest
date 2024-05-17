//
//  DEDigitEntryAccessibilityInstructionsStrings.h
//  SquareReader
//
//

#import "DEDigitEntryScreenStrings.h"


@interface DEDigitEntryAccessibilityInstructionsStrings : NSObject <DEDigitEntryScreenStrings>

- (nonnull instancetype)initWithInstructionsPage1Text:(nonnull NSString *)instructionsPage1Text
                                instructionsPage2Text:(nonnull NSString *)instructionsPage2Text
                           skipInstructionsButtonText:(nonnull NSString *)skipInstructionsButtonText
                         replayInstructionsButtonText:(nonnull NSString *)replayInstructionsButtonText
                                   continueButtonText:(nonnull NSString *)continueButtonText;

@property (nonatomic, copy, nonnull, readonly) NSString *instructionsPage1Text;
@property (nonatomic, copy, nonnull, readonly) NSString *instructionsPage2Text;
@property (nonatomic, copy, nonnull, readonly) NSString *skipInstructionsButtonText;
@property (nonatomic, copy, nonnull, readonly) NSString *replayInstructionsButtonText;
@property (nonatomic, copy, nonnull, readonly) NSString *continueButtonText;

@end
