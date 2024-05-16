// Copyright(C) 2012-2014 Square, Inc. All Rights Reserved.

#pragma once

#include <cardreader/cr_comms_types.h>
#include <cardreader_shared/crs_link.h>
#include <stdint.h>
#include <stdbool.h>

#define CR_COMMS_MODEM_MAX_ENCODE_DATA_LEN (CRS_LINK_MAX_UNENCODED_PAYLOAD_LENGTH)

// Types

typedef struct cr_comms_modem_t cr_comms_modem_t;

// Functions

/*! Sets up state and allocates fast and slow packet demodulators once.

 \return Pointer to the modem control block.
 */
cr_comms_modem_t *
cr_comms_modem_init(void);

/*! Free all memory allocated to demodulators.

 \param modem Pointer to control block.

 This should be called when the modem structure is no longer needed.
 */
void
cr_comms_modem_shutdown(cr_comms_modem_t * modem);

/*! Uses cr_comms_modem_demod_sq_link and cr_comms_modem_decode_sq_link to demodulate and then decode a SqLink packet.

 \param modem Pointer to the control block containing demodulators.
 \param link Link type.
 \param samples Pointer to the samples.
 \param num_samples Number of samples.
 \param packet Pointer to return structure.
 \param stats Pointer to where stats are stored.

 \return Result of the decode.
 */
cr_comms_decode_result_t
cr_comms_modem_demod_and_decode_sq_link(cr_comms_modem_t * modem,
                                        cr_comms_link_type_t link,
                                        int16_t const * samples,
                                        uint32_t num_samples,
                                        cr_comms_packet_t * packet,
                                        cr_comms_sqlink_demod_stats_t * stats);
