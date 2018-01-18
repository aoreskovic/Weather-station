#include "strings.h"

#include "tm_stm32f4_usart.h"
/*
int strlen(const char *str)
{
    const char *s;

    s = str;
    while (*s)
        s++;
    return s - str;
}
*/
char *strrevv(char *str)
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

char *itoaa(int n, char *s, int b)
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

    return strrevv(s);
}

int splitString(char *str, int *starts)
{

    char buf[10];
    char *index;
    int i = 0;
    int cnt = 0;

    TM_USART_Puts(USART2, "\r\nsplitString: ");
    TM_USART_Puts(USART2, itoaa(strlen(str), buf, 10));
    TM_USART_Puts(USART2, "\r\n");

    for(i=0; i < 91; i++){
        if(str[i] == '$'){
            starts[cnt] = i;
            cnt++;
            if(cnt == 15)
                return -1;
        }
    }

    return cnt;
}

void getNthString(char* out, char* in, int* indexes, int n){
    if(n > 12){
        out[0] = '\0';
        return;
    }
        

    int i;
    int k = 0;
    for(i = indexes[n] + 1; i < indexes[n+1]; i++){
        out[k] = in[i];
        k++;
    }
    out[k] = '\0';

}

