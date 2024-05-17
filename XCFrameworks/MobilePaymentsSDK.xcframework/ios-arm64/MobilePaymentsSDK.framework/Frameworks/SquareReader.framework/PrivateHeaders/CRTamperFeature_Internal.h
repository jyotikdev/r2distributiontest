//
//  CRTamperFeature_Testing.h
//  cardreader
//
//  Created by Dave Terra on 2/18/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import "CRTamperFeature.h"

#import <cardreader/cr_tamper_feature.h>


@interface CRTamperFeature ()

@property (nonatomic, readonly, assign) struct cr_tamper_t *tamper;

@property (nonatomic, readonly, assign) cr_tamper_event_api_t event_api;

@end
