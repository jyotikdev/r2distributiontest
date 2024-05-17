//
//  CRContext.h
//  SquareReader
//
//  Created by Robert Gilliam on 8/4/17.
//

#import <Foundation/Foundation.h>

/**
 Stores opaque key material to provide to CR secure session.

 Think of this as the 'CRWhiteboxKeyStore'.

 Methods are intentionally named vaguely to hinder reverse engineering.

 For example, this is useful for providing unique keys for
 different third party apps using PaySDK.
 */
@interface CRContext : NSObject

/// @returns YES if contextWithData: has been called, NO otherwise.
+ (BOOL)hasContext;

/// @returns The keyMaterial passed to configureWithKeyMaterial:, or nil otherwise.
/// May also return nil if configured with nil key material.
+ (NSData *)data;

/// Call only once. Stores the provided key material, which will be used for all future secure session establishments.
/// Must call before card readers can be brought up.
+ (void)contextWithData:(NSData *)data;

/// For testing only.
+ (void)debug_reset;

@end
