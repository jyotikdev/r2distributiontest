//
//  CRFeature_Protected.h
//  cardreader
//
//  Created by Tamir Duberstein on 11/6/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>


@class CRDispatch;


@interface CRFeature ()

@property (nonatomic, strong, readonly) CRDispatch *dispatch;

@end // CRFeature
