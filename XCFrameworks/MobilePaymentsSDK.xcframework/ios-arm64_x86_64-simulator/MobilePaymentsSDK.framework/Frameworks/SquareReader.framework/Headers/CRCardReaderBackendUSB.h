//
//  CRCardReaderBackendUSB.h
//  cardreader
//
//  Created by Dave Apgar on 5/7/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import "CRCardReaderBackend.h"


@interface CRCardReaderBackendUSB : CRCardReaderBackend <CRUSBTransportDelegate>

@property (nonatomic, strong) id<CRUSBTransport> transport;

@end
