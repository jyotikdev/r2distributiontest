//
//  CRCardReaderBackend.h
//  cardreader
//
//  Created by Dave Apgar on 5/7/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import <SquareReader/CRTransport.h>


@class CRDispatch;


/// Abstract base class for LCR comms backends.
@interface CRCardReaderBackend : NSObject

@property (nonatomic, weak, readonly) CRDispatch *dispatch;

/// Call to tear down and unload the backend. Should be called before deallocating the instance.
- (void)unloadAndTearDownBackend;

@end
