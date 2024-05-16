// Copyright(C) 2013-2014 Square, Inc. All Rights Reserved.

#pragma once

#include <cardreader/M1Packet.h>
#include <cardreader/SqLinkPacket.h>
#include <cardreader_shared/crs_coder_in.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
#define X(sym, name) CR_COMMS_LINK_TYPE_ ## sym,
#include <cardreader/cr_comms_link_types.def>
#undef X
} cr_comms_link_type_t;

typedef enum {
#define X(sym, name) CR_COMMS_DECODE_RESULT_ ## sym,
#include <cardreader/cr_comms_results.def>
#undef X
} cr_comms_decode_result_t;

extern char *cr_comms_decode_result_str[];

/* This is technically 2400 according to the spec, but classification is better
 * when set to 2375 Hz.
 */
#define CR_COMMS_O1_BAUD (2375)
#define CR_COMMS_M1_FAST_BAUD (6000)
#define CR_COMMS_M1_SLOW_BAUD (1500)

/* Desired sample rate.  Input might be different, but samples will be interpolated
 * to match this sample rate for classification and demodulation.
 */
#define CR_COMMS_SAMPLE_RATE (44100)

typedef struct
{
    /* Multiplier and adder on the current baseline above which a packet is
     * considered to have started.
     */
    float packetStartBaselineMultiplier;
    uint32_t packetStartBaselineAdder;

    /* Multiplier on the packet start threshold below which a packet is considered
     * to have ended.
     */
    float packetThresholdEndMultiplier;

    /* If a packet was a candidate for restarting and if the short-window signal
     * strength is larger than this factor times the long-window signal strength,
     * then the packet is restarted.  This is to account for false packet starts
     * just above the baseline that continue onto a real packet.
     */
    float packetRestartMultiplier;

    /* Multiplier on the packet restart threshold below which a packet is considered
     * to have ended.
     */
    float packetRestartThresholdEndMultiplier;

    /* Multiplier on the long-window signal strength that defines the threshold for
     * the end of a packet in progress.
     */
    float packetEndMultiplier;

    /* The number of consecutive frames below the threshold a Gen2 packet must be
     * before the packet is considered complete.
     */
    uint32_t framesBelowLimitGen2;

    /* The number of consecutive frames below the threshold an O1/M1 packet must be
     * before the packet is considered complete.
     */
    uint32_t framesBelowLimitNonGen2;

    /* Config parameters for the short-window signal strength averager. */
    uint32_t shortAbsAveragerSize;
    uint32_t shortAbsAveragerInitTriggerCount;
    uint32_t shortAbsAveragerTriggerCount;

    /* Config parameters for the long-window signal strength averager. */
    uint32_t longAbsAveragerSize;
    uint32_t longAbsAveragerDelay;

    /* Size of the averager used for removing the DC offset for HW that doesn't have
     * fast AGC.
     */
    uint32_t averagerSize;

    /* Minimum number of samples needed before classifying a packet. */
    uint32_t minSamplesForClassify;

    /* Number of samples padded on the front of a Gen2 packet after it has been detected. */
    uint32_t startPaddingGen2;

    /* Number of samples padded on the front of an O1/M1 packet after it has been detected. */
    uint32_t startPaddingNonGen2;

    /* Number of samples padded on the end of an O1 packet after it has been detected. */
    uint32_t endPaddingO1;

    /* Packets that start before this time are considered early.  Any failure should be
     * ignored.
     */
    float earlyStartThresholdSec;
} cr_comms_carrier_detect_config_t;

