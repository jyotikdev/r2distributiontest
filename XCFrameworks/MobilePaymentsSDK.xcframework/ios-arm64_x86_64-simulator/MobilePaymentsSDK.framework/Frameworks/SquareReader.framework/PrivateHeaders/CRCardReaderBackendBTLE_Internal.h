//
//  CRCardReaderBackendBTLE_Internal.h
//  cardreader
//
//  Created by Ken Wigginton on 11/17/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_backend_ble.h>

#import <SquareReader/CRCardReaderBackendBTLE.h>

#import "CRCardReaderBackend_Internal.h"


@interface CRCardReaderBackendBTLE ()

@property (nonatomic, assign) cr_comms_backend_ble_t *backend_ble;

/**
 Returns a new instance of the LCR Bluetooth LE backend.
 @param commsVersionData An opaque blob of NSData indicating the commsVersion to LCR.
 */
- (instancetype)initWithTransport:(id<CRBTLETransport>)transport dispatch:(CRDispatch *)dispatch timerAPI:(const crs_timer_api_t *)timer_api commsVersionData:(NSData *)commsVersionData NS_DESIGNATED_INITIALIZER;

@end
