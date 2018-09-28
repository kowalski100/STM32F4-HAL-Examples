/*
@author:    Ijaz Ahmad

@descp:     This Program configures USART3 for asynchronous
            communication with 9600 baudrate.
            PB10 ----> Tx
            PB11 ----> Rx
            The data received is simply looped backed/trasmitted
            back on the same port.
            
@Method:    Interrupt based reception

@warrenty:  void
*/
/**************************************************************
REQUIREMENTS:
    1. 9600 baud rate
    2. 8-bits data bits
    3. 1 start/stop bits
    4. No Parity    
***************************************************************/

/*  
    Web Link: 
*/

/**************************************************************/

#include "stm32f4xx_hal.h"

void initUSART(void);
void initGPIOs(void);

UART_HandleTypeDef usartHandle;

uint8_t rxData;

int main () {

  
  /*
    Initialize HAL libararies.
  */
  HAL_Init();
  
  /*
    initialize GPIOs pins multiplexed with
    USART3
  */
  initGPIOs();
  
  /*
    Initialize USART3 for Asynchronous
    communication
  */
  initUSART();
  
  
  while (1) {}

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    /*
        This will be called once transmission completes
        you can add you own functinality code here
    */
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    
    /*
        This will be called once data is received successfully,
        via interrupts.
    */
    
     /*
       loop back received data
     */
     HAL_UART_Transmit_IT (&usartHandle, &rxData, 1);    
    
}


void initGPIOs(void) {
  
  /*
      Configure GPIOs as an alternate function 
      for USART3.
  */
  
  GPIO_InitTypeDef GPIO_InitStruct;

	/*
		enable clock to GPIOB for it pins will be used by USART3
	*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
  
    /*
		USART3 GPIO Configuration    
		PB10     ------> USART3_TX
		PB11     ------> USART3_RX 
    */
	
	GPIO_InitStruct.Pin 		= GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode 	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull 	= GPIO_PULLUP;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
}

void initUSART(void) {
  
  /*
    enable clock to USART-3
  */
	__HAL_RCC_USART3_CLK_ENABLE();
  
  /*
    USART-3 configuration
  */
  usartHandle.Instance = USART3;
  usartHandle.Init.BaudRate = 9600;
  usartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  usartHandle.Init.StopBits = UART_STOPBITS_1;
  usartHandle.Init.Parity = UART_PARITY_NONE;
  usartHandle.Init.Mode = UART_MODE_TX_RX;
  usartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  usartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&usartHandle) != HAL_OK)
  {
    while (1); // usart-3 initialization failed
  }
  
  NVIC_EnableIRQ(USART3_IRQn);
  /*
    enable Tx, Rx Interrupts
  */
  __HAL_UART_ENABLE_IT(&usartHandle, UART_IT_RXNE);
  __HAL_UART_ENABLE_IT(&usartHandle, UART_IT_TC);
  
  
}

