//
//  SquareReader.h
//  SquareReader
//
//  Created by Ken Wigginton on 9/29/16.
//  Copyright © 2016 Square. All rights reserved.
//

#pragma mark - Card Reader

#import <SquareReader/CRCardReader.h>
#import <SquareReader/CRDispatch.h>
#import <SquareReader/CRTransport.h>
#import <SquareReader/CRContext.h>

#pragma mark - Value Types

#import <SquareReader/CRPaymentAccount.h>
#import <SquareReader/CRPaymentApplication.h>
#import <SquareReader/CRPaymentCard.h>

#pragma mark - Generic Features

#import <SquareReader/CRFeature.h>
#import <SquareReader/CRCoredumpFeature.h>
#import <SquareReader/CREventlogFeature.h>
#import <SquareReader/CRFirmwareUpdateFeature.h>
#import <SquareReader/CRPaymentFeature.h>
#import <SquareReader/CRPowerFeature.h>
#import <SquareReader/CRReaderFeature.h>
#import <SquareReader/CROmnichannelCommsFeature.h>
#import <SquareReader/CRSystemFeature.h>
#import <SquareReader/CRSystemFeature+CRCardReaderAdditions.h>
#import <SquareReader/CRTamperFeature.h>
#import <SquareReader/CRUserInteractionFeature.h>

#pragma mark - CoreSwipe

#import <SquareReader/CSAnalogInterfaceController.h>
#import <SquareReader/CSAnalogInterface.h>
#import <SquareReader/CSAnalogSignalDecoder.h>
#import <SquareReader/CSAnalogSignalDecoderResult.h>
#import <SquareReader/CSAnalogSignalDecoderResultCarrierDetectStatistics.h>
#import <SquareReader/CSBMC.h>
#import <SquareReader/CSCardReaderController.h>
#import <SquareReader/CSGen2AnalogSignalDecoderResult.h>
#import <SquareReader/CSLocalMaximaAnalogSignalDecoder.h>
#import <SquareReader/CSLog.h>
#import <SquareReader/CSOerstedAnalogSignalDecoder.h>
#import <SquareReader/CSOerstedAnalogSignalDecoderResult.h>
#import <SquareReader/CSPredictiveAnalogSignalDecoder.h>
#import <SquareReader/CSReactiveAnalogSignalDecoder.h>
#import <SquareReader/CSSqLinkAnalogSignalDecoderResult.h>
#import <SquareReader/CSSwipedPaymentCard.h>
#import <SquareReader/CSSwipeEventFilter.h>
#import <SquareReader/CSUtility.h>
#import <SquareReader/NSBundle+CSAdditions.h>

#pragma mark - MinesweeperLogging

#import <SquareReader/MSWLogger.h>

#pragma mark - Digit Entry

#import <SquareReader/DECancelTransactionStrings.h>
#import <SquareReader/DECoordinatorView.h>
#import <SquareReader/DEDigitEntryAccessibilityInstructionsStrings.h>
#import <SquareReader/DEDigitEntryAccessibilitySelectionStrings.h>
#import <SquareReader/DEDigitEntryGlyphs.h>
#import <SquareReader/DEDigitEntryStrings.h>
#import <SquareReader/DEDigitEntryTheme.h>
#import <SquareReader/DEDigitEntryView.h>
#import <SquareReader/DEDigitEntryViewDelegate.h>
#import <SquareReader/DEEnterDigitsStrings.h>
#import <SquareReader/DEIncorrectDigitsStrings.h>

#pragma mark - Additions

#import <SquareReader/NSUUID+LCRAdditions.h>
#import <SquareReader/NSData+LCRAdditions.h>
#import <SquareReader/NSString+SQRCSAdditions.h>