typedef struct
{
    /* Enables the low-pass filter before Gen2 analysis. */
    bool lowPassBeforeGen2Analysis;

    /* Score threshold for a packet to be classified as an M1 Fast packet. */
    float m1FastScoreThreshold;

    /* Score threshold for a packet to be classified as an M1 Slow packet. */
    float m1SlowScoreThreshold;

    /* Score threshold for a packet to be classified as an O1 packet. */
    float o1ScoreThreshold;

    /* Score threshold for a packet to be classified as an Gen2 packet. */
    float gen2ScoreThreshold;

    /* Step size (multiplier on FFT length) when summing FFTs. */
    float fftMultStep;

    /* Max multiplier when summing FFTs. */
    float fftMultMax;

    /* Window length in samples when looking for peaks when classifying Gen2 packets. */
    uint32_t peakWindowLen;

    /* Multiplier on the magnitude of the last Gen2 peak that the current peak must
     * exceed to be considered a peak.
     */
    float lastPeakMagMult;

    /* Minimum Gen2 peak magnitude. */
    float peakMagMin;

    /* Decay of the magnitude of Gen2 peaks, to prevent high amplitude noise from
     * crippling peak detection.
     */
    float lastPeakMagDecay;

    /* Minimum harmonic factor when analyzing the O1 and M1 packets, which have
     * fixed frequencies.
     */
    float harmonicFactorMin;

    /* Maximum harmonic factor. */
    float harmonicFactorMax;

    /* Harmonic factor step size. */
    float harmonicFactorStep;
} cr_comms_classify_config_t;

typedef enum
{
    CR_COMMS_DEMODULATOR_FILTER_NONE,
    CR_COMMS_DEMODULATOR_FILTER_CHEBYSHEV,
    CR_COMMS_DEMODULATOR_FILTER_FIR_9K
} cr_comms_demodulator_filter_t;

typedef struct
{
    /* Tolerance when looking for the preamble frequency. */
    float freqTolerance;

    /* Type of filter to run on the packet before demodulation. */
    cr_comms_demodulator_filter_t filter;

    /* Offset in seconds of the carrier detected signal where the start of the
     * preamble is approximately located for preamble frequency estimation.
     */
    float preambleStartSec;

    /* Offset in seconds of the carrier detected signal where the end of the
     * preamble is approximately located for approximating the number of sub-packets.
     */
    float preambleEndSec;

    /* Offset in seconds of the carrier detected signal to start looking for the first sync. */
    float firstSyncSearchStartSec;

    /* Offset in seconds of the carrier detected signal to stop looking for the first sync. */
    float firstSyncSearchEndSec;

    /* Tolerance for where to look for the sync after the first, based on the previous
     * sync location and frequency.
     */
    float nextSyncSearchTolerance;
} cr_comms_demod_config_t;

typedef struct
{
    uint64_t totalRuntime;
    uint64_t totalTime;
} cr_comms_carrier_detect_stats_t;

typedef struct
{
    uint32_t runtime;
    uint32_t lowPassFilterRuntime;
    uint32_t analyzeRfRuntime;
    uint32_t calcSquareAndRemoveMeanRuntime;
    uint32_t fftsRuntime;
    uint32_t analyzeFftsRuntime;
    uint32_t gen2LowPassFilterRuntime;
    uint32_t normalizeAndCenterAroundMeanRuntime;
    uint32_t findPeaksRuntime;
    uint32_t calcSpacingsAndVariabilityRuntime;
    float rfScore;
    float o1Score;
    float m1FastScore;
    float m1SlowScore;
    float gen2Score;
    int peakCount;
    float peakVar;
} cr_comms_classify_stats_t;

typedef struct
{
    uint64_t totalRuntime;
    uint64_t totalTime;
    uint16_t startThreshold;
    uint16_t startAvg;
    uint16_t endThreshold;
    uint16_t endAvg;
    bool allowRestart;
    bool classify1StatsValid;
    cr_comms_link_type_t classify1Result;
    cr_comms_classify_stats_t classify1Stats;
    bool classify2StatsValid;
    cr_comms_link_type_t classify2Result;
    cr_comms_classify_stats_t classify2Stats;
    uint32_t inPacketRuntime;
} cr_comms_carrier_detect_packet_stats_t;

