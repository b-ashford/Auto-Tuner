
#include "autotune.h"
#include "device_api.h"
#include "mpm.h"
#include "debug/usart.h"
#include "filter.h"

float32_t signal_buffer[SIGNAL_BUFF_LEN];
uint16_t adc_buffer[ADC_BUFF_LEN];

void start_autotune_mode(void)
{
    device_register_adc_conv_complete_callback(autotune_mode);
    arm_biquad_cascade_df1_init_f32(
        &iir_settings,
        NUM_IIR_STAGES,
        &iir_taps[0],
        &iir_state[0]);
    device_start_adc(adc_buffer, ADC_BUFF_LEN);
}

void autotune_mode(int buffer_section)
{
    uint32_t start_cycles = DWT->CYCCNT;

    device_toggle_led();
    if (buffer_section == FIRST_HALF)
        adc_to_guitar_signal(&adc_buffer[0], signal_buffer);
    else
        adc_to_guitar_signal(&adc_buffer[SIGNAL_BUFF_LEN], signal_buffer);

    float pitch = mpm_get_pitch(signal_buffer);

    uint32_t end_cycles = DWT->CYCCNT;
    uint32_t processing_cycles = end_cycles - start_cycles;

    // Convert to microseconds (80MHz CPU)
    float processing_time_us = (float)processing_cycles / 80.0f;

    // Pack both values into single array
    float debug_data[2] = {pitch, processing_time_us};
    debug_uart_dma_float_buffer(debug_data, 2, 500);
}

void adc_to_guitar_signal(uint16_t *src, float32_t *guitar_signal)
{
    int dc_bias = 2280.0f;
    for (int i = 0; i < SIGNAL_BUFF_LEN; i++)
        guitar_signal[i] = (float32_t)src[i] - dc_bias;

    arm_biquad_cascade_df1_f32(
        &iir_settings,
        guitar_signal,
        guitar_signal,
        SIGNAL_BUFF_LEN);
}