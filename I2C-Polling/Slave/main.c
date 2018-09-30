/*
    @author:    Ijaz Ahmad
    
    @warrenty:  void
    
    @description:   A simple I2C slave node that receives
                    dummy data continuously on I2C 
                    bus in standard mode. If data received
                    correctly (0xAA see Master node code),
                    it toggles onboard Green LED.
*/

/*
    global variable/handles
*/

#include "stm32f4xx_hal.h"

I2C_HandleTypeDef i2cInitStruct;

void configI2C1 (void);

void initLED (void);

int main () {
    
    uint8_t rxData = 0;
    
    HAL_Init();
    
    /*
       init I2C1 interface and respective 
       GPIOs Pins
    */    
    configI2C1();
    
    /*
       Configure on-board LED (PD.15)
    */    
    initLED();
    
    for (;;) {
        
        HAL_I2C_Slave_Receive(&i2cInitStruct, &rxData, 1, 50);
        
        if (rxData == 0xAA) {
            
            /*
                each time correct data is received,
                the on-board LED is toggaled!
            */
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
            
            rxData = 0;   
        }
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
    i2cInitStruct.Init.OwnAddress1 = 250;   /* Own address = 125 */
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

void initLED (void) {
    
    GPIO_InitTypeDef GPIO_InitStruct;

    /*
        onboard Green LED (PD.15) 
        Configuration
    */
    
    /* Enable clock to GPIOD */
  __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin : PD.15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);    
}

