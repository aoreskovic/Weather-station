/* main.c */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stm32f4_discovery.h>

#include <main.h>
//#include <stdlib.h>
//#include <string.h>

// forward declarations of task functions
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);

xTaskHandle xTaskHandle1, xTaskHandle2, xTaskHandle3, xTaskHandle4;

int strlen(const char *str)
{
    const char *s;

    s = str;
    while (*s)
        s++;
    return s - str;
}

char *strrev(char *str)
{
    char *p1, *p2;

    if (!str || !*str)
        return str;

    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }

    return str;
}

char *itoa(int n, char *s, int b)
{ //b is for number base; 2,8,10,16 etc
    static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0, sign;

    if ((sign = n) < 0)
        n = -n;

    do
    {
        s[i++] = digits[n % b];
    } while ((n /= b) > 0);

    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    return strrev(s);
}

int main(void)
{
    uint8_t data;
    //Describe special character:
    uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
    uint8_t drop[8] = {0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x1F, 0x1F, 0x0E};
    uint8_t sun_l[8] = { 0x08, 0x04, 0x01, 0x03, 0x1B, 0x03, 0x01, 0x04};
    uint8_t sun_d[8] = { 0x02, 0x04, 0x10, 0x18, 0x1B, 0x18, 0x10, 0x04};
    uint8_t obl_l[8] = {0x00, 0x03, 0x04, 0x0C, 0x12, 0x10, 0x0F, 0x00};
    uint8_t obl_d[8] = { 0x00, 0x10, 0x0C, 0x12, 0x01, 0x01, 0x1E, 0x00};
    int i;
    int x;
    char buf[10];

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
    LCDI2C_createChar(3, sun_d);
    LCDI2C_clear();

    LCDI2C_createChar(4, obl_l);
    LCDI2C_clear();
    LCDI2C_createChar(5, obl_d);
    LCDI2C_clear();



    // -------  blink backlight  -------------
    LCDI2C_backlight(); //Turn on Backlight
    Delay(100);
    LCDI2C_noBacklight(); //Turn off Backlight
    Delay(100);
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
    x = 0;
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
        Delay(500);
        LCDI2C_setCursor(15, (x + 1) % 4);
        LCDI2C_write_String(" ");
        LCDI2C_setCursor(16, (x + 2) % 4);
        LCDI2C_write_String(" ");
        LCDI2C_setCursor(17, (x + 0) % 4);
        LCDI2C_write_String(" ");
    }

    gpio_init();
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
    xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, &xTaskHandle1);
    vTaskStartScheduler();
}

void vTask1(void *pvParameters)
{
    uint32_t value, tasksCount, led_state;
    tasksCount = 1; // we have only one task at this point
    led_state = 1;
    while (1)
    {
        vTaskDelay(500 / portTICK_RATE_MS);       // LED blinking frequency
        gpio_led_state(LED4_GREEN_ID, led_state); // green LED
        led_state = (led_state == 1) ? 0 : 1;
        value = STM_EVAL_PBGetState(BUTTON_USER);
        if (value == 1)
        {
            // add new task
            tasksCount++;
            // keypressed - add new task
            if (tasksCount == 5)
            {
                // kill all tasks except this
                vTaskDelete(xTaskHandle2);
                vTaskDelete(xTaskHandle3);
                vTaskDelete(xTaskHandle4);
                // turn off all leds
                gpio_led_state(LED3_ORANGE_ID, 0);
                gpio_led_state(LED5_RED_ID, 0);
                gpio_led_state(LED6_BLUE_ID, 0);
                tasksCount = 1;
            }
            else
            {
                switch (tasksCount)
                {
                case 2:
                    xTaskCreate(vTask2, "TASK2",
                                configMINIMAL_STACK_SIZE,
                                NULL, 1, &xTaskHandle2);
                    break;
                case 3:
                    xTaskCreate(vTask3, "TASK3",
                                configMINIMAL_STACK_SIZE,
                                NULL, 1, &xTaskHandle3);
                    break;
                case 4:
                    xTaskCreate(vTask4, "TASK4",
                                configMINIMAL_STACK_SIZE,
                                NULL, 1, &xTaskHandle4);
                    break;
                }
            }
        }
    }
}

void vTask2(void *pvParameters)
{
    uint32_t led_state;
    led_state = 1;
    while (1)
    {
        vTaskDelay(500 / portTICK_RATE_MS);        // LED blinking frequency
        gpio_led_state(LED3_ORANGE_ID, led_state); // orange LED
        led_state = (led_state == 1) ? 0 : 1;
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
