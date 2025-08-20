
#include "main.h"
#include "stm32l4xx_hal.h" 
#include "debug-utils.h"    
#include <stdio.h>    
#include <stdint.h>

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

