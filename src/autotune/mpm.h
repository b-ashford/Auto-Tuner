/**
 * @file mpm.h
 * @brief McLeod Pitch Method - High-accuracy pitch detection
 *
 * Implementation of the McLeod Pitch Method for robust fundamental
 * frequency estimation using normalised square difference function.
 * See: docs/smarter_way_to_find_pitch.md
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

#define MAX_NUM_PEAKS 15
#define EPS 0.001f
#define MPM_BUFF_SIZE 150

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
float32_t mpm_get_pitch_f32(
    float32_t *signal,
    const uint32_t len,
    const uint32_t fs,
    const uint32_t stop_search,
    const float32_t clarity_ratio,
    const float32_t peak_threshold);

#endif
