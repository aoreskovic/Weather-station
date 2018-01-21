/* main.c */

#include <stdio.h>
#include <string.h>



#include <main.h>
//#include <stdlib.h>
//#include <string.h>

#define MAX_STRLEN 300

volatile char received_string[MAX_STRLEN + 1]; // this will hold the recieved string

// forward declarations of task functions
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

char uart_buffer[401] = "";

char LCD_output[4][41];

int main(void)
{
    SystemInit();
    uint8_t data;
    //Describe special character:

    TM_USART_Init(USART2, TM_USART_PinsPack_1, 115200);

    /* Put string to USART */

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

    LCDI2C_createChar(6, celz);
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
    LCDI2C_write(6);

    //Set cursor position. (column, line) Top line is 0, bottom is 1.
    LCDI2C_setCursor(0, 1);

    //Write an integer:
    LCDI2C_write_String("Weather Station");

    //USART_puts(USART2, "Init complete! Hello World!\r\n"); // just send a message to indicate that it works

    gpio_init();
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
    xTaskCreate(vTask1, "TASK1", 1000, NULL, 1, NULL);
    vTaskStartScheduler();
}


void vTask1(void *pvParameters)
{   
    char buf[10];
    int numchar = 0;

    int strindex[15];
    int i;
    char out[12][16];

    while (1)
    {
        vTaskDelay(1000 / portTICK_RATE_MS);

        numchar = TM_USART_Gets(USART2, uart_buffer, 400);
        if((numchar == 0) || (uart_buffer[0] != '^'))
            continue;

        printf("\r\nNumchar: %d\r\n", numchar);
        TM_USART_Puts(USART2, uart_buffer);


        printf("\r\nsplitString: %d", splitString(uart_buffer, strindex));
        printf("\r\nDal radi scanf?");

        char bu1[25] = "";
        char bu2[25] = "";
        char bu3[25] = "";
        char bu4[25] = "";

        char weat[10] = "";


        getNthString(bu1, uart_buffer, strindex, 0);
        getNthString(bu2, uart_buffer, strindex, 3);
        getNthString(bu3, uart_buffer, strindex, 7);
        {
            int j = 0;
            if((strstr(bu2, "clear") != NULL) || (strstr(bu3, "clear") != NULL) || (strstr(bu2, "sun") != NULL) || (strstr(bu3, "sun") != NULL)){
                weat[j++] = 2;
                weat[j++] = 3;
            }
            if((strstr(bu2, "cloud") != NULL) || (strstr(bu3, "cloud") != NULL)){
                weat[j++] = 4;
                weat[j++] = 5;
            }
            if((strstr(bu2, "rain") != NULL) || (strstr(bu3, "rain") != NULL)){
                weat[j++] = 1;
            }
            weat[j] = 0;
        }
        sprintf(LCD_output[0], "%s", bu1);


        getNthString(bu1, uart_buffer, strindex, 1);
        getNthString(bu2, uart_buffer, strindex, 5);
        getNthString(bu3, uart_buffer, strindex, 4);
        sprintf(LCD_output[1], "T %s\6(%s-%s)\6C %s", bu1, bu2, bu3, weat);

        sprintf(LCD_output[2], " Za sutra:");


        //---------------------------------------
        getNthString(bu2, uart_buffer, strindex, 11);
        {
            int j = 0;
            if(strstr(bu2, "clear") != NULL){
                weat[j++] = 2;
                weat[j++] = 3;
            }
            if(strstr(bu2, "cloud") != NULL){
                weat[j++] = 4;
                weat[j++] = 5;
            }
            if(strstr(bu2, "rain") != NULL){
                weat[j++] = 1;
            }
            weat[j] = 0;
        }


        getNthString(bu2, uart_buffer, strindex, 9);
        getNthString(bu3, uart_buffer, strindex, 8);
        sprintf(LCD_output[3], "T %s-%s\6C     %s", bu2, bu3, weat);

        for(i = 0; i < 4; i++){
            LCDI2C_setCursor(0, i);
            LCDI2C_write_String(LCD_output[i]);
        }
        
    }
}
