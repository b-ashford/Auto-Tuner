/**
 * @file mpm.h
 * @brief McLeod Pitch Method - High-accuracy pitch detection
 *
 * Implementation of the McLeod Pitch Method for robust fundamental
 * frequency estimation using normalised square difference function.
 * See: docs/smarter_way_to_find_pitch.md
 *
 * @author Bronston Ashford
 * @date 15 Aug 2025
 */

#ifndef AT_MPM_H
#define AT_MPM_H

#include "arm_math.h"

//==============================================================================
// Configuration Constants
//==============================================================================

#define MPM_EPS 0.001f

//==============================================================================
// Public API
//==============================================================================
/**
 * @brief   Detect fundamental frequency with McLeod Pitch Method (MPM).
 *
 * @param   signal         Input audio buffer (float32_t).
 * @param   len            Signal length.
 * @param   fs             Sampling rate in Hz.
 * @param   stop_search    Stop searching for peaks at lag number stop_search.
 * @param   clarity_ratio  Threshold for determining the dominant peak.
 * @param   peak_threshold Aboslute threshold for peak picking
 *
 * @return  Fundamental frequency in Hz, or 0.0f if none detected.
 * @note    Destroys input signal.
 */
float mpm_get_pitch_f32(
    float32_t *signal,
    const uint32_t len,
    const uint32_t fs,
    const uint32_t stop_search,
    const float32_t clarity_ratio,
    const float32_t peak_threshold);

#endif /* AT_MPM_H */
