//
//  CoreSwipeDefines_Internal.h
//  CoreSwipe
//
//  Created by Michael White & Matthias Plappert on 1/22/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


static inline NSTimeInterval CSNSTimeIntervalFromMicroseconds(uint64_t usecs)
{
    return ((NSTimeInterval)usecs / (NSTimeInterval)1e6);
}
