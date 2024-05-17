// Copyright (c) 2014 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief Cardreader Payment Defines
 *
 * @details This file defines a number of constants related to the EMV
 * specification for payment processing, and card information. In addition to
 * those specification related values, it defines the different payment
 * interactions, card types, and various other payment processing related
 * defines internal to Square's specific handling of card transactions.
 *
 * @author Ken Wigginton <kenw@squareup.com>
 *
 * @copyright 2014 Square, Inc.
 */

/**
 * @addtogroup lcr_payment_defines Payment Definitions
 *
 * @ingroup lcr_payment
 *
 * @brief Definitions for the payment feature.
 * @{
 */

#pragma once

#include <cardreader_shared/crs_payment.h>

/**
 * @brief Default current codes for transactions if not specified.
 *
 * @note `840` corresponds to the ISO 4217:2015 code for `USD` (United States
 * Dollar).
 *
 * @see https://www.six-group.com/en/products-services/financial-information/data-standards.html
 */
#define CR_PAYMENT_DEFAULT_CURRENCY_CODE (840)

/**
 * @brief Bitmask for extracting CVM (Card Verification Method) from the CVM
 * result of a card transaction.
 */
#define CR_CARDHOLDER_VERIFICATION_PERFORMED_MASK (0x3F)

/**
 * @brief Constants related to EMV (Europay, Mastercard, Visa) Technical
 * Fallback.
 *
 * @details Technical Fallback is the mechanism by which secure transaction
 * mechanisms like dip or tap can fall back to unsecure transaction mechanisms
 * like magswipe. For example, a chip card that requires PIN entry may require
 * that dip fail three times before the customer can swipe their card; if the
 * customer were to swipe their card instead, it would fail the transaction.
 */
enum {
    /**
     * @brief Minimum number of ICC (Integrated Circuit Card) (read: chip)
     * failures because Technical Fallback can be used.
     */
    CR_PAYMENT_MAX_ICC_FAILURES = 3,

    /**
     * @brief Timeout value for timer used to detect duplicate magswipe
     * packets; measured in milliseconds.
     *
     * @details If another magswipe packet is received within
     * #CR_PAYMENT_MAGSWIPE_DUPLICATE_TIMEOUT_MS milliseconds from the last
     * magswipe packet, then the packet is treated as a duplicate, and ignored
     * unless the last 4 bytes of the packet, or issue data from the packet
     * are different.
     */
    CR_PAYMENT_MAGSWIPE_DUPLICATE_TIMEOUT_MS = 4 * 1000,

    /**
     * @brief Maximum time between M1 packets in milliseconds.
     */
    CR_PAYMENT_MAGSWIPE_MAX_TIME_BETWEEN_M1_MESSAGES_MS = 150,

    /**
     * @brief Maximum amount of time, in milliseconds to wait for PIN (Personal
     * Identifier Number) entry.
     */
    CR_PAYMENT_PIN_ENTRY_TIMEOUT_MS = 60 * 1000,

    /**
     * @brief Maximum length of a payment timing label.
     *
     * @details Payment timing labels are used by LCR to internally track the
     * timings of different parts of a transction for payment speed profiling,
     * such as the amount of time it takes for the dip transaction.
     */
    CR_PAYMENT_TIMING_LABEL_SIZE = 16,

    /**
     * @brief Maximum amount of timings that can be recorded at a given point
     * in time.
     */
    CR_PAYMENT_MAX_TIMINGS = 16,
};

enum {
    /**
     * @brief Application definition filename max.
     */
    CR_PAYMENT_APP_ADF_NAME_MAX = 16,

    /**
     * @brief Application label name max.
     */
    CR_PAYMENT_APP_LABEL_MAX = 16,

    /**
     * @brief Application preferred name max.
     */
    CR_PAYMENT_APP_PREFNAME_MAX = 16,

    /**
     * @brief Number of digits for the last four of a card's PAN (Primary
     * Account Number).
     */
    CR_PAYMENT_LAST4_LENGTH = 4,

