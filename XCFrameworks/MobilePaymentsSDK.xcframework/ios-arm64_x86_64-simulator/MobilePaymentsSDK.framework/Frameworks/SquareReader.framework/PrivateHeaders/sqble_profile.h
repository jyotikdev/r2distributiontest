// Copyright 2017, Square Inc.

/**
 * @file
 *
 * @brief SQBLE Profile
 *
 * @details This library defines the types and methods for the Square (Block)
 * reader's BLE services and characteristics.
 *
 * @author Eldon Rivers <eldon@squareup.com>
 * @author Imran Khan <imran@squareup.com>
 */

/**
 * @addtogroup sqble_profile Square BLE Profile
 * @ingroup lib
 * @{
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Macro to reverse the endianness of a UUID (Unique Universal
 * Identifier).
 *
 * @note CC2640 and BTStack require opposite endianness UUIDs.
 */
#define REVERSE_UUID(x) {x[15], x[14], x[13], x[12], x[11], x[10], x[9], x[8], x[7], x[6], x[5], x[4], x[3], x[2], x[1], x[0]}

/**
 * @brief Size of a SQBLE transmission frame header (in bytes).
 */
#define SQBLE_PROFILE_FRAME_HDR_SIZE   (1u)

/**
 * @brief Maximum number of parallel transmission frames at any given point in
 * time.
 */
#define SQBLE_PROFILE_MAX_FRAME_COUNT  (8u)

/**
 * @brief Size of an ATT (Attribute) UUID (Unique Universal Identifier).
 */
#define ATT_UUID_SIZE                  (16u)

/**
 * @brief Enumeration of the different SQBLE profile characteristics. This
 * mapping acts a way of mapping identifiers to GATT DB characteristics.
 */
typedef enum {
    /**
     * @brief Undefined characteristic.
     */
    SQBLE_PROFILE_CHAR_NONE = -1,

    /**
     * @brief Reader RX characteristic. Used to transmit data from the host
     * (e.g. POS) to the reader.
     *
     * @note This characteristic is a 'Write' characteristic.
     */
    SQBLE_PROFILE_READER_RX_CHAR,

    /**
     * @brief Reader TX characteristic. Used to transmit data from the reader
     * to the host (e.g. POS).
     *
     * @note This characteristic is a 'Read - Indicate' characteristic.
     */
    SQBLE_PROFILE_READER_TX_CHAR,

    /**
     * @brief Maximum Transmission Unit characteristic. This characteristic is
     * used to transmit the MTU value that was negotiated between the BLE
     * server, and peripheral.
     *
     * @note This characteristic is 1-byte in size.
     *
     * @note This characteristic is a 'Read - Indicate' characteristic.
     */
    SQBLE_PROFILE_MTU_CHAR,

    /**
     * @brief Connection interval characteristic. This characteristic is used
     * to transmit the connection interval that was negotiated between the BLE
     * server, and peripheral. The value is multiplied by `1.25` to get the
     * total number of milliseconds between Bluetooth events.
     *
     * @note This characteristic is 1-byte in size.
     *
     * @note This characteristic is a 'Read' characteristic.
     */
    SQBLE_PROFILE_CONN_INTERVAL_CHAR,

    /**
     * @brief Bonding status characteristic. This characteristic is used to
     * communicate the current bonding status of the reader, and is one of
     * #sqble_profile_bond_status_t. When bonded to the currently connected
     * client, this is #SQBLE_PROFILE_BOND_STATUS_BONDED_TO_CONNECTED_PEER.
     *
     * @note This characteristic is 1-byte in size.
     *
     * @note This characteristic is a 'Read - Indicate' characteristic.
     */
    SQBLE_PROFILE_BOND_STATUS_CHAR,

    /**
     * @brief Connection control characteristic. This characteristic is used to
     * control different aspects of the BLE connection / transport. The BLE
     * client writes to this characteristic with a value from the enumeration
     * #sqble_profile_conn_control_t.
     *
     * @note This characteristic is 1-byte in size.
     *
     * @note This characteristic is a 'Write' characteristic.
     */
    SQBLE_PROFILE_CONN_CONTROL_CHAR,

    /**
     * @brief Communication version characteristic. This characteristic is used
     * to communicate a 3 byte-array comprised of the transport version,
     * application version, and endpoint version.
     *
     * @note This characteristic is variable in size (up to 30 bytes), but
     * generally 3-bytes in size.
     *
     * @note This characteristic is a 'Read' characteristic.
     */
    SQBLE_PROFILE_COMMS_VERSION_CHAR,

    /**
     * @brief Acknowledgement (ACK) vector characteristic. This characteristic
     * is used to acknowledge a RX frame received from a BLE client.
     *
     * @note This characteristic is 2-bytes in size.
     *
     * @note This characteristic is a 'Read' characteristic.
     */
    SQBLE_PROFILE_ACK_VECTOR_CHAR,

    /**
     * @brief Service version characteristic. This characteristic is used to
     * communicate the current version of the BLE profile ('SQBLE' profile)
     * within the reader. This value is incremented when there are
     * characteristic, or service changes between firmware versions.
     *
     * @note This characteristic is 1-byte in size.
     *
     * @note This characteristic is a 'Read' characteristic.
     */
    SQBLE_PROFILE_SERVICE_VERSION_CHAR,

    /**
     * @brief Total number of characteristics.
     */
    SQBLE_PROFILE_NUM_OF_CHARS
} sqble_profile_char_id_t;

