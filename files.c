#include "files.h"
#include "asm.h"
#include "numbers.h"


int getDirname(char* fileName, char* directory)
{
	int addr = 0;
	int indx = 0;
	int match = 0;
	while (addr < 16 && match == 0)
	{
		match = 1;
		for (indx = 0; indx < 6; indx++)
		{
			match = match && (fileName[indx] == directory[(addr * 32) + indx]);
		}
		addr++;
	}
	
	if (match == 0) return -1;
	else return addr;
}

void readFile(char* buffer, char* fileName, int* sectorsRead)
{
	// readFile implementation here
	int dirAddr;	
	char directory[512];
	char* sector;
	char secbuf[512];
	int i;

	readSector(directory, 2);
	dirAddr = getDirname(fileName, directory);
	
	

	if (dirAddr == -1)
	{
		*sectorsRead = 0;
		return;
	}

	sector = directory + (dirAddr * 16) + 6;
	while (*sector != 0)
	{
		
		readSector(secbuf, *sector);
		for (i = 0; i < 512; i++)
		{
			buffer[(*sectorsRead * 512) + i] = secbuf[i];
		}
		*sectorsRead = *sectorsRead + 1;
		sector++;
	}

	
}

