//
//  NSUUID+LCRAdditions.h
//  SquareReader
//
//  Created by Zachary Drayer on 12/17/18.
//  Copyright (c) 2018 Square, Inc. All rights reserved.
//

@import Foundation;


@interface NSUUID (LCR_UUID)

@property (nonatomic, nonnull, copy, readonly) NSString *LCR_alphaNumericString;
@property (nonatomic, nonnull, copy, readonly) NSString *LCR_miryoCRUUIDString;

@end
