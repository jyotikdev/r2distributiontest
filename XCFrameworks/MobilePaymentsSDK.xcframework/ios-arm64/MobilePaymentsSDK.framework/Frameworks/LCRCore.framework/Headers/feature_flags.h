// Copyright 2020 Square, Inc.

/**
 * @file
 *
 * @brief Feature Flag Library
 *
 * @details The feature flag module provides an interface for managing feature
 * flags on the host.  Flags are received from the Feature Flag service, and
 * parsed / cached locally, controlling behaviour on the host.  Flags can be
 * sent downstream to the connected device in order to alter its behaviour.
 *
 * @author Ford Peprah <ford@squareup.com>
 *
 * @see https://docs.google.com/document/d/1K8MEboJsG87MtFLEp5t0yw4FNhxnezbIPmr2n4Fqmd4
 */

/**
 * @addtogroup feature_flag_lib Feature Flag Library
 * @ingroup lib
 * @{
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Type definition for a feature flag set.
 */
typedef struct feature_flag_set_t feature_flag_set_t;

/**
 * @brief Definition for the handle to a feature flag subsystem instance.
 */
typedef feature_flag_set_t * feature_flag_handle_t;

/**
 * @brief Definition for the flag identifiers.
 */
typedef int16_t feature_flag_id_t;

/**
 * @brief Maximum number of feature flags for any subsystem.
 */
#define MAX_NUM_FEATURE_FLAGS        (256)


/**
 * @brief Maximum number of feature sets allowed.
 */
#define MAX_NUM_FEATURE_SETS         (0xFFFF)

/**
 * @brief Invalid handle for a feature flag set.
 */
#define FEATURE_FLAG_INVALID_HANDLE  ((feature_flag_handle_t)NULL)

/**
 * @brief Invalid identifier for a feature flag key.
 */
#define FEATURE_FLAG_INVALID_ID      (-1)

/**
 * @brief Enumeration of the different feature flag subsystems.
 */
typedef enum {
    /**
     * @brief Enumeration for the feature flag set for readers.
     */
    FEATURE_FLAG_SUBSYSTEM_READER = (0),

    /**
     * @brief Enumeration for the feature flag set for LCR (LibCardreader).
     */
    FEATURE_FLAG_SUBSYSTEM_LCR    = (1),

    /**
     * @brief Number of feature flag subsystems.
     */
    NUM_FEATURE_FLAG_SUBSYSTEMS
} feature_flag_subsystem_t;

/**
 * @brief Initializes the feature flag module.
 */
void feature_flag_init(void);

/**
 * @brief Allocates a feature set for a specific subsystem.
 *
 * @param subsystem Enumeration specifying the feature flag subsystem.
 * @param num_flags Number of flags to use for the specific subsystem.
 *
 * @return Handle to the allocated feature flag set instance.
 *
 * @note Returns `FEATURE_FLAG_INVALID_HANDLE` on failure.
 */
feature_flag_handle_t feature_flag_alloc(feature_flag_subsystem_t subsystem, size_t num_flags);

/**
 * @brief Frees a previously allocated feature flag set instance specified by
 * the given handle.
 *
 * @param handle Handle to the allocated feature flag set instance.
 *
 * @note Handle must correspond to a valid handle returned by
 * `feature_flag_alloc()` that has not already been fried.
 */
void feature_flag_free(feature_flag_handle_t handle);

/**
 * @brief Returns the number of feature flags present for a given subsystem in
 * order to be set.
 *
 * @param subsystem Enumeration specifying the feature flag subsystem.
 *
 * @return Number of flags defined, compiled, for the specific subsystem.
 */
uint8_t feature_flag_get_subsystem_num_flags(feature_flag_subsystem_t subsystem);

/**
 * @brief Returns the number of features flags present for the set specified by
 * the given handle.  A value of `0` is returned if the handle is invalid, or
 * no flags are available.
 *
 * @param handle Handle to the feature flag set.
 *
 * @return Number of flags available in the given set.
 */
uint16_t feature_flag_get_num_flags(feature_flag_handle_t handle);

/**
 * @brief Returns the feature flag identifier for feature with the given name.
 * If not found, then `FEATURE_FLAG_INVALID_ID` is returned instead.
 *
 * @param subsystem    The subsystem to find the feature within.
 * @param feature_name The name of the feature.
 *
 * @return The identifier for the feature flag.
 */
feature_flag_id_t feature_flag_get_flag_id(feature_flag_subsystem_t subsystem, const char *feature_name);

/**
 * @brief Returns the current set value of the specified feature under the
 * feature set indicated by the provided handle.  If no flag is found, then
 * an undefined value is returned.
 *
 * @param handle  Handle for the feature flag set.
 * @param flag    The enumeration / key value for the feature flag.
 * @param version The version of the device corresponding to the subsystem of
 *                the specified feature set.
 *
 * @return The value of the specified feature flag.
 */
uint8_t feature_flag_get_flag(feature_flag_handle_t handle, feature_flag_id_t flag, uint32_t version);

/**
 * @brief Returns the default value of the specified feature under the
 * specified feature subsystem.
 *
 * @param subsystem Feature flag subsystem.
 * @param flag      Value of the flag (key/enumeration) to get the default
 *                  value for.
 *
 * @return The default value of the specified flag.
 */
uint8_t feature_flag_get_subsystem_flag_value(feature_flag_subsystem_t subsystem, feature_flag_id_t flag);

/**
 * @brief Returns the name for a given feature flag corresponding to the given
 * subsystem.  If the flag does not exist, then a `NULL` value is returned.
 *
 * @param subsystem Feature flag subsystem.
 * @param flag      Value of the flag (key/enumeration) to find.
 *
 * @return Pointer to the name of the flag if found.
 */
const char *feature_flag_get_flag_name(feature_flag_subsystem_t subsystem, feature_flag_id_t flag);

/**
 * @brief Sets the value for a given feature in the feature set specified by
 * the given handle.  The given feature name will be parsed to match against
 * available features, and version information.
 *
 * @param handle       Handle for the feature flag set.
 * @param feature_name Name of the feature to set.
 * @param value        Value to set for the feature.
 *
 * @return `True` if the value was set, otherwise `False`.
 */
bool feature_flag_set_flag(feature_flag_handle_t handle, const char *feature_name, uint8_t value);

/** @} */
