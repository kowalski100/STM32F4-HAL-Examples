/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @descp:        In this tutorial we will send 0x55 from 
 *                SPI-1 (master) to SPI-2. Upon successful 
 *                reception, PD.15 (on board BLUE LED) is
 *                toggled.
 *
 */

/*
 * @WebLink: 
 */
#include <stdint.h>
#include "stm32f4xx_hal.h"

void configGPIO(void);
void configSPI(void);

SPI_HandleTypeDef SPI1_InitStruct;
SPI_HandleTypeDef SPI2_InitStruct;

uint8_t spiTxData = 0x55;
uint8_t spiRxData = 8;


int main () {
    
    volatile unsigned int i = 0;
    
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

    /*configure GPIOs (user button, Blue-LED, SPI-multiplexed Pins)*/
    configGPIO();
  
    /*configure SPI units*/
    configSPI();
  
    __HAL_SPI_ENABLE(&SPI1_InitStruct);
    __HAL_SPI_ENABLE(&SPI2_InitStruct);
     
    for (;;) {
      
       if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET) {

         /* for button debounce */
          for (i = 0;i < 40000; i++);
         
          if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET) {
              
              HAL_SPI_Transmit(&SPI1_InitStruct,&spiTxData,1, 500);
             
              /*wait for the transmission to complete*/
              while (__HAL_SPI_GET_FLAG(&SPI2_InitStruct,SPI_FLAG_TXE) != 1);
            
              /*prevent multiple transmission at a time*/
              while ((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)));
          }
           
           if (__HAL_SPI_GET_FLAG(&SPI2_InitStruct,SPI_FLAG_RXNE) == 1) {
             
             HAL_SPI_Receive(&SPI2_InitStruct,&spiRxData,1,500);
             
             if (spiRxData == 0x55) {
                HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
                spiRxData = 0;
             }
           }
       }
       
    }
    
}

void configSPI (void) {

  /*
    enable clock to SPI-1,2
  */
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

  if (HAL_SPI_Init(&SPI1_InitStruct) != HAL_OK)
  {
    while (1);  // spi configuration failed
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
  if (HAL_SPI_Init(&SPI2_InitStruct) != HAL_OK)
  {
    while (1);  // spi configuration failed
  }

}

void configGPIO(void) {

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  
  /* 
    USER button Configuration
    Configure GPIO pin : PA0 
  */

  /* Set GPIOA Pin#0 Parameters */
  GPIO_InitStruct.Pin     = GPIO_PIN_0;
  GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    /* Init GPIOA Pin#0 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* 
    Configure Blue-LED on-board
    Configure GPIO pin : PD15 
  */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*
    Configure SPI-1 Pens
  */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*
    Configure SPI-2 Pens
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
  
}

