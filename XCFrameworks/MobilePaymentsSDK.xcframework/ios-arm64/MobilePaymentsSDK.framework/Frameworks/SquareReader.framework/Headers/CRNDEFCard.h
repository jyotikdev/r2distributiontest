//
//  CRNDEFCard.h
//  cardreader
//
//  Created by Zella Urquhart on 8/24/20.
//

#import <Foundation/Foundation.h>


typedef NS_ENUM(NSUInteger, CRNDEFCardApplicationType) {
    CRNDEFCardApplicationTypeUnknown = 0,
    CRNDEFCardApplicationTypeTeamManagement,
    CRNDEFCardApplicationTypeLoyalty,
    CRNDEFCardApplicationTypeGift
};


@interface CRNDEFCard : NSObject

@property (nonatomic, readonly, copy) NSData *merchantID;
@property (nonatomic, readonly, copy) NSData *cardID;

@property (nonatomic, assign, readonly) CRNDEFCardApplicationType applicationType;
@property (nonatomic, readonly) int cardType;

+ (instancetype) new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithMerchantID:(NSData *)merchantID
                            cardID:(NSData *)cardID
                   applicationType:(CRNDEFCardApplicationType)applicationType
                          cardType:(int)cardType NS_DESIGNATED_INITIALIZER;

@end