    /**
     * @brief Maximum length for a cardholder's name taken from the Track 1
     * data.
     */
    CR_PAYMENT_NAME_MAX_LENGTH = 26,

    /**
     * @brief Specifies the maximum prefix length of an issuer identification
     * number in a card PAN (Primary Account Number).
     */
    CR_PAYMENT_PAN_IIN_PREFIX_LENGTH = 8,

    /**
     * @brief Maximum length for the account type, it is list of 1 byte account
     * types.
     */
    CR_PAYMENT_ACCOUNT_TYPE_MAX = 6,
};

/**
 * @brief Enumeration of the Standard CVM (Card Verification Method) results.
 *
 * @details This is encoded according to byte 1 of EMVCo Book 4, Annex A4. See
 * also Book 3 Annex C3 Table 39.
 */
typedef enum {
    /**
     * @brief The CVM failed.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_FAILED = 0x00,

    /**
     * @brief The ICC performed a plaintext PIN verification offline.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_OFFLINE_PLAINTEXT_PIN = 0x01,

    /**
     * @brief An enciphered PIN was verified online.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_ONLINE_ENCIPHERED_PIN = 0x02,

    /**
     * @brief The ICC performed a plaintext PIN verification offline and is
     * requesting signature.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_OFFLINE_PLAINTEXT_PIN_AND_SIGNATURE = 0x03,

    /**
     * @brief The ICC performed an enciphered PIN verification offline.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_OFFLINE_ENCIPHERED_PIN = 0x04,

    /**
     * @brief The ICC performed an enciphered PIN verification offline and is
     * requesting signature.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_OFFLINE_ENCIPHERED_PIN_AND_SIGNATURE = 0x05,

    /**
     * @brief The CVM was performed on the contactless device. (e.g. ApplePay
     * thumbprint or AndroidPay passcode).
     *
     * @note This value is arbitrarily chosen in the range of CVM result codes
     * allocated for application use.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_ON_DEVICE = 0x2A,

    /**
     * @brief The card is requesting a signature.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_SIGNATURE = 0x1E,

    /**
     * @brief No CVM necessary.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_NONE = 0x1F,

    /**
     * @brief Unknown CVM performed.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_UNKNOWN = 0x3F,
} cr_cardholder_verification_performed_t;

enum {
    /**
     * @brief Unused.
     *
     * @details This bit is set on CVM results tag `9F34` if the terminal
     * should apply succeeding CV Rule if this CVM is unsuccessful. This bit
     * should be stripped when parsing `9F34` results to populate CVM using
     * #cr_cardholder_verification_performed_t.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_FLAG_CVM_FALLTHROUGH = 1 << 6,

    /**
     * @brief Unused.
     *
     * @details This bit is reserved for use, but unspecified. As such it should
     * be stripped out of the CVM results for population of CVM using
     * #cr_cardholder_verification_performed_t.
     */
    CR_CARDHOLDER_VERIFICATION_PERFORMED_FLAG_RFU_BIT8 = 1 << 7,
};

/**
 * @brief Bitmasks for indicating which track data is present in a transaction.
 */
typedef enum {
    /**
     * @brief No track data is present if no bits are set in the bitmask, which
     * this enumeration corresponds to.
     */
    CR_MAGSWIPE_TRACK_TYPE_BITMASK_NONE = 0,

    /**
     * @brief Track 1 data is present.
     */
    CR_MAGSWIPE_TRACK_TYPE_BITMASK_ONE = 1 << 0,

    /**
     * @brief Track 2 data is present.
     */
    CR_MAGSWIPE_TRACK_TYPE_BITMASK_TWO = 1 << 1,

    /**
     * @brief Unused.
     */
    CR_MAGSWIPE_TRACK_TYPE_BITMASK_THREE = 1 << 2,
} cr_magswipe_track_type_bitmask_t;

/**
 * @brief Definition of an EMV application.
 *
 * @details The first step in the EMV transaction flow is Application
 * selection. An EMV card may have several applications: e.g. Credit, ATM,
 * Loyalty, etc. Each reader may have a list of support applications of its own
 * as well.
 */
