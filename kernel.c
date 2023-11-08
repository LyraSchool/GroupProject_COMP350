
//Made By:
//Lyra Brown
//Alicia Kenneally
//Lauren Rezendes


void readString(char*);
void printChar(char);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void printString(char*);


/* Depends on printString to exist. */
void main()
{
	/* Variables have to be declared at the top of the function. */
	/*
	char buf[50];
	char buffer[512];
	char line[80];

	printString("Hello World");
	printString("\r\n");

	printString("Enter a string: ");

	readString(buf);
	printString(buf);
	printString("\r\n");

	readSector(buffer, 30);
	printString(buffer);
	printString("\r\n");

	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	*/

	
	
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

void readSector(char* buffer, int sector)
{
    int ax = 2 * 256 + 1;
    int bx = buffer;
    int cx = 0 * 256 + (sector + 1);
    int dx = 0 * 256 + 0x80;

    interrupt(0x13, ax, bx, cx, dx);
}

void printString(char* chars)
{
    int i = 0;
    while (chars[i] != 0x0)
    {
        printChar(chars[i]);
        i++;
    }

}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if ( ax == 0 ) {
		printString((char*) bx);
	} else if ( ax == 1 ) {
		readString((char*) bx);
	} else if ( ax == 2 ) {
		readSector((char*) bx, cx);
	} else {
		printString("Invalid ax for Interrupt21\r\n");
	}

}

