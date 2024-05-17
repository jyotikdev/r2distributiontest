// Copyright (c) 2014 Square, Inc. All rights reserved.

/**
 * @file
 *
 * @brief Cardreader Power Feature
 */

/**
 * @defgroup lcr_power Power Feature
 *
 * @brief Cardreader Power Feature
 *
 * @details This file defines the API for the Cardreader Power Feature. The
 * power feature is responsible for controlling the power state of the device,
 * which includes functionality such as resetting, powering off the device,
 * getting the battery status of the device, and controlling the behaviour of
 * the device's LPS / LPM (Low Power State / Low Power Mode).
 *
 * @author Dave Terra <davet@squareup.com>
 *
 * @copyright 2014 Square, Inc.
 *
 * @ingroup lcr
 * @{
 */

#pragma once

#include <cardreader_shared/crs_power_msgs.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Enumeration of the possible result codes for the `cr_power_*`
 * functions.
 */
typedef enum {
    /**
     * @brief API call was successful.
     */
    CR_POWER_RESULT_SUCCESS,

    /**
     * @brief Invalid parameter was passed to the API function.
     */
    CR_POWER_RESULT_INVALID_PARAMETER = 1,

    /**
     * @brief Power Feature was not initialized before an API function call was
     * attempted; #cr_power_init() must be called before any other API
     * functions are called.
     */
    CR_POWER_RESULT_NOT_INITIALIZED = 2,

    /**
     * @brief Power Feature was attempted to be initialized while already
     * initialized.
     */
    CR_POWER_RESULT_ALREADY_INITIALIZED = 3,

    /**
     * @brief Freeing of the Power Feature was attempted when the feature has
     * not yet been terminated.
     */
    CR_POWER_RESULT_NOT_TERMINATED = 4,

    /**
     * @brief Termination of the Power Feature was attempted when the feature
     * has already been terminated.
     */
    CR_POWER_RESULT_ALREADY_TERMINATED = 5,

    /**
     * @brief Error encountered when sending a request to the device, or if
     * there is an issue with the Cardreader instance.
     */
    CR_POWER_RESULT_SESSION_ERROR = 7,

    /**
     * @brief Request made during an invalid state of the Power Feature.
     */
    CR_POWER_RESULT_CALL_UNEXPECTED = 8,

    /**
     * @brief Un-recoverable error.
     */
    CR_POWER_RESULT_FATAL = 9,
} cr_power_result_t;

/**
 * @brief Structure encapsulating the data informing LCR about when the device
 * will send its battery voltage reports.
 */
typedef struct cr_power_report_configuration_t {
    /**
     * @brief If non-zero, specifies the percent interval for the SOC (State of
     * Charge) at which a battery voltage report will be sent. A value of `10`
     * for instance, indicates at every `10%` interval; e.g. `10%`, `20%`,
     * `30%`, etc.
     */
    uint8_t soc_threshold;
} cr_power_report_configuration_t;

/**
 * @brief Forward declaration for the Power Feature.
 */
struct cr_power_t;

/**
 * @brief Forward declaration for the cardreader instance.
 */
struct cr_cardreader_t;

/**
 * @brief Callback invoked when the reader provides battery voltage information.
 *
 * @details The battery voltage callback is invoked in two cases; either as a
 * response to a call to #cr_power_get_battery_voltage(), or when the device
 * determines that it should send a battery voltage report to the host, and
 * sends one on its own. In either case, the voltage report is received
 * asynchronously.
 *
 * @param context           User-supplied context pointer.
 * @param power             The Power Feature instance.
 * @param charge_percentage The device's SOC, in percent, from `0 - 100`.
 * @param current           The device's current measurement in mA.
 * @param voltage           The device's voltage measurement in mV.
 * @param temperature       The temperature of the device's battery in °C.
 * @param is_critical       Boolean indicating if the device's battery is
 *                          critically low.
 * @param battery_mode      Enum describing the state of the battery; see
 *                          crs_battery_mode_t.
 */
typedef void (*cr_power_battery_voltage_cb_t)(void *context,
                                              struct cr_power_t *power,
                                              uint32_t const charge_percentage,
                                              int32_t const current,
                                              uint32_t const voltage,
                                              int32_t const temperature,
                                              bool const is_critical,
                                              crs_battery_mode_t battery_mode);

/**
 * @brief Callback invoked when a request to reset, or power off the device has
 * been acknowledged.
 *
 * @param context User-supplied context pointer to the Power Feature.
 * @param power   Pointer to the Power Feature instance.
 */
typedef void (*cr_power_acknowledge_cb_t)(void *context, struct cr_power_t *power);

