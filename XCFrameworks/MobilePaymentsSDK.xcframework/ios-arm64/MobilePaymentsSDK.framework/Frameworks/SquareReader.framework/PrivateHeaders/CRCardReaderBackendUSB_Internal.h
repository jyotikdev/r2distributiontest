//
//  CRCardReaderBackendUSB_Internal.h
//  cardreader
//
//  Created by Ken Wigginton on 11/17/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_backend_usb.h>
#import <SquareReader/CRCardReaderBackendUSB.h>

#import "CRCardReaderBackend_Internal.h"


@interface CRCardReaderBackendUSB ()

@property (nonatomic, assign) cr_comms_backend_usb_t *backend_usb;

@end
