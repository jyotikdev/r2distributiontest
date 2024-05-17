//
//  CSSwipedPaymentCard_Protected.h
//  CoreSwipe
//
//  Created by Michael White on 12/24/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#import <SquareReader/CSSwipedPaymentCard.h>


BOOL CSTrackTypeRequiresEncryptedData(CSTrackType trackType);
BOOL CSTrackTypeRequiresKeyData(CSTrackType trackType);
BOOL CSTrackTypeContainsTrack2(CSTrackType trackType);


@class CPCString;


@interface CSSwipedPaymentCard ()

@property (nonatomic, assign, readonly) CSTrackType trackType;
@property (nonatomic, assign, readwrite) CPCPaymentCardReaderType readerType;

@end