/**
 * @brief Enumeration of bonding statuses.
 */
typedef enum {
    /**
     * @brief Device is currently not bonded to any peer.
     */
    SQBLE_PROFILE_BOND_STATUS_NOT_BONDED = 0,

    /**
     * @brief Bonding failed with the current peer.
     */
    SQBLE_PROFILE_BOND_STATUS_BONDING_FAILED,

    /**
     * @brief Bonding succeeded with the current peer.
     */
    SQBLE_PROFILE_BOND_STATUS_BONDED_TO_CONNECTED_PEER,

    /**
     * @brief Client wants to initiate bonding, but the reader is currently
     * bonded to another peer.
     */
    SQBLE_PROFILE_BOND_STATUS_BONDED_TO_OTHER_PEER
} sqble_profile_bond_status_t;

/**
 * @brief Enumeration of the connection control requests.
 */
typedef enum {
    /**
     * @brief Command to force the reader to forget the bonding information for
     * the currently connected client.
     */
    SQBLE_PROFILE_CONN_CONTROL_FORGET_BOND = 0,

    /**
     * @brief Command to check whether the current connected client is bonded
     * to the reader.
     */
    SQBLE_PROFILE_CONN_CONTROL_INITIATE_BONDING,

    /**
     * @brief Command to force re-negotiation of the negotiated MTU (Maximum
     * Transmission Unit).
     */
    SQBLE_PROFILE_CONN_CONTROL_EXCHANGE_MTU,

    /**
     * @brief Command to force re-negotiation of the negotiated connection
     * parameters.
     */
    SQBLE_PROFILE_CONN_CONTROL_UPDATE_CONN_PARAMS,

    /**
     * @brief Command to force encryption of the BLE comms. Encryption is
     * required to access certain GATT DB values.
     *
     * @note From BLE5+, encryption happens automatically when the BLE client
     * attempts to access an encrypted field; this encryption will force a
     * re-pairing.
     */
    SQBLE_PROFILE_CONN_CONTROL_SEND_SLAVE_SECURITY_REQ,

    /**
     * @brief Command to force the reader to disconnect from the currently
     * connected BLE client.
     */
    SQBLE_PROFILE_CONN_CONTROL_DISCONNECT,

    /**
     * @brief Forces a service change indication. A service change indication
     * is used by the server to indicate that a range of characteristics were
     * added or removed from the GATT database, and that the Bluetooth client
     * should re-fetch them if it has them cached.
     *
     * @note For GATT databases that contain the 'Generic Attribute Service',
     * and 'Service Changed' characteristic, the stack will monitor local
     * database changes, and send out the indications to the clients on behalf
     * of the application. In this case, it is not necessary for this command
     * to be used.
     *
     * @note In BLE5+ with GATT caching, the Bluetooth client does not need to
     * send this command, as it will be handled by the stack of the Bluetooth
     * server, and the client.
     *
     * @see https://docs.silabs.com/bluetooth/latest/general/gatt-protocol/service-change-indication
     */
    SQBLE_PROFILE_CONN_CONTROL_INDICATE_SERVICE_CHANGE,

    /**
     * @brief Command to reset the transport layer. This wipes the ACK vector,
     * and dumps any recorded RX frames.
     */
    SQBLE_PROFILE_CONN_CONTROL_RESET_TRANSPORT,
} sqble_profile_conn_control_t;

