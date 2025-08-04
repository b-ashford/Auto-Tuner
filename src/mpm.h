/**
 * @file mpm.h
 * @brief McLeod Pitch Method - High-accuracy pitch detection
 *
 * Implementation of the McLeod Pitch Method for robust fundamental
 * frequency estimation using normalised square difference function.
 *
 * @author Bronston Ashford
 * @date 26 Jul. 2022
 */

#ifndef MPM_H
#define MPM_H

#include "arm_math.h"

//==============================================================================
// Configuration Constants
//==============================================================================

/** @brief Maximum signal length supported by MPM algorithm
 *  @note Must accommodate 2x signal length for internal correlation buffer
 */

#define MPM_MAX_SIGNAL_SIZE 2048 // Needs to fit 2*signal length at minimum

//==============================================================================
// Public API
//==============================================================================

/**
 * @brief Extract fundamental frequency from audio signal using McLeod Pitch Method
 *
 * @param signal      Input audio signal buffer
 * @param signal_len  Length of input signal (must be <= MPM_MAX_SIGNAL_SIZE/2)
 * @param sample_rate Sample rate of input signal in Hz
 * @param threshold   Peak detection threshold (typically 0.3 - 0.9)
 *
 * @return Detected fundamental frequency in Hz, or 0.0f if no pitch found
 *
 * @note Function uses internal static memory for correlation calculations
 * @note Higher threshold values require stronger, clearer pitch signals
 */
float mpm_get_pitch_f32(
    float32_t *signal,
    int signal_len,
    int sample_rate,
    float threshold);

#endif
