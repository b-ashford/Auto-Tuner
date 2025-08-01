#include "main.h"
#include "board_interface.h"
#include "debug/usart.h"

static float guitar_signal[ADC_BUF_LEN];

int main(void)
{
  init_board();
  while (1)
  {
    board_copy_adc_samples(guitar_signal, ADC_BUF_LEN);
    uart_dma_float_buffer(guitar_signal, ADC_BUF_LEN, (uint32_t) 250);
    //uart_dma_test_float_sine_wave();
    //toggle_and_wait_led(10);
    //test_uart_simple();
  }
}