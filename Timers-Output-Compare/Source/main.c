/*
 * @author:         Ijaz Ahmad
 *
 * @Warrenty:       void
 *
 * @description:    This code configures Timer-4 output
 *                  channels, connected to onboard GPIOS
 *                  in ouput compare mode. Each Channel
 *                  upon successful match, toggle one of 
 *                  the onboard LED.
 */

/*
 * @WebLink: 
 */

#include "stm32f4xx_hal.h"

#define     UP_COUNTER      1U

/*
    Functions Prototypes
*/
static void initLEDs(void);
static void initTimer(void);

/*
    Global Variables for External Linkage
*/
TIM_HandleTypeDef TIM_InitStruct;


int main () {
    
    /*
        initialize HAL Library. This step is mandatory
        for HAL libraries to work properly.
     */
    HAL_Init();

    /*
        Set Prioirty group to 0. OPTIONAL
    */
    NVIC_SetPriorityGrouping(0);
    
    /*
        After HAL_Init(), System is running on default HSI 
        clock (16MHz). PLL is disabled. The clock to APB1
        bus is 16Mhz (HSI clock).
   */
    
   /*
        initialize LED GPIO
   */
    initLEDs();
    
   /*
        initialize timer base
   */
   initTimer();
    
   while (1);
   
}

/*
    configures onboard LEDs (GPIOD Pin#14,15).
 */
static void initLEDs(void) {

    GPIO_InitTypeDef GPIO_InitStruct;
        
    /* Enable clock to GPIO-D */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* Set GPIOD Pin#12,13,14,15 Parameters */
    GPIO_InitStruct.Pin         =   GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode        =   GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate   =   GPIO_AF2_TIM4;
    
    /* Init GPIOD Pins */
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

static void initTimer(void) {
    
    TIM_OC_InitTypeDef TIM_Channel_InitStruct;
    
    /* 
        Enable clock to Timer-4 
        
        NOTE: This is lagacy Macro. The better approach is the
        use of HAL_TIM_ConfigClockSource function.
    */
    __HAL_RCC_TIM4_CLK_ENABLE();
    
    /*
        From STM32F407 datasheet, Timer4 is clocked from
        APB1 bus (42Mhz max). In default configuration
        Timer-4 is receiving 16Mhz (HSI) bus clock.
    */        
    
    /***************************************************
                   Timer-4 Configuration:
    ****************************************************/
    
    /* 
        Select Timer-4 for further configuration 
        The reason for selecting Timer-4 is that
        its output channels are directly connected
        to GPIOD - The same GPIOS to which STM32F4
        onboard LEDs are connected.
    */
    TIM_InitStruct.Instance = TIM4;
    
    /*
        Divide the timer-4 input frequency (16Mhz)
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
        We want the time count to be 500msec (0.5-second).
        As the input frequency is 16khz so the total
        counts required for 500msec delay:
        
        total counts = 1000msec * f
                     = (0.5 sec) * 16,000
                     = 8,000
    */

    TIM_InitStruct.Init.Period = 8000;

    /*
        Finally initialize Timer-4, for Output Compare
    */
    while (HAL_TIM_OC_Init(&TIM_InitStruct)!= HAL_OK);

    /* All channels common configuration */
    TIM_Channel_InitStruct.OCMode       =   TIM_OCMODE_TOGGLE;
    TIM_Channel_InitStruct.OCPolarity   =   TIM_OCPOLARITY_HIGH;
    TIM_Channel_InitStruct.OCFastMode   =   TIM_OCFAST_DISABLE;
    
    /* Channel-1 Configuration */
    TIM_Channel_InitStruct.Pulse        =   2000;
    HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_1);

    /* Channel-2 Configuration */
    TIM_Channel_InitStruct.Pulse        =   4000;
    HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_2);

    /* Channel-3 Configuration */
    TIM_Channel_InitStruct.Pulse        =   6000;
    HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_3);

    /* Channel-4 Configuration */
    TIM_Channel_InitStruct.Pulse        =   8000;
    HAL_TIM_OC_ConfigChannel(&TIM_InitStruct, &TIM_Channel_InitStruct, TIM_CHANNEL_4);


    HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&TIM_InitStruct, TIM_CHANNEL_4);

}
