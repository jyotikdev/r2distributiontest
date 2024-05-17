//
//  CREnums.h
//  SquareReader
//
//  Created by Ken Wigginton on 1/14/17.
//  Copyright (c) 2017 Square, Inc. All rights reserved.
//

#import <cardreader/cr_firmware_update_feature.h>
#import <cardreader/cr_payment_defines.h>
#import <cardreader/cr_payment_feature.h>
#import <cardreader/cr_tamper_feature.h>

#import <SquareReader/SquareReader.h>


CRTamperFeatureStatus CRTamperFeatureStatusFromCType(cr_tamper_status_t);
CRPaymentFeatureCardPresence CRPaymentFeatureCardPresenceFromCType(cr_payment_card_presence_t);
CRPaymentFeatureCardEntryType CRPaymentFeatureCardEntryTypeFromCType(cr_payment_card_entry_type_t);
CRPaymentCardVerificationMethod CRPaymentCardVerificationMethodFromCType(cr_cardholder_verification_performed_t);
CRPaymentCardBrand CRPaymentCardBrandFromCType(crs_payment_issuer_id_t);
CRBankBrand CRBankBrandFromCType(crs_payment_bank_id_t issuer);
CRPaymentFeatureStandardMessage CRPaymentFeatureStandardMessageFromCType(cr_payment_std_msg_t);
CRFirmwareUpdateReaderFailureResult CRFirmwareUpdateReaderFailureResultFromCType(crs_fwup_result_t);
CRCardReaderReaderType CRCardReaderReaderTypeFromCType(cr_cardreader_reader_type_t);
CRCardReaderReaderSubtype CRCardReaderReaderSubtypeFromCType(cr_cardreader_reader_type_t readerType);
CRPaymentFeatureLoyaltyResult CRPaymentFeatureLoyaltyResultFromCType(cr_payment_loyalty_result_t loyaltyResult);

cr_cardreader_reader_type_t CRCardReaderReaderTypeToCType(CRCardReaderReaderType readerType, CRCardReaderReaderSubtype readerSubtype);
cr_payment_card_entry_type_t CRPaymentFeatureCardEntryTypeToCType(CRPaymentFeatureCardEntryType);
