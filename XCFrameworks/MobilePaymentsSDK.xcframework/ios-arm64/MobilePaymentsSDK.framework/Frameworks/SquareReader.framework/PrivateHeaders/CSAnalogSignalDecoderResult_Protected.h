//
//  CSAnalogSignalDecoderResult_Protected.h
//  CoreSwipe
//
//  Created by Martin Mroz on 8/13/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import "CSAnalogSignalDecoderResult.h"


NSString *CSNSStringFromCSTrackDecodeOutcomes(NSArray *trackDecodeOutcomes);


@interface CSAnalogSignalDecoderResult (/* Protected */)

- (NSDictionary *)debugAttributesDictionary;

@end
