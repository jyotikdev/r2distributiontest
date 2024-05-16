@import Foundation;


@interface NSString (SQRCSAdditions)

+ (instancetype)SQRCS_stringWithCString:(const char *)cString length:(size_t)length encoding:(NSStringEncoding)encoding;

@end