/**
 * @brief Header for a a SQBLE transport frame.
 */
typedef struct __attribute__((packed)) {
    /**
     * @brief Sequence number for this frame.
     */
    uint8_t sequence;
} sqble_profile_frame_hdr_t;

/**
 * @brief Struct defining the order of bytes in the BLE advertising manufacturer data.
 */
typedef struct __attribute__((packed)) {
    /**
     * @brief 16-bit company identifier in little-endian format.
     */
    uint8_t company_identifier[2];

    /**
     * @brief flag to indicate to LCR if it should request pairing.
     */
    uint8_t should_request_pairing;

    /**
     * @brief Pointer to the start of an optional extended payload, if present.
     */
    uint8_t payload[0];
} sqble_profile_advertising_mfg_data_t;

/**
 * @brief Structure representing a TX or RX frame.
 */
typedef struct __attribute__((packed)) {
    /**
     * @brief Length of the data of the frame in bytes.
     */
    uint16_t data_len;

    /**
     * @brief Structure encapsulating the received or transmitted data.
     */
    struct __attribute__((packed)) {
        /**
         * @brief Header for the frame.
         */
        sqble_profile_frame_hdr_t hdr;

        /**
         * @brief Body for the frame.
         */
        uint8_t body[];
    } data;
} sqble_profile_frame_t;

/**
 * @brief Type definition for an ATT UUID.
 */
typedef const uint8_t att_uuid_t[ATT_UUID_SIZE];

/**
 * @brief UUID for the SQBLE service.
 */
extern att_uuid_t sqble_profile_service_uuid;

/**
 * @brief UUID for the RX characteristic.
 */
extern att_uuid_t sqble_profile_reader_rx_char_uuid;

/**
 * @brief UUID for the TX characteristic.
 */
extern att_uuid_t sqble_profile_reader_tx_char_uuid;

/**
 * @brief UUID for the MTU characteristic.
 */
extern att_uuid_t sqble_profile_mtu_char_uuid;

/**
 * @brief UUID for the connection interval characteristic.
 */
extern att_uuid_t sqble_profile_conn_interval_char_uuid;

/**
 * @brief UUID for the bonding status characteristic.
 */
extern att_uuid_t sqble_profile_bond_status_char_uuid;

/**
 * @brief UUID for the connection control characteristic.
 */
extern att_uuid_t sqble_profile_conn_control_char_uuid;

/**
 * @brief UUID for the communication version characteristic.
 */
extern att_uuid_t sqble_profile_comms_version_char_uuid;

/**
 * @brief UUID for the ACK vector characteristic.
 */
extern att_uuid_t sqble_profile_ack_vector_char_uuid;

/**
 * @brief UUID for the service version characteristic.
 */
extern att_uuid_t sqble_profile_service_version_char_uuid;

/**
 * @brief Returns whether a sequence number is in the window for the next
 * frames that are expected to be received / transmitted.
 *
 * @details This method determines whether a sequence number is expected or
 * not. The sequence number must be at least equal to the next expected
 * sequence number, and less than the maximum allowed next sequence number.
 *
 * @param seq_num      The sequence number of the frame.
 * @param next_seq_num The next expected frame sequence number.
 *
 * @return 'true' if number is in window, otherwise 'false'.
 */
static inline bool sqble_profile_seq_num_in_window(uint8_t seq_num, uint8_t next_seq_num)
{
    uint16_t window_start;
    uint16_t window_end;

    window_start = next_seq_num;
    window_end = (uint16_t)next_seq_num + SQBLE_PROFILE_MAX_FRAME_COUNT - 1u;

    if (window_end > 0xFFu) {
        return ((seq_num >= window_start) || (seq_num <= (window_end % SQBLE_PROFILE_MAX_FRAME_COUNT)));
    }
    return ((seq_num >= window_start) && (seq_num <= window_end));
}

/**
 * @brief Converts a given UUID to a string.
 *
 * @param uuid Pointer to the UUID of the characteristic or service.
 *
 * @return Pointer to a constant string name for the characteristic, or service
 * if found, otherwise `NULL`.
 */
const char *sqble_profile_uuid_to_pretty_name(att_uuid_t uuid);

/** @} */
