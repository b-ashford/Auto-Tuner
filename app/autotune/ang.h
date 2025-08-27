#ifndef ANG_H
#define ANG_H

#include <stdbool.h>
#include "arm_math.h"

#define BUFF_LEN 20
#define EPS 1e-20f

typedef struct
{
    float *buf;
    int buf_len;
    int curr_idx;
} CBufState;

typedef struct
{
    CBufState *cbuf;
    int percentile;
    float32_t delta_on_db;
    float32_t delta_off_db;
    int n_hang_frames;
    float leak_factor;
    int hang_count;
    float32_t prev_floor_db;
    bool init;
    bool leaky_open;
    bool gate_open;
} AngState;

bool adaptive_noise_gate(float32_t *frame, int frame_len);

#endif