//
//  DEEnterDigitsScreen.h
//  SquareReader
//
//  Created by Andrew Klinker on 5/7/21.
//

#import "DEDigitEntryScreen.h"

@interface DEEnterDigitsScreen : NSObject <DEDigitEntryScreen>

@property (nonatomic, readwrite) NSUInteger numberDigitsEntered;
@property (nonatomic, readwrite) BOOL shouldShowNotEnoughDigits;

@end