/**
 * @brief Callback invoked when there is an error with the device's battery.
 *
 * @details This callback is invoked asynchronously in response to a call to
 * #cr_power_get_battery_voltage().
 *
 * @param context User-supplied context pointer to the Power Feature.
 * @param power   Pointer to the Power Feature instance.
 */
typedef void (*cr_power_battery_error_cb_t)(void *context, struct cr_power_t *power);

/**
 * @brief Callback invoked when the power report configuration has been
 * updated.
 *
 * @details This callback is invoked asynchronously in response to a call to
 * #cr_power_set_soc_report_threshold(), or on a new connection when the
 * power report configuration is anything but the default values.
 *
 * @param context       User-supplied context poitner to the Power Feature.
 * @param power         Pointer to the Power Feature instance.
 * @param configuration Pointer to a configuration instance.
 */
typedef void (*cr_power_report_configuration_cb_t)(void *context,
                                                   struct cr_power_t *power,
                                                   const cr_power_report_configuration_t *configuration);

/**
 * @brief Type definition for the event API outlining the different callbacks
 * that are invoked by the Power Feature.
 */
typedef struct cr_power_event_api_t {
    /**
     * @brief Callback invoked when LCR receives a battery voltage report from
     * the device.
     *
     * @details A battery voltage report can be sent at any time by the device,
     * but is also sent asynchronously in response to
     * #cr_power_get_battery_voltage().
     */
    cr_power_battery_voltage_cb_t on_battery_voltage_response;

    /**
     * @brief Callback invoked when LCR cannot read the battery voltage
     * information from the device due to an error with the battery.
     *
     * @details This callback is invoked asynchronously in response to a call
     * to #cr_power_get_battery_voltage() if the device determines it is not
     * possible to produce a battery voltage report due to an error with the
     * physical battery.
     */
    cr_power_battery_error_cb_t on_battery_error;

    /**
     * @brief Callback invoked when LCR receives acknowledgement that the
     * device will soon reset in response to a call to #cr_power_reset().
     */
    cr_power_acknowledge_cb_t on_reset_acknowledged;

    /**
     * @brief Callback invoked when LCR receives acknowledgement that the
     * device will soon shut off in response to a call to #cr_power_off().
     */
    cr_power_acknowledge_cb_t on_off_acknowledged;

    /**
     * @brief Callback invoked when LCR receives acknowledgement that the
     * device has received, and processed a request to change the LPS
     * timeout; see #cr_power_set_lps_timeout().
     */
    cr_power_acknowledge_cb_t on_set_lps_timeout_acknowledged;

    /**
     * @brief Callback invoked when LCR receives a power report configuration
     * response from the device. This can be in response to a call to
     * #cr_power_set_soc_report_threshold(), but also on boot to inform the
     * host about when power reports will be sent by the device.
     */
    cr_power_report_configuration_cb_t on_power_report_configuration;

    /**
     * @brief User-specified pointer that will be passed to registered
     * callbacks.
     */
    void *context;
} cr_power_event_api_t;

/**
 * @brief Allocates the memory for a cr_power_t.
 *
 * @details The internals of the Power Feature are private to the module. In
 * order to get an instance of a Power Feature with the associated necessary
 * memory, this method must be called.
 *
 * @return Pointer to an allocated Power Feature.
 */
struct cr_power_t *cr_power_alloc(void);

/**
 * @brief Initializes a Power Feature instance (cr_power_t) with the specified
 * callbacks, and registers the feature with the specified Cardreader instance.
 *
 * @details This method initailizes an instance of the Power Feature with the
 * specified callbacks defined in the @p event_api, and registers the feature
 * with the specified @p cardreader.
 *
 * @param power      Pointer to the Power Feature instance.
 * @param cardreader Pointer to the Cardreader instance.
 * @param event_api  Pointer to the callback API.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note The Power Feature instance must have been allocated by a call to
 * #cr_power_alloc().
 *
 * @note This method must be called before calling other API functions on a
 * Power Feature.
 */
cr_power_result_t cr_power_init(struct cr_power_t *power,
                                struct cr_cardreader_t *cardreader,
                                cr_power_event_api_t const *event_api);

/**
 * @brief Terminates a Power Feature instance instantiated by a call to
 * #cr_power_init(), and de-registers the feature from Cardreader.
 *
 * @param power Pointer to the Power Feature instance.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note The Power Feature instance must have been initialized by a call to
 * #cr_power_init().
 *
 * @note The Power Feature instance should be terminated before freeing the
 * associated memory by calling #cr_power_free().
 */
