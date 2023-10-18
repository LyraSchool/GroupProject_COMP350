#include "strings.h"

#define VIDMEM 0xb800

int vMemOffset = 0;

void puts(char* string)
{
	char* s = string;
	while (*s != '\0')
	{
		putInMemory(VIDMEM, vMemOffset, *s);
		vMemOffset++;
		putInMemory(VIDMEM, vMemOffset, 0x7);
		vMemOffset++;
		s++;
	}
}





