/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    This program uses STM32F4-Discovery Timer-2 
 *                  in UP/DOWN-COUNTER mode to generate constant 
 *                  time delay (0.5sec) /interrupt. Upon each interrupt,  
 *                  the on-board Blue-LED (PD#15) is toggled.
 */

/*
 * @WebLink: 
 */

#include "stm32f4xx_hal.h"

/* SET UP_COUNTER TO 0 IF DOWN COUNTER IS REQUIRED */
#define     UP_COUNTER      1


/*
    Global Variables
*/
TIM_HandleTypeDef TIM_InitStruct;

/*
    functions prototypes  (Only declarations are required)
*/
static void initLED(void);

int main () {

    /*
        initialize HAL Library. This step is mandatory
        for HAL libraries to work properly. It configures
        SysTick etc. for its internal configurations.
    */
    HAL_Init();
   

    /*
        After HAL_Init(), System is running on default HSI 
        clock (16MHz). PLL is disabled. The clock to APB1
        bus is 16Mhz (HSI clock).
    */
    
   /*
        initialize LED GPIO - Blue-LED
    */
    initLED();
    
    /* 
        Enable clock to Timer-2 
        
        NOTE: This is lagacy Macro. The better approach is the
        use of HAL_TIM_ConfigClockSource function.
    */
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    /*
        From STM32F407 datasheet, Timer2 is clocked from
        APB1 bus (42Mhz max). In default configuration
        Timer-2 is receiving 16Mhz (HSI) bus clock.
    */        
    
    /***************************************************
                   Timer-2 Configuration:
    ****************************************************/
    
    /* Select Timer-2 for further configuration */
    TIM_InitStruct.Instance = TIM2;
    
    /*
        Divide the timer-2 input frequency (16Mhz)
        by a factor of 1000 (16,000,000/1,000 = 16,000 = 16Khz) 
    */
    TIM_InitStruct.Init.Prescaler   = 1000;
    
    #if (UP_COUNTER)
     /* Up-Counter mode*/
     TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    #else 
      TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_DOWN;        
    #endif

    /*
        We want the time count to be 500msec (half a second).
        As the input frequency is 16khz so the total
        counts required for 500msec delay:
        
        total counts = 500msec * f
                     = (.5 sec) * 16,000
                     = 8,000
                     = 0x1F40
    */
    TIM_InitStruct.Init.Period = 8000;
        
    /*
        Finally initialize Timer-2
    */
    while (HAL_TIM_Base_Init(&TIM_InitStruct)!= HAL_OK);

    /*
        Enable timer-2 IRQ interrupt
    */
    HAL_TIM_Base_Start_IT(&TIM_InitStruct);

    /* Enable interrupt at IRQ-Level */
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    
    /*
        Start the timer
    */
    HAL_TIM_Base_Start(&TIM_InitStruct);
    
    for (;;) {}
        
}

/*
    configures Blue-LED (GPIOD Pin#15).
 */
static void initLED(void) {

    GPIO_InitTypeDef GPIO_InitStruct;
        
    /* Enable clock to GPIO-D */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* Set GPIOD Pin#15 Parameters */
    GPIO_InitStruct.Pin     = GPIO_PIN_15;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    
    /* Init GPIOD Pin#15 */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);    
}

