
#include "autotune/autotune.h"
#include "autotune/pitch.h"
#include "autotune/guitar.h"
#include "autotune/params.h"
#include "autotune/filter.h"
#include "autotune/ang.h"

#include "board_api.h"
#include "debug/utils.h"

#include <stdbool.h>

static ALIGN4 uint16_t adc_buffer[ADC_BUFF_LEN];
static ALIGN4 float32_t signal_buffer_f32[SIGNAL_BUFF_LEN];

static float32_t *process_adc(int buffer_section);

//============================================================================
// Public API
//============================================================================
void autotune_mode_init(void)
{
    init_iir_filter_f32();
    board_register_adc_conv_complete_callback(autotune_mode);

}

void autotune_mode_run(void)
{
    board_start_adc(adc_buffer, ADC_BUFF_LEN);
    board_motor_on();
}

void autotune_mode(int buffer_section)
{
    
    
    uint32_t t0 = debug_start_cycle_count();
    float32_t *signal = process_adc(buffer_section);
    
    bool gate_open = adaptive_noise_gate(signal, SIGNAL_BUFF_LEN);
    
    //board_motor_adjust_speed(-39);
    if (gate_open == false)
        return;
    board_toggle_led();
    float error_cents = get_pitch_error_cents(signal, SIGNAL_BUFF_LEN);

    float t_us = debug_end_cycle_us(t0, 80.0f);
    //debug_uart_dma_float_buffer(&t_us,1 , 1000);
}

//============================================================================
// Private
//============================================================================
static float32_t *process_adc(int buffer_section)
{
    if (buffer_section == FIRST_HALF)
    {

        bandpass_filter_f32(
            &adc_buffer[0],
            signal_buffer_f32,
            SIGNAL_BUFF_LEN);
    }
    else
    {
        bandpass_filter_f32(
            &adc_buffer[SIGNAL_BUFF_LEN],
            signal_buffer_f32,
            SIGNAL_BUFF_LEN);
    }
    return signal_buffer_f32;
}
