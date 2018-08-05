/*
@author:          Ijaz Ahmad

@description:     This Program configures STM32f4-Discover
                  on-board USER button and blinks on-board 
                  Blue - LED connected to PD.15 when button 
                  is pressed.

@warrenty:        void
*/

/*  
    Web Link:
*/


#include "stm32f4xx_hal.h"

int main () {
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
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
 
  /*********************************
  
        Configure LED PD.15
  
   *********************************/
      
    /* Enable clock to GPIO-D */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* Set GPIOD Pin#15 Parameters */
    GPIO_InitStruct.Pin     = GPIO_PIN_15;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    /* Init GPIOD Pin#15 */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


   /********************************
  
     Configure Blue Button PA.0
  
   *********************************/

    /* Enable clock to GPIO-A */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Set GPIOA Pin#0 Parameters */
    GPIO_InitStruct.Pin     = GPIO_PIN_0;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    /* Init GPIOA Pin#0 */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    for (;;) {
      
      if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ) {

        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);        

      } else {

        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

      }
       
    }
    
}

