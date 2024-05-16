//
//  DEIncorrectDigitsScreen.h
//  SquareReader
//
//  Created by Andrew Klinker on 5/12/21.
//

#import "DEDigitEntryScreen.h"

@interface DEIncorrectDigitsScreen : NSObject <DEDigitEntryScreen>

@property (nonatomic, readwrite) CGRect cancelTransactionButtonRect;
@property (nonatomic, readwrite) CGRect tryAgainButtonRect;

@end
