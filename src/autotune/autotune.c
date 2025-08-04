
#include "autotune.h"
#include "device_api.h"
#include "mpm.h"
#include "debug/usart.h"
#include "filter.h"

float32_t signal_buffer_f32[SIGNAL_BUFF_LEN];
q15_t signal_buffer_q15[SIGNAL_BUFF_LEN];
uint16_t adc_buffer[ADC_BUFF_LEN];

//==============================================================================
// Public API
//==============================================================================

void start_autotune_mode(void)
{
    device_register_adc_conv_complete_callback(autotune_mode);
    //init_iir_filter_f32();
    init_iir_filter_f32();
    device_start_adc(adc_buffer, ADC_BUFF_LEN);
}

void autotune_mode(int buffer_section)
{
    uint32_t start_cycles = DWT->CYCCNT;

    device_toggle_led();
    if (buffer_section == FIRST_HALF)
        preprocess_and_filter_adc_f32(
            &adc_buffer[0],
            signal_buffer_f32,
            SIGNAL_BUFF_LEN);
    else
        preprocess_and_filter_adc_f32(
            &adc_buffer[SIGNAL_BUFF_LEN],
            signal_buffer_f32,
            SIGNAL_BUFF_LEN);

    float pitch = mpm_get_pitch_f32(
        signal_buffer_f32,
        SIGNAL_BUFF_LEN,
        ADC_SAMPLE_RATE,
        0.9);

    uint32_t end_cycles = DWT->CYCCNT;
    uint32_t processing_cycles = end_cycles - start_cycles;

    // Convert to microseconds (80MHz CPU)
    float processing_time_us = (float)processing_cycles / 80.0f;

    // Pack both values into single array
    float debug_data[2] = {pitch, processing_time_us};
    debug_uart_dma_float_buffer(debug_data, 2, 1000);
}

//==============================================================================
// Private
//==============================================================================