typedef struct cr_payment_application_t {
    /**
     * @brief Application filename.
     *
     * @note Should be displayed on the receipt.
     */
    uint8_t application_definition_filename[CR_PAYMENT_APP_ADF_NAME_MAX];

    /**
     * @brief Length of #cr_payment_application_t::application_definition_filename
     * in bytes.
     */
    size_t application_definition_filename_length;

    /**
     * @brief Label shown to the user when a selection of application is
     * required. This is also the name displayed on receipts.
     */
    char application_label[CR_PAYMENT_APP_LABEL_MAX + 1];

    /**
     * @brief The preferred name of the application.
     */
    char application_prefname[CR_PAYMENT_APP_PREFNAME_MAX + 1];

    /**
     * @brief Index for lookup in the transaction code table.
     */
    uint32_t icodetableidx;
} cr_payment_application_t;

/**
 * @brief Definition for a payment timing record.
 */
typedef struct {
    /**
     * @brief Name of the timing being recorded.
     */
    char label[CR_PAYMENT_TIMING_LABEL_SIZE];

    /**
     * @brief Amount of time, in milliseconds, leading up to the given timing.
     */
    uint32_t delta_ms;
} cr_payment_timing_t;

/**
 * @brief Card information for the card used during a transaction.
 */
typedef struct cr_card_info_t {
    /**
     * @brief The issuing brand of the card.
     */
    crs_payment_issuer_id_t issuer_id;

    /**
     * @brief The last 4 digits of the PAN (Primary Account Number).
     *
     * @note This is a `NULL` terminated string.
     */
    char last4[CR_PAYMENT_LAST4_LENGTH + 1];

    /**
     * @brief The cardholder name.
     *
     * @note This is a `NULL` terminated string.
     */
    char name[CR_PAYMENT_NAME_MAX_LENGTH + 1];

    /**
     * @brief The EMV card's chosen application. To be ignored in the case of a
     * magswipe transaction.
     */
    cr_payment_application_t application;

    /**
     * @brief Bitmask that specifies what tracks were read successfully.
     */
    cr_magswipe_track_type_bitmask_t magswipe_track_type;

    /**
     * @brief CVM code for the CVM performed and/or negotiated.
     */
    cr_cardholder_verification_performed_t cvm_performed;

    /**
     * @brief First 6 or 8 digits of PAN is the IIN, otherwise known as BIN.
     *
     * @note This is a `NULL` terminated string.
     */
    char issuer_identification_number[CR_PAYMENT_PAN_IIN_PREFIX_LENGTH + 1];

    /**
     * @brief Expiry year of card in `YY`.
     */
    uint8_t card_expiry_year;

    /**
     * @brief Expiry date of card in `MM` (`01 - 12`).
     */
    uint8_t card_expiry_month;

    /**
     * @brief The issuing bank of the card if recognized. This differs from
     * `issuer_id` which reflects the issuing brand (ex. AMEX, VISA).
     */
    crs_payment_bank_id_t bank_id;
} cr_card_info_t;

/**
 * @brief Enumeration of the different card presence statuses.
 */
typedef enum {
    /**
     * @brief Initial card presence state. State of contact card is not known.
     */
    CR_PAYMENT_CARD_PRESENCE_UNKNOWN = 0,

    /**
     * @brief Contact card was removed from dip slot, or there is no card
     * present in the dip slot.
     */
    CR_PAYMENT_CARD_PRESENCE_NOT_PRESENT,

    /**
     * @brief Contact card is present; there is currently a card in the dip slot.
     */
    CR_PAYMENT_CARD_PRESENCE_PRESENT,
} cr_payment_card_presence_t;

/**
 * @brief Enumeration of the different reader types. Used as an identifier for
 * which reader is being used. New reader types should be added to the end of
 * this list.
 *
 * @note For each product defined here, you can generally access their
 * documentation at `go/<product>prd`; for example,
 * `CR_CARDREADER_READER_TYPE_R6 -> go/r6prd`.
 */
