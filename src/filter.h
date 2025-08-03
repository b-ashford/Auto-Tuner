#ifndef FILTER_H
#define FILTER_H

#include "arm_math.h"

#define NUM_IIR_STAGES 3

arm_biquad_casd_df1_inst_f32 iir_settings;
float iir_state[4 * NUM_IIR_STAGES];


// CMSIS-DSP coefficient format: [b0, b1, b2, -a1, -a2] per stage
static float iir_taps[] = {
    // Stage 1: HP Filter (80Hz)
    0.95653708, -1.91307417, 0.95653708, 1.91118480, -0.91496354,

    // Stage 2: HP Filter (80Hz)
    0.95653708, -1.91307417, 0.95653708, 1.91118480, -0.91496354,

    // Stage 3: Lowpass Filter (1kHz)
    0.125, 0.25, 0.125, 0.0, 0.0};

#endif /* FILTER_H */

