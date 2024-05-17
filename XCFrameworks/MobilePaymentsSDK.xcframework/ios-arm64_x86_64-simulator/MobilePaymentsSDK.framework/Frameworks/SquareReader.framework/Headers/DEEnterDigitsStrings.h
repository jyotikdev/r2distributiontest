//
//  DEEnterDigitsStrings.h
//  SquareReader
//
//

#import "DEDigitEntryScreenStrings.h"


@interface DEEnterDigitsStrings : NSObject <DEDigitEntryScreenStrings>

- (nonnull instancetype)initWithTitle:(nonnull NSString *)title
                            clearText:(nonnull NSString *)clearText
                        clearFullText:(nonnull NSString *)clearFullText
                         buttonTitles:(nonnull NSArray<NSString *> *)buttonTitles
                  notEnoughDigitsText:(nonnull NSString *)notEnoughDigitsText
              lastFourMissingFallback:(nonnull NSString *)lastFourMissingFallback
          lastFourLocalizedBrandNames:(nonnull NSDictionary <NSNumber /* CRPaymentCardBrand */ *, NSString *> *)lastFourLocalizedBrandNames
                     unknownBrandName:(nonnull NSString *)unknownBrandName;

@property (nonatomic, copy, nonnull, readonly) NSString *title;
@property (nonatomic, copy, nonnull, readonly) NSString *clearText;
@property (nonatomic, copy, nonnull, readonly) NSString *clearFullText;
@property (nonatomic, copy, nonnull, readonly) NSArray<NSString *> *buttonTitles;
@property (nonatomic, copy, nonnull, readonly) NSString *notEnoughDigitsText;

@property (nonatomic, copy, nonnull, readonly) NSString *lastFourMissingFallback;
@property (nonatomic, copy, nonnull, readonly) NSDictionary <NSNumber /* CRPaymentCardBrand */ *, NSString *> *lastFourLocalizedBrandNames;
@property (nonatomic, copy, nonnull, readonly) NSString *unknownBrandName;

@end
