/**
 * @file mpm.c
 * @brief McLeod Pitch Method Implementation
 *
 * A smarter way to find pitch using normalized auto-correlation
 * and parabolic interpolation for increased accuracy.
 *
 */

#include "mpm.h"

//==============================================================================
// Private Constants & Memory
//==============================================================================
static float32_t mpm_reserved_memory[MPM_MAX_SIGNAL_SIZE] = {0};

//==============================================================================
// Private Function Declarations
//==============================================================================
static void mpm_sum_f32(float32_t *pSrc, uint16_t scrLen, float32_t *pRes);
static uint16_t mpm_find_peak_bin_f32(float32_t *pSrc, int len, float threshold);
static void mpm_NSDF_f32(float32_t *src, float32_t **dst, int len);
static float mpm_parabolic_interpolation_f32(
	uint16_t x_pos,
	float32_t a,
	float32_t b,
	float32_t c);

//==============================================================================
// Public API Implementation
//==============================================================================

float mpm_get_pitch_f32(
	float32_t *signal,
	int signal_len,
	int sample_rate,
	float threshold)
{
	float32_t *p_ncorr;
	mpm_NSDF_f32(signal, &p_ncorr, signal_len);
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

//==============================================================================
// Private Helper Functions
//==============================================================================
static void mpm_sum_f32(float32_t *signal, uint16_t scrLen, float32_t *pRes)
{
	*pRes = 0;
	for (uint16_t i = 0; i < scrLen; i++)
	{
		*pRes += *signal;
		signal++;
	}
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

static void mpm_NSDF_f32(float32_t *src, float32_t **dst, int len)
{

	float32_t *xcorr = &mpm_reserved_memory[1];

	arm_correlate_f32(&src[0], len, &src[0], len, xcorr);

	float32_t *r = &xcorr[len - 1];
	*dst = r;

	float32_t *xs = &mpm_reserved_memory[0];
	float32_t *p_xs1 = &xs[0];
	float32_t *p_xs2 = &xs[len - 1];
	float32_t xs1, xs2;

	arm_mult_f32(&src[0], &src[0], &xs[0], len);
	mpm_sum_f32(&xs[0], len, &xs1);
	xs2 = xs1;

	for (uint16_t tau = 0; tau < len / 2; tau++)
	{
		*r = 2 * (*r) / (xs1 + xs2);

		xs1 = xs1 - (*p_xs1);
		xs2 = xs2 - (*p_xs2);

		r++;
		p_xs1++;
		p_xs2--;
	}
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
