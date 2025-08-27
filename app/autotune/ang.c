
#include "ang.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "debug/utils.h"

static void cbuf_append(float value, CBufState *cbuf);
static void cbuf_clear(CBufState *cbuf);
static int cmp_float(const void *a, const void *b);
static inline int max_float32_t(float32_t a, float32_t b);
static float cbuf_percentile(const CBufState *cbuf, int percentile);

static float buff[BUFF_LEN] = {0.0f};

static CBufState cbuf = {
    .buf = buff,
    .buf_len = BUFF_LEN,
    .curr_idx = 0};

static AngState st = {
    .cbuf = &cbuf,
    .percentile = 20,
    .delta_on_db = 9.0f,
    .delta_off_db = 4.0f,
    .n_hang_frames = 8,
    .leak_factor = 0.05f,
    .hang_count = 0,
    .prev_floor_db = 0.0f,
    .init = false,
    .leaky_open = true,
    .gate_open = false};

//--------------------------------------//
//         ADAPTIVE NOISE GATE          //
//--------------------------------------//
bool adaptive_noise_gate(float32_t *frame, int frame_len)
{
    float32_t energy = EPS;
    arm_power_f32(frame, frame_len, &energy);
    float32_t E_db = 10.0f * log10f(energy + EPS);

    if (st.init == false)
    {
        st.init = true;
        cbuf_clear(st.cbuf);
        cbuf_append(E_db, st.cbuf);
        st.gate_open = false;
        st.hang_count = st.n_hang_frames;
        st.prev_floor_db = E_db;
        return st.gate_open;
    }
    float32_t gate_switch_on = st.prev_floor_db + st.delta_on_db;
    float32_t gate_switch_off = st.prev_floor_db + st.delta_off_db;

    if (st.gate_open == false)
    {
        if (E_db >= gate_switch_on)
        {
            st.gate_open = true;
            st.hang_count = st.n_hang_frames;
        }
    }
    else
    {
        if (E_db <= gate_switch_off && st.hang_count == 0)
            st.gate_open = false;
        else if (st.hang_count > 0)
            st.hang_count--;
    }

    if (st.gate_open == false)
        cbuf_append(E_db, st.cbuf);
    else if (st.leaky_open)
    {
        float32_t k = (1.0f - st.leak_factor);
        float32_t E_db_leak = k * st.prev_floor_db + st.leak_factor * E_db;
        cbuf_append(E_db_leak, st.cbuf);
    }

    float32_t floor_db = cbuf_percentile(st.cbuf, st.percentile);
    st.prev_floor_db = floor_db;
    return st.gate_open;
}

//--------------------------------------//
//           CIRCULAR BUFFER            //
//--------------------------------------//
static void cbuf_append(float value, CBufState *cbuf)
{
    cbuf->buf[cbuf->curr_idx] = value;
    cbuf->curr_idx = (cbuf->curr_idx + 1) % cbuf->buf_len;
}

static void cbuf_clear(CBufState *cbuf)
{
    for (int i = 0; i < cbuf->buf_len; i++)
        cbuf->buf[i] = EPS;
    cbuf->curr_idx = 0;
}
static float cbuf_percentile(const CBufState *cbuf, int percentile)
{
    percentile = (percentile < 0) ? 0 : (percentile > 100 ? 100 : percentile);

    float tmp[BUFF_LEN];
    memcpy(tmp, cbuf->buf, cbuf->buf_len * sizeof(float));
    qsort(tmp, cbuf->buf_len, sizeof(float), cmp_float);

    int idx = (percentile * (cbuf->buf_len - 1)) / 100;
    return tmp[idx];
}
static int cmp_float(const void *a, const void *b)
{
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb);
}

//--------------------------------------//
//               HELPERS                //
//--------------------------------------//
static inline int max_float32_t(float32_t a, float32_t b)
{
    return (a > b) ? a : b;
}