cr_power_result_t cr_power_term(struct cr_power_t *power);

/**
 * @brief Frees an instance of a cr_power_t that was allocated by a call to
 * #cr_power_alloc().
 *
 * @param power Pointer to the Power Feature instance.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note The Power Feature instance must have been previously terminated by a
 * call to #cr_power_term().
 */
cr_power_result_t cr_power_free(struct cr_power_t *power);

/**
 * @brief Requests the device send a battery voltage report.
 *
 * @details This method allows for retrieving the battery voltage in addition
 * to other battery information. For full details, see
 * #cr_power_battery_voltage_cb_t().
 *
 * @param power Pointer to the Power Feature instance.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note When the battery voltage report is ready, the Power Feature's
 * #cr_power_event_api_t.on_battery_voltage_response  will be called
 * asynchronously.
 *
 * @note The Power Feature must have been instantiated by a call to
 * #cr_power_init() before this method is called.
 */
cr_power_result_t cr_power_get_battery_voltage(struct cr_power_t *power);

/**
 * @brief Requests that the device reset itself.
 *
 * @param power Pointer to the Power Feature instance.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note When the request has been acknowledged by the device, the Power
 * Feature's #cr_power_event_api_t.on_reset_acknowledged will be called
 * asynchronously.
 *
 * @note The Power Feature must have been instantiated by a call to
 * #cr_power_init() before this method is called.
 */
cr_power_result_t cr_power_reset(struct cr_power_t *power);

/**
 * @brief Requests that the device power off.
 *
 * @details Requests the device power off. If the device is externally powered,
 * then it is not possible to power it off; instead, the device will be reset.
 *
 * @param power Pointer to the Power Feature instance.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note When the request has been acknowledged by the device, the Power
 * Feature's #cr_power_event_api_t.on_off_acknowledged will be called
 * asynchronously.
 *
 * @note The Power Feature must have been instantiated by a call to
 * #cr_power_init() before this method is called.
 */
cr_power_result_t cr_power_off(struct cr_power_t *power);

/**
 * @brief Sends a request to the the device to change the LPS timeout.
 *
 * @details Devices with a battery feature a LPS (Low Power State) timeout
 * value, measured in milliseconds. Once the device has spent sufficient time
 * in a low power state such that the LPS timeout is reached, the device will
 * power off if it is not externally powered. This method provides the ability
 * to override the default timeout, and configure one's own value.
 *
 * @param power      Pointer to the Power Feature instance.
 * @param timeout_ms New LPS timeout value, in milliseconds.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * outlined in #cr_power_result_t.
 *
 * @note When the new timeout has been acknowledged by the device, the Power
 * Feature's #cr_power_event_api_t.on_set_lps_timeout_acknowledged will be
 * called.
 *
 * @note The Power Feature must have been instantiated by a call to
 * #cr_power_init() before this method is called.
 */
cr_power_result_t cr_power_set_lps_timeout(struct cr_power_t *power, uint32_t timeout_ms);

/**
 * @brief Converts a raw ADC value to a voltage value.
 *
 * @param power    Pointer to the Power Feature instance.
 * @param adc_val  The raw ADC value.
 *
 * @return Voltage measured in Volts.
 */
float cr_power_convert_adc_value_to_voltage(struct cr_power_t *power, float adc_val);

/**
 * @brief Sends a request to set the SOC (State of Charge) threshold upon which
 * battery voltage reports should be made.
 *
 * @details This method allows for a caller to request that the device send
 * battery reports pro-actively once a given threshold in battery percentage
 * is reached. For example, specifying an @p soc_threshold of `10`, `20`,
 * `30` tells the device to send a battery voltage report at `10%` charge,
 * `20%` charge, `30%` charge, etc.
 *
 * @param power         Pointer to the Power Feature instance.
 * @param soc_threshold SOC threshold, in percent, from `0 - 100%`.
 *
 * @return #CR_POWER_RESULT_SUCCESS on success, otherwise a failure code as
 * defined in #cr_power_result_t.
 *
 * @note A value of `0` given for @p soc_threshold can be used to disable the
 * pro-active reporting.
 *
 * @note The Power Feature must have been instantiated by a call to
 * #cr_power_init() before this method is called.
 *
 * @note When an SOC threshold has been set by the device, or if the SOC
 * threshold has previously been set and the device is re-connecting, the
 * current threshold will be sent, asynchronously, to the host by a call to
 * the Power Feature's #cr_power_event_api_t.on_power_report_configuration.
 */
cr_power_result_t cr_power_set_soc_report_threshold(struct cr_power_t *power, uint8_t soc_threshold);

/** @} */
