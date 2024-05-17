//
//  CRFirmwareUpdateFeature.h
//  cardreader
//
//  Created by Peter Mattis on 10/26/14.
//  Copyright (c) 2014 Square, Inc. All rights reserved.
//

#import <SquareReader/CRFeature.h>

@class CRCardReader;
@class CRFirmwareUpdateFeature;

/*
Enum value translated from crs_fwup_result_t type to determine the cause and solution of a firmware update failure
Used to display proper firmware update failure modal text and actions

Generic means the error has a known enum value and it isn't a WrongServerEnvironment error
WrongServerEnvironment means a production reader tried to apply a staging firmware update
Unknown means we got a value that is not defined in the enum
*/
typedef NS_ENUM(NSUInteger, CRFirmwareUpdateReaderFailureResult) {
    CRFirmwareUpdateReaderFailureResultUnknown,
    CRFirmwareUpdateReaderFailureResultGeneric,
    CRFirmwareUpdateReaderFailureResultWrongServerEnvironment,
    CRFirmwareUpdateReaderFailureResultPartialSuccess
};


@interface CRFirmwareUpdateFeatureFileVersion : NSObject

@property (nonatomic, copy, readonly) NSString *filename;
@property (nonatomic, copy, readonly) NSString *version;

+ (instancetype)fileVersionWithFilename:(NSString *)filename version:(NSString *)version;

- (instancetype)initWithFilename:(NSString *)filename version:(NSString *)version;

@end

/**
 Firmware version type enumeration that covers the types of firmware assets that various Square readers install and utilize.
 
 Not a one to one mapping with chips on a reader.
 */
typedef NS_ENUM(NSUInteger, CRFirmwareUpdateVersionType) {
    CRFirmwareUpdateVersionTypeUnknown = 0,
    CRFirmwareUpdateVersionTypeK21,
    CRFirmwareUpdateVersionTypeK400CPU0,
    CRFirmwareUpdateVersionTypeK400CPU1,
    CRFirmwareUpdateVersionTypeBLE,
    CRFirmwareUpdateVersionTypeTMS,
    CRFirmwareUpdateVersionTypeCAPKS,
    CRFirmwareUpdateVersionTypeTMSCAPKS,
    CRFirmwareUpdateVersionTypeFPGA,
    CRFirmwareUpdateVersionTypeK450CPU0,
    CRFirmwareUpdateVersionTypeK450CPU1,
    CRFirmwareUpdateVersionTypeEFR32Firmware,
    CRFirmwareUpdateVersionTypeEFR32Bootloader,
    CRFirmwareUpdateVersionTypeNFCConfig
};

/**
 Contains a version and type pairing which describes the version of an individual asset on a Square reader.
 */
@interface CRFirmwareUpdateVersionInfoPair : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype) new NS_UNAVAILABLE;

/// Used for tracking underlying firmware info version type, the result will always represent the internal component being updated
@property (nonatomic, copy, readonly) NSNumber *opaqueVersionType;
/// Mapping of underlying component to a known component, the result of the mapping may be unknown if the component is not identified at compile time
@property (nonatomic, assign, readonly) CRFirmwareUpdateVersionType versionType;
@property (nonatomic, copy, readonly) NSString *version;

@end


@protocol CRFirmwareUpdateDelegate <NSObject>

- (void)firmwareManifestReceived:(NSData *)manifestData updateRequired:(BOOL)required;

/**
 Called when the reader's current CPU and asset versions are ready.
 Used for PCI-PTS compliance to display correct revision and asset versions.

 @param versionInformation An array of asset and version pairings to describe the version of each installed asset on the connected reader.
 */
- (void)firmwareUpdate:(CRFirmwareUpdateFeature *)firmwareUpdateFeature receivedVersionInformation:(NSArray<CRFirmwareUpdateVersionInfoPair *> *)versionInformation;

- (void)firmwareUpdateProgressReceived:(float)progress;

/**
 Called when the firmware update has failed.
 @param readerReason A string representing the reason for the failure. May be nil.
 */
- (void)firmwareUpdateFailedWithReaderReason:(NSString *)readerReason readerFailureResult:(CRFirmwareUpdateReaderFailureResult)readerFailureResult;
- (void)firmwareUpdateSucceeded;
- (void)firmwareUpdateDeviceDidReset;
- (void)firmwareUpdate:(CRFirmwareUpdateFeature *)firmwareUpdateFeature receivedInvalidVersionInfo:(NSDictionary *)versionInfo;

- (void)firmwareVersionReceived:(NSString *)version;


@end // CRFirmwareUpdateDelegate


@interface CRFirmwareUpdateFeature : CRFeature

@property (nonatomic, weak) id<CRFirmwareUpdateDelegate> delegate;

- (instancetype)initWithCardReader:(CRCardReader *)cardReader;

/// Request the firmware manifest.
/// [CRFirmwareUpdateDelegate firmwareManifestReceived]
/// will be invoked on the delegate queue.
- (BOOL)requestManifest;

/// Send a firmware image to the reader. The data is copied internally and trickled to the
/// reader. [CRFirmwareUpdateDelegate firmwareUpdateProgressReceived] is invoked on the delegate
/// queue periodically as the data is sent with the fraction complete (a value in the range
/// [0,1]). [CRFirmwareUpdateDelegate firmwareUpdateSucceeded] is invoked on the delegate queue
/// upon successful upload and [CRFirmwareUpdateDelegate firmwareUpdateFailed] is invoked on the
/// delegate queue upon failure.
///
/// The contents of header and data are intended to be opaque for Register. Behind the scenes,
/// header must be a squareup_spe_k21_AssetHeaderMessage structure and data is a signed
/// firmware image.
- (BOOL)sendFirmwareData:(NSData *)data withHeader:(NSData *)header;

- (BOOL)sendFirmwareData:(NSData *)data withHeader:(NSData *)header withBlockIndexTable:(NSData *)table;

@end // CRFirmwareUpdateFeature
