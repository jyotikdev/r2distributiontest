//
//  CSSqLinkAnalogSignalDecoder.h
//  CoreSwipe
//
//  Created by Alan Fineberg on 6/3/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_types.h>
#import <cardreader/cr_comms_modem.h>
#import <cardreader/M1Packet.h>

#import <SquareReader/CSAnalogSignalDecoder.h>


@interface CSSqLinkAnalogSignalDecoder : CSAnalogSignalDecoder

/// Providing the control block does not take ownership. It is the callers responsibility to maintain lifecycle.
+ (instancetype)decoderWithLinkType:(cr_comms_link_type_t)gumLink demodulator:(cr_comms_modem_t *)controlBlock;

@end
