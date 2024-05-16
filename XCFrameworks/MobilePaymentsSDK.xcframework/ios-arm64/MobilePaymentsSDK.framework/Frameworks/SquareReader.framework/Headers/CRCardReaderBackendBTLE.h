//
//  CRCardReaderBackendBTLE.h
//  cardreader
//
//  Created by Robert Gilliam on 6/18/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import "CRCardReaderBackend.h"


@interface CRCardReaderBackendBTLE : CRCardReaderBackend <CRBTLETransportDelegate>

@property (nonatomic, strong) id<CRBTLETransport> transport;

@end
