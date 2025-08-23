
#include "autotune/filter.h"

float32_t iir_state_f32[4 * NUM_IIR_STAGES];
arm_biquad_casd_df1_inst_f32 iir_settings_f32;

float32_t iir_taps_f32[] = {
    // HP 40 Hz (x2)
    0.97803048f,
    -1.95606096f,
    0.97803048f,
    1.95557824f,
    -0.95654368f,
    0.97803048f,
    -1.95606096f,
    0.97803048f,
    1.95557824f,
    -0.95654368f,
    // LP 400 Hz (x2)
    0.01903683f,
    0.03807366f,
    0.01903683f,
    1.47967422f,
    -0.55582154f,
    0.02188385f,
    0.04376770f,
    0.02188385f,
    1.70096433f,
    -0.78849974f,
};
void init_iir_filter_f32(void)
{
    arm_biquad_cascade_df1_init_f32(
        &iir_settings_f32,
        NUM_IIR_STAGES,
        &iir_taps_f32[0],
        &iir_state_f32[0]);
}

void bandpass_filter_f32(const uint16_t *src, float32_t *y, int len)
{
    for (int i = 0; i < len; i++)
        y[i] = ((float32_t)src[i] - 2048.0f) / 2048.0f;

    arm_biquad_cascade_df1_f32(&iir_settings_f32, y, y, len);
}
