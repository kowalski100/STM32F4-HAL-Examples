/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @descp:        The program configures PA.0 to generate interrupt
 *                on rising edge of input signal. Upon receiving 
 *                interrupt, PD.15 (BLUE-LED) is toggled.
 *                The signal to PA.0 is given from PD.14, which is
 *                also connected to on-board RED-LED.
 *
 */

/*
 * @WebLink: 
 */

#include "stm32f4xx_hal.h"

void configGPIO(void);
void configSPI(void);

SPI_HandleTypeDef SPI1_InitStruct;
SPI_HandleTypeDef SPI2_InitStruct;

int main () {
    

    
    /*
        initialize HAL Library. This step is mandatory
        for HAL libraries to work properly. It configures
        SysTick etc. for its internal configurations.
    */
    HAL_Init();
    
    /*
        After HAL_Init(), System is running on default HSI 
        clock (16MHz). PLL is disabled.
    */
    
    configGPIO();
    configSPI();

    __HAL_SPI_ENABLE_IT(&SPI2_InitStruct, SPI_IT_RXNE);
  
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
  
    __HAL_SPI_ENABLE(&SPI1_InitStruct);
    __HAL_SPI_ENABLE(&SPI2_InitStruct);
    
    for (;;) {
        
    }
}

void configSPI (void) {

  __HAL_RCC_SPI1_CLK_ENABLE();
  __HAL_RCC_SPI2_CLK_ENABLE();
  
    /* SPI1 parameter configuration*/
  SPI1_InitStruct.Instance = SPI1;
  SPI1_InitStruct.Init.Mode = SPI_MODE_MASTER;
  SPI1_InitStruct.Init.Direction = SPI_DIRECTION_2LINES;
  SPI1_InitStruct.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI1_InitStruct.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SPI1_InitStruct.Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI1_InitStruct.Init.NSS = SPI_NSS_SOFT;
  SPI1_InitStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  SPI1_InitStruct.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI1_InitStruct.Init.TIMode = SPI_TIMODE_DISABLE;
//  SPI1_InitStruct.State = 
  SPI1_InitStruct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI1_InitStruct.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&SPI1_InitStruct) != HAL_OK)
  {
    while (1); // spi configuration failed
  }

    /* SPI2 parameter configuration*/
  SPI2_InitStruct.Instance = SPI2;
  SPI2_InitStruct.Init.Mode = SPI_MODE_SLAVE;
  SPI2_InitStruct.Init.Direction = SPI_DIRECTION_2LINES;
  SPI2_InitStruct.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI2_InitStruct.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SPI2_InitStruct.Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI2_InitStruct.Init.NSS = SPI_NSS_SOFT;
  SPI2_InitStruct.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI2_InitStruct.Init.TIMode = SPI_TIMODE_DISABLE;
  SPI2_InitStruct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI2_InitStruct.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&SPI2_InitStruct) != HAL_OK)
  {
    while (1); // spi configuration failed
  }

  
}

void configGPIO(void) {

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configuratoin for user button: PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configuratoin fo Blue LED : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*
    Alternate pin configuration for SPI-1
  */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*
    Alternate pin configuration for SPI-2
  */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  
}

