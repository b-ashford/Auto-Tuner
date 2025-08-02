#include "main.h"
#include "device_api.h"
#include "debug/usart.h"

static float guitar_signal[ADC_BUF_LEN];
uint16_t adc_buffer[ADC_BUF_LEN];


int main(void)
{
  device_init();
  device_start_adc(adc_buffer, ADC_BUF_LEN);

  while (1)
  {

    uart_dma_float_buffer(guitar_signal, ADC_BUF_LEN, (uint32_t)250);
    // uart_dma_test_float_sine_wave();
    // toggle_and_wait_led(10);
    // test_uart_simple();
  }
}