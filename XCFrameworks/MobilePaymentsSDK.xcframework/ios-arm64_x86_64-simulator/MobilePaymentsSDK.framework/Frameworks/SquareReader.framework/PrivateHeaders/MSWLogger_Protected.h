//  Copyright (c) 2013-2016 Square Inc. All rights reserved.

#import "MSWLogger.h"


@interface MSWLogger ()

- (void)fetchValidTicketCompletionOnMainQueue:(dispatch_block_t)completion;

- (BOOL)readPolicy:(BOOL (^)(const void *data, const size_t data_len))onRead;
- (BOOL)writePolicy:(const void *)data withLength:(const size_t)length;
- (BOOL)deletePolicy;
- (BOOL)hasPolicy;

@end
