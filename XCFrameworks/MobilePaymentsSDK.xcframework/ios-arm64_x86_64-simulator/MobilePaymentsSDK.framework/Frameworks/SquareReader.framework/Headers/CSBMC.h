//
//  CSBMC.h
//  CoreSwipe
//
//  Created by Sam Wen on 1/13/10.
//  Copyright (c) 2013 Square, Inc. All rights reserved.
//
//  CSSquare is used to decode F2F (also called Aiken Biphase, Biphase Mark
//  Code) analog signals.
//

#import <SquareReader/CSUtility.h>


typedef struct {
    CSRawSignalRef signal;
    long *features;
    int featuresCount;
} CSBMCSignal;

typedef CSBMCSignal *CSBMCSignalRef;

typedef struct {
    CSBitArrayRef bitBuffer;
    CSTrackRef track;
} CSBMCTrack;

typedef CSBMCTrack *CSBMCTrackRef;

typedef struct {
    int wordLength;
    const char *symbolTable;
    int symbolTableLength;
    CSBitArrayRef startSentinel;
    CSBitArrayRef separator;
    CSBitArrayRef endSentinel;
    bool *columnParity;
    size_t minimumDecodedLength;
    int minimumCardDataLength;
    int maximumCardDataLength;
    int minimumCardPresentDataLength;
    int maximumCardPresentDataLength;
    CSTrackType trackType;
} CSBMCDecoder;

typedef CSBMCDecoder *CSBMCDecoderRef;

// These are defines because they are used to define the length of constant arrays
#define CSBMCTrack1SymbolTableLength 64
#define CSBMCTrack2SymbolTableLength 16
#define CSBMCTrack1WordLength 7
#define CSBMCTrack2WordLength 5

extern const char CSBMCTrack1SymbolTable[CSBMCTrack1SymbolTableLength + 1];
extern const char CSBMCTrack2SymbolTable[CSBMCTrack2SymbolTableLength + 1];

extern const int CSBMCSignalPeakBufferMaximumLength;
extern const int CSBMCSignalPeakBufferMaximumLengthTrack1;
extern const int CSBMCSignalPeakBufferMinimumLengthTrack1;
extern const int CSBMCSignalPeakBufferMaximumLengthTrack2;
extern const int CSBMCSignalPeakBufferMinimumLengthTrack2;
extern const int CSBMCTrack1BitBufferMaximumLength;
extern const int CSBMCTrack1BitBufferMinimumLength;
extern const int CSBMCTrack2BitBufferMaximumLength;
extern const int CSBMCTrack2BitBufferMinimumLength;
extern const size_t CSBMCTrack1DecodedMaximumLength;
extern const size_t CSBMCTrack1DecodedMinimumLength;
extern const size_t CSBMCTrack2DecodedMaximumLength;
extern const size_t CSBMCTrack2DecodedMinimumLength;
extern const int CSBMCTrack1CardPresentDataMinimumLength;
extern const int CSBMCTrack1CardPresentDataMaximumLength;
extern const int CSBMCTrack1CardDataMinimumLength;
extern const int CSBMCTrack1CardDataMaximumLength;
extern const int CSBMCTrack2CardPresentDataMinimumLength;
extern const int CSBMCTrack2CardPresentDataMaximumLength;
extern const int CSBMCTrack2CardDataMinimumLength;
extern const int CSBMCTrack2CardDataMaximumLength;

extern const uint32_t CSBMCAnalogInterfaceControllerSamplingRate;


/**
 * Signal Functions
 *
 * These functions initializes and frees a signal object that holds the original,
 * raw signal and peak information. The function expects a non-null array, or
 * throws an assertion.
 *
 * Is accompanied by a release method that does not require a non-null
 * signal.
 */
CSBMCSignalRef CSBMCSignalCreate(const short *rawSignal, const long rawSignalLength);
void CSBMCSignalRelease(CSBMCSignalRef signal);

/**
 * Inplace Signal Reversal
 *
 * Reverses the original raw signal in-place to be run through the algorithm in
 * reverse order. Expects a non-null signal or throws an assertion.
 */
void CSBMCSignalReverse(CSBMCSignalRef signal);

/**
 * Inplace Peak Reversal
 *
 * Reverses the ordering of the peaks as well as the signal.
 */
void CSBMCSignalPeaksReverse(CSBMCSignalRef signal);

/**
 * Track Functions
 *
 * Functions that creates and release track information, which contains the bit
 * information and the decoded information.
 *
 * Also has a helper function that attempts to determine the track type based on
 * the input signal. Expects a non-null signal or throws an assertion.
 */
CSBMCTrackRef CSBMCTrackCreate(CSTrackType trackType);
void CSBMCTrackRelease(CSBMCTrackRef track);
CSTrackType CSBMCTrackGuessTrackType(CSBMCSignalRef signal);

