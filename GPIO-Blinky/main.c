/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    Blinks LED (GPIOD-15) on STM32F407-Discovery 
 *
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
        clock (16MHz). PLL is disabled
    */
    
    /* Enable clock to GPIO-D */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* Set GPIOD Pin#15 Parameters */
    GPIO_InitStruct.Pin     = GPIO_PIN_15;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    /* Init GPIOD Pin#15 */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    for (;;) {
        
        /* Toggle GPIOD Pin#15 --> BLUE LED on STM32F407-Discovery */
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
        
        /* 100ms second delay */
        HAL_Delay(100);
        
    }
    
}

