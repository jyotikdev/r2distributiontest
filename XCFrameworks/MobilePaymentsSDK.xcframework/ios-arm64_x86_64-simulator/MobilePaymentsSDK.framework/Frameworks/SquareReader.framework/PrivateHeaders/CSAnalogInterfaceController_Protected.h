//
//  CSAnalogInterfaceController_Protected.h
//  CoreSwipe
//
//  Created by Alan Fineberg on 6/4/13.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR


#import <SquareReader/CSAnalogInterfaceController.h>


@interface CSAnalogInterfaceController ()

@property (nonatomic, strong, readwrite) CSAnalogInterface *analogInterface;
@property (nonatomic, assign, getter=isInputQueueRunning) BOOL inputQueueRunning;
@property (nonatomic, assign, getter=isOutputQueueRunning) BOOL outputQueueRunning;

- (void)_tryToStartInterfaceAndInputQueueDueToRecentAttachment:(BOOL)wasRecentlyAttached;
- (void)_tryToStartInterfaceAndInputQueueIfNeededWithIsDueToRecentAttachment:(BOOL)wasRecentlyAttached;

- (void)_tryToStartOutputQueue;
- (BOOL)_isConnectedToUSBAudioOutputRoute;

/// The operation queue that the Analog Interface Controller will perform thread-unsafe operations on.
/// Defaults to the main thread. Should always be the main thread in production (AVAudioSession is likely not thread safe).
@property (nonatomic, strong, readwrite) NSOperationQueue *operationQueue;

@end


#endif
