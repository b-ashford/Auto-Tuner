#include "usart.h"
#include "../setup/error_handler.h"
#include <math.h>
#include "string.h"

#define TEST_BUFFER 1024

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

static volatile uint8_t uart_tx_busy = 0;

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 921600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_ENABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if (uartHandle->Instance == USART1)
  {

    /** Initializes the peripherals clock
     */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_2;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart1_tx);

    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART1)
  {

    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
  }
}

void debug_uart_dma_test_float_sine_wave(void)
{
   static float adc_buffer[TEST_BUFFER];
   static float phase = 0.0f;
   
   for (int i = 0; i < TEST_BUFFER; i++) {
       float time = (phase + i) / 8000.0f;
       adc_buffer[i] = 2.0f * sinf(2.0f * 3.14159f * 100.0f * time);
   }
   
   // Send buffer
   if (debug_uart_dma_float_buffer(adc_buffer, TEST_BUFFER, (uint32_t) 250) == HAL_OK) {
       phase += 1.0f * TEST_BUFFER;
       if (phase >= 8000.0f) phase = 0.0f;
   }
}

HAL_StatusTypeDef debug_uart_dma_float_buffer(float *buff, int len, uint32_t wait_ms)
{
   static uint32_t last_send_time = 0;
   
   uint32_t current_time = HAL_GetTick();
   
   if (current_time - last_send_time < wait_ms) {
       return HAL_BUSY; 
   }
   
   if (huart1.gState != HAL_UART_STATE_READY) {
       return HAL_BUSY;  
   }
   
   HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(&huart1, (uint8_t *)buff, len * sizeof(float));
   
   if (result == HAL_OK) {
       last_send_time = current_time;
   }
   
   return result;
}

HAL_StatusTypeDef debug_uart_dma_uint16_buffer(uint16_t *buff, int len, uint32_t wait_ms)
{
  static uint32_t last_send_time = 0;
  
  uint32_t current_time = HAL_GetTick();
  
  if (current_time - last_send_time < wait_ms) {
      return HAL_BUSY; 
  }
  
  if (huart1.gState != HAL_UART_STATE_READY) {
      return HAL_BUSY;  
  }
  
  HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(&huart1, (uint8_t *)buff, len * sizeof(uint16_t));
  
  if (result == HAL_OK) {
      last_send_time = current_time;
  }
  
  return result;
}


HAL_StatusTypeDef debug_uart_dma_print_string(const char *str)
{
   if (huart1.gState != HAL_UART_STATE_READY) {
       return HAL_BUSY;  
   }
   return HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str, strlen(str));
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_tx_busy = 0;
}
