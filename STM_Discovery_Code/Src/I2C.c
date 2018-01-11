
#include "I2C.h"

//Global structures as handles for peripheral devices:
GPIO_InitTypeDef i2c_gpio;
I2C_InitTypeDef i2c;

#define SLAVE_ADDRESS 0x27 // the slave address (example)

void init_I2C1(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;

  // enable APB1 peripheral clock for I2C1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  // enable clock for SCL and SDA pins
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* setup SCL and SDA pins
	 * You can connect I2C1 to two different
	 * pairs of pins:
	 * 1. SCL on PB6 and SDA on PB7 
	 * 2. SCL on PB8 and SDA on PB9
	 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // we are going to use PB6 and PB7
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;           // set pins to alternate function
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      // set GPIO speed
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;         // set output to open drain --> the line has to be only pulled low, not driven high
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;           // enable pull up resistors
  GPIO_Init(GPIOB, &GPIO_InitStruct);                 // init GPIOB

  // Connect I2C1 pins to AF
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); // SCL
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA

  // configure I2C1
  I2C_InitStruct.I2C_ClockSpeed = 100000;                                // 100kHz
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                                // I2C mode
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;                        // 50% duty cycle --> standard
  I2C_InitStruct.I2C_OwnAddress1 = 0x00;                                 // own address, not relevant in master mode
  I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;                              // disable acknowledge when reading (can be changed later on)
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
  I2C_Init(I2C1, &I2C_InitStruct);                                       // init I2C1

  // enable I2C1
  I2C_Cmd(I2C1, ENABLE);
}

/*******************************************************************/
void I2C_StartTransmission(I2C_TypeDef *I2Cx, uint8_t transmissionDirection, uint8_t slaveAddress)
{
  // Wait until I2C module is idle:
  while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    ;

  // Generate the start condition
  I2C_GenerateSTART(I2Cx, ENABLE);
  //
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    ;

  //Send the address of the slave to be contacted:
  I2C_Send7bitAddress(I2Cx, slaveAddress << 1, transmissionDirection);

  //If this is a write operation, set I2C for transmit
  if (transmissionDirection == I2C_Direction_Transmitter)
  {
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
      ;
  }
  //Or if a read operation, set i2C for receive
  if (transmissionDirection == I2C_Direction_Receiver)
  {
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
      ;
  }
}

/*******************************************************************/
void I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t data)
{
  // Write the data on the bus
  I2C_SendData(I2Cx, data);
  //Wait until transmission is complete:
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    ;
}

/*******************************************************************/
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx)
{
  // Wait until receive is completed
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
    ;
  uint8_t data;
  // Read one byte of data from I2c
  data = I2C_ReceiveData(I2Cx);
  return data;
}
