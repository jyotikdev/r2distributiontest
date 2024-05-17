//
//  CRCardReaderBackendAudio.h
//  cardreader
//
//  Created by Dave Apgar on 5/6/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import <SquareReader/CRTransport.h>

#import <SquareReader/CRCardReaderBackend.h>


@interface CRCardReaderBackendAudio : CRCardReaderBackend

@property (nonatomic, weak) id<CRAudioTransport> transport;

- (void)playCRWakeUpToneOverAnalogInterface;

@end
