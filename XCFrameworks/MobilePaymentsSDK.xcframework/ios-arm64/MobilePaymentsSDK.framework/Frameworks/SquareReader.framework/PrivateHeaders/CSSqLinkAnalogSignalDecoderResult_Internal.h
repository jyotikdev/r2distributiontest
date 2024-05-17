//
//  CSSqLinkAnalogSignalDecoderResult_Internal.h
//  CoreSwipe
//
//  Created by Ken Wigginton on 11/22/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_types.h>

#import <SquareReader/CSAnalogSignalDecoderResult.h>


@interface CSSqLinkAnalogSignalDecoderResult ()

- (void)populateWithStatistics:(cr_comms_sqlink_demod_stats_t)statistics;

@end
