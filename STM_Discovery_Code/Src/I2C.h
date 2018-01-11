#include <stm32f4xx.h> // common stuff
#include <stm32f4xx_gpio.h> // gpio control
#include <stm32f4xx_rcc.h> // reset anc clocking

#include "delay.h"
#include "stm32f4xx_i2c.h"


void init_I2C1(void);
void I2C_StartTransmission(I2C_TypeDef *I2Cx, uint8_t transmissionDirection, uint8_t slaveAddress);
void I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t data);
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx);
