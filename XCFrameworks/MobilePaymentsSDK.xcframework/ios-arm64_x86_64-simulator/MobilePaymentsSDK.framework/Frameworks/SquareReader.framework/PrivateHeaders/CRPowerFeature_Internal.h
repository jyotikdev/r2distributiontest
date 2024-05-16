//
//  LCRPowerFeature_Internal.h
//  cardreader
//
//  Created by Dave Terra on 2/18/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import "CRPowerFeature.h"

#import <cardreader/cr_power_feature.h>


@interface CRPowerFeature ()

@property (nonatomic, readonly, assign) struct cr_power_t *power;

@property (nonatomic, readonly, assign) cr_power_event_api_t event_api;

@end
