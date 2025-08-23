#include "main.h"
#include "board_api.h"
#include "debug/utils.h"

#include "autotune/autotune.h"

int main(void)
{
  debug_dwt_enable();
  board_init();
  autotune_mode_init();
  autotune_mode_run();

  while (1)
  {

    // uart_dma_float_buffer(guitar_signal, ADC_BUF_LEN, (uint32_t)250);
    //  uart_dma_test_float_sine_wave();
    //  toggle_and_wait_led(10);
    //  test_uart_simple();
  }
}