//
//  CRPaymentAccount.h
//  SquareReader
//
//  Created by Zachary Drayer on 7/15/18.
//

#import <Foundation/Foundation.h>


typedef NS_CLOSED_ENUM(NSInteger, CRPaymentAccountType) {
    CRPaymentAccountTypeDefault,
    CRPaymentAccountTypeSavings,
    CRPaymentAccountTypeDebit,
    CRPaymentAccountTypeCredit,
    CRPaymentAccountTypeCount,
};


@interface CRPaymentAccount : NSObject <NSCopying>

- (nonnull instancetype)initWithAccountType:(CRPaymentAccountType)type sessionID:(nonnull NSUUID *)sessionID NS_DESIGNATED_INITIALIZER;
- (nonnull instancetype)init NS_UNAVAILABLE;
+ (nonnull instancetype) new NS_UNAVAILABLE;

@property (nonatomic, readonly) CRPaymentAccountType type;
@property (nonatomic, readonly, nonnull) NSUUID *sessionID;

@end
