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

//==============================================================================
// Private Constants & Memory
//==============================================================================
static float32_t mpm_reserved_memory_f32[MPM_MAX_SIGNAL_SIZE] = {0};
static q15_t mpm_reserved_memory_q15[MPM_MAX_SIGNAL_SIZE] = {0};

//==============================================================================
// Private Function Declarations
//==============================================================================
static float32_t mpm_sum_f32(float32_t *signal, uint16_t scrLen);
static uint16_t mpm_find_peak_bin_f32(float32_t *pSrc, int len, float threshold);
static float32_t *mpm_NSDF_f32(float32_t *src, int len);
static float mpm_parabolic_interpolation_f32(
	uint16_t x_pos,
	float32_t a,
	float32_t b,
	float32_t c);

static q15_t *mpm_NSDF_q15(q15_t *src, int len);
static q31_t mpm_sum_q15(q15_t *signal, int len);
static uint16_t mpm_find_peak_bin_q15(q15_t *signal, int len, int threshold);
//==============================================================================
// Public API
//==============================================================================

float mpm_get_pitch_f32(
	float32_t *signal,
	int signal_len,
	int sample_rate,
	float threshold)
{
	float32_t *p_ncorr = mpm_NSDF_f32(signal, signal_len);
	uint16_t tau = mpm_find_peak_bin_f32(p_ncorr, signal_len, threshold);

	if (tau > signal_len - 2)
		tau = signal_len - 2;

	uint16_t xp = tau;
	float32_t a = p_ncorr[tau - 1];
	float32_t b = p_ncorr[tau];
	float32_t c = p_ncorr[tau + 1];

	float delta_tau = mpm_parabolic_interpolation_f32(xp, a, b, c);

	float pitch = sample_rate * 1.0f / delta_tau;
	return pitch;
}
/**
 * @brief Extract fundamental frequency from audio signal using McLeod Pitch Method (Q15 version)
 *
 * @param signal      Input audio signal buffer (Q15, signed, normalized: -32768 to 32767)
 * @param signal_len  Length of input signal (must be <= MPM_MAX_SIGNAL_SIZE/2)
 * @param sample_rate Sample rate of input signal in Hz
 * @param threshold   Peak detection threshold in Q15 (e.g., 9830 for 0.3, 29491 for 0.9)
 *
 * @return Detected fundamental frequency in Hz, or 0.0f if no pitch found
 *
 * @note Function uses internal static memory for correlation calculations
 * @note Threshold is in Q15 format (not float); higher values require clearer pitch
 */

float mpm_get_pitch_q15(
	q15_t *signal,
	int signal_len,
	int sample_rate,
	int threshold)
{
	q15_t *p_ncorr = mpm_NSDF_q15(signal, signal_len);
	int tau = mpm_find_peak_bin_q15(p_ncorr, signal_len, threshold);

	if (tau > signal_len - 2)
		tau = signal_len - 2;

	// Do the rest in floating point since its only called once and prescision is needed.
	int xp = tau;
	float a = (float)p_ncorr[tau - 1] * Q15_TO_FLOAT;
	float b = (float)p_ncorr[tau] * Q15_TO_FLOAT;
	float c = (float)p_ncorr[tau + 1] * Q15_TO_FLOAT;

	float delta_tau = mpm_parabolic_interpolation_f32(xp, a, b, c);

	float pitch = sample_rate * 1.0f / delta_tau;
	return pitch;
}

//==============================================================================
// Private: MPM q15 fixed-point implementation helper functions
//==============================================================================
/*
NSDF:                n(τ) =  2r'(τ) / m'(τ)

					r'(τ) = ∑ x_j *  x_{j+τ}
					m'(τ) = ∑ (x^2_j + x^2_{j+τ})
*/

