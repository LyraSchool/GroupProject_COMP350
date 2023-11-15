
//Made By:
//Lyra Brown
//Alicia Kenneally
//Lauren Rezendes

#include "files.h"
#include "program.h"


void readString(char*);
void printChar(char);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void printString(char*);
void terminate();


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

	char buffer[13312];
	int sectorsRead;

	// Clears the screen
	interrupt(0x10, 0x03, 0, 0, 0);
	// Creates int0x21
	makeInterrupt21();


	// interrupt(0x21, 3, "messag", buffer, &sectorsRead);
	
	// if (sectorsRead > 0)
	// 	interrupt(0x21, 0, buffer, 0, 0);
	// else
	// 	interrupt(0x21, 0, "messag not found\r\n", 0, 0);

	// interrupt(0x21, 0, "Finished with readfile, moving on to executeProgram\r\n", 0, 0);

	// interrupt(0x21, 4, "tstpr2", 0, 0);

	interrupt(0x21, 4, "shell", 0, 0);

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

void terminate()
{
	// char term[14];
	// term[0] = 'T';
	// term[1] = 'e';
	// term[2] = 'r';
	// term[3] = 'm';
	// term[4] = 'i';
	// term[5] = 'n';
	// term[6] = 'a';
	// term[7] = 't';
	// term[8] = 'i';
	// term[9] = 'n';
	// term[10] = 'g';
	// term[11] = '\r';
	// term[12] = '\n';
	// term[13] = '\0';
	
	//printString("Terminating");
	// printString(term);
	// while(1);

	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';
	executeProgram(shellname);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	char printbuf[31];

	if ( ax == 0 ) {
		printString((char*) bx);
	} else if ( ax == 1 ) {
		readString((char*) bx);
	} else if ( ax == 2 ) {
		readSector((char*) bx, cx);
	} else if ( ax == 3 ) {
		readFile((char*)cx, (char*)bx, (int*)dx);
	} else if ( ax == 4) {
		executeProgram((char*) bx);
	} else if ( ax == 5) {
		terminate();
	} else {
		// Invalid ax for Interrupt21\r\n
		printbuf[ 0] = 'I';
		printbuf[ 1] = 'n';
		printbuf[ 2] = 'v';
		printbuf[ 3] = 'a';
		printbuf[ 4] = 'l';
		printbuf[ 5] = 'i';
		printbuf[ 6] = 'd';
		printbuf[ 7] = ' ';
		printbuf[ 8] = 'a';
		printbuf[ 9] = 'x';
		printbuf[10] = ' ';
		printbuf[11] = 'f';
		printbuf[12] = 'o';
		printbuf[13] = 'r';
		printbuf[14] = ' ';
		printbuf[15] = 'I';
		printbuf[16] = 'n';
		printbuf[17] = 't';
		printbuf[18] = 'e';
		printbuf[19] = 'r';
		printbuf[20] = 'r';
		printbuf[21] = 'u';
		printbuf[22] = 'p';
		printbuf[23] = 't';
		printbuf[24] = '2';
		printbuf[25] = '1';
		printbuf[26] = '\r';
		printbuf[27] = '\n';
		printbuf[28] = '\0';
		
		printString(printbuf);
	}

}

