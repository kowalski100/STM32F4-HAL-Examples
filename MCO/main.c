/*
@author:    Ijaz Ahmad

@description:     This Program configures PA.8 Pin (MCO1)
                  to output HSI clock as Test Clock.

@warrenty:  void
*/

/*  
    Web Link: 
*/

#include "stm32f4xx_hal.h"

void configureMCO_1 (void);

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

 /**********************************************************************/

  
  /*
    configure MCO_1 to output HSI/4
    assuming HSI as default clock source selected.
  */
  configureMCO_1();
  
  __ASM {
    
    BKPT #0x3
    
  }
  
}

void configureMCO_1 (void) {

  GPIO_InitTypeDef GPIO_InitStruct;

  /* MCO1 --> PA.8 */
  
  /*
    Enable clock to GPIOA
  */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*********************
    PA.8 Configuration
  *********************/
  
  /* PA8 will be used for clock output (MCO1) and not
  
    as GPIO so configure it for alternate funciton
  */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /********************
      MCO1 Settings 
  ********************/
  /*
    -> Route HSI to MCO
    -> Prescale by factor of 4 --> 4Mhz output on MCO1 (in range low quality analyzer)
  */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_4);
  
}

