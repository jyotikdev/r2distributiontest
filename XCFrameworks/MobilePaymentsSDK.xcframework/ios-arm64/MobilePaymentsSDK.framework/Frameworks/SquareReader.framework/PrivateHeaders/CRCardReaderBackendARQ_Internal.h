//
//  CRCardReaderBackendARQ_Internal.h
//  cardreader
//
//  Created by Zachary Drayer on 2/24/20.
//  Copyright (c) 2020 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_backend_arq.h>
#import <SquareReader/CRCardReaderBackendARQ.h>

#import "CRCardReaderBackend_Internal.h"


@interface CRCardReaderBackendARQ ()

@property (nonatomic, unsafe_unretained) cr_comms_backend_arq_t *backend_arq;

@end
