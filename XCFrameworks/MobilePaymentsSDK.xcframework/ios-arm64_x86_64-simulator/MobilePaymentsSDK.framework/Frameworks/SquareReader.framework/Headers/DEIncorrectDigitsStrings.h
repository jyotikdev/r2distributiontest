//
//  DEIncorrectDigitsStrings.h
//  SquareReader
//
//

#import "DEDigitEntryScreenStrings.h"


@interface DEIncorrectDigitsStrings : NSObject <DEDigitEntryScreenStrings>

- (nonnull instancetype)initWithTitle:(nonnull NSString *)title
                      lastAttemptText:(nonnull NSString *)lastAttemptText
                         tryAgainText:(nonnull NSString *)tryAgainText;

@property (nonatomic, copy, nonnull, readonly) NSString *title;
@property (nonatomic, copy, nonnull, readonly) NSString *lastAttemptText;
@property (nonatomic, copy, nonnull, readonly) NSString *tryAgainText;

@end
