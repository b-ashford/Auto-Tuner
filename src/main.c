#include "main.h"
#include "device_api.h"
#include "debug/usart.h"

#include "autotune.h"



int main(void)
{
  device_init();
  
  start_autotune_mode();

  while (1)
  {

    //uart_dma_float_buffer(guitar_signal, ADC_BUF_LEN, (uint32_t)250);
    // uart_dma_test_float_sine_wave();
    // toggle_and_wait_led(10);
    // test_uart_simple();
  }
}