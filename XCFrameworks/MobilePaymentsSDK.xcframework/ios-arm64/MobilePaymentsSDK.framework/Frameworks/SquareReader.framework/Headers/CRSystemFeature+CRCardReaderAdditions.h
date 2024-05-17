//
//  CRSystemFeature+CRCardReaderAdditions.h
//  cardreader
//
//  Created by Robert Gilliam on 5/3/15.
//  Copyright (c) 2015 Square, Inc. All rights reserved.
//

#import <SquareReader/CRCardReader.h>
#import <SquareReader/CRSystemFeature.h>

/**
 A portion of the interface of the `CRSystemFeature` intended only for use
 by the CRCardReader.
 */
@interface CRSystemFeature (CRCardReaderAdditions)

- (void)cardReader:(CRCardReader *)cardReader didReceiveInCommsRate:(NSString *)inCommsRate outCommsRate:(NSString *)outCommsRate;

@end
