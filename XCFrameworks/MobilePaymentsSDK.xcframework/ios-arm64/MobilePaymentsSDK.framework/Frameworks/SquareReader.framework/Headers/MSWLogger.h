//  Copyright (c) 2013-2015 Square Inc. All rights reserved.

#import <Foundation/Foundation.h>


@protocol MSWLoggerNetworkingDelegate;
@protocol MSWLoggerProtoDelegate;
@protocol MSWLoggerKeychainProvider;
@protocol MSWLoggerLifecycleTaskHandler;


@interface MSWLogger : NSObject

/**
 The current instance registered with Minesweeper's subsystem.
 
 There can only be one of these instances. It is set by calling configureAndStartMinesweeper.
 */
@property (strong, class, readonly) MSWLogger *minesweeperRegisteredLogger;

- (instancetype)initWithNetworkingDelegate:(id<MSWLoggerNetworkingDelegate>)networkingDelegate
                             protoDelegate:(id<MSWLoggerProtoDelegate>)protoDelegate
                          keychainProvider:(id<MSWLoggerKeychainProvider>)keychainProvider
                          lifecycleHandler:(id<MSWLoggerLifecycleTaskHandler>)lifecycleHandler
                            ticketMetadata:(NSString *)ticketMetadata NS_DESIGNATED_INITIALIZER;

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype) new NS_UNAVAILABLE;

/// Provides networking connectivity to the logger.
@property (weak, atomic, readonly) id<MSWLoggerNetworkingDelegate> networkingDelegate;

/// Provides proto parsing and generation to the logger.
@property (weak, atomic, readonly) id<MSWLoggerProtoDelegate> protoDelegate;

/// Provides keychain access to the logger.
@property (weak, atomic, readonly) id<MSWLoggerKeychainProvider> keychainProvider;

- (void)updateTicketAsync;
- (void)configureAndStartMinesweeper;

#ifdef DEBUG_MENU
- (void)forceStoredTicketExpiration;
- (void)clearStoredTicket;
#endif

@end


@protocol MSWLoggerLifecycleTaskHandler

- (id)beginBackgroundTaskWithExpirationHandler:(void(^)(void))handler;
- (void)endBackgroundTask:(id)identifier;

@end


/// Provider which provides access to keychain storage on the device.
@protocol MSWLoggerKeychainProvider <NSObject>

- (BOOL)minesweeperLogger:(MSWLogger *)logger storeObjectInKeychain:(NSData *)object forKey:(NSString *)key;
- (BOOL)minesweeperLogger:(MSWLogger *)logger removeObjectInKeychainWithKey:(NSString *)key;

- (NSData *)minesweeperLogger:(MSWLogger *)logger objectInKeychainForKey:(NSString *)key;

@end


typedef void (^MSWLoggerNetworkingDelegateResponseHandler)(NSData *responseData, NSData *newTicket, NSData *msFrame, NSError *error);

/// Delegate necessary to facilitate network requests.
@protocol MSWLoggerNetworkingDelegate <NSObject>

/// Perform a flipper ticket request with data response handler. The responseHandler is required.
- (void)performFlipperTicketRequestWithFrameData:(NSData *)frameData ticketData:(NSData *)ticketData responseHandler:(MSWLoggerNetworkingDelegateResponseHandler)responseHandler;

@end


/// Delegate necessary to facilitate accessing properties of Flipper protobuf data.
@protocol MSWLoggerProtoDelegate <NSObject>

/// Returns the properties for a given SQPBClientFlipperSealedTicket data blob.
- (void)propertiesForTicketData:(NSData *)ticketData propertiesBlock:(void (^)(uint64_t creation, uint64_t expiration))propertiesBlock;

/// Sets the tickets expiration.
- (NSData *)setExpiration:(uint64_t)expiration forTicketData:(NSData *)ticketData;

@end

// Callback for LCR's securesession_host to provide the flipper stored ticket used in SS validation requests.
// If there is no ticket stored, this function returns false and leaves ticket/ticket_len alone
void MSWLoggerGetTicketCallback(void *callback_ref, void (*return_func)(uint8_t **ticket_data, size_t *ticket_len, void *return_func_args, bool has_ticket), void *return_func_args);