/**
 * Decoders Functions
 *
 * Decoders contain character set information that is used to decode a track.
 * The first type functions create and release a decoder object, which is
 * created based on a track type.
 *
 * CoreSquare provides a method that converts a stream of bits contained
 * in a track into a decoded stream of characters in the track object.
 *
 * All functions expect non-null objects, and tracktype must be either track1
 * or track2, or throws an assertion.
 */
CSBMCDecoderRef CSBMCDecoderCreate(CSTrackType trackType);
void CSBMCDecoderRelease(CSBMCDecoderRef decoder);
bool CSBMCDecoderConvertBitsToWords(CSBMCDecoderRef decoder, CSBMCTrackRef track);

/**
 * Oersted Detector
 * 
 * The Oersted Detector detects features for the Gen3 reader, which we've called
 * Oersted. This is a stream of binary high-low data that literally represents 0s
 * and 1s, sent to us at 2400 baud (2400 bits per second). When we take into
 * account the sampling rate that we set all of our decoding at (48 kHz), this
 * works out to ~20 samples per bit.
 *
 * This algorithm is fairly simple. It uses a window to determine relative
 * highs and lows in order to determine a threshold for transitions. This
 * results in an adaptive progression of feature detection to account for
 * DC drift, possible if there are a large number of consecutive bits of
 * the same polarity.
 */
void CSBMCDetectOersted(CSBMCSignalRef signal);

/**
 * Reactive Peak Detector
 *
 * This algorithm consists of a peak detector that steps through the signal
 * and attempts to obtain the "correct" peak each time. The peak bagging
 * algorithm is purely heuristic, and goes something like this:
 *
 * 1. Locate either a positive or negative peak, indicated by a sign change
 *    in slope.
 * 2. Locate the next peak, indicated by a change in slope.
 * 3. Check if the found peak is the same polarity as the previous peak, and if
 *    so, take the second peak. (This accounts for ringing caused by some
 *    adaptive audio filters).
 * 4. If we detect a series of "wiggles" in the signal, this means we are
 *    clipping, and we take the midpoint of the threshold crossings we use
 *    to detect a peak.
 *
 * Once the peaks are bagged, the digitizer steps through all found peaks and
 * compares the distances between peaks to determine 0 or 1.
 */
void CSBMCDetectPeaksReactive(CSBMCSignalRef signal, short signalFloor, short adaptivePercent, short hysteresisPercent);

/**
 * Predictive Peak Detector
 *
 * The predictive algorithm has a simpler peak bagger than Reactive.  It attempts
 * to bag as many peaks as possible. The digitizer, however, requires a seed for
 * the bit length frequency, and seeks until it finds a peak that's equal to the
 * bit length, or a peak before and after the bit length. It either selects the
 * location in the signal that's equal to the bit length, or the peak that's
 * nearest to the bit length. If there is a tie, the detector prefers the peak
 * that occurs earlier in time since people tend to accelerate.
 *
 * Finally, we check the polarity of the selected peak to determine 0 or 1.
 */
void CSBMCDetectPeaksPredictive(CSBMCSignalRef signal, short signalFloor);

/**
 * Local Maxima Peak Detector
 *
 * This particular peak detector simply searches for the highest peak in a window
 * of signal that exceeds a minimum noise threshold, and uses these as peaks.
 */
void CSBMCDetectPeaksLocalMaxima(CSBMCSignalRef signal, short signalFloor);

/**
 * Reactive Peak Digitizer
 *
 * This is a simple peak digitizer that steps through each peak, examines the
 * distances between them, and determines the bit.
 */
bool CSBMCDigitizePeaksReactive(CSBMCSignalRef signal, CSBMCTrackRef track, float seedDistanceRange);

/**
 * Predictive Peak Digitizer
 *
 * This digitizer attempts to seed with an appropriate bit length, and steps
 * through peaks until the expected bit length is found. It uses this location
 * to look forward and backward at adjacent peaks, and uses the peak polarity to
 * determine the bit. This digitizer works much better on noisier and dirtier
 * signals.
 */
bool CSBMCDigitizePeaksPredictiveDistance(const CSBMCSignalRef signal, CSBMCTrackRef track, float seedDistanceRange);

/**
 * Translation of Oersted features to ASCII
 *
 * Feature translation of an Oersted feature set is done by looking at the
 * polarity of the first feature sample. If this is positive, then we determine
 * that we have an inverted signal; otherwise, it is a noninverted signal. Once
 * we've done this, we record 1s and 0s based on relative positions of feature
 * locations in relationship to the ~20 samples/bit.
 *
 * For example, if the first bit is positive, and feature locations exist at
 * sample numbers:
 *
 *      0, 20, 60, 100
 *
 * Then the bit stream is:
 *
 *      01100
 *
 */
bool CSBMCOerstedFeaturesToData(CSBMCSignalRef signal, CSBMCTrackRef track);
