//
//  CRCardReaderBackend_Internal.h
//  cardreader
//
//  Created by Ken Wigginton on 11/17/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <SquareReader/CRCardReaderBackend.h>
#import <cardreader_shared/crs_timer.h>


@class CRDispatch;


@interface CRCardReaderBackend ()

/// Initialize with a dispatch queue and timer API.
- (instancetype)initWithDispatch:(CRDispatch *)dispatch timerAPI:(crs_timer_api_t const *)timer_api;

@end
