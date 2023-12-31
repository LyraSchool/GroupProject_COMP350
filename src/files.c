#include "files.h"
#include "asm.h"
#include "numbers.h"

#define ENTRY_LENGTH 32

void writeSector(char*, int);


int strncmp(char* s1, char* s2, int n)
{
    int i;
    int diff;
    for (i = 0; i < n; i++)
    {
        diff = s1[i] - s2[i];
        if (diff != 0) return diff;
    }
    return 0;
}

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


void printSector(char* sector)
{
	int i = 0;
	char pb[3];
	for (i = 0; i < 512; i++)
	{
		if (imod(i, 26) == 0)
		{
			printChar('\r');
			printChar('\n');			
		}

		itohex(pb, sector[i]);
		printString(pb);
		printChar(' ');
	}
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
	char pb[3];

	// printChar('K');
	// printChar(':');
	// printChar(' ');
	// printChar('R');
	// printChar('-');
	// printChar('D');
	// printChar('I');
	// printChar('R');
	// printChar('\r');
	// printChar('\n');
	
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

		// printChar('C');
		// printChar('"');
		// itohex(pb, fname[0]);
		// printString(pb);
		// itohex(pb, fname[1]);
		// printString(pb);
		// itohex(pb, fname[2]);
		// printString(pb);
		// itohex(pb, fname[3]);
		// printString(pb);
		// itohex(pb, fname[4]);
		// printString(pb);
		// itohex(pb, fname[5]);
		// printString(pb);
		// printChar('"');
		// printChar('A');
		// printChar('"');
		// itohex(pb, fileName[0]);
		// printString(pb);
		// itohex(pb, fileName[1]);
		// printString(pb);
		// itohex(pb, fileName[2]);
		// printString(pb);
		// itohex(pb, fileName[3]);
		// printString(pb);
		// itohex(pb, fileName[4]);
		// printString(pb);
		// itohex(pb, fileName[5]);
		// printString(pb);
		// printChar('"');
		// printChar('\r');
		// printChar('\n');
		
		
		
		for (i = 0; i < 6; i++)
		{
			if (directory[(direntry * 32) + i] != fileName[i])
			{
				match = 0;
				break;
			}

			if (fileName[i] == '\0') break;

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
		if (sectors[i] == 0x00) break;

		readSector(sectordata, sectors[i]);

		for (j = 0; j < 512; j++)
		{
			buffer[(i * 512) + j] = sectordata[j];
		}

		secRead++;
	}

	*sectorsRead = secRead;
}

//Write file: AX = 8
// BX = address of character array holding the file to be written
// CX = address of character array holding the file name 
// DX = number of sectors 
void writeFile(char* buffer, char* filename, int numberOfSectors)
{
	char dir[512];
	char map[512];
	char sectors[24];
	char sectordata[512];

	int i, j;

	int direntry = -1;
		
	readSector(map, 1);
	readSector(dir, 2);


	// find empty directory entry
	for (i = 3; i < 16; i++)
	{
		if (dir[i * 32] == '\0')
		{
			direntry = i;
			break;
		}
	}

	if (direntry == -1)	return;

	// Copy over filename
	for (i = 0; i < 6; i++)
	{
		dir[(direntry * 32) + i] = filename[i];
	}



	// find free sectors
	for (i = 0; i < numberOfSectors; i++)
	{
		for (j = 3; j < 512; j++)
		{
			if (map[j] == 0x00)
			{
				// mark sector as used and add to direntry
				sectors[i] = j;
				dir[(direntry * 32) + 6 + i] = j;
				map[j] = 0xFF;
				break;
			}
		}
	}

	for (i = 0; i < numberOfSectors; i++)
	{
		for (j = 0; j < 512; j++)
		{
			sectordata[j] = buffer[(i * 512) + j];
		}

		writeSector(sectordata, sectors[i]);
	}

	interrupt(0x21, 6, dir, 2);
	interrupt(0x21, 6, map, 1);
}

void deleteFile(char* filename) {
    char dir[512];
    char map[512];
    int sectorIndex;
    int entryIndex;

    readSector(dir, 2);
    readSector(map, 1);


    for (entryIndex = 0; entryIndex < 16; entryIndex++) {

        if (strncmp(filename, dir + (entryIndex * ENTRY_LENGTH), 6) == 0) {

            dir[entryIndex * ENTRY_LENGTH] = '\0';
 
            for (sectorIndex = 6; sectorIndex < 32; sectorIndex++) {
                int sectorNumber = dir[entryIndex * ENTRY_LENGTH + sectorIndex];
                if (sectorNumber == 0) {
                    break;
                }
                map[sectorNumber] = 0;
            }


            writeSector(dir, 2);
            writeSector(map, 1);

            break;
        }
    }
}
