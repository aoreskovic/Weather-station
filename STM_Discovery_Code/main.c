/* main.c */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stm32f4_discovery.h>

#include <main.h>

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
    int i;
    int x;
    char buf[10];

    LCDI2C_init(0x27, 16, 2); //Setup for I2C address 27, 16x2 LCD.

    // Create and write special character at location 0.
    //  Locations 0-7 are available in the 44780 controller:
    LCDI2C_createChar(0, heart);
    // It was necessary to do a LCDI2C_clear after the char definition.
    // Probably a quirk of the library.
    LCDI2C_clear();

    // -------  blink backlight  -------------
    LCDI2C_backlight(); //Turn on Backlight
    Delay(100);
    LCDI2C_noBacklight(); //Turn off Backlight
    Delay(100);
    LCDI2C_backlight(); //Turn on Backlight

    LCDI2C_write_String("STM32F103 ");

    LCDI2C_write(0); //Write the special character at the next available location.
    // Another ASCII character can also be written.

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
        x = x + 1;
        Delay(200);
        if (x > 255)
            x = 0;
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
