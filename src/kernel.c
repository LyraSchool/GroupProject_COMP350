
#define VIDMEM 0xb800
void main()
{
	char* message = "Help";
	int offset = 0;
	while (*message != '\0')
	{
		putInMemory(VIDMEM, offset, *message);
		offset++;
		putInMemory(VIDMEM, offset, 0x7);
		offset++;
		message++;
	}

	while(1);
}

