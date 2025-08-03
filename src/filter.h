#ifndef FILTER_H
#define FILTER_H

#include "arm_math.h"

arm_biquad_casd_df1_inst_f32 iir_settings;
float iir_state[4];
/*
static float iir_taps[5] = {
    0.997987115675119,
    -1.995974231350238,
    0.997987115675119,
    1.995970179642828,
    -0.995978283057647};
*/
static float iir_taps[5] = {
    0.997987115675119,
    -1.995974231350238,
    0.997987115675119,
    1.995970179642828,
    -0.995978283057647};


#endif /* FILTER_H */