//
//  CSAnalogSignalDecoder.h
//  CoreSwipe
//
//  Created by Martin Mroz on 5/14/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <SquareReader/CSBMC.h>
#import <SquareReader/CSAnalogSignalDecoderResult.h>


@interface CSAnalogSignalDecoder : NSObject

- (CSAnalogSignalDecoderResult *)decode:(CSBMCSignalRef)signal;

@end
