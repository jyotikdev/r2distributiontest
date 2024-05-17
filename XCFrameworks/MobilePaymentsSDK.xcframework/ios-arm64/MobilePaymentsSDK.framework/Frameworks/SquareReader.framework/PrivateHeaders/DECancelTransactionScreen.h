//
//  DECancelTransactionScreen.h
//  SquareReader
//
//  Created by Andrew Klinker on 5/7/21.
//

#import "DEDigitEntryScreen.h"

@interface DECancelTransactionScreen : NSObject <DEDigitEntryScreen>

@property (nonatomic, readwrite) CGRect cancelTransactionButtonRect;
@property (nonatomic, readwrite) CGRect backButtonRect;

@end
