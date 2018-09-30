/*
    @author:    Ijaz Ahmad
    
    @warrenty:  void
    
    @description:   A simple I2C master node that sends
                    dummy data (0xAA) continuously on I2C1 
                    interface in standard mode.
*/

/*
    global variable/handles
*/

#include "stm32f4xx_hal.h"

I2C_HandleTypeDef i2cInitStruct;

void configI2C1 (void);

int main () {
    
    uint8_t txData = 0xAA;
    
    HAL_Init();
    
    /*
        Initialize I2C1 interface and 
        respective GPIOs.
    */
    configI2C1();
    
    for (;;) {
        
        /* Transmit 0xAA to slave node */
        HAL_I2C_Master_Transmit(&i2cInitStruct, (125U << 1), &txData, 1, 5);
        
        /*
            500msec delay - for demonstration purpose only
        */
        HAL_Delay(500);
    }
    
}

void configI2C1 (void) {

    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*
        I2C1 initialization
    */
    
    /* Enable clock to I2C1 */
    __HAL_RCC_I2C1_CLK_ENABLE();

    
    i2cInitStruct.Instance = I2C1;
    i2cInitStruct.Init.ClockSpeed = 100000;
    i2cInitStruct.Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2cInitStruct.Init.OwnAddress1 = 242; /* Own Address 121 */
    i2cInitStruct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2cInitStruct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2cInitStruct.Init.OwnAddress2 = 0;
    i2cInitStruct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2cInitStruct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&i2cInitStruct);
    
  
    /*
        I2C1 multiplexed GPIOs initialization
    */
    /*******************************
    I2C1 GPIO Configuration    
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA 
    *******************************/
    
    
    /*
        enable clock to GPIO-B
    */
    __HAL_RCC_GPIOB_CLK_ENABLE();
  
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