typedef struct
{
    bool isFast;
    uint32_t runtime;
    uint32_t lowPassFilterRuntime;
    uint32_t findPreambleFreqRuntime;
    uint32_t firstFindSyncRuntime;
    uint32_t restFindSyncRuntime;
    uint32_t demodulatePacketsRuntime;
    uint32_t deconvolveRuntime;
    uint32_t viterbiRuntime;
    bool inverted;
    float preambleFreq;
    uint32_t syncIndexes[CRS_IN_MAX_SUBPACKET_COUNT];
    float packetFrequencies[CRS_IN_MAX_SUBPACKET_COUNT];
    uint32_t deconvolveBitErrors;
    uint32_t deconvolveNumBits;
} cr_comms_sqlink_demod_stats_t;

/* SqLink results. */
typedef struct
{
    /* Packet data. */
    SqLinkPacketHeader_t * hdrPtr;
    M1PacketCardDataPlainText_t * plainTextPtr;
    M1PacketCardDataAuthenticated_t * authenticatedPtr;
    M1PacketCardDataEncrypted_t * encryptedPtr;
} cr_comms_sqlink_results_t;

typedef struct
{
    /* Overall demodulation results. */
    uint8_t * packetPtr;
    uint32_t packetBytes;
    crs_coder_in_reader_t readerType;
    cr_comms_sqlink_results_t sqLink;
} cr_comms_packet_t;

/*! Events that the modem can generate.
 *
 * Some of the events require no client action, but all events should be logged.
 */
typedef enum {
#define X(SYM, DESC) CR_COMMS_EVENT_CARRIER_DETECT_ ## SYM,
#include <cardreader/cr_comms_events.def>
#undef X
} cr_comms_event_t;

/*! Event data. */
typedef struct
{
    /* The type of packet that this is classified as.  Only valid if this is a
     * success event.
     */
    cr_comms_link_type_t linkType;

    /* The offset since the last plug in event in samples of the start of the
     * packet. This can be used for placing in time two packets relative to one
     * another.
     */
    uint64_t packetStart;

    /* Number of samples in the packet. */
    uint32_t numSamples;

    /* Pointer to the samples. This is memory shared with GumPack and should not
     * be modified. Also, this data can change at any time after the callback
     * returns, so it is safest to copy this data into another block of memory
     * if not using it immediately.
     */
    const int16_t *samplesPtr;

    /* A flag that notes if this packet arriving quickly after the plug in event.
     * If this packet fails demodulation, it can be ignored. Examples of early
     * packets are the packet that O1 sends on plug in and the Awake packet that
     * R4 sends on power up.
     */
    bool isEarlyPacket;

    /* Statistics structure for logging purposes only. */
    cr_comms_carrier_detect_packet_stats_t stats;
} cr_comms_event_data_t;

/*! Function called when an event occurs. */
typedef void (*cr_comms_event_handler_t)(void *context,
                                         cr_comms_event_t event,
                                         cr_comms_event_data_t *event_data);

typedef enum
{
    CR_COMMS_ENCODE_RESULT_SUCCESS,
    CR_COMMS_ENCODE_RESULT_NOT_READY,
    CR_COMMS_ENCODE_RESULT_INVALID_LEN,
} cr_comms_encode_result_t;

typedef struct cr_comms_wav_t
{
    uint32_t sampleRate;
    uint8_t numChannels;
    uint32_t numSamples;
    int16_t *samplesPtr;
} cr_comms_wav_t;

typedef struct cr_comms_build_wav_stats_t
{
    uint8_t dummy;
} cr_comms_build_wav_stats_t;

typedef enum
{
    CR_COMMS_CHANNEL_LEFT,
    CR_COMMS_CHANNEL_RIGHT
} cr_comms_channel_t;

typedef enum
{
    CR_COMMS_CHANNEL_DATA_TYPE_DATA,
    CR_COMMS_CHANNEL_DATA_TYPE_NONE,
    CR_COMMS_CHANNEL_DATA_TYPE_BLANK,
    CR_COMMS_CHANNEL_DATA_TYPE_INVERSE,
    CR_COMMS_CHANNEL_DATA_TYPE_SAME,
    CR_COMMS_CHANNEL_DATA_TYPE_INTERFERENCE
} cr_comms_channel_data_type_t;
