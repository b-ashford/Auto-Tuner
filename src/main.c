#include "main.h"
#include "board_interface.h"
#include "debug/usart.h"

static float guitar_signal[ADC_BUF_LEN];

int main(void)
{
  init_board();
  while (1)
  {
    uart_dma_sine_wave();

    //board_copy_adc_samples(guitar_signal, ADC_BUF_LEN);
    //board_send_floats_uart(guitar_signal, 1);
    toggle_and_wait_led(250);
    //test_uart_simple();
  }
}