static q15_t *mpm_NSDF_q15(q15_t *src, int len)
{
	// Calculate r'(τ)
	q15_t *xcorr = &mpm_reserved_memory_q15[1];

	arm_correlate_q15(&src[0], len, &src[0], len, xcorr); // output is 2*len-1 long
	q15_t *r = &xcorr[len - 1];
	q15_t *nsdf = r;

	// Calculate m'(τ)
	q15_t *xs = &mpm_reserved_memory_q15[0];
	q15_t *p_xs1 = &xs[0];
	q15_t *p_xs2 = &xs[len - 1];
	q31_t xs1, xs2;

	arm_mult_q15(&src[0], &src[0], &xs[0], len);
	xs1 = mpm_sum_q15(&xs[0], len);
	xs2 = xs1;

	for (int tau = 0; tau < len / 2; tau++, r++, p_xs1++, p_xs2--)
	{
		q31_t numer = 2 * (q31_t)(*r);
		q31_t denom = xs1 + xs2;

		if (denom == 0)
			*r = 0;
		else
		{
			q31_t result = (numer << 15) / denom;
			*r = __SSAT(result, 16);
		}

		xs1 = xs1 - (q31_t)(*p_xs1);
		xs2 = xs2 - (q31_t)(*p_xs2);
	}
	return nsdf;
}
static q31_t mpm_sum_q15(q15_t *signal, int len)
{
	q31_t sum = 0;
	for (int i = 0; i < len; i++, signal++)
		sum += (q31_t)*signal;
	return sum;
}

static uint16_t mpm_find_peak_bin_q15(q15_t *signal, int len, int threshold)
{
	int past_zero = 0;
	int valid_peak_past_zero = 0;
	q15_t peak_value = 0;
	int tau = 1;
	for (int i = 0; i < len; i++)
	{
		if (past_zero == 0 && *signal < 0)
			past_zero = 1;

		if (past_zero == 1)
		{
			if (*signal > peak_value && *signal > threshold)
			{
				peak_value = *signal;
				tau = i;
				valid_peak_past_zero = 1;
			}
			else if (valid_peak_past_zero == 1)
				return tau;
		}
		signal++;
	}
	return tau;
}

//==============================================================================
// Private: MPM floating point implementation helper functions
//==============================================================================
static float32_t mpm_sum_f32(float32_t *signal, uint16_t len)
{
	float32_t sum = 0;
	for (uint16_t i = 0; i < len; i++, signal++)
		sum += *signal;
	return sum;
}

static uint16_t mpm_find_peak_bin_f32(float32_t *signal, int len, float threshold)
{
	uint16_t past_zero = 0;
	uint16_t valid_peak_past_zero = 0;
	float32_t peak_value = 0;
	uint16_t tau = 1;
	for (uint16_t i = 0; i < len; i++)
	{
		if (past_zero == 0 && *signal < 0)
			past_zero = 1;

		if (past_zero == 1)
		{
			if (*signal > peak_value && *signal > threshold)
			{
				peak_value = *signal;
				tau = i;
				valid_peak_past_zero = 1;
			}
			else if (valid_peak_past_zero == 1)
				return tau;
		}
		signal++;
	}
	return tau;
}

static float32_t *mpm_NSDF_f32(float32_t *src, int len)
{
	

	float32_t *xcorr = &mpm_reserved_memory_f32[1];
	arm_correlate_f32(&src[0], len, &src[0], len, xcorr);
	float32_t *r = &xcorr[len - 1];

	//float32_t *xcorr = &mpm_reserved_memory_f32[len];
	//one_sided_autocorr_f32(src, xcorr, len);
	//float32_t *r = xcorr;
	
	float32_t *nsdf = r;

	float32_t *xs = &mpm_reserved_memory_f32[0];
	float32_t *p_xs1 = &xs[0];
	float32_t *p_xs2 = &xs[len - 1];
	float32_t xs1, xs2;

	arm_mult_f32(&src[0], &src[0], &xs[0], len);
	xs1 = mpm_sum_f32(&xs[0], len);
	xs2 = xs1;

	for (int tau = 0; tau < len / 2; tau++, r++, p_xs1++, p_xs2--)
	{
		*r = 2 * (*r) / (xs1 + xs2);

		xs1 = xs1 - (*p_xs1);
		xs2 = xs2 - (*p_xs2);
	}
	return nsdf;
}

static float mpm_parabolic_interpolation_f32(
	uint16_t x_pos,
	float32_t a,
	float32_t b,
	float32_t c)
{
	a = 20 * log10(a);
	b = 20 * log10(b);
	c = 20 * log10(c);

	float delta_pos = 0.5 * (a - c) / (1 - 2.0 * b + c);

	return x_pos + delta_pos;
}
