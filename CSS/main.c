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

/*
  Interrupt Handler for NMI
*/
void NMI_Handler (void) {

  /*
    Verify if code security system has generated as interrupt  
  */  
  if ( RCC->CIR & (1U << 7)) {
    
    /*
      clear the interrupt
    */
    RCC->CIR |= (1U << 23);
    
    /*
     * Take appropriate action to reconfigure all the peripherals etc.
     * based on the new HIGH SPEED Internal RC OSCILLATOR-HSI (16Mhz);
     * AT THIS POINT SYSTEM IS RUNNING ON 16MHZ
     */
    
    /* place for reconfiguration */
  
  }
}


