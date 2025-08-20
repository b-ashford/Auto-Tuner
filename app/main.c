#include "main.h"
#include "board_api.h"
#include "debug/usart.h"

#include "autotune.h"

int main(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  board_init();

  start_autotune_mode();

  while (1)
  {

    // uart_dma_float_buffer(guitar_signal, ADC_BUF_LEN, (uint32_t)250);
    //  uart_dma_test_float_sine_wave();
    //  toggle_and_wait_led(10);
    //  test_uart_simple();
  }
}