//
//  CRCardReaderBackendAudio_Internal.h
//  cardreader
//
//  Created by Ken Wigginton on 11/17/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <cardreader/cr_comms_backend_audio.h>

#import <SquareReader/CRCardReaderBackendAudio.h>

#import "CRCardReaderBackend_Internal.h"


@protocol CRCardReaderBackendAudioDelegate;


@interface CRCardReaderBackendAudio () <CRAudioTransportDelegate>

@property (nonatomic, weak) id<CRCardReaderBackendAudioDelegate> delegate;
@property (nonatomic, assign, readonly) cr_comms_backend_audio_t *backend_audio;

@property (nonatomic, assign) cr_comms_backend_audio_send_raw_cb_t send_raw_cb;

@end

/// Callbacks from the backend to transform the raw data into data sent to the transport.
/// It is the responsiblity of the receiver to handle dispatching these on the appropriate dispatch queue.
@protocol CRCardReaderBackendAudioDelegate <NSObject>

- (void)readerReceiveInCommsRate:(cr_cardreader_comms_rate_t)in_comms_rate outCommsRate:(cr_cardreader_comms_rate_t)out_comms_rate;

@end
