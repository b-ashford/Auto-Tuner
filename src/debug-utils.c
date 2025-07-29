
#include "main.h"
#include "stm32l4xx_hal.h" 
#include "debug-utils.h"    
#include <stdio.h>    

void init_debug(void)
{
    SWO_Init();
    ITM_Init();
}

void SWO_Init(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_3; // SWO pin
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_SWJ; // SWJ alternate function
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

// Initialize ITM
void ITM_Init(void)
{
  // Enable ITM
  ITM->LAR = 0xC5ACCE55; // Unlock ITM
  ITM->TER = 0x00000003; // Enable ports 0 and 1
  ITM->TCR = 0x0001000D; // Enable ITM with sync packets

  // Configure TPIU for SWO
  TPI->SPPR = 0x00000002;               // Use async mode
  TPI->ACPR = (80000000 / 2000000) - 1; // Set SWO speed (CPU_FREQ / SWO_FREQ - 1)
}

// Pre-calculated sine values (smoother)
const int sine_table[100] = {
    0, 6, 12, 18, 24, 30, 36, 42, 47, 52, 57, 62, 66, 70, 74, 77, 80, 82, 84, 86,
    87, 88, 88, 88, 87, 86, 84, 82, 80, 77, 74, 70, 66, 62, 57, 52, 47, 42, 36, 30,
    24, 18, 12, 6, 0, -6, -12, -18, -24, -30, -36, -42, -47, -52, -57, -62, -66, -70,
    -74, -77, -80, -82, -84, -86, -87, -88, -88, -88, -87, -86, -84, -82, -80, -77,
    -74, -70, -66, -62, -57, -52, -47, -42, -36, -30, -24, -18, -12, -6
};

void Stream_Sine_Wave(UART_HandleTypeDef *uart)
{
    static int angle = 0;
    static uint32_t timestamp = 0;
    
    int sine_value = sine_table[angle];
    
    char buffer[50];
    int len = snprintf(buffer, sizeof(buffer), "%lu,%d\n", timestamp, sine_value);
    HAL_UART_Transmit(uart, (uint8_t*)buffer, len, 100);
    
    angle++;
    if (angle >= 100) angle = 0;
    timestamp++;
}