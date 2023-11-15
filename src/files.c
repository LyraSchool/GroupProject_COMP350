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


// void q_readFile(char* buffer, char* fileName, int* sectorsRead)
// {
// 	// readFile implementation here
// 	int dirAddr;	
// 	char directory[512];
// 	char* sector;
// 	char secbuf[512];
// 	int i;
// 	int secRead = 0;

// 	// Debug vars
// 	char printbuf[200];

// 	readSector(directory, 2);
// 	dirAddr = getDirname(fileName, directory);
// 	*sectorsRead = 0;
	
// 	if (dirAddr == -1) return;


// 	sector = directory + (dirAddr * 16) + 6;
// 	while (*sector != 0)
// 	{
// 		interrupt(0x21, 0, "Reading sector ", 0, 0);
// 		itoa(*sector, printbuf);
// 		interrupt(0x21, 0, printbuf, 0, 0);
// 		interrupt(0x21, 0, "\r\n", 0, 0);
		
		
// 		readSector(secbuf, *sector);
// 		interrupt(0x21, 0, "Reading sector\r\n", 0, 0);
// 		for (i = 0; i < 512; i++)
// 		{
// 			// interrupt(0x21, 0, "Writing to address ", 0, 0);
// 			// itoa((secRead * 512) + i, printbuf);
// 			// interrupt(0x21, 0, printbuf, 0, 0);
// 			// interrupt(0x21, 0, "\r\n", 0, 0);
// 			buffer[(secRead * 512) + i] = secbuf[i];
// 		}
// 		interrupt(0x21, 0, "read sector\r\n", 0, 0);
		
// 		secRead = secRead + 1;
// 		sector++;
// 		interrupt(0x21, 0, "moving to sector ", 0, 0);
// 		itoa(*sector, printbuf);
// 		interrupt(0x21, 0, printbuf, 0, 0);
// 		interrupt(0x21, 0, "\r\n", 0, 0);

// 	}

// 	*sectorsRead = secRead;

// 	interrupt(0x21, 0, "Done reading", 0, 0);
		

	
// }

void readFile(char* buffer, char* fileName, int* sectorsRead)
{
	char directory[512];
	char sectordata[512];
	char sectors[26];


	int secRead = 0;

	int direntry, i, j;
	int match = 1;

	// Debug vars
	char printbuf[100];
	char fname[7];

	readSector(directory, 2);

	for (direntry = 0; direntry < 16; direntry++)
	{
		match = 1;

		fname[0] = directory[(direntry * 32) + 0];
		fname[1] = directory[(direntry * 32) + 1];
		fname[2] = directory[(direntry * 32) + 2];
		fname[3] = directory[(direntry * 32) + 3];
		fname[4] = directory[(direntry * 32) + 4];
		fname[5] = directory[(direntry * 32) + 5];
		fname[6] = '\0';
		
		for (i = 0; i < 6; i++)
		{
			if (directory[(direntry * 32) + i] != fileName[i]) match = 0;
		}

		if (match)
		{
			
			break;
		}
	}

	if (!match)
	{
		*sectorsRead = 0;
		return;
	}

	for (i = 0; i < 26; i++)
	{
		sectors[i] = directory[(direntry * 32) + 6 + i];

	}


	for (i = 0; i < 26; i++)
	{
		readSector(sectordata, sectors[i]);

		for (j = 0; j < 512; j++)
		{
			buffer[(i * 512) + j] = sectordata[j];
		}

		secRead++;
	}

	*sectorsRead = secRead;
}