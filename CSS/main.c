/*
@author:          Ijaz Ahmad

@description:     This Program configures the onchip
                  RCC Clock Security System (CSS).
                  
@warrenty:        void
*/

/*  
    Web Link: 
*/

#include "stm32f4xx_hal.h"

int main () {
 
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  
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

 /**********************************************************************/

  
  /*
    CSS is only applicable if system is directly or 
    indirectly (via PLL) fed from HSE oscillator.
  
    -> so turn HSE ON first
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while (1); // clock initialization failed
  }

  /*
    configure BUS frequencies
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    while (1); // clock initialization failed
  }

  
    /*****************************************

        Enables the Clock Security System 

     ****************************************/
  HAL_RCC_EnableCSS();
  
  
  __ASM {
    
    BKPT #0x3

  }
  
}

