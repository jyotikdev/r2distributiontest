//
//  CRDispatch.h
//  cardreader
//
//  Created by Tamir Duberstein on 11/7/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CRDispatch : NSObject

- (instancetype)initWithDelegateQueue:(NSOperationQueue *)delegateQueue;
- (void)performDelegateBlock:(void (^)(void))block;
- (void)performLCRBlockAndWait:(void (^)(void))block;

@end // CRDispatch