typedef enum {
    /**
     * @brief Reader has not yet been identified.
     */
    CR_CARDREADER_READER_TYPE_UNKNOWN,

    /**
     * @brief Non-encrypted magstripe audio jack reader.
     *
     * @deprecated This product is no longer manufactured or sold.
     */
    CR_CARDREADER_READER_TYPE_GEN2,

    /**
     * @brief Encrypted magstripe audio jack reader.
     *
     * @note THis was the first mass-produced Square reader, and the precursor
     * to R4.
     *
     * @note The `O` in `O1` stands for `Ørsted`.
     *
     * @deprecated This product is no longer manufactured or sold.
     */
    CR_CARDREADER_READER_TYPE_O1,

    /**
     * @brief Square Magstripe Reader.
     *
     * @details This product is a magswipe only device that plugs into either
     * the lightning port, or headphone jack of a device.
     *
     * @see https://wiki.sqprod.co/display/roadmap/R4+Magstripe+Reader
     */
    CR_CARDREADER_READER_TYPE_R4,

    /**
     * @brief Square Chip Card Reader.
     *
     * @details This product is a magswipe and chip card reader that plugs into
     * the headphone jack of a device.
     *
     * @see https://docs.google.com/document/d/1bC2o8sJ5lw0MGOqunVTI0oWBKU07dUJFJnct1MG2Ebg/
     *
     * @deprecated This product is no longer manufactured or sold as of 2019.
     */
    CR_CARDREADER_READER_TYPE_R6,

    /**
     * @brief Square Contactless and Chip Card Reader Generation 1.
     *
     * @details This product features a K21F, K400, and CC2640.
     *
     * @see https://docs.google.com/document/d/16pOXzbJM1-Us_AyD38jcVpKmCq7kmiPjHsBs0e8_izQ/
     */
    CR_CARDREADER_READER_TYPE_R12,

    /**
     * @brief Muira reader.
     *
     * @details Before Square manufactured in-home devices with a buyer-facing
     * display, the Muira reader was a 3PP off-the-shelf device made available
     * for merchant use cases where a pin-pad and buyer-facing display were
     * required.
     *
     * @see https://drive.google.com/file/d/0B88JOgf2RmUNMHJxTHpvT3FoNVotMTFvTHIxZ3lNd2Z5VHpR/view
     *
     * @deprecated This product is no longer supported by Square.
     */
    CR_CARDREADER_READER_TYPE_A10,

    /**
     * @brief Square Register.
     *
     * @details This product is a SQUID (Square Android) based device that
     * consists of a seller-facing display (Hodor), and buyer-facing display
     * (Bran). The SPE (Secure Payment Enclave) is a part of the buyer-facing
     * display, and contains a K21F, K400, and STM32F (touch controller).
     *
     * @see https://docs.google.com/document/d/1wZ7BAmzwivK9f2j_--lJ8v09xcysxE2_8qtgsBOuFBg/
     */
    CR_CARDREADER_READER_TYPE_X2,

    /**
     * @brief Square Contactless and Chip Card Reader Generation 1.
     *
     * @details This product features a K450, and CC2640.
     *
     * @see https://docs.google.com/document/d/12sO84HOEr2n9OVrE61_T8GxxxisDq4puImp7ZCsoX88/
     */
    CR_CARDREADER_READER_TYPE_R12C,

    /**
     * @brief Square Terminal.
     *
     * @details This product is a SQUID (Square Android) based handheld
     * terminal device with a display. The SPE is a part of the main device,
     * and consists of a K21F, K400, and STM32F (touch controller).
     *
     * @see https://docs.google.com/document/d/1X3V-J_w1DUUPRfABemu4iA3Z1Ab6Ecy0FaqQhlXpMWc/
     */
    CR_CARDREADER_READER_TYPE_T2,

    /**
     * @brief Square Register.
     *
     * @details This product is a SQUID (Square Android) based device that
     * consists of a seller-facing display (Hodor), and buyer-facing display
     * (Bran). The SPE (Secure Payment Enclave) is a part of the buyer-facing
     * display, and contains a K450, and STM32F (touch controller).
     *
     * @see https://docs.google.com/document/d/1J_CfHRyHnI4xJwDd800qM1tIitFHUQOcQMWeYZMPxFE/
     */
    CR_CARDREADER_READER_TYPE_X2B,

    /**
     * @brief Square Terminal.
     *
     * @details This product is a SQUID (Square Android) based handheld
     * terminal device with a display. The SPE is a part of the main device,
     * and consists of a K450, and STM32F (touch controller).
     *
     * @see https://docs.google.com/document/d/1Z1j_5VTRcSpi6eZN9_WW13Epd-soFxDoWDrk5G0YCTM/
     */
    CR_CARDREADER_READER_TYPE_T2B,

    /**
     * @brief Square Stand Second Generation.
     *
     * @details This product is part of Square's BYOD (Bring Your Own Device)
     * offering. The stand is used with an Apple iPad, which sits in the top of
     * the device and runs Square POS. The Stand itself provides Magstripe,
     * Chip, and Contactless payment methods. The chip and contactless payment
     * methods are available through an embedded card reader connected to the
     * rest of the stand thorugh an internal USB.
     *
     * @see https://docs.google.com/document/d/12xBgo4WEh56VuDEtwZIrtgRU08AWq5uj1h8J2r7wCJc/
     */
    CR_CARDREADER_READER_TYPE_S3,

    /**
     * @brief Prototype Square Contactless and Chip Card Reader Second
     * Generation.
     *
     * @deprecated *PLEASE READ*: This definition was used during bring-up, and
     * should not be used by any production code or products.
     */
    CR_CARDREADER_READER_TYPE_R12D_K450,

    /**
     * @brief Square Contactless and Chip Card Reader Second Generation.
     *
     * @details This product is the second generation of the Square Contactless
     * and Chip Card Reader that brings new chips to the product, as well as an
     * USB-C interface. This product features a K480, and EFR32.
     *
     * @see https://docs.google.com/document/d/1yLfw_OOkCU52-jfi4-LWMI_EkVeV6zb8-Y5P03YRHNQ/
     */
    CR_CARDREADER_READER_TYPE_R12D,

    /**
     * @brief Square Terminal.
     *
     * @details This product is a SQUID (Square Android) based handheld
     * terminal device with a display. The SPE is a part of the main device,
     * and consists of a K480, and TBD (touch controller).
     *
     * @see https://docs.google.com/document/d/1dz1SfzB7LjOIlAcSSgWDBYKE4EYg93eI7x-fOkWz-i4/
     */
    CR_CARDREADER_READER_TYPE_T3A,

    /**
     * @brief Embedded Chip Card Reader.
     *
     * @details This is the identifier for on-device mobile contactless chip
     * card reader.
     */
    CR_CARDREADER_READER_TYPE_ECR
} cr_cardreader_reader_type_t;

