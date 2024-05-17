// Copyright (c) 2014 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief Cardreader Payment Features
 *
 * @details This file defines a number of constants related to the EMV
 * specification for payment processing, and card information. In addition to
 * those specification related values, it defines the different payment
 * interactions, card types, and various other payment processing related
 * defines internal to Square's specific handling of card transactions.
 *
 *
 * @copyright 2014 Square, Inc.
 */

#pragma once

#include <cardreader/cr_payment_defines.h>

#include <cardreader_shared/crs_log.h>
#include <cardreader_shared/crs_codetalker_msgs.h>
#include <cardreader_shared/crs_magswipe_msgs.h>
#include <cardreader_shared/crs_emv_notification.h>
#include <cardreader_shared/crs_payment_defs.h>
#include <cardreader_shared/crs_tmn_msgs.h>

#include <squareup/spe/tmn.pb.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup lcr_payment Payment Feature
 *
 * @ingroup lcr
 *
 * @brief Interface for the payment feature.
 *
 * ================================================================================================
 *
 * Overall Usage and Flow
 * ======================
 *  For functions that return a cr_payment_result_t, it is advised to always ensure that the
 *   result is equal to `CR_PAYMENT_RESULT_SUCCESS`.
 *
 *  Allocation and initialization
 *  First step will always be to allocate and initialize:
 *   1. Allocate payment object `payment = cr_payment_alloc()`
 *   2. Initialize the object `result = cr_payment_init(payment, cardreader, ...)`
 *  After this, we must wait for reader ready before any events will arrive.
 *  Any number of transactions may be performed before termination and free of the payment object.
 *
 *  Magswipe Behavior
 *  =================
 *  Securesession establishment is required for EMV payments, however, magswipe packets may still
 *  be processed if the card does not have an ICC. In this case the `on_payment_complete`
 *  callback will be called with result `CR_PAYMENT_PAYMENT_RESULT_MAGSWIPE`.
 *
 *  If a bad swipe is received or if the card swiped has an ICC the `on_card_action_required`
 *  will be called with action `CR_PAYMENT_CARD_ACTION_SWIPE_AGAIN` or
 *  `CR_PAYMENT_CARD_ACTION_INSERT` respectively.
 *
 *  A call to `cr_payment_start_payment` is not required for pure magswipe payments. However,
 *  such a call would not result in any error. Pure magswipe payments are unique in that they
 *  can result in an `on_payment_complete` without a matching `cr_payment_start_payment`.
 *
 *  One exception, is when `cr_payment_enable_swipe_passthrough` is called. This enables passthrough
 *  of all swipes via the `on_swipe_passthrough` callback, irrespective of current payment state and
 *  ICC presence.
 *
 *  E-Money TMN Payment
 *  =================
 *  Securesession must be established for E-Money TMN payments.
 *  `cr_payment_tmn_start_transaction` can be called at any time after initialization.
 *
 *  General payment flow is as follows:
 *   1. `cr_payment_tmn_start_transaction` is called to start a transaction instead of (not in
 *     addition to) `cr_payment_start_payment`. For a payment this is called with `transaction_type`
 *     of TMN_REQUEST_TRANSACTION, along with `transaction_id`, `brand_id`, and `amount`.
 *
 *   2. During TMN payments data will be passed between Reader and TMN server using. Reader will call
 *     `on_tmn_data` with data to be sent to TMN Connection, and TMN connection data should be sent
 *     to reader using `cr_payment_tmn_send_bytes_to_reader`.
 *
 *   3. TMN transaction completion will be indicated by `on_tmn_transaction_complete` which will be
 *     called with an result error code and message.
 *
 *  EMV Payment
 *  ===========
 *  Securesession must be established for EMV payments.
 *  `cr_payment_start_payment` can be called at any time after initialization.
 *  The `on_card_presence_changed` callback is intended to inform when a card is present
 *  indicating that the terminal is ready for the payment to start.
 *  If payment is ready but card state is unkown the `cr_payment_request_card_presence` may be used.
 *
 *  General payment flow is as follows:
 *   1. `cr_payment_start_payment` is called with amount, account, and time information.
 *     This can be prompted by `on_card_presence_changed` or user interaction with register.
 *     (See Technical Fallback section for additional behavior)
 *
 *   2. [Optional] If multiple applications are mutually supported by the terminal and the card, the
 *     `on_application_selection_required` callback will be called with the available applications.
 *
 *     The list must be presented to the user and the subsequent selection must be sent to
 *     the reader via `cr_payment_select_application`.
 *
 *     The `application_definition_filename` is used as the unique identifier when selecting an
 *     application. The application label is used for display purposes only.
 *
 *   3. [Optional] If account type selection is required by the brand, the reader will also send up
 *     the list of account types. The `on_account_type_selection_required` callback will be called
 *     with the available account types and language preferences.
 *
 *     Similar to application selection, the list must be presented to the user and the subsequent
 *     selection must be sent to the reader via `cr_emv_flow_select_account_type`.
 *
 *   4. [Optional] For most transactions an online authorization will be required.
 *     However, exceptions are offline decline and transaction error (offline approval is not
 *     supported at this time). In these cases this step will be skipped.
 *
 *     For online authorizations the `on_emv_authorization_request` or `on_contactless_emv_authorization_request`
 *     (depending on type of payment) callback will be called with data to be sent to the payment server
 *     for authorization. The server response must then be sent to the reader via
 *     `cr_payment_process_authorization_response`, even in the case where the server indicates a decline.
 *
 *   5. `on_payment_complete` callback will always be called at the end of a transaction,
 *     for both error and success cases.
 *
 *  Record API transaction
 *  ==================
 *  For Record API requires either a valid secure session or STS enviorment
 *  General payment flow is as follows:
 *
 *
 *  Technical Fallback
 *  ==================
 *  Sometimes a card may be present but the terminal is unable to establish intial communications.
 *  We refer to this generally as "card power failure".
 *
 *  If a card fails to power the `on_card_action_required` callback will be called with action
 *  `CR_PAYMENT_CARD_ACTION_INSERT_AGAIN` and the user must re-insert card to continue.
 *
 *  If three card power failures occur we must fall back to magswipe. In this case, after three
 *  failures, the `on_card_action_required` callback will be called with action
 *  `CR_PAYMENT_CARD_ACTION_SWIPE_TECHNICAL`. At this point we will revert to the
 *  "Magswipe behavior" above with the caveat being the `on_payment_complete` result now equal to
 *  `CR_PAYMENT_PAYMENT_RESULT_MAGSWIPE_TECHNICAL_FALLBACK`.
 *
 *  Terminate and free
 *  ==================
 *  To deallocate the payment object the proper sequence is:
 *   1. Terminate the object `cr_payment_term`.
 *   2. Free memory allocated for the payment object `cr_payment_free`.
 *  A new payment object must be allocated and initialized for further payment processing.
 *
 * ================================================================================================
 * @{
 */

typedef struct cr_payment_t cr_payment_t;

typedef enum
{

    /**
     * @brief Call was completed successfully.
     *
     */
    CR_PAYMENT_RESULT_SUCCESS,
    /**
     * @brief A given parameter to the function was invalid.
     *
     */
    CR_PAYMENT_RESULT_INVALID_PARAMETER,
    /**
     * @brief cr_payment_init must be called before using other functions.
     *
     */
    CR_PAYMENT_RESULT_NOT_INITIALIZED,
    /**
     * @brief Returned by cr_payment_init when initialized was already called.
     *
     */
    CR_PAYMENT_RESULT_ALREADY_INITIALIZED,
    /**
     * @brief cr_payment_init must be called before cr_payment_free.
     *
     */
    CR_PAYMENT_RESULT_NOT_TERMINATED,
    /**
     * @brief Returned by cr_payment_term when terminate was already called.
     *
     */
    CR_PAYMENT_RESULT_ALREADY_TERMINATED,
    /**
     * @brief There was a session error.
     *
     */
    CR_PAYMENT_RESULT_SESSION_ERROR,
    /**
     * @brief Call was unexpected at this time, a violation of the internal state machine.
     *
     */
    CR_PAYMENT_RESULT_CALL_UNEXPECTED,
    /**
     * @brief A fatal, unrecoverable error has occurred, payment should be aborted.
     *
     */
    CR_PAYMENT_RESULT_FATAL,
    /**
     * @brief ADF_NAME given to cr_payment_select_application was invalid (not a given choice).
     *
     */
    CR_PAYMENT_RESULT_BAD_ADF_NAME,
    /**
     * @brief Invalid notification received from k21
     *
     */
    CR_PAYMENT_RESULT_INVALID_NOTIFICATION,
    /**
     * @brief A field was not found where requested or expected. (e.g. Retrieving cardholder name from arqc).
     *
     */
    CR_PAYMENT_RESULT_NOT_FOUND,
    /**
     * @brief Selection account type was not valid.
     *
     */
    CR_PAYMENT_RESULT_INVALID_ACCOUNT_TYPE,
} cr_payment_result_t;

enum
{
    /**
     * @brief maximum number of supported languages
     *
     */
    CR_EMV_FLOW_LANG_PREF_MAX = 4,
};

enum
{
    /**
     * @brief merchant ID maximum length for ndef api
     *
     */
    CR_PAYMENT_NDEF_MERCHANT_ID_MAX_LENGTH = 16,
    /**
     * @brief maximum card ID length
     *
     */
    CR_PAYMENT_NDEF_CARD_ID_MAX_LENGTH = 80,
};

enum
{
    /**
     * @brief uid max length
     *
     */
    CR_PAYMENT_RECORD_UID_MAX_LENGTH = 10,
    /**
     * @brief merchant id minimum length
     *
     */
    CR_PAYMENT_RECORD_MERCHANT_ID_MIN_LENGTH = 13,
    /**
     * @brief merchant id maximum length
     *
     */
    CR_PAYMENT_RECORD_MERCHANT_ID_MAX_LENGTH = 32,
    /**
     * @brief maximum record size
     *
     */
    CR_PAYMENT_RECORD_MAX_LENGTH = 255,
};

/**
 * @brief This matches the `crs_emv_l2ctl_app_langpref_t` struct defined in ritual.yaml.
 *
 */
typedef struct cr_payment_language_preference_t
{
    uint8_t chars[2];
} cr_payment_language_preference_t;

/**
 * @brief list of account types for account selection
 *
 */
typedef struct cr_payment_account_type_list_data_t
{
    uint8_t account_type_list[6];
} cr_payment_account_type_list_data_t;

/**
 * @brief list of enum account types for account selection
 *
 */
typedef struct cr_payment_account_type_enum_list_data_t
{
    cr_payment_account_type_t account_type_list[6];
} cr_payment_account_type_enum_list_data_t;

/**
 * @brief merchant loyalty ID for VAS
 *
 */
typedef struct cr_payment_merchant_loyalty_id_t
{
    uint8_t id[32];
} cr_payment_merchant_loyalty_id_t;

/**
 * @brief list of merchant loytality ID
 *
 */
typedef struct cr_payment_merchant_loyalty_id_list_t
{
    cr_payment_merchant_loyalty_id_t ids[3];
    uint8_t count;
} cr_payment_merchant_loyalty_id_list_t;

/**
 * @brief loyality VAS transaction result
 *
 */
typedef enum
{
    CR_PAYMENT_LOYALTY_RESULT_SUCCESS,
    CR_PAYMENT_LOYALTY_RESULT_VAS_DATA_NOT_FOUND,
    CR_PAYMENT_LOYALTY_RESULT_CANCELED,
    CR_PAYMENT_LOYALTY_RESULT_TERMINATED,
    CR_PAYMENT_LOYALTY_RESULT_SKIPPED,
    CR_PAYMENT_LOYALTY_RESULT_URL_PUSH_FAILED
} cr_payment_loyalty_result_t;

/**
 * @brief VAS protocol used
 *
 */
typedef enum
{
    CR_PAYMENT_VAS_PROTOCOL_URL,
    CR_PAYMENT_VAS_PROTOCOL_FULL,
    CR_PAYMENT_VAS_PROTOCOL_NONE
} cr_payment_vas_protocol_t;

/**
 * @brief Merchant ID used in ndef transaction
 * @details in NDEF merchant
 * ID will be stored inside the card data file, it will eventually be checked if the ID doesn't match
 * the request merchant ID, transaction should fail
 *
 */
typedef struct cr_payment_ndef_merchant_id_t
{
    uint8_t id[CR_PAYMENT_NDEF_MERCHANT_ID_MAX_LENGTH];
} cr_payment_ndef_merchant_id_t;

/**
 * @brief Ndef card ID is used to identify specific ndef card
 *
 */
typedef struct cr_payment_ndef_card_id_t
{
    uint8_t id[CR_PAYMENT_NDEF_CARD_ID_MAX_LENGTH];
} cr_payment_ndef_card_id_t;

/**
 * @brief  Record stored inside a desfire card
 * @param storage_type          defines how the data is stored inside the record, encrypted or plaintext
 * @param length                length of the record data
 * @param data                  record data
 *
 */
typedef struct cr_payment_record_t
{
    uint8_t storage_type;
    uint16_t length;
    uint8_t data[CR_PAYMENT_RECORD_MAX_LENGTH];
} cr_payment_record_t;

/**
 * @brief payment transaction mode
 *
 */
typedef enum
{
    CR_PAYMENT_TRANSACTION_MODE_PAYMENT = 0,
    CR_PAYMENT_TRANSACTION_MODE_CARD_INFO = 1,
    CR_PAYMENT_TRANSACTION_MODE_ECP_VAS_OR_PAYMENT = 2,
    CR_PAYMENT_TRANSACTION_MODE_ECP_VAS_AND_PAYMENT = 3,
    CR_PAYMENT_TRANSACTION_MODE_ECP_VAS_ONLY = 4,
    CR_PAYMENT_TRANSACTION_MODE_ECP_PAYMENT_ONLY = 5,
    CR_PAYMENT_TRANSACTION_MODE_ECP_VAS_AND_PAYMENT_SQ = 6,
    CR_PAYMENT_TRANSACTION_MODE_STORE_AND_FORWARD_PAYMENT = 7,
    CR_PAYMENT_TRANSACTION_MODE_NONE = 255,
} cr_payment_transaction_mode_t;


/**
 * @brief Callback for when the state of the card detect switch on the reader has been requested or
 * state has changed due to user interaction (user inserted or removed card).
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param last_card_presence    last known card presence,
 * @param card_presence         updated card presence
 * @param will_continue_payment notify if we need to terminate
 *
 */
typedef void (*cr_payment_card_presence_changed_t)(void *context,
                                                   cr_payment_t *payment,
                                                   cr_payment_card_presence_t last_card_presence,
                                                   cr_payment_card_presence_t card_presence,
                                                   bool will_continue_payment);

/**
 * @brief Callback for when specific user interactions are required.
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param action_requested      is the action desired of the user
 * @param std_msg               is information that shall be displayed to the user.
 *
 * @note  For example:
 * CR_PAYMENT_CARD_ACTION_INSERT -- A card needs to be inserted into the reader. Happens when
 * either an ICC enabled card is swiped or a start_payment is called when no card is present.
 *
 * CR_PAYMENT_CARD_ACTION_INSERT_AGAIN -- A card error has been detected. Card needs to be removed
 * then reinserted.
 * CR_PAYMENT_CARD_ACTION_SWIPE -- The card does not have an ICC or the ICC cannot be powered
 * on. Card needs to be swiped.
 * CR_PAYMENT_CARD_ACTION_SWIPE_AGAIN --  The card swipe failed. The card needs to be reswiped.
 */
typedef void (*cr_payment_card_action_required_t)(void *context,
                                                  cr_payment_t *payment,
                                                  cr_payment_card_action_t action_requested,
                                                  cr_payment_std_msg_t std_msg);

/**
 * @brief Callback to notify that user must select an application from the list provided.
 *
 * @details (Note that application selection is different from account selection which happens before
 * the start of the payment) This is required when the terminal cannot automatically determine which
 * application to use. This occurs when there is more then 1 mutually supported application or when the
 * single application requires explicit selection.
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 * @param apps              is an array of `cr_payment_application_t` values with a length of `apps_len`.
 * @param apps_len          length of apps
 *
 */
typedef void (*cr_payment_application_selection_required_t)(void *context,
                                                            cr_payment_t *payment,
                                                            cr_payment_application_t const *apps,
                                                            size_t apps_len);




/**
 * @brief Callback to notify that the user must select an account type from the list provided.
 * @details (Note that account type selection is different from application selection.
 * And note that there is a concept of an account selection at the start of the payment,
 * which was inherited from R6 -- it might have be implemented for certification purpose.
 * In R12, this start payment account selection is defaulted to 00.
 * The second concept of account type selection from the card which is only used in R12.)
 * This is required for certain brands, such as Interac.
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param account_types         is an array of `cr_payment_account_type_t` values with a length of `account_types_len`.
 * @param account_types_len     length of account_types
 * @param language_prefs        is an array of `cr_payment_language_preference_t` values.
 * Each language is represented by a two-byte code. The card will always return 4 2-byte language codes.
 * If a specific language isn't supported, then the value will be 0.
 * @param aid
 *
 * @note that the language preference from the card is also sent up with this callback,
 * but isn't needed to make an account type selection. No information about the selected
 * language needs to be returned to the reader.
 */
typedef void (*cr_payment_account_type_selection_required_t)(void *context,
                                                             cr_payment_t *payment,
                                                             cr_payment_account_type_t const *account_types,
                                                             size_t account_types_len,
                                                             cr_payment_language_preference_t const *language_prefs,
                                                             uint8_t *aid);


/**
 * @brief Callback for when the payment has completed. This does not necessarily indicate successfully
 * completed of a transaction.
 *
 * @details In the case of a successful payments (result is CR_PAYMENT_PAYMENT_RESULT_SUCCESS_*) and all emv
 * payments (result is CR_PAYMENT_PAYMENT_RESULT_*_ICC_*) `data` will be non-NULL and is intended
 * for the payment server. In all other cases, `data` should be NULL.
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param data                  is either an M1 packet or EMV clearing/reversal message or NULL. If not NULL, data
 * shall be sent to the server on the appropriate endpoint.
 * @param data_len              length of data
 * @param result                shows if the payment was successful, or how it failed.
 * @param approved_offline      indicates if transaction is approved offline
 * @param card_info             contains information that shall be displayed to the user.
 * @param std_msg               is information that shall be displayed to the user.
 * @param payment_timings       payment timing record.
 * @param payment_timings_count count of payment timing entries
 * @param card_action           indicates card action that card holder should take
 */
typedef void (*cr_payment_payment_complete_t)(void *context,
                                              cr_payment_t *payment,
                                              void const *data,
                                              size_t data_len,
                                              cr_payment_payment_result_t result,
                                              bool approved_offline,
                                              cr_card_info_t const *card_info,
                                              cr_payment_std_msg_t std_msg,
                                              cr_payment_timing_t const *payment_timings,
                                              size_t payment_timings_count,
                                              cr_payment_card_action_t card_action);

/**
 * @brief Callback containing the information required to be sent to the server.
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param buf                   data to be sent with auth request
 * @param buf_len               length of buf
 * @param is_card_presence_required indicates if we need to keep the card inserted
 * @param card_info             contains information that shall be displayed to the user.
 */
typedef void (*cr_payment_emv_authorization_request_t)(void *context,
                                                       cr_payment_t *payment,
                                                       void const *buf,
                                                       size_t buf_len,
                                                       bool is_card_presence_required,
                                                       cr_card_info_t const *card_info);

/**
 * @brief Callback containing the information required to be sent to the server.
 *
 * @param context Pointer to the user-supplied context pointer.
 * @param payment Pointer to the Payment feature instance.
 * @param buf     data to be sent with the auth request
 * @param buf_len len of buf
 */
typedef void (*cr_payment_tmn_authorization_request_t)(void *context,
                                                       cr_payment_t *payment,
                                                       void const *buf,
                                                       size_t buf_len);


/**
 * @brief Callback with card information from a passthrough magswipe. A passthrough swipe must be enabled
 * with cr_payment_enable_swipe_passthrough. This enables the merchant to take payments in off-line
 * mode, and perform certain other non-payment operations, such as use a swipe to search for
 * customer data in a back-of-house flow.
 *
 * @param context       Pointer to the user-supplied context pointer.
 * @param payment       Pointer to the Payment feature instance.
 * @param data          M1 packet on successful swipe event, otherwise `NULL`. If not `NULL`,
 *                      data shall be sent to the server on the appropriate endpoint.
 * @param data_len      Length of the M1 packet in bytes, `0` on failure.
 * @param card_info     Contains information that shall be displayed to the user.
 * @param payment_src   Identifier of the MSR that the swipe was recorded on.
 */
typedef void (*cr_payment_swipe_passthrough_t)(void *context,
                                               cr_payment_t *payment,
                                               void const *data,
                                               size_t data_len,
                                               cr_card_info_t *card_info,
                                               cr_payment_source_t payment_src);


/**
 * @brief Callback with card information containing at the very least the cardholder's name.
 * If the transaction was contactless, the name might be intentionally obfuscated and appear
 * something like "\." or "Valued Customer". Card information will also contain valid issuer_id
 * (brand).
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 * @param card_info         contains information that shall be displayed to the user.
 */
typedef void (*cr_payment_cardholder_name_received_t)(void *context,
                                                      cr_payment_t *payment,
                                                      cr_card_info_t const *card_info);

/**
 * @brief Callback for when an acknowledgment is received from an M1 configure or clear_configuration command.
 * NOTE: For factory use only.
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 */
typedef void (*cr_payment_magswipe_flow_configuration_callback_t)(void *context, cr_payment_t *payment);


/**
 * @brief Callback for when M1 health data is received from the reader.
 * NOTE: For factory use only.
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 * @param status
 */
typedef void (*cr_payment_magswipe_flow_health_callback_t)(void *context, cr_payment_t *payment, uint32_t status);


/**
 * @brief Callback for when an error message is received from an M1 configure or clear_configuration command.
 * NOTE: For factory use only.
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 */
typedef void (*cr_payment_magswipe_flow_error_callback_t)(void *context, cr_payment_t *payment);

/**
 * @brief Callback for when M1 dump data is received from the reader.
 * NOTE: For factory use only.
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 * @param dump_data
 */
typedef void (*cr_payment_magswipe_flow_dump_callback_t)(void *context,
                                                         cr_payment_t *payment,
                                                         const crs_magswipe_dump_msg_t *dump_data);

/**
 * @brief Callback for when a TMN transaction has completed.
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param result                transaction result status  (tmn_transaction_result_t from crs_tmn_msgs.h)
 * @param payment_timings       payment timing record.
 * @param payment_timings_count count of payment timing entries
 */
typedef void (*cr_payment_tmn_transaction_complete_t)(void *context,
                                                      cr_payment_t *payment,
                                                      tmn_transaction_result_t result,
                                                      cr_payment_timing_t const *payment_timings,
                                                      size_t payment_timings_count);

/**
 * @brief Callback for when a TMN transaction needs to request a UI message to be displayed.
 *
 * @param context           Pointer to the user-supplied context pointer.
 * @param payment           Pointer to the Payment feature instance.
 * @param msg_id            msg ID for the display request
 * @param amount_string     null terminated amount string (empty if none, max len 32)
 * @param balance_string    null terminated balance string (empty if none, max len 32)
 */
typedef void (*cr_payment_tmn_ui_request_t)(void *context,
                                            cr_payment_t *payment,
                                            crs_tmn_message_t msg_id,
                                            const char *amount_string,
                                            const char *balance_string);


/**
 * @brief Callback for when a TMN transaction needs to request a audio request.
 *
 * @param context  Pointer to the user-supplied context pointer.
 * @param payment  Pointer to the Payment feature instance.
 * @param audio_id Identifier for the audio request.
 */
typedef void (*cr_payment_tmn_audio_request_t)(void *context, cr_payment_t *payment, crs_tmn_audio_t audio_id);


/**
 * @brief Callback for TMN data to be sent to TMN Connection.
 *
 * @param context        Pointer to the user-supplied context pointer.
 * @param payment        Pointer to the Payment feature instance.
 * @param transaction_id The transaction_id to use for this TMN message
 * @param data           Payment result data intended for payment server (via TMN Proxy).
 * @param data_len       Length of the payment result data.
 */
typedef void (*cr_payment_tmn_data_packet_callback_t)(void *context,
                                                      cr_payment_t *payment,
                                                      const char *transaction_id,
                                                      void const *data,
                                                      size_t data_len);


/**
 * @brief Callback to notify that write operation is in progress.
 *
 * @details  Callback for when a TMN transaction is entering the final stage. After this
 * event the transaction cannot be canceled and miryou flow will be needed if a transaction
 * result is not received.
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param balance_before        Balance on the card before the transaction.
 * @param amount                Transaction amount.
 * @param miryo_data            Information needed for Miryo Auth in the event that Miryo is required.
 * @param miryo_data_len        Length of the Miryo Auth information in bytes.
 */
typedef void (*cr_payment_tmn_write_notify_t)(void *context,
                                              cr_payment_t *payment,
                                              uint32_t balance_before,
                                              uint32_t amount,
                                              void const *miryo_data,
                                              size_t miryo_data_len);

/**
 * @brief Callback for audio visual request
 *
 * @param context               Pointer to the user-supplied context pointer.
 * @param payment               Pointer to the Payment feature instance.
 * @param audio_visual_id       audio to be played/ visual image t be displayed.
 */
typedef void (*cr_payment_audio_visual_request_t)(void *context,
                                                  cr_payment_t *payment,
                                                  cr_audio_visual_id_t audio_visual_id);

/**
 * @brief Callback for VAS response
 *
 * @param context               User-supplied context pointer
 * @param payment               Pointer to the payment feature instance
 * @param protocol              VAS protocol used
 * @param result                Result code for VAS transaction
 * @param finished              This can be the final response or followed by auth
 * @param merchant_loyalty_id   merchant loyalty id that the data is associated with
 * @param data                  encrypted buyer loyalty id
 * @param data_len              encrypted buyer loyalty id length
 */
typedef void (*cr_payment_vas_response_t)(void *context,
                                          cr_payment_t *payment,
                                          cr_payment_vas_protocol_t protocol,
                                          cr_payment_loyalty_result_t result,
                                          bool finished,
                                          cr_payment_merchant_loyalty_id_t const *merchant_loyalty_id,
                                          void const *data,
                                          size_t data_len);


/**
 * @brief Callback for NDEF Read complete
 *
 * @param context          User-supplied context pointer
 * @param payment          Pointer to the payment feature instance
 * @param application_type Team management, Loyalty or Gift card
 * @param merchant_id      The merchant id on the card
 * @param card_id          card unique id for the application, similar to PAN
 * @param card_id_length   actual length of card id
 * @param card_type        sub card type
 * @param result           result of the read ndef command
 */
typedef void (*cr_payment_read_ndef_complete_t)(void *context,
                                                cr_payment_t *payment,
                                                cr_payment_ndef_application_type_t application_type,
                                                cr_payment_ndef_merchant_id_t const *merchant_id,
                                                cr_payment_ndef_card_id_t const *card_id,
                                                uint8_t card_id_length,
                                                uint8_t card_type,
                                                cr_payment_ndef_result_t result);

/**
 * @brief Callback for NDEF Write complete
 *
 * @param context     Pointer to the user-supplied context pointer.
 * @param payment     Pointer to the Payment feature instance.
 * @param result      result of the write ndef command
 */
typedef void (*cr_payment_write_ndef_complete_t)(void *context, cr_payment_t *payment, cr_payment_ndef_result_t result);


/**
 * @brief Callback for Record Read and write complete
 *
 * @param context                   Pointer to the user-supplied context pointer.
 * @param payment                   Pointer to the Payment feature instance.
 * @param application_type          It is returned to verify the correct application type is read.
 * @param merchant_id               it is returned to verify the correct merchant id is read.
 * @param merchant_id_length        length of merchant_id
 * @param uid                       UID of the card is returned since it can be used for cryptographic operations, like encryption and description.
 * @param uid_length                length of uid
 * @param record_idx                record index that was read
 * @param total_records             It indicates if there are more records to be read
 * @param record                    Record sotored in the card
 * @param result                    Result of the read operation. If that is not successful, record and record_idx should be ignored
 */
typedef void (*cr_payment_record_request_complete_t)(void *context,
                                                    cr_payment_t *payment,
                                                    cr_payment_record_application_type_t application_type,
                                                    uint8_t const *merchant_id,
                                                    size_t merchant_id_length,
                                                    uint8_t const *uid,
                                                    size_t uid_length,
                                                    uint8_t record_idx,
                                                    uint8_t total_records,
                                                    cr_payment_record_t *record,
                                                    cr_payment_record_result_t result);

/**
 * @brief Request for merchant secret.
 *
 * @details <In the event when there is no secure channel, the key will be generated on the pos side.
 * This protobufs allow reader to request the merchant key when there is no merchant secret provided>
 *
 * @param context     Pointer to the user-supplied context pointer.
 * @param payment     Pointer to the Payment feature instance.
 * @param uid         The unique ID of the card that is returned. Will be used to generate the key
 * @param uid_length  Length of the UID
 *
 * @note The length of the uid should be 7 bytes for mifare cards.
 */
typedef void (*cr_payment_record_mid_request_t)(void *context,
                                                cr_payment_t *payment,
                                                uint8_t const *uid,
                                                size_t uid_length);

/**
 * @brief Callback for NDEF Write complete
 *
 * @param context     Pointer to the user-supplied context pointer.
 * @param payment     Pointer to the Payment feature instance.
 * @param result      result of the write ndef command
 */
typedef void (*cr_payment_write_ndef_complete_t)(void *context, cr_payment_t *payment, cr_payment_ndef_result_t result);

typedef struct cr_payment_event_api_t
{
    cr_payment_card_presence_changed_t on_card_presence_changed;
    cr_payment_card_action_required_t on_card_action_required;
    cr_payment_application_selection_required_t on_application_selection_required;
    cr_payment_account_type_selection_required_t on_account_type_selection_required;
    cr_payment_emv_authorization_request_t on_emv_authorization_request;
    cr_payment_emv_authorization_request_t on_contactless_emv_authorization_request;
    cr_payment_payment_complete_t on_payment_complete;
    cr_payment_swipe_passthrough_t on_swipe_passthrough;
    cr_payment_cardholder_name_received_t on_cardholder_name_received;

    /**
     * @brief Optional callbacks for MSR factory tests
     *
     */
    cr_payment_magswipe_flow_configuration_callback_t on_msr_configure;
    cr_payment_magswipe_flow_health_callback_t on_msr_health;
    cr_payment_magswipe_flow_error_callback_t on_msr_error;
    cr_payment_magswipe_flow_dump_callback_t on_msr_dump;

    /**
     * @brief Callbacks for TMN payment flow
     *
     */
    cr_payment_tmn_data_packet_callback_t on_tmn_data;
    cr_payment_tmn_transaction_complete_t on_tmn_transaction_complete;
    cr_payment_tmn_ui_request_t on_display_request;
    cr_payment_tmn_audio_request_t on_audio_request;
    cr_payment_tmn_authorization_request_t on_tmn_authorization_request;
    cr_payment_tmn_write_notify_t on_tmn_write_notify;

    /**
     * @brief Callback for audio request from payment stack
     *
     */
    cr_payment_audio_visual_request_t on_audio_visual_request;

    /**
     * @brief Callback for VAS response
     *
     */
    cr_payment_vas_response_t on_vas_response;

    /**
     * @brief Callback for ndef read completion
     *
     */
    cr_payment_read_ndef_complete_t on_read_ndef_complete;
    /**
     * @brief Callback for ndef write complete
     *
     */
    cr_payment_write_ndef_complete_t on_write_ndef_complete;

    /**
     * @brief Registered callback to handle an MID request for a record.
     * transaction.
     */
    cr_payment_record_mid_request_t on_mid_request;

    /**
     * @brief Callback for Read/Write record completion
     *
     */
    cr_payment_record_request_complete_t on_record_request_complete;

    /**
     * @brief Callback for secure Read/Write record completion
     *
     */
    cr_payment_record_request_complete_t on_secure_record_request_complete;

    void *context;
} cr_payment_event_api_t;

typedef struct cr_payment_cfg_t
{
    /**
     * @brief merchant category code
     */
    uint16_t mcc;
    /**
     * @brief ISO 4217
     *
     */
    uint16_t transaction_currency_code;
} cr_payment_cfg_t;

struct cr_cardreader_t;

// Feature bring up and tear down.
cr_payment_t *cr_payment_alloc(void);
/**
 * @brief
 *
 * @param payment                       payment content pointer
 * @param cardreader                    cardreader context pointer
 * @param api
 * @param cfg
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_init(cr_payment_t *payment,
                                    struct cr_cardreader_t *cardreader,
                                    cr_payment_event_api_t const *api,
                                    cr_payment_cfg_t const *cfg);
cr_payment_result_t cr_payment_term(cr_payment_t *payment);
/**
 * @brief
 *
 * @param payment payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_free(cr_payment_t *payment);

/**
 * @brief
 *
 * @return crs_log_subsystem_t
 */
crs_log_subsystem_t cr_payment_get_log_subsystem(void);

/**
* @brief Start payment transaction mode
 *
 * @param payment                       Pointer to the Payment feature instance.
 * @param auth_amount                   Transaction amount "EMV tag 9F02" and it is in cents
 * @param transaction_currency_code     Transaction currency code example "0840" USD
 * @param transaction_type              Type of payment transaction, example pusrchase 00 or Refund
 * @param year                          Year of transaction
 * @param month                         Month (1-12) of transaction
 * @param day                           Day of transcation, it can be within range (1-31)
 * @param hour                          Hour of transaction, it can be within range (0-23)
 * @param minute                        Minute of transcation, it can be within range (0-59)
 * @param second                        Second of transaction, it can be within (0-59)
 *
 *
 * @details This is the Entry point for beginning a payment, it generates a StartPaymentRequest and sends it via proto ritual.
 * It supports both payment and VAS transactions as well as one-tap (VAS + payment)
 *
 * @note For R6 we will prepare ritual proto "l2ctl_prepare", and for the rest "l2ctl_start_pym_req" ritual proto will be sent
 *
 * @note If cr_payment_set_loyalty_url_info was called beforehand, the reader will push the URL in ApplePay transactions.
 *
 * @note If cr_payment_set_loyalty_vas_info was called beforehand, the reader will first start a VAS transaction and then
 * a payment transaction. If not ApplePay, the reader will convert VAS transaction into payment transaction
 * to avoid the overhead of two transactions.
 *
*/
cr_payment_result_t cr_payment_start_payment_interaction(cr_payment_t *payment,
                                                         uint32_t auth_amount,
                                                         uint16_t transaction_currency_code,
                                                         cr_payment_transaction_type_t transaction_type,
                                                         uint8_t year,
                                                         uint8_t month,
                                                         uint8_t day,
                                                         uint8_t hour,
                                                         uint8_t minute,
                                                         uint8_t second);

/**
 * @brief Set store and forward payment transaction mode
 *
 * @param payment payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_set_payment_store_and_forward(cr_payment_t *payment);

/**
 * @brief Set start payment v2 transaction mode
 *
 * @param payment  payment content pointer
 * @param transaction_mode transaction mode
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_set_payment_transaction_mode(cr_payment_t *payment,
                                                            cr_payment_transaction_mode_t transaction_mode);

/**
 * @brief Get start payment v2 transaction mode used in current transaction
 *
 * @param payment payment content pointer
 * @return cr_payment_transaction_mode_t
 */
cr_payment_transaction_mode_t cr_payment_get_payment_transaction_mode(cr_payment_t *payment);

/**
 * @brief Get card information only.
 *
 * @param payment   payment content pointer
 * @param year      time of transaction year
 * @param month     time of transaction month
 * @param day       time of transaction day
 * @param hour      time of transaction hour
 * @param minute    time of transaction minute
 * @param second    time of transaction seconds
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_get_card_info(cr_payment_t *payment,
                                             uint8_t year,
                                             uint8_t month,
                                             uint8_t day,
                                             uint8_t hour,
                                             uint8_t minute,
                                             uint8_t second);

/**
 * @brief Set parameters for URL pushing in ApplePay transactions.
 *
 * @param payment               payment content pointer
 * @param merchant_loyalty_id   merchant loyality id for URL
 * @param merchant_loyalty_url  URL to be set maximum 64 characters
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_set_loyalty_url_info(
    cr_payment_t *payment,
    cr_payment_merchant_loyalty_id_t const *merchant_loyalty_id,
    char const *merchant_loyalty_url);

/**
 * @brief Set merchant loyalty ids for VAS transactions.
 *
 * @param payment               payment content pointer
 * @param merchant_loyalty_ids  list of merchant loyality ids to be set for full VAS
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_set_loyalty_vas_info(
    cr_payment_t *payment,
    cr_payment_merchant_loyalty_id_list_t const *merchant_loyalty_ids);

/**
 * @brief Start a VAS transaction
 *
 * @param payment               payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_get_vas_data(cr_payment_t *payment);

/**
 * @brief Cancel a payment already in progress (before payment_complete).
 *
 * @param payment               payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_cancel_payment(cr_payment_t *payment);

/**
 * @brief Select the application on the card.
 * @details Should be called from `on_application_selection_required`
 * `adf_name` and `adf_name_len` is selected from the cr_payment_application_t list passed into the callback.
 *
 * @param payment               payment content pointer
 * @param adf_name              application name selected
 * @param adf_name_len          length of application name
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_select_application(cr_payment_t *payment, uint8_t const *adf_name, size_t adf_name_len);


/**
 * @brief Select the account type to use on the card.
 * @details Should be called from `on_account_type_selection_required`.
 * The account_type passed to this call should come from the list of cr_payment_account_type_t passed
 * to the `on_account_type_selection_required` callback.
 *
 * @param payment               payment content pointer
 * @param account_type          account type
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_select_account_type(cr_payment_t *payment, cr_payment_account_type_t account_type);


/**
 * @brief process auth response from server
 * @details Tell the reader to process the server response for the authorization request from `on_emv_authorization_request`
 * `arpc` is the data sent from the server.
 * @param payment               payment content pointer
 * @param arpc                  authorization response code from server
 * @param arpc_len              length of arpc
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_process_authorization_response(cr_payment_t *payment, void const *arpc, size_t arpc_len);

/**
 * @brief Request the card presence to be reported.
 * @details May be called when reader is plugged in (after reader_ready).
 * Useful for when a card is already dipped prior to plugging into the device.
 *
 * @param payment               payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_request_card_presence(cr_payment_t *payment);


/**
 * @brief Enable or disable magswipe passthrough.
 * @details When enabled all swipes (whether part of a payment or not) will result in
 * a `on_swipe_passthrough` callback. If enabled, swipes will not trigger a `on_payment_complete`.
 *
 * @param payment               payment content pointer
 * @param enabled               enable or disable
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_enable_swipe_passthrough(cr_payment_t *payment, bool enabled);

/**
 * @brief Get last known card presence status lcr has w/o querying the reader
 *
 * @param payment                           payment content pointer
 * @param last_known_card_presence_out      last known card presence, updated by callbacks on
 *                                          RIT_l1_{no_chip,chip_present} messages from the reader and reset to
 *                                          unknown on reader_ready.
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_get_last_known_card_presence(cr_payment_t *payment,
                                                            cr_payment_card_presence_t *last_known_card_presence_out);

/**
 * @brief Tell the reader that a payment is likely to happen soon and to powerup k400 in preperation
 *
 * @param payment                           payment content pointer
 * @param timeout_seconds
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_send_powerup_hint(cr_payment_t *payment, uint32_t timeout_seconds);

/**
 * @brief MSR factory tests for LCRPy
 *
 * @param payment                           payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_get_status(cr_payment_t *payment);

/**
 * @brief
 *
 * @param payment                           payment content pointer
 * @param payment_src
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_get_status_v2(cr_payment_t *payment, cr_payment_source_t payment_src);

/**
 * @brief
 *
 * @param payment                           payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_configure(cr_payment_t *payment);

/**
 * @brief
 *
 * @param payment                           payment content pointer
 * @param payment_src
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_configure_v2(cr_payment_t *payment, cr_payment_source_t payment_src);

/**
 * @brief
 *
 * @param payment                           payment content pointer
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_clear_configuration(cr_payment_t *payment);

/**
 * @brief
 *
 * @param payment                           payment content pointer.
 * @param payment_src
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_clear_configuration_v2(cr_payment_t *payment, cr_payment_source_t payment_src);

/**
 * @brief
 *
 * @param payment                           payment content pointer.
 * @param payment_src
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_msr_dump(cr_payment_t *payment, cr_payment_source_t payment_src);

/**
 * @brief Entry point for starting a TMN transaction.
 *
 * @param payment                           payment content pointer.
 * @param transaction_type                  type of transaction (TMN_REQUEST_TRANSACTION for payments)
 * @param transaction_id                    32 character transaction id (generated by App)
 * @param brand_id                          E-Money brand id
 * @param amount                            transaction amount
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_start_transaction(cr_payment_t *payment,
                                                     crs_tmn_request_type_t transaction_type,
                                                     char const *transaction_id,
                                                     crs_tmn_brand_id_t brand_id,
                                                     uint32_t amount);


/**
 * @brief Check the result of a previous transaction.
 *
 * @param payment                           payment content pointer.
 * @param transaction_id                    32 character transaction id (generated by App) for this new request
 * @param brand_id                          E-Money brand id
 * @param prev_transaction_id               Transaction ID for the transaction we would like the result of
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_check_result(cr_payment_t *payment,
                                                char const *transaction_id,
                                                crs_tmn_brand_id_t brand_id,
                                                char const *prev_transaction_id);


/**
 * @brief Start Miryo recovery for a previous transaction.
 *
 * @details This will initiate a "check result"
 * transaction with TMN, and in some cases may also lead to a "check balance" transaction.
 * This is expected to eventually lead to a UI message (TMN_MSG_MIRYO_RESULT_SUCCESS,
 * TMN_MSG_MIRYO_RESULT_FAILURE, or TMN_MSG_MIRYO_RESULT_UNKNOWN) and a matching result
 * code (TMN_RESULT_SUCCESS, error reason, or TMN_RESULT_MIRYO_RESOLUTION_FAILURE).
 * @param payment                           payment content pointer.
 * @param miryo_data                        Miryo data from the 'write_notify' of the original payment
 * @param miryo_data_len                    length of miryo data
 * @param miryo_transaction_id              32 character transaction id (generated by App) for this new request.
                                            miryo_transaction_id should begin with the characters 'miryoCR'
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_start_miryo(cr_payment_t *payment,
                                               uint8_t const *miryo_data,
                                               size_t miryo_data_len,
                                               char const *miryo_transaction_id);

/**
 * @brief Void a transaction reported as 'unknown' result.
 *
 * @param payment                           payment content pointer.
 * @param transaction_id                    32 character transaction id (generated by App) for this new request
 * @param brand_id                          the felica card brand
 * @param prev_transaction_id               Transaction ID for the transaction we would like the result of
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_void_unknown(cr_payment_t *payment,
                                                char const *transaction_id,
                                                crs_tmn_brand_id_t brand_id,
                                                char const *prev_transaction_id);

/**
 * @brief Send bytes to reader (from TMN)
 *
 * @param payment                           payment content pointer.
 * @param data                              data from tmn that needs to be sent to reader
 * @param data_len                          length of data
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_send_bytes_to_reader(cr_payment_t *payment, uint8_t const *data, size_t data_len);

/**
 * @brief Acknowledge Write notification
 *
 * @param payment                           payment content pointer.
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_write_notify_ack(cr_payment_t *payment);

/**
 * @brief Cancel an active TMN transaction
 *
 * @details Cancel an active TMN transaction. If the transaction is waiting for polling it will be
    canceled using a TMN cancel request. If the card has been detected but the reader has not
    received a response to `write_notify` then the payment will be forced to terminate.
    Cancel after polling or during `write_notify` should only be used if we cannot Auth. If
    `write_notify_ack` was sent the cancel will be ignored and the payment will continue.
 * @param payment                           payment content pointer.
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_tmn_cancel_request(cr_payment_t *payment);

/**
 * @brief Start a Read NDEF Data Request.
 *
 * @param payment                           payment content pointer.
 * @param application_type                  Application saved in the card
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_read_ndef(cr_payment_t *payment,
                                         cr_payment_ndef_application_type_t application_type,
                                         cr_payment_ndef_merchant_id_t const *merchant_id);

/**
 * @brief Start a Write NDEF Data Request.
 *
 * @param payment                           Payment feature pointer.
 * @param application_type                  Application saved in the card.
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record.
 * @param card_id                           Unique card id stored inside the card.
 * @param card_id_length                    Length of card id in bytes.
 * @param card_type                         Type of card used.
 * @param override                          `true` if existing record should be overriten, othewrise `false`.
 *
 * @return cr_payment_result_t
 */
cr_payment_result_t cr_payment_write_ndef(cr_payment_t *payment,
                                          cr_payment_ndef_application_type_t application_type,
                                          cr_payment_ndef_merchant_id_t const *merchant_id,
                                          cr_payment_ndef_card_id_t const *card_id,
                                          uint8_t card_id_length,
                                          uint8_t card_type,
                                          bool override);
/**
 * @brief Merchant secret response.
 *
 * @see https://docs.google.com/document/d/11vfcIsqoc7-g9vXiWnHOEhC8HyAn22bSitMqZSMAJlk/edit#heading=h.ckgonlfzd7rj
 *
 * @details <In the event when there is no secure channel, the key will be generated on the pos side.
 * This function returns the merchant secret to reader>
 *
 * @param payment                           Pointer to the Payment feature instance.
 * @param merchant_id                       Pointer to the merchant ID
 * @param merchant_id_length                Length of merchant ID
 */
cr_payment_result_t cr_payment_mid_response(cr_payment_t *payment,
                                            uint8_t const *merchant_id,
                                            size_t merchant_id_length);

/**
 * @brief Read records on card
 *
 * @details start a read record transaction with STS
 *
 * @see https://docs.google.com/document/d/11vfcIsqoc7-g9vXiWnHOEhC8HyAn22bSitMqZSMAJlk/edit#heading=h.ckgonlfzd7rj
 *
 * @param payment                           payment content pointer.
 * @param application_type                  Application saved in the card
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record
 * @param merchant_id_length                length should be between 13 to 32 bytes long.
 * @param record_idx                        Record to be read on a card
 * @param timeout                           Card present timeout
 */
cr_payment_result_t cr_payment_read_record(cr_payment_t *payment,
                                           cr_payment_record_application_type_t application_type,
                                           uint8_t const *merchant_id,
                                           size_t merchant_id_length,
                                           uint8_t record_idx,
                                           uint32_t timeout);
/**
 * @brief Read records on card with secure session
 *
 * @details start a read record transaction with Secures session
 *
 * @see https://docs.google.com/document/d/11vfcIsqoc7-g9vXiWnHOEhC8HyAn22bSitMqZSMAJlk/edit#heading=h.ckgonlfzd7rj
 *
 * @param payment                           payment content pointer.
 * @param application_type                  Application saved in the card
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record
 * @param merchant_id_length                length should be between 13 to 32 bytes long.
 * @param record_idx                        Record to be read on a card
 * @param timeout                           Card present timeout
 */
cr_payment_result_t cr_payment_secure_read_record(cr_payment_t *payment,
                                                  cr_payment_record_application_type_t application_type,
                                                  uint8_t const *merchant_id,
                                                  size_t merchant_id_length,
                                                  uint8_t record_idx,
                                                  uint32_t timeout);

/**
 * @brief Write records to card with STS
 *
 * @details start a write record transaction with Secures session
 *
 * @see https://docs.google.com/document/d/11vfcIsqoc7-g9vXiWnHOEhC8HyAn22bSitMqZSMAJlk/edit#heading=h.ckgonlfzd7rj
 *
 * @param payment                           Payment feature pointer.
 * @param application_type                  Application saved in the card.
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record.
 * @param merchant_id_length                Length should be between 13 to 32 bytes long.
 * @param record                            Record to be written on a card
 * @param record_idx                        Index of the record, we want to write to.
 * @param force_overwrite                   `true` if existing record should be overwritten, otherwise `false`.
 * @param uid                               UID of the card, currently we are extracting UID from card
 *                                          instead so this should be left empty.
 * @param uid_length                        Due to @p uid, this should be set to `0`.
 * @param timeout                           Card present timeout.
 */
cr_payment_result_t cr_payment_write_record(cr_payment_t *payment,
                                            cr_payment_record_application_type_t application_type,
                                            uint8_t const *merchant_id,
                                            size_t merchant_id_length,
                                            cr_payment_record_t *record,
                                            uint8_t record_idx,
                                            bool force_overwrite,
                                            uint8_t const *uid,
                                            size_t uid_length,
                                            uint32_t timeout);
/**
 * @brief Write records to card with SS
 *
 * @details start a write record transaction with Secures session
 *
 * @see https://docs.google.com/document/d/11vfcIsqoc7-g9vXiWnHOEhC8HyAn22bSitMqZSMAJlk/edit#heading=h.ckgonlfzd7rj
 *
 * @param payment                           Payment feature pointer.
 * @param application_type                  Application saved in the card.
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record.
 * @param merchant_id_length                Length should be between 13 to 32 bytes long.
 * @param record                            Record to be written on a card.
 * @param record_idx                        Index of the record, we want to write to.
 * @param force_overwrite                   `true` if existing record should be overwritten, otherwise `false`.
 * @param uid                               UID of the card, currently we are extracting UID from card instead,
 *                                          so this should be left empty.
 * @param uid_length                        Due to @p uid, this should be set to `0`.
 * @param timeout                           Card present timeout.
 */
cr_payment_result_t cr_payment_secure_write_record(cr_payment_t *payment,
                                                   cr_payment_record_application_type_t application_type,
                                                   uint8_t const *merchant_id,
                                                   size_t merchant_id_length,
                                                   cr_payment_record_t *record,
                                                   uint8_t record_idx,
                                                   bool force_overwrite,
                                                   uint8_t const *uid,
                                                   size_t uid_length,
                                                   uint32_t timeout);

/**
 * @brief Write records to card
 *
 * @details start a write record transaction with Secures session
 *
 * @see https://docs.google.com/document/d/11vfcIsqoc7-g9vXiWnHOEhC8HyAn22bSitMqZSMAJlk/edit#heading=h.ckgonlfzd7rj
 *
 * @param payment                           Payment feature pointer.
 * @param application_type                  Application saved in the card.
 * @param merchant_id                       Merchant Token of a merchant, that intends to read a record.
 * @param merchant_id_length                Length of the @p merchant_id in bytes; should be between 13 to 32 bytes
 *                                          long.
 * @param storage_type                      Type identifying how data is stored inside the record (i.e. encrypted,
 *                                          or plaintext.
 * @param record_length                     Length of the @p record_data in bytes.
 * @param record_data                       Record to be written on a card.
 * @param record_idx                        Index of the record, we want to write to.
 * @param force_overwrite                   `true` if existing record should be overwritten, otherwise `false`.
 * @param uid                               UID of the card, currently we are extracting UID from card instead,
 *                                          so this should be left empty.
 * @param uid_length                        UID is currently not used, so this should be set to `0`.
 * @param timeout                           Card presence timeout.
 * @param use_secure                        `true` when secure session is availible and `false` when STS is used.
 */
cr_payment_result_t cr_payment_write_record_wrapper(cr_payment_t *payment,
                                                    cr_payment_record_application_type_t application_type,
                                                    uint8_t const *merchant_id,
                                                    size_t merchant_id_length,
                                                    uint8_t storage_type,
                                                    uint16_t record_length,
                                                    uint8_t const *record_data,
                                                    uint8_t record_idx,
                                                    bool force_overwrite,
                                                    uint8_t const *uid,
                                                    size_t uid_length,
                                                    uint32_t  timeout,
                                                    bool use_secure);


/**
 * @brief
 * @details Although the reader-originated l2io messages are in codetalker format,
 * ECR does not utilize codetalker. Instead, hieroglyph performs POS-side
 * encryption on the clear portion of the codetalker message.
 * This utility function returns the passed codetalker message back for HW readers
 * and terminals while it returns clear portion of the codetalker message for ECR.
 * @param payment                           payment content pointer.
 * @param codetalker_msg
 * @param codetalker_msg_len
 * @param out_data_len
 * @return void const*
 */
void const *cr_payment_get_server_msg(cr_payment_t *payment,
                                      void const *codetalker_msg,
                                      size_t codetalker_msg_len,
                                      size_t *out_data_len);

/**
 * @brief Toggle the payment declined sound.
 *
 * @param payment                     Payment content pointer.
 * @param value                       If true, payment declined sound will play after a declined transaction
 * @return cr_payment_result_t
 */
cr_payment_result_t enable_payment_declined_sound(cr_payment_t *payment,
                                                  bool value);
/** @} */
