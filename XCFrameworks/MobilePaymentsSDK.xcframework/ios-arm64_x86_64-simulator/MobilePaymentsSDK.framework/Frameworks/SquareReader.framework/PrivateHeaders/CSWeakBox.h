//
//  CSWeakBox.h
//  SquareReader
//
//  Created by Zachary Drayer on 8/3/21.
//

#import <Foundation/Foundation.h>

@interface CSWeakBox<__covariant ObjectType> : NSObject

+ (nullable instancetype)new NS_UNAVAILABLE;
- (nullable instancetype)init NS_UNAVAILABLE;
- (nonnull instancetype)initWithObject:(nonnull ObjectType)object callback:(void (^_Nonnull)(ObjectType _Nonnull object))block NS_DESIGNATED_INITIALIZER;

@property (nullable, nonatomic, readonly, weak) ObjectType object;
@property (nonnull, nonatomic, copy) void (^block)(ObjectType _Nonnull);

- (void)fire:(nonnull NSTimer *)timer;

@end