/**
 * @brief Enumeration of the different card action results that are surfaced to
 * the mobile application indicating what action the customer should take.
 *
 * @details This enumeration contains a set of actions that must be
 * communicated to the customer who is attempting to pay. Only
 * #CR_PAYMENT_CARD_ACTION_NONE can be treated as no action required.
 */
typedef enum {
    /**
     * @brief No card action required.
     */
    CR_PAYMENT_CARD_ACTION_NONE = 0,

    /**
     * @brief A card needs to be inserted into the reader. Happens when an ICC
     * enabled card is swiped or cr_payment_start_payment() is called when no
     * card is present.
     */
    CR_PAYMENT_CARD_ACTION_INSERT,

    /**
     * @brief A card error has been detected. Card needs to be removed then
     * re-inserted.
     */
    CR_PAYMENT_CARD_ACTION_INSERT_AGAIN,

    /**
     * @brief The card does not have an ICC or the ICC cannot be powered on.
     * Card needs to be swiped.
     */
    CR_PAYMENT_CARD_ACTION_SWIPE_TECHNICAL,

    /**
     * @brief The card has an ICC which isn't currently supported, but magswipe
     * is allowed. Card needs to be swiped.
     */
    CR_PAYMENT_CARD_ACTION_SWIPE_SCHEME,

    /**
     * @brief The card swipe failed. The card needs to be reswiped.
     */
    CR_PAYMENT_CARD_ACTION_SWIPE_AGAIN,

    /**
     * @brief A contactless card needs to be inserted into the reader.
     *
     * @note This happens when a contactless card with chip requests that the
     * chip be used.
     */
    CR_PAYMENT_CARD_ACTION_INSERT_FROM_CONTACTLESS,

    /**
     * @brief The contactless portion of the presented card cannot be used.
     * Swipe, insert or try another card contactlessly.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_CARD_ERROR_TRY_ANOTHER_CARD,

    /**
     * @brief A transient error was experienced with the presented contactless
     * card, such as removing it too quickly from the field. Try presenting it
     * again.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_CARD_ERROR_TRY_AGAIN,

    /**
     * @brief A phone was presented as a contactless payment instrument. The
     * buyer's attention should be directed towards their phone which will
     * provide them with further instructions.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_SEE_PHONE_FOR_INSTRUCTION,

    /**
     * @brief Multiple contactless cards were presented. Only one of those
     * cards should be presented.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_PRESENT_ONLY_ONE,

    /**
     * @brief A locked Android Pay phone was presented as a contactless payment
     * instrument. The buyer's attention should be directed towards their phone
     * to unlock it so the payment can happen.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_UNLOCK_PHONE_TO_PAY,

    /**
     * @brief The contactless portion of the presented card cannot be used
     * because the transaction or cumulative limit was exceeded. Try another
     * card.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_CARD_LIMIT_EXCEEDED_ERROR_TRY_ANOTHER_CARD,

    /**
     * @brief The contactless portion of the presented card cannot be used
     * because the transaction or cumulative limit was exceeded. Swipe or
     * insert card.
     */
    CR_PAYMENT_CARD_ACTION_CONTACTLESS_CARD_LIMIT_EXCEEDED_INSERT_CARD,

    /**
     * @brief If an Interac card is dipped and the transaction is under $100,
     * then prompt the buyer to tap card instead since we do not support
     * Interac dip or swipe.
     */
    CR_PAYMENT_CARD_ACTION_REQUEST_TAP,

    /**
     * @brief A contactless card is requested to be presented again in response
     * to an online issuer auth response (ex. issuer scripts, issuer-initiated
     * restart).
     */
    CR_PAYMENT_CARD_ACTION_ISSUER_REQUESTED_PRESENT_CARD_AGAIN,
} cr_payment_card_action_t;

