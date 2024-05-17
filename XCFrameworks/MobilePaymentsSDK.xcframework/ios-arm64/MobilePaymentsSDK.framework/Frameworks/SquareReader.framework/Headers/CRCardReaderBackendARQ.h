//
//  CRCardReaderBackendARQ.h
//  SquareReader
//
//  Created by Zachary Drayer on 2/24/20.
//

#import <Foundation/Foundation.h>
#import <SquareReader/CRCardReaderBackend.h>

@protocol CRARQTransportDelegate;


#pragma mark - CRARQTransport


/// Transport for readers using ARQ comms. Communicates with a CRARQTransportDelegate.
@protocol CRARQTransport <CRTransportBase>

@property (nonatomic, weak) id<CRARQTransportDelegate> delegate;

/**
 Send bluetooth data to the underlying hardware.
 @param data The raw data to send to the ARQ interface.
 */
- (void)sendARQData:(NSData *)data;

@end


#pragma mark - CRARQTransportDelegate


/// Delegate of CRARQTransport. Should handle passing ARQ data back to LCR.
@protocol CRARQTransportDelegate <NSObject>

/// consumeReceivedARQData is invoked when data is received from the hardware.
- (BOOL)consumeReceivedARQData:(NSData *)data;

@end


#pragma mark - CRCardReaderBackendARQ


@interface CRCardReaderBackendARQ : CRCardReaderBackend <CRARQTransportDelegate>

@property (nonatomic, strong) id<CRARQTransport> transport;

@end
