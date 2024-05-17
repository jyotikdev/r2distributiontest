//
//  DECancelTransactionStrings.h
//  SquareReader
//
//


@interface DECancelTransactionStrings : NSObject

- (nonnull instancetype)initWithTitle:(nonnull NSString *)title
                cancelTransactionText:(nonnull NSString *)cancelTransactionText
                             backText:(nonnull NSString *)backText;

@property (nonatomic, copy, nonnull, readonly) NSString *title;
@property (nonatomic, copy, nonnull, readonly) NSString *cancelTransactionText;
@property (nonatomic, copy, nonnull, readonly) NSString *backText;
@end
