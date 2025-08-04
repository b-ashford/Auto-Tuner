
#include "filter.h"

float32_t iir_state_f32[4 * NUM_IIR_STAGES];
q15_t iir_state_q15[4 * NUM_IIR_STAGES];
arm_biquad_casd_df1_inst_f32 iir_settings_f32;
arm_biquad_casd_df1_inst_q15 iir_settings_q15;

q15_t iir_taps_q15[] = {
    // Stage 1: HP Filter (80 Hz)
     15672,  -31344,  15672,  31313,  -14991,
    // Stage 2: HP Filter (80 Hz)
     15672,  -31344,  15672,  31313,  -14991,
    // Stage 3: LP Filter (1 kHz)
      2048,    4096,   2048,      0,       0
};


float32_t iir_taps_f32[] = {
    // Stage 1: HP Filter (80Hz)
    0.95653708, -1.91307417, 0.95653708, 1.91118480, -0.91496354,
    // Stage 2: HP Filter (80Hz)
    0.95653708, -1.91307417, 0.95653708, 1.91118480, -0.91496354,
    // Stage 3: Lowpass Filter (1kHz)
    0.125, 0.25, 0.125, 0.0, 0.0};

void init_iir_filter_f32(void)
{
    arm_biquad_cascade_df1_init_f32(
        &iir_settings_f32,
        NUM_IIR_STAGES,
        &iir_taps_f32[0],
        &iir_state_f32[0]);
}

void init_iir_filter_q15(void)
{
    arm_biquad_cascade_df1_init_q15(
        &iir_settings_q15,
        NUM_IIR_STAGES,
        &iir_taps_q15[0],
        &iir_state_q15[0],
        0);
}

void preprocess_and_filter_adc_f32(uint16_t *src, float32_t *guitar_signal, int len)
{
    int dc_bias = 2280.0f;
    for (int i = 0; i < len; i++)
        guitar_signal[i] = (float32_t)src[i] - dc_bias;

    arm_biquad_cascade_df1_f32(
        &iir_settings_f32,
        guitar_signal,
        guitar_signal,
        len);
}

void preprocess_and_filter_adc_q15(uint16_t *src, q15_t *guitar_signal, int len)
{
    const int16_t dc_bias = 2280;
    for (int i = 0; i < len; i++)
    {
        int32_t diff = (int32_t)src[i] - dc_bias;
        guitar_signal[i] = (q15_t)__SSAT(diff << 4, 16);
    }
    arm_biquad_cascade_df1_q15(
        &iir_settings_q15,
        guitar_signal,
        guitar_signal,
        len);
}