//
//  CRUserInteractionFeature.h
//  cardreader
//
//  Created by Ken Wigginton on 3/24/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>

@class CRCardReader;
@class CRUserInteractionFeature;


@interface CRUserInteractionFeature : CRFeature

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

/**
 Requests that the reader identify itself as appropriate for that device family.
 On R12, this amounts to blinking the LEDs. On A10 and R6, this does nothing.
 @return YES if the request to identify the reader was successful.
 */
- (BOOL)identify;

@end
