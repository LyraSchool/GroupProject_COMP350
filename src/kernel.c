
//Made By:
//Lyra Brown
//Alicia Kenneally
//Lauren Rezendes

#include "files.h"
#include "numbers.h"

#define MAX_PID 8

void handleInterrupt21(int, int, int, int);
void handleTimerInterrupt(int, int);

void returnFromTimer(int segment, int sp);

void makeTimerInterrupt();
void makeInterrupt21();

void putInMemory(int segment, int address, char character);
int interrupt(int number, int AX, int BX, int CX, int DX);

void initializeProgram(int segment);
void launchProgram(int segment);

int setKernelDataSegment();
void restoreDataSegment(int segment);

void readString(char*);
void printChar(char);
void readSector(char*, int);
void printString(char*);
void terminate();
void writeSector(char*, int);
void killProcess(int);

int processWaitingFor[MAX_PID];
int processActive[MAX_PID];
int processStackPointer[MAX_PID];
int currentProcess;


#define pidToSeg(i) (i + 2) * 0x1000

/* Depends on printString to exist. */
void main()
{
	int i;
	
	// Creates int0x21
	makeInterrupt21();

	for (i = 0; i < MAX_PID; i++)
	{
		processActive[i] = 0;
		processStackPointer[i] = 0xFF00;
	}
	currentProcess = -1;


	
	


	// Clears the screen
	interrupt(0x10, 0x03, 0, 0, 0);
	// Launch to shell
	interrupt(0x21, 4, "shell", 0, 0);
	
	// Create interrupt for the process scheduler
	makeTimerInterrupt();

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

void executeProgram(char* filename, int* rc)
{
	char buffer[13312];
	
	int sectorsRead;
	int segment = 0x2000;
	int i;
	int oldSeg;
	int new_pid;

	readFile(buffer, filename, &sectorsRead);


	if (sectorsRead == 0)
	{
		oldSeg = setKernelDataSegment();
		printString("Unable to find program \"");
		restoreDataSegment(oldSeg);

		printString(filename);
		
		oldSeg = setKernelDataSegment();
		printString("\"\r\n");
		restoreDataSegment(oldSeg);
		
		return;
	}

	oldSeg = setKernelDataSegment();
	for (new_pid = 0; new_pid < MAX_PID; new_pid++)
	{
		if (processActive[new_pid] == 0) break;
	}
	restoreDataSegment(oldSeg);

	segment = pidToSeg(new_pid);

	for ( i=0; i<13312; i++){
		putInMemory(segment, i, buffer[i]);
	}

	initializeProgram(segment);

	oldSeg = setKernelDataSegment();
	processActive[new_pid] = 1;
	processStackPointer[new_pid] = 0xFF00;
	restoreDataSegment(oldSeg);

	// launchProgram(segment);
}

void killProcess(int pid)
{
	char pb[2];
	int dataSeg = setKernelDataSegment();
	printString("Killing Process ");
	itoa(pid, pb);
	printString(pb);
	printString("\r\n");
	processActive[pid] = 0;
	restoreDataSegment(dataSeg);
}

void terminate()
{
	char pb[2];
	int dataSeg = setKernelDataSegment();
	printString("Killing Process ");
	itoa(currentProcess, pb);
	printString(pb);
	printString("\r\n");
	processActive[currentProcess] = 0;
	while (1);
}

void waitForPid(int pid)
{
	int dataseg = setKernelDataSegment();
	processWaitingFor[currentProcess] = pid;
	processActive[currentProcess] = 2;
	restoreDataSegment(dataseg);
}


void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	int oldSeg;

	if ( ax == 0 ) {
		printString((char*) bx);
	} else if ( ax == 1 ) {
		readString((char*) bx);
	} else if ( ax == 2 ) {
		readSector((char*) bx, cx);
	} else if ( ax == 3 ) {
		readFile((char*)cx, (char*)bx, (int*)dx);
	} else if ( ax == 4) {
		executeProgram((char*) bx, (int*) cx);
	} else if ( ax == 5) {
		terminate();
	} else if ( ax == 6) {
		writeSector((char*)bx, cx);
	} else if ( ax == 7) {
		deleteFile((char*)bx);
	} else if ( ax == 8) {
		writeFile((char*)bx, (char*)cx, dx);
	} else if ( ax == 9){
		killProcess(bx);
	} else if ( ax == 10) {
		waitForPid(bx);
	} else {
		oldSeg = setKernelDataSegment();
		printString("Invalid ax for Interrupt 21\r\n");
		restoreDataSegment(oldSeg);
	} 

}

void handleTimerInterrupt(int segment, int sp)
{
	int new_segment, new_sp;
	int i;
	char pb[10];

	int dataseg = setKernelDataSegment();

	for(i=0; i<8; i++)
        {
                putInMemory(0xb800,60*2+i*4,i+0x30);
                if(processActive[i]==1)
                        putInMemory(0xb800,60*2+i*4+1,0x20);
                else
                        putInMemory(0xb800,60*2+i*4+1,0);
        }

	if (currentProcess != -1)
	{
		processStackPointer[currentProcess] = sp;
	}

	do
	{
		currentProcess++;
		if (currentProcess >= MAX_PID) currentProcess = 0;
	} while (processActive[currentProcess] != 1);


	segment = pidToSeg(currentProcess);
	sp = processStackPointer[currentProcess];

	restoreDataSegment(dataseg);

	returnFromTimer(segment, sp);
}

/*
	Notes for Lyra:
	
	Step 7:
		Need to edit terminate() to find any processes waiting on this pid and set them to active

*/

