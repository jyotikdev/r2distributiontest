//
//  CRSystemFeature_Internal.h
//  cardreader
//
//  Created by Dave Terra on 2/17/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import <SquareReader/CRSystemFeature.h>

#import <cardreader/cr_system_feature.h>


@interface CRSystemFeature ()

@property (nonatomic, readonly, assign) struct cr_system_t *system;

@property (nonatomic, readonly, assign) cr_system_event_api_t event_api;

@end
