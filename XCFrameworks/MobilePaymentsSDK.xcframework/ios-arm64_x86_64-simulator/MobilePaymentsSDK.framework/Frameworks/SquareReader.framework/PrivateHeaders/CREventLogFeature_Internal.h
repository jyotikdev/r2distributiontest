//
//  LCREventlogFeature_Internal.h
//  cardreader
//
//  Created by Martin Mroz on 2/18/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import "CREventlogFeature.h"

#import <cardreader/cr_eventlog_feature.h>


@interface CREventlogFeature ()

@property (nonatomic, readonly, assign) struct cr_eventlog_t *eventlog;

@property (nonatomic, assign, readonly) cr_eventlog_data_cb_t on_data_received_cb;
@property (nonatomic, assign, readonly) cr_eventlog_profile_cb_t on_profile_received_cb;

- (void)processEventsAndInformDelegateOneAtATime:(crs_eventlog_utils_parsed_event_t const *)events eventCount:(size_t)eventCount;
- (void)processEventsAndInformDelegateAllAtOnce:(crs_eventlog_utils_parsed_event_t const *)events eventCount:(size_t)eventCount;

// if set, will ensure `processEventsAndInformDelegateOneAtATime:eventCount:` is called, even if
// the callback for `processEventsAndInformDelegateAllAtOnce:eventCount:` is implemented.
@property (nonatomic, assign) BOOL forcePerEventCallback;

@end
