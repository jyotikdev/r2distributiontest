// Copyright 2017 Square, Inc.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum {
    CRP_SPI_DEFAULT_BAUD_RATE = 2500000u, // 2.5 MHz
};

/**
 * @enum crp_spi_status_t
 * @brief SPI status codes.
 */
typedef enum {
    /**
     * @brief SPI operation was successful.
     */
    CRP_SPI_STATUS_SUCCESS = 0,

    /**
     * @brief Unable to retrieve the number of SPI channels.
     */
    CRP_SPI_STATUS_CANNOT_GET_NUM_CHANNELS,

    /**
     * @brief No SPI channels are available.
     */
    CRP_SPI_STATUS_NO_SPI_CHANNELS_AVAILABLE,

    /**
     * @brief Unable to open the SPI channel.
     */
    CRP_SPI_STATUS_CANNOT_OPEN_SPI_CHANNEL,

    /**
     * @brief Unable to initialize the SPI channel.
     */
    CRP_SPI_STATUS_CANNOT_INIT_CHANNEL,

    /**
     * @brief Unable to close the SPI channel.
     */
    CRP_SPI_STATUS_CANNOT_CLOSE_CHANNEL,

    /**
     * @brief General SPI error.
     */
    CRP_SPI_STATUS_ERROR_GENERAL
} crp_spi_status_t;

/**
 * @brief Initialize the SPI interface and configure the parameters.
 *
 * @param baud Baud rate for the SPI interface. Pass in 0 to use the default baud rate.
 * @return crp_spi_status_t Status of the SPI initialization.
 */
crp_spi_status_t crp_spi_init(uint32_t baud);

/**
 * @brief Cleanup the SPI interface.
 *
 * @return crp_spi_status_t Status of the SPI deinitialization.
 */
crp_spi_status_t crp_spi_quit(void);

/**
 * @brief Send data over the SPI interface.
 *
 * @param send_buf Buffer containing the data to send.
 * @param len Length of the data to send in bytes.
 *
 * @return bool True if the send was successful, false otherwise.
 */
bool crp_spi_send(uint8_t *send_buf, size_t len);

/**
 * @brief Read data from the SPI interface.
 *
 * @param recv_buf Buffer to store the received data.
 * @param len Length of the data to read in bytes.
 *
 * @return bool True if the read was successful, false otherwise.
 */
bool crp_spi_read(uint8_t *recv_buf, size_t len);

/**
 * @brief Transmit data over the SPI interface and receive response.
 *
 * @param send_buf Buffer containing the data to transmit.
 * @param recv_buf Buffer to store the received data.
 * @param len Length of the data to transmit and receive in bytes.
 *
 * @return bool True if the transmission was successful, false otherwise.
 */
bool crp_spi_transmit(uint8_t *send_buf, uint8_t *recv_buf, size_t len);
