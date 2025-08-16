
#include "autotune.h"
#include "device_api.h"
#include "mpm.h"
#include "debug/usart.h"
#include "filter.h"
#include "tuning_logic.h"

#define ALIGN4 __attribute__((aligned(4)))

static ALIGN4 float32_t signal_buffer_f32[SIGNAL_BUFF_LEN];
static ALIGN4 uint16_t adc_buffer[ADC_BUFF_LEN];
static ALIGN4 float32_t debug_tx[SIGNAL_BUFF_LEN];


static float inline get_pitch_error_cents(float32_t *signal);
static float32_t *process_adc(int buffer_section);

float target = 0.0f;
float detected = 0.0f;

//============================================================================
// Public API
//============================================================================
void start_autotune_mode(void)
{
    device_register_adc_conv_complete_callback(autotune_mode);
    init_iir_filter_f32();
    device_start_adc(adc_buffer, ADC_BUFF_LEN);
    

}

void autotune_mode(int buffer_section)
{
    device_toggle_led();
    uint32_t start_cycles = DWT->CYCCNT;
    float32_t *signal = process_adc(buffer_section);
    float error_cents = get_pitch_error_cents(signal);

    uint32_t end_cycles = DWT->CYCCNT;
    uint32_t processing_cycles = end_cycles - start_cycles;
    float processing_time_us = (float)processing_cycles / 80.0f;

    // Pack both values into single array
    debug_tx[0] = error_cents;
    debug_tx[1] = target;
    debug_tx[2] = detected;
    debug_tx[3] = processing_time_us;
    debug_uart_dma_float_buffer(debug_tx, 4, 1000);
}

//============================================================================
// Private
//============================================================================
static float inline get_pitch_error_cents(float32_t *signal)
{

    

    float detected_freq = mpm_get_pitch_f32(
        signal,
        SIGNAL_BUFF_LEN,
        ADC_SAMPLE_RATE,
        LAG_STOP_SEARCH,
        CLARITY_RATIO,
        PEAK_THRESHOLD);

    float target_freq = get_target_freq(detected_freq);
    float error_cents = get_error_in_cents(detected_freq, target_freq);
    target = target_freq;
    detected = detected_freq;
    
    return error_cents;
}

static float32_t *process_adc(int buffer_section)
{
    if (buffer_section == FIRST_HALF)
    {

        filter_and_remove_dc_f32(
            &adc_buffer[0],
            signal_buffer_f32,
            SIGNAL_BUFF_LEN);
    }
    else
    {
        filter_and_remove_dc_f32(
            &adc_buffer[SIGNAL_BUFF_LEN],
            signal_buffer_f32,
            SIGNAL_BUFF_LEN);
    }

    //debug_uart_dma_uint16_buffer(&adc_buffer[0], SIGNAL_BUFF_LEN, 1000);
    for (int i = 0; i < 1024; i++)
        debug_tx[i] = signal_buffer_f32[i];
    //debug_uart_dma_float_buffer(debug_tx, SIGNAL_BUFF_LEN, 1000);
    
    return signal_buffer_f32;
}