/**
 * @brief Enumeration of the different card entry methods for which a reader
 * can initiate a new transaction.
 */
typedef enum {
    /**
     * @brief Contact (chip) transaction type.
     */
    CR_PAYMENT_CARD_ENTRY_TYPE_CONTACT = 0,

    /**
     * @brief Contactless (tap) transaction type.
     */
    CR_PAYMENT_CARD_ENTRY_TYPE_CONTACTLESS = 1,

    /**
     * @brief Swipe (magswipe) transaction type.
     */
    CR_PAYMENT_CARD_ENTRY_TYPE_SWIPE = 2,
} cr_payment_card_entry_type_t;

/**
 * @brief Source of the card data for a magswipe payment.
 *
 * @details This enumeration defines the payment source for when a swipe
 * transaction is being performed.
 */
typedef enum {
    /**
     * @brief Unspecified track.
     */
    CR_PAYMENT_SRC_DEFAULT = -1,

    /**
     * @brief Data is from MSR (Magnetic Stripe Reader) Track 0.
     */
    CR_PAYMENT_SRC_MSR_0 = 0,

    /**
     * @brief Data is from MSR (Magnetic Stripe Reader) Track 1.
     */
    CR_PAYMENT_SRC_MSR_1 = 1,

    /**
     * @brief Count of the number of payment sources.
     */
    CR_PAYMENT_SRC_COUNT = 2,
} cr_payment_source_t;

/** @} */
