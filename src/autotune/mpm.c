/**
 * @file mpm.c
 * @brief McLeod Pitch Method Implementation
 *
 * A smarter way to find pitch using normalized auto-correlation
 * and parabolic interpolation for increased accuracy.
 *
 */

#include "mpm.h"
#include "filter.h"
#include "debug/usart.h"
#include <stdbool.h>

//==============================================================================
// Private Function Declarations
//==============================================================================
static void nsdf(float32_t *signal, const uint32_t len, float32_t *out);
static uint16_t estimate_pitch_period_lag(
	float32_t *signal,
	uint16_t signal_len,
	uint16_t stop_search,
	float32_t clarity_ratio,
	float32_t min_peak_threshold);
static inline float32_t parabolic_interpolation(float32_t x_pos,
												float32_t a,
												float32_t b,
												float32_t c);
static void xcorr_positive_sided(
	const float32_t *in1,
	const float32_t *in2,
	const uint32_t len,
	float32_t *out);
//==============================================================================
// Public API
//==============================================================================
float32_t mpm_get_pitch_f32(
    float32_t *signal,
    const uint32_t len,
    const uint32_t fs,
    const uint32_t stop_search,
    const float32_t clarity_ratio,
    const float32_t peak_threshold)
{
    float32_t nsdf_signal[MPM_BUFF_SIZE] = {0.0f};
    nsdf(signal, stop_search, nsdf_signal);
    uint16_t lag = estimate_pitch_period_lag(
        nsdf_signal,
        len,
        stop_search,
        clarity_ratio,
        peak_threshold);
    if (lag == 0)
        return 0.0f;
    float32_t a = nsdf_signal[lag - 1];
    float32_t b = nsdf_signal[lag];
    float32_t c = nsdf_signal[lag + 1];

    float32_t delta_lag = parabolic_interpolation(lag, a, b, c);
    return fs / delta_lag;
}
//==============================================================================
// Private
//==============================================================================

static void nsdf(float32_t *signal, const uint32_t len, float32_t *out)
{
    xcorr_positive_sided(signal, signal, len, out);

    float32_t *x_squared = signal;
    for (uint32_t i = 0; i < len; i++)
        signal[i] = signal[i] * signal[i];

    float32_t sum = 0;
    for (uint32_t i = 0; i < len; i++)
        sum += signal[i];

    float32_t sum_xj_squared = sum;
    float32_t sum_xj_tau_squared = sum;

    const float32_t epsilon = 1e-6f;

    for (uint32_t lag = 0; lag < len; lag++)
    {
        // Prevent sums from going negative due to rounding
        sum_xj_squared = fmaxf(0.0f, sum_xj_squared);
        sum_xj_tau_squared = fmaxf(0.0f, sum_xj_tau_squared);

        float32_t m = sum_xj_squared + sum_xj_tau_squared;
        m = fmaxf(m, epsilon);

        out[lag] = 2 * out[lag] / m;

        sum_xj_squared -= x_squared[lag];
        sum_xj_tau_squared -= x_squared[len - lag - 1];
    }
}

static uint16_t estimate_pitch_period_lag(
    float32_t *signal,
    uint16_t signal_len,
    uint16_t stop_search,
    float32_t clarity_ratio,
    float32_t min_peak_threshold)
{
    if (stop_search >= signal_len)
        stop_search = signal_len - 1;

    // peak tracking
    float32_t max_peak_value = 0.0f;
    uint16_t max_peak_lag = 0;

    // Results storage
    float32_t peaks[MAX_NUM_PEAKS] = {0.0};
    uint16_t lags[MAX_NUM_PEAKS] = {0};
    uint16_t peak_counter = 0;
    float32_t global_max_peak_value = 0.0f;

    // Find first zero-crossing before starting peak search
    uint16_t start_search = 0;
    float32_t *curr_val = signal;
    float32_t *next_val = signal + 1;
    for (uint16_t lag = 0; lag < stop_search; lag++, curr_val++, next_val++)
    {
        if ((*curr_val >= -EPS) && (*next_val < -EPS))
        {
            start_search = lag;
            break;
        }
    }
    // if no zero crossings than signal is not useful
    if (start_search == 0)
        return 0;
    start_search++;
    curr_val = &signal[start_search];
    next_val = &signal[start_search + 1];
    for (uint16_t lag = start_search; lag < stop_search; lag++, curr_val++, next_val++)
    {
        bool save_max_peak = false;

        // save_max_peak when exiting positive lobe
        if ((*curr_val >= -EPS) && (*next_val < -EPS))
            save_max_peak = true;

        // save_max_peak if end-of-window and descending
        if ((lag == stop_search - 1) && (*next_val < *curr_val - EPS))
            save_max_peak = true;

        // update max_peak_value with larger value if turning point found
        if ((*curr_val > max_peak_value) && (*next_val < *curr_val - EPS))
        {
            max_peak_value = *curr_val;
            max_peak_lag = lag;
        }

        if (save_max_peak &&
            (max_peak_value > min_peak_threshold) &&
            peak_counter < MAX_NUM_PEAKS)
        {
            peaks[peak_counter] = max_peak_value;
            lags[peak_counter] = max_peak_lag;
            peak_counter++;
            if (max_peak_value > global_max_peak_value)
                global_max_peak_value = max_peak_value;
            max_peak_value = 0.0f;
            max_peak_lag = 0;
        }
    }
    if (global_max_peak_value <= 0.0f)
        return 0;
    // The F0 peak is defined as the first major peak close to the global maximum.
    for (uint16_t i = 0; i < peak_counter; i++)
    {
        if (peaks[i] >= clarity_ratio * global_max_peak_value)
            return lags[i];
    }
    return 0;
}

static void xcorr_positive_sided(
    const float32_t *in1,
    const float32_t *in2,
    const uint32_t len,
    float32_t *out)
{
    for (uint32_t lag = 0; lag < len; lag++)
        arm_dot_prod_f32(in1 + lag, in2, len - lag, &out[lag]);
}

static inline float32_t parabolic_interpolation(float32_t x_pos,
                                                float32_t a,
                                                float32_t b,
                                                float32_t c)
{
    // Guard: log10 requires positive inputs
    const float32_t eps = 1e-12f;
    if (a <= 0.0f || b <= 0.0f || c <= 0.0f)
        return x_pos;

    a = 20.0f * log10f(a);
    b = 20.0f * log10f(b);
    c = 20.0f * log10f(c);

    float32_t denom = (a - 2.0f * b + c);
    if (fabsf(denom) < eps)
        return x_pos;

    float32_t delta_pos = 0.5f * (a - c) / denom;
    return x_pos + delta_pos;
}

