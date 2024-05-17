//
//  CSLog.h
//  Pods
//
//  Created by Robert Gilliam on 9/27/16.
//
//

#import <Foundation/Foundation.h>


/**
 * Notification posted whenever CSLog() logs a message.
 * The object is the message logged as an NSString.
 */
extern NSString *const CSLogMessageWasLoggedNotification;


/// Log a message constructed by the provided format string and arguments.
OBJC_EXTERN void CSLog(NSString *format, ...) NS_FORMAT_FUNCTION(1, 2);

/**
 * Log a message containing the description of self, the current selector and a user specified message.
 * Useful for tracing method execution for methods taking no parameters.
 */
OBJC_EXTERN void CSLogTraceWithSelfAndSelector(id self, SEL _cmd, NSString *format, ...) NS_FORMAT_FUNCTION(3, 4);
OBJC_EXTERN void CSLogTraceWithSelector(SEL _cmd, NSString *format, ...) NS_FORMAT_FUNCTION(2, 3);

/**
 * Log a message containing the description of self, the current selector and a user specified message.
 * Useful for tracing method execution for methods taking no parameters.
 */
#define CSLogTraceMessage(...) CSLogTraceWithSelfAndSelector(self, _cmd, __VA_ARGS__)
#define CSLogMethodTraceMessage(...) CSLogTraceWithSelector(_cmd, __VA_ARGS__)

/// Start writing all CSLog() messages to console.
OBJC_EXTERN void CSLogStartLoggingToConsole(void);

/// Stop writing all CSLog() messages to console.
OBJC_EXTERN void CSLogStopLoggingToConsole(void);

/**
 * Returns YES if all CSLog() log messages are being written to console, NO
 * otherwise. Defaults to NO.
 */
OBJC_EXTERN BOOL CSLogIsLoggingToConsole(void);
