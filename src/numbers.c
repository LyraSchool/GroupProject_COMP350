#include "numbers.h"

void itohex(char* buf, int value)
{
	char dict[17];
	dict[ 0] = '0';
	dict[ 1] = '1';
	dict[ 2] = '2';
	dict[ 3] = '3';
	dict[ 4] = '4';
	dict[ 5] = '5';
	dict[ 6] = '6';
	dict[ 7] = '7';
	dict[ 8] = '8';
	dict[ 9] = '9';
	dict[10] = 'A';
	dict[11] = 'B';
	dict[12] = 'C';
	dict[13] = 'D';
	dict[14] = 'E';
	dict[15] = 'F';
	dict[16] = '\0';

	buf[0] = dict[(value & 0xF0) >> 4];
	buf[1] = dict[(value & 0x0F) >> 0];
	buf[2] = '\0';
}

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
    for (i = 0, j = strlen(buffer) - 1; i < j; i++, j--)
    {
        c = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = c;
    }
}

int strlen(char* s)
{
    int len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}
