#ifndef FILTER_H
#define FILTER_H

#include "arm_math.h"

#define NUM_IIR_STAGES 3


extern float32_t iir_taps_f32[];
extern q15_t iir_taps_q15[];
extern float32_t iir_state_f32[4 * NUM_IIR_STAGES];
extern q15_t iir_state_q15[4 * NUM_IIR_STAGES];
extern arm_biquad_casd_df1_inst_f32 iir_settings_f32;
extern arm_biquad_casd_df1_inst_q15 iir_settings_q15;

void init_iir_filter_f32(void);
void init_iir_filter_q15(void);
void preprocess_and_filter_adc_f32(uint16_t *src, float32_t *guitar_signal, int len);
void preprocess_and_filter_adc_q15(uint16_t *src, q15_t *guitar_signal, int len);
#endif /* FILTER_H */
