#include "string.h"

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

int splitString(char* str, int* starts){
    
    char* index;
    int i = 0;
    while(1){
        index = strstr(str, "$");
        if(index == NULL)
            return 0;
        starts[i] = index - str;
        i++;
    }
}
