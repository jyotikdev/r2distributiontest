//
//  CRCardReader_Internal.h
//  SquareReader
//
//  Created by Ken Wigginton on 12/8/16.
//  Copyright (c) 2016 Square, Inc. All rights reserved.
//

#import <cardreader/cr_cardreader.h>
#import <SquareReader/CRCardReaderBackendAudio_Internal.h>

#import <SquareReader/CRCardReader.h>


@interface CRCardReader () <CRCardReaderBackendAudioDelegate>

@property (nonatomic, assign, readwrite) cr_cardreader_cfg_t cardreader_cfg;

@end
