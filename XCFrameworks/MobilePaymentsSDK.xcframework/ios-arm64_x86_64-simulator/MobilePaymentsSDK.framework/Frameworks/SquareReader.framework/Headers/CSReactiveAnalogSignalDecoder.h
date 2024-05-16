//
//  CSReactiveAnalogSignalDecoder.h
//  CoreSwipe
//
//  Created by Alan Fineberg on 5/16/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import "CSAnalogSignalDecoder.h"


@interface CSReactiveAnalogSignalDecoder : CSAnalogSignalDecoder

@property (nonatomic, assign) short hysteresisPercentage;
@property (nonatomic, assign) short adaptivePercentage;

@end
