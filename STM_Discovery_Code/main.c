/* main.c */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "string.h"
#include "icons.h"

#include <stm32f4_discovery.h>

#include "defines.h"
#include "tm_stm32f4_usart.h"

#include <main.h>
//#include <stdlib.h>
//#include <string.h>

#define MAX_STRLEN 300

volatile char received_string[MAX_STRLEN + 1]; // this will hold the recieved string

// forward declarations of task functions
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);

xTaskHandle xTaskHandle1, xTaskHandle2, xTaskHandle3, xTaskHandle4;

char uart_buffer[401] = "";

char LCD_output[4][41];

int main(void)
{
    SystemInit();
    uint8_t data;
    //Describe special character:

    TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

    /* Put string to USART */
    TM_USART_Puts(USART2, "Hello world\n\r");

    int i;

    LCDI2C_init(0x27, 20, 4); //Setup for I2C address 27, 16x2 LCD.

    // Create and write special character at location 0.
    //  Locations 0-7 are available in the 44780 controller:
    LCDI2C_createChar(0, heart);
    LCDI2C_clear();
    LCDI2C_createChar(1, drop);
    // It was necessary to do a LCDI2C_clear after the char definition.
    LCDI2C_clear();

    LCDI2C_createChar(2, sun_l);
    LCDI2C_clear();
    LCDI2C_createChar(3, sun_r);
    LCDI2C_clear();

    LCDI2C_createChar(4, cloud_l);
    LCDI2C_clear();
    LCDI2C_createChar(5, cloud_r);
    LCDI2C_clear();

    // -------  blink backlight  -------------
    /*LCDI2C_backlight(); //Turn on Backlight
    Delay(100);
    LCDI2C_noBacklight(); //Turn off Backlight
    Delay(100);*/
    LCDI2C_backlight(); //Turn on Backlight

    LCDI2C_write_String("Ikone: ");

    LCDI2C_write(0); //Write the special character at the next available location.
    // Another ASCII character can also be written.
    LCDI2C_write(1);
    LCDI2C_write(2);
    LCDI2C_write(3);
    LCDI2C_write(4);
    LCDI2C_write(5);

    //Set cursor position. (column, line) Top line is 0, bottom is 1.
    LCDI2C_setCursor(0, 1);

    //Write an integer:
    LCDI2C_write_String("ARM-GCC  x=");

    //USART_puts(USART2, "Init complete! Hello World!\r\n"); // just send a message to indicate that it works

    gpio_init();
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
    xTaskCreate(vTask1, "TASK1", 1000, NULL, 1, &xTaskHandle1);
    xTaskCreate(vTask2, "TASK2", 1000, NULL, 1, NULL);
    vTaskStartScheduler();
}

void vTask1(void *pvParameters)
{
    int x = 0;
    char buf[10];

    while (1)
    {
        itoa(x, buf, 16); // itoa takes number base as 3rd argument. Here; hex.
        LCDI2C_setCursor(11, 1);
        LCDI2C_write_String(buf);
        LCDI2C_write_String("  ");

        //LCDI2C_setCursor(18, x % 4 + 1);
        //LCDI2C_write_String("  ");
        x = x + 1;
        if (x > 3)
            x = 0;

        LCDI2C_setCursor(15, (x + 1) % 4);
        LCDI2C_write(1);
        LCDI2C_setCursor(16, (x + 2) % 4);
        LCDI2C_write(1);
        LCDI2C_setCursor(17, (x + 0) % 4);
        LCDI2C_write(1);
        vTaskDelay(500 / portTICK_RATE_MS);
        LCDI2C_setCursor(15, (x + 1) % 4);
        LCDI2C_write_String(" ");
        LCDI2C_setCursor(16, (x + 2) % 4);
        LCDI2C_write_String(" ");
        LCDI2C_setCursor(17, (x + 0) % 4);
        LCDI2C_write_String(" ");
    }
}

void vTask2(void *pvParameters)
{   
    char buf[10];
    int numchar = 0;

    int strindex[15];

    while (1)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);

        numchar = TM_USART_Gets(USART2, uart_buffer, 400);
        if((numchar == 0) || (uart_buffer[0] != '^'))
            continue;

        
        TM_USART_Puts(USART2, "\r\nNumchar: ");
        TM_USART_Puts(USART2, itoa(numchar, buf,10));
        TM_USART_Puts(USART2, "\r\n");
        TM_USART_Puts(USART2, uart_buffer);

        TM_USART_Puts(USART2, "\r\nsplitString: ");
        TM_USART_Puts(USART2, itoa(splitString(uart_buffer, strindex), buf,10));
        TM_USART_Puts(USART2, "\r\n");

        //printf(LCD_output[0], "Temperatura: %.1f (%1.f-%.1f) C", 1.1, 2.3, 4.5 );


        
    }
}

void vTask3(void *pvParameters)
{
    uint32_t led_state;
    led_state = 1;
    while (1)
    {
        vTaskDelay(500 / portTICK_RATE_MS);     // LED blinking frequency
        gpio_led_state(LED5_RED_ID, led_state); // orange LED
        led_state = (led_state == 1) ? 0 : 1;
    }
}

void vTask4(void *pvParameters)
{
    uint32_t led_state;
    led_state = 1;
    while (1)
    {
        vTaskDelay(500 / portTICK_RATE_MS);      // LED blinking frequency
        gpio_led_state(LED6_BLUE_ID, led_state); // orange LED
        led_state = (led_state == 1) ? 0 : 1;
    }
}
