/* main.h */
#ifndef MAIN_H
#define MAIN_H

#include <stm32f4xx.h>					// common stuff
#include <stm32f4xx_gpio.h>				// gpio control
#include <stm32f4xx_rcc.h>				// reset anc clocking
#include <stm32f4xx_usart.h> 
#include "gpio.h"
#include "delay.h"
#include "I2C.h"
#include "LiquidCrystal_I2C.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "strings.h"
#include "icons.h"

#include <stm32f4_discovery.h>

#include "defines.h"
#include "tm_stm32f4_usart.h"

#endif
