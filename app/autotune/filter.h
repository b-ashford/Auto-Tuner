#ifndef FILTER_H
#define FILTER_H

#include "arm_math.h"

#define NUM_IIR_STAGES 4

extern float32_t iir_taps_f32[];
extern float32_t iir_state_f32[4 * NUM_IIR_STAGES];
extern arm_biquad_casd_df1_inst_f32 iir_settings_f32;

void init_iir_filter_f32(void);
void bandpass_filter_f32(const uint16_t *src, float32_t *y, int len);
void one_sided_autocorr_f32(
    const float32_t *src,
    float32_t *xcorr,
    uint32_t len);

#endif /* FILTER_H */
