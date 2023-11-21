#include "numbers.h"

void itoa(int val, char* buffer)
{
    int i, sign;

    if ((sign = val) < 0) val = -val;

    i = 0;
    do {
        buffer[i++] = imod(val, 10) + '0';
    } while ((val /= 10) > 0);

    if (sign < 0) buffer[i++] = '-';
    buffer[i] = '\0';
    reverse(buffer);
}

int imod(int val, int modulus)
{
    return val - (val / modulus) * modulus;
}

void reverse(char* buffer)
{
    int i, j;
    char c;
    for (i = 0, j = slen(buffer) - 1; i < j; i++, j--)
    {
        c = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = c;
    }
}

int slen(char* s)
{
    int len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}
