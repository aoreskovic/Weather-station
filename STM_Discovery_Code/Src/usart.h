#include <stm32f4xx.h>					// common stuff
#include <stm32f4xx_gpio.h>				// gpio control
#include <stm32f4xx_rcc.h>				// reset anc clocking
#include <stm32f4xx_usart.h> 

#define MAX_STRLEN 5000                        // this is the maximum string length of our string in characters

void init_USART1(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);


