/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    Selects and configures various clock sources of STM32F4-Discovery  
 *
 */

/*
 * @WebLink: 
 */

#include "stm32f4xx_hal.h"

void SysClock_configHSI(void);
void SysClock_configHSE(void);
void SysClock_configPLL(void);

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
    NOTE: uncomment the one required 
  */
  
  SysClock_configHSI();  //Configure Internal 16Mhz oscillator as System Clock
//  SysClock_configHSE();  //Configure external 8Mhz clock as System Clock
//  SysClock_configPLL();  //Configure PLL as System Clock (feed from internal 16Mhz oscillator)
  
  __ASM {    
    BKPT #0x3    
  }
  
}

/*
  select HSI as main system clock
*/
void SysClock_configHSI(void) {

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /*
    select HSI for Configuration
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  
  /*
    turn HSI ON
  */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  /*
    disable PLL
  */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while (1); // clock selection failed
  }

  
  /*
    configure SYSCLK, HCLK, PCLK1, PCLK2
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  
  
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  /*
    no division -> adjustable
  */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    while (1); // clock selection failed
  }
  
}

/*
  select HSE as main system clock
*/
void SysClock_configHSE(void) {

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /*
    select HSE for Configuration
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  
  /*
    turn HSE ON
  */
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;

  /*
    disable PLL
  */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while (1); // clock selection failed
  }

  
  /*
    configure SYSCLK, HCLK, PCLK1, PCLK2
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;

  /*
    no division -> adjustable
  */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    while (1); // clock selection failed
  }
  
}

/*
  select PLL as main system clock
  SoC runs at its full capacity of 168Mhz
*/
void SysClock_configPLL(void) {
  
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /*
    turn ON HSI
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;  
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  /*
    feed PLL from HSI
  */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;

  /*
    SYSCLK = 168Mhz (max allowed fro STM32F4-Discovery
  */
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while (1); // pll initialization failed
  }

  /*
    Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  /*
    HCLK = 168Mhz -> max allowed
  */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  
  /*
    PCLK1 = 42Mhz -> max allowed
  */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;

  /*
    PCLK2 = 84Mhz -> max allowed
  */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while (1); // pll initialization failed
  }
  
  
}
