//
//  CRReaderFeature_Internal.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <cardreader/cr_cardreader.h>

#import "CRReaderFeature.h"


@interface CRReaderFeature ()

- (cr_cardreader_feature_api_t)makeAPI;

@end
