//
//  CRPaymentCard_Internal.h
//  cardreader
//
//  Created by Ken Wigginton on 11/17/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <cardreader/cr_payment_defines.h>

#import <SquareReader/CRPaymentCard.h>


@interface CRPaymentCard ()

/**
 Creates a new card info from the underlying LCR card info.
 NOTE: card_info.application is currently ignored

 @param card_info The underlying card info. Cannot be NULL.
 @return a new CRPaymentCard Object or nil if underlying card_info is NULL or malformed.
 */
- (instancetype)initWithCardInfo:(cr_card_info_t const *)card_info;

@end
