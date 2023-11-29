
//Made By:
//Lyra Brown
//Alicia Kenneally
//Lauren Rezendes

#include "files.h"

void handleInterrupt21(int, int, int, int);
void handleTimerInterrupt(int, int);

void returnFromTimer(int segment, int sp);

void makeTimerInterrupt();
void makeInterrupt21();

void readString(char*);
void printChar(char);
void readSector(char*, int);
void printString(char*);
void terminate();
void writeSector(char*, int);

int processActive[8];
int processStackPointer[8];
int currentProcess;


#define iToSeg(i) (i + 2) * 0x1000

/* Depends on printString to exist. */
void main()
{
	int i;
	
	// Creates int0x21
	makeInterrupt21();

	for (i = 0; i < 8; i++)
	{
		processActive[i] = 0;
		processStackPointer[i] = 0xFF00;
	}
	currentProcess = -1;


	
	// Create interrupt for the process scheduler
	makeTimerInterrupt();


	// Clears the screen
	interrupt(0x10, 0x03, 0, 0, 0);
	// Launch to shell
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
    buffer[bufferIndex] = 0xa;
	buffer[bufferIndex + 1] = 0x0;
	
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

void writeSector(char* buffer, int sector){

	int ax = 3 * 256 + 1;
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

void executeProgram(char* filename)
{
	// executeProgram implementation here
	int sectorsRead;
	int segment = 0x2000;
	char buffer[13312];
	int i;
	char printbuf[25];

	// printString("executeProgram called\r\n");

	//Step 1 of Step 2
	// interrupt(0x21, 0, "EXP - Reading program\r\n", 0, 0);
	readFile(buffer, filename, &sectorsRead);

	if (sectorsRead == 0)
	{
		printbuf[ 0] = 'U';
		printbuf[ 1] = 'n';
		printbuf[ 2] = 'a';
		printbuf[ 3] = 'b';
		printbuf[ 4] = 'l';
		printbuf[ 5] = 'e';
		printbuf[ 6] = ' ';
		printbuf[ 7] = 't';
		printbuf[ 8] = 'o';
		printbuf[ 9] = ' ';
		printbuf[10] = 'f';
		printbuf[11] = 'i';
		printbuf[12] = 'n';
		printbuf[13] = 'd';
		printbuf[14] = ' ';
		printbuf[15] = 'p';
		printbuf[16] = 'r';
		printbuf[17] = 'o';
		printbuf[18] = 'g';
		printbuf[19] = 'r';
		printbuf[20] = 'a';
		printbuf[21] = 'm';
		printbuf[22] = ' ';
		printbuf[23] = '\"';
		printbuf[24] = '\0';
		printString(printbuf);
		printString(filename);
		printbuf[0] = '\"';
		printbuf[1] = '\r';
		printbuf[2] = '\n';
		printbuf[3] = '\0';
		printString(printbuf);
		return;
	}

	//Step 2 of Step 2
	// interrupt(0x21, 0, "Starting the for loop\r\n", 0, 0);

	for ( i=0; i<13312; i++){
		putInMemory(segment, i, buffer[i]);
	}
	// interrupt(0x21, 0, "launching\r\n", 0, 0);
	//Step 3 of Step 2
	launchProgram(segment);
}

void terminate()
{
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
	} else if ( ax == 6) {
		writeSector((char*)bx, cx);
	} else if ( ax == 7) {
		deleteFile((char*)bx);
	} else if ( ax == 8) {
		writeFile((char*)bx, (char*)cx, dx);
	} else {
		// Invalid ax for Interrupt21\r\n
		printChar('I');
		printChar('n');
		printChar('v');
		printChar('a');
		printChar('l');
		printChar('i');
		printChar('d');
		printChar(' ');
		printChar('a');
		printChar('x');
		printChar(' ');
		printChar('f');
		printChar('o');
		printChar('r');
		printChar(' ');
		printChar('I');
		printChar('n');
		printChar('t');
		printChar('e');
		printChar('r');
		printChar('r');
		printChar('u');
		printChar('p');
		printChar('t');
		printChar('2');
		printChar('1');
		printChar('\r');
		printChar('\n');
	}

}

void handleTimerInterrupt(int segment, int sp)
{
	// printChar('T');
	// printChar('i');
	// printChar('c');

	
	returnFromTimer(segment, sp);
}

