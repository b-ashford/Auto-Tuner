#ifndef AUTOTUNE_DSP_H
#define AUTOTUNE_DSP_H

#include "arm_math.h"
#include <stdbool.h>

#define FEH_LEN 20

typedef struct
{
    float history[FEH_LEN];
    int len;
    int idx;
    bool gate_open;
    float k_on;
    float k_off;
} FrameEnergyHistory;

float dsp_get_pitch_error_cents(float32_t *signal, int len);
void dsp_init(void);
bool dsp_should_process(float32_t *signal, int len);
#endif /* AUTOTUNE_DSP_H */