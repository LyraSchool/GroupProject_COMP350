#include "program.h"
#include "files.h"
#include "asm.h"

void putInMemory(int segment, int address, char character);
void launchProgram(int segment);

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

//Step 4 of Step 2
// void interrupt0x21(char* bx){

// 	executeProgram(bx);

// }

