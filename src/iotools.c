#include "iotools.h"
#include "numbers.h"

void printSector(char* sector)
{
	int i = 0;
	char pb[4];
    pb[2] = ' ';
	for (i = 0; i < 512; i++)
	{
		if (imod(i, 26) == 0)
		{
            puts("\r\n");			
		}

		itohex(pb, sector[i]);
		puts(pb);
	}
}

void puts(char* s)
{
    syscall(0x0, s, 0, 0);
}

void putc(char c)
{
    char pb[2];
    pb[0] = c;
    syscall(0x0, pb, 0, 0);
}

int isprintable(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' || c == '-';
}
