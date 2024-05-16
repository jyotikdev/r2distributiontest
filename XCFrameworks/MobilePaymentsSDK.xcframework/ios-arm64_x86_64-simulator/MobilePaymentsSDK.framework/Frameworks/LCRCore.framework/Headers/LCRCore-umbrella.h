#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "eventlog_parser.h"
#import "feature_flags.h"
#import "pb.h"
#import "squareup/spe/eventlog_definition.pb.h"

FOUNDATION_EXPORT double LCRCoreVersionNumber;
FOUNDATION_EXPORT const unsigned char LCRCoreVersionString[];

