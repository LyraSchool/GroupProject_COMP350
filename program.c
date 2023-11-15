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

	//Step 1 of Step 2
	interrupt(0x21, 0, "EXP - Reading program\r\n", 0, 0);
	readFile(buffer, filename, &sectorsRead);

	//Step 2 of Step 2
	interrupt(0x21, 0, "Starting the for loop\r\n", 0, 0);

	for ( i=0; i<13312; i++){
		putInMemory(segment, i, buffer[i]);
	}
	interrupt(0x21, 0, "launching\r\n", 0, 0);
	//Step 3 of Step 2
	launchProgram(segment);
}

//Step 4 of Step 2
// void interrupt0x21(char* bx){

// 	executeProgram(bx);

// }

