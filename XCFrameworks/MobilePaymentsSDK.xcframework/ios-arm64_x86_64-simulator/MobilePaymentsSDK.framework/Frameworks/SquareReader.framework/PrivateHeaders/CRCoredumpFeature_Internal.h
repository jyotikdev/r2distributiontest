//
//  CRCoredumpFeature_Internal.h
//  cardreader
//
//  Created by Martin Mroz on 2/17/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import "CRCoredumpFeature.h"

#import <cardreader/cr_coredump_feature.h>


@interface CRCoredumpFeature ()

@property (nonatomic, readonly, assign) struct cr_coredump_t *coredump;

@property (nonatomic, readonly, assign) cr_coredump_received_coredump_info_cb_t received_coredump_info_cb;
@property (nonatomic, readonly, assign) cr_coredump_received_coredump_data_cb_t received_coredump_data_cb;
@property (nonatomic, readonly, assign) cr_coredump_received_coredump_erased_cb_t received_coredump_erased_cb;
@property (nonatomic, readonly, assign) cr_coredump_received_coredump_triggered_cb_t received_coredump_triggered_cb;
@property (nonatomic, readonly, assign) cr_coredump_receive_data_progress_cb_t receive_data_progress_cb;

@end
