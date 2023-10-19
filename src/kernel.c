void readString(char*);
void printChar(char);

void printString(char*);

/* Depends on printString to exist. */
void main()
{
	/* Variables have to be declared at the top of the function. */
	char buf[50];

	printString("Enter a string: ");

	readString(buf);
	printString(buf);
	printString("\r\n");

	while(1);
}

/*
	Utility function, feel free to use/write your own! 
	also, remove this one if you do write another since
	you can't have two functions with the same name lol 
*/
void printChar(char c)
{
    int ax = (0xe << 8) | c;
    interrupt(0x10, ax, 0, 0, 0);
}


void readString(char* buffer)
{   
    int bufferIndex = 0;
    char input = 0;
    while (input != 0xd)
    {
        input = interrupt(0x16, 0, 0, 0, 0);
        if (input == 0x8)
        {
            if (bufferIndex > 0)
            {
                bufferIndex--;
                buffer[bufferIndex] = 0x0;
                printChar(input);
                printChar(' ');
                printChar(input);
            }
        }
        else
        {
            buffer[bufferIndex] = input;
            bufferIndex++;
            printChar(input);
        }
    }
    buffer[bufferIndex] = 0x0;
    printChar(0xd); // CR
    printChar(0xa); // LF
}

