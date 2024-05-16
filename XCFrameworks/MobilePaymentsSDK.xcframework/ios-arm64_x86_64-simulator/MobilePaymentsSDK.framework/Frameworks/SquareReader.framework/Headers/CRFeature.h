//
//  CRFeature.h
//  cardreader
//
//  Created by Peter Mattis on 11/03/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@class CRCardReader;


@interface CRFeature : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithCardReader:(CRCardReader *)cardReader NS_DESIGNATED_INITIALIZER;

- (void)unloadFeature __attribute__((objc_requires_super));

@end // CRFeature
