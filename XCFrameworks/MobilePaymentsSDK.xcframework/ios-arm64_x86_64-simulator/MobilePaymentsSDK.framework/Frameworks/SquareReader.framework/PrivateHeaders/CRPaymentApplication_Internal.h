//
//  CRPaymentApplication_Internal.h
//  Reader
//
//  Created by Ken Wigginton on 9/30/16.
//  Copyright © 2016 Square. All rights reserved.
//

#import <cardreader/cr_payment_feature.h>

#import <SquareReader/CRPaymentApplication.h>


@interface CRPaymentApplication ()

/**
 Creates a new payment application from the underlying LCR payment application.

 @param app The underlying payment application. Cannot be NULL.
 @return a new CRPaymentApplication Object or nil if underlying application is NULL or malformed.
 */
- (instancetype)initWithPaymentApplication:(cr_payment_application_t const *)app;

@end
