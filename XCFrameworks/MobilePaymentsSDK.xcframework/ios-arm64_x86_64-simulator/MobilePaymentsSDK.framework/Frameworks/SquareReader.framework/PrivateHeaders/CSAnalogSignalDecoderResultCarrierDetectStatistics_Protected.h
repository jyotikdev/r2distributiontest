//
//  CSAnalogSignalDecoderResultCarrierDetectStatistics_Protected.h
//  CoreSwipe
//
//  Created by Scott Perry on 9/16/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_types.h>

#import "CSAnalogSignalDecoderResultCarrierDetectStatistics.h"


@interface CSAnalogSignalDecoderResultCarrierDetectStatistics (Protected)

- (instancetype)initWithEvent:(cr_comms_event_t)event startSample:(uint64_t)startSample numberOfSamples:(uint32_t)samples classifyStatistics:(cr_comms_carrier_detect_packet_stats_t const *)statistics;

@end
