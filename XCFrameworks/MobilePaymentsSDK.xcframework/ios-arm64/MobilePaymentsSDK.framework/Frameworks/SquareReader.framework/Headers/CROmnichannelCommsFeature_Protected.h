//
//  CROmnichannelCommsFeature_Protected.h
//  SquareReader
//
//  Created by Andrew Klinker on 3/9/21.
//

@class CRPaymentCard;
@class DEDigitEntryView;

#import <SquareReader/CROmnichannelCommsFeature.h>


@interface CROmnichannelCommsFeature ()

/// Initialize a DEDigitEntryView for the provided card
- (DEDigitEntryView *)buildDigitEntryViewWithPaymentCard:(CRPaymentCard *)paymentCard
                                         bypassAllowed:(BOOL)bypassAllowed
                                           lastAttempt:(BOOL)lastAttempt;

// Used to send a digit to LCR
- (BOOL)submitDigit:(uint8_t)digit;

// Encrypts the digits stored in LCR and sends them to the reader
- (BOOL)submitDigits;

// Clears all digits stored in LCR
- (void)resetDigits;

@end
