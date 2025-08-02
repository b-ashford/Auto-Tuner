
#include "autotune.h"
#include "device_api.h"

#include "debug/usart.h"

// static float guitar_signal[ADC_BUF_LEN];
uint16_t adc_buffer[ADC_BUFF_LEN];

void start_autotune_mode(void)
{
    device_register_adc_conv_complete_callback(process_adc);
    device_start_adc(adc_buffer, ADC_BUFF_LEN);
}

void process_adc(void)
{
    device_toggle_led();

    debug_uart_dma_uint16_buffer(adc_buffer, ADC_BUFF_LEN / 2, 250);
}
