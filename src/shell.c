#include "numbers.h"
#include "usrlib.h"
#include "iotools.h"

#define BUFFERLEN 200

void handleCommand(char* buffer);
int strncmp(char* s1, char* s2, int n);

void main()
{
    char* prompt = "\r\nA:> ";
    char linebuffer[BUFFERLEN];
    void enableInterrupts();

    while (1)
    {
        syscall(0, prompt, 0, 0);
        syscall(1, linebuffer, 0, 0);
        handleCommand(linebuffer);
    }

    syscall(5, 0, 0, 0);
    while (1);
}

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

void memcopy(char* target, char* source, int amt, int targetOffset, int sourceOffset)
{
	int i;

	for (i = 0; i < amt; i++)
	{
		target[targetOffset + i] = source[sourceOffset + i];
	}
}

void memset(char* target, char val, int amt, int offset)
{
	int i;

	for (i = 0; i < amt; i++)
	{
		target[offset + i] = val;
	}
}


void getFileName(char* dir, char* fname, int entry)
{
	int i;
	for (i = 0; i < 6; i++)
	{
		fname[i] = dir[(entry * 32) + i];
		if (fname[i] == '\0') fname[i] = ' ';
	}
	fname[6] = '\0';
}


void builtin_type(char* buffer)
{
    char filedata[13312];
    char filename[7];
    int sectorsRead = 0;

    filename[0] = buffer[5];
    filename[1] = buffer[6];
    filename[2] = buffer[7];
    filename[3] = buffer[8];
    filename[4] = buffer[9];
    filename[5] = buffer[10];
    filename[6] = '\0';


	// puts("SH: Attempting to read file \"");
	// puts(filename);
	// puts("\"\r\n");
    syscall(3, filename, filedata, &sectorsRead);

    if (sectorsRead == 0)
    {
        syscall(0, "Unable to find file \"", 0, 0);
        syscall(0, filename, 0, 0);
        syscall(0, "\"\r\n", 0, 0);
        return;
    }

    syscall(0, filedata, 0, 0);
    
    //syscall(5, 0, 0, 0);
}

void builtin_execbg(char* buffer)
{
    char filename[7];
	int pid;

    filename[0] = buffer[7];
    filename[1] = buffer[8];
    filename[2] = buffer[9];
    filename[3] = buffer[10];
    filename[4] = buffer[11];
    filename[5] = buffer[12];
    filename[6] = '\0';

    syscall(4, filename, &pid, 0);
}

void builtin_exec(char* buffer)
{
    char filename[7];
	int pid;

    filename[0] = buffer[5];
    filename[1] = buffer[6];
    filename[2] = buffer[7];
    filename[3] = buffer[8];
    filename[4] = buffer[9];
    filename[5] = buffer[10];
    filename[6] = '\0';

    syscall(4, filename, &pid, 0);
	syscall(10, pid, 0, 0);
}

void builtin_dir(char* buffer)
{
	char dir[512];
	char direntry[32];
	int filecount;
	int numSectors;
	char printbuf[10];
	char fname[7];
	int i, j;

	filecount = 0;

	syscall(2, dir, 2, 0);

	for (i = 0; i < 16; i++)
	{
		if (dir[i * 32] == '\0') continue;
		filecount++;
		getFileName(dir, fname, i);
		numSectors = 0;
		for (j = 6; j < 32; j++)
		{
			if (dir[(i * 32) + j] != 0x0) numSectors++;
		}

		syscall(0, fname, 0, 0);
		syscall(0, " ", 0, 0);
		itoa(numSectors * 512, printbuf);
		syscall(0, printbuf, 0, 0);
		syscall(0, "bytes\r\n", 0, 0);
	}

	itoa(filecount, printbuf);
	syscall(0, printbuf, 0, 0);
	syscall(0, " files found\r\n", 0, 0);
}

void builtin_create(char* buffer)
{
	char fname[7];
	char fdata[13312];
	int foffset = 0;
	char line[200];
	int numSectors;
	int i;
	char pb[20];
	char c[2];

	for (i = 0; i < 6; i++)
	{
		fname[i] = buffer[7 + i];
	}
		
	while (1)
	{
		// clear line buffer
		for (i = 0; i < 200; i++) line[i] = 0;

		// readLine into line
		syscall(0x1, line, 0, 0);
		
		// if line empty, break
		if (line[0] == 0xd) break;
		
		// copy all line content into fdata
		for (i = 0; line[i] != '\0'; i++)
		{
			
			fdata[foffset + i] = line[i];
		}


		foffset += i;
	}

	numSectors = (foffset / 512) + 1;

	syscall(0x8, fdata, fname, numSectors);

}

void builtin_del(char* buffer)
{
	char fname[7];
	int i;

	for (i = 0; i < 6; i++)
	{
		fname[i] = buffer[4 + i];
	}

	syscall(0x7, fname, 0, 0);
}

void builtin_copy(char* buffer)
{
	char source[7];
	char target[7];
	char printBuffer[20];
	int i, j;
	int numSectors;

	char filedata[13312];

	//char* thing = "copy fnam me2";
	for (i = 0; i < 6 && buffer[5 + i] != ' '; i++)
	{
		//if (isprintable(buffer[5+i]))
		source[i] = buffer[5 + i];
	}
	source[i] = '\0';
	i++;

	for (j = 0; j < 6; j++)
	{
		target[j] = buffer[5 + i + j];
	}
	target[j] = '\0';


	// puts("Source: \"");
	// puts(source);
	// puts("\" | Target: \"");
	// puts(target);
	// puts("\"\r\n");


	// puts("Reading file data in.");
	syscall(0x3, source, filedata, &numSectors);
	// puts("Read ");
	// itoa(numSectors, printBuffer);
	// puts(printBuffer);
	// puts(" sectors in, and writing out.");
	syscall(0x8, filedata, target, numSectors);

}

void builtin_kill(char* buffer)
{
	int pid = buffer[5] - '0';
	syscall(9, pid, 0, 0);
}
void sanitizeCommand(char* buffer)
{
    int i;
    for (i = 0; i < BUFFERLEN; i++)
    {
        if (buffer[i] == '\0' && i > 1)
        {
            buffer[i - 2] = '\0';
        }
    }
}

void handleCommand(char* buffer)
{
    sanitizeCommand(buffer);

    if (!strncmp(buffer, "type", 4))
    {
        builtin_type(buffer);
    }
    else if (!strncmp(buffer, "execbg", 6))
    {
        builtin_execbg(buffer);
    }
    else if (!strncmp(buffer, "exec", 4))
    {
        builtin_exec(buffer);
    }
	else if (!strncmp(buffer, "dir", 3))
	{
		builtin_dir(buffer);
	}
	else if (!strncmp(buffer, "create", 5))
	{
		builtin_create(buffer);
	}
	else if (!strncmp(buffer, "del", 3))
	{
		builtin_del(buffer);
	}
	else if (!strncmp(buffer, "copy", 4))
	{
		builtin_copy(buffer);
	}
	else if (!strncmp(buffer, "kill", 4))
	{
		builtin_kill(buffer);
	}
    else
    {
        syscall(0, "Unrecognized command: \"", 0, 0);
        syscall(0, buffer, 0, 0);
        syscall(0, "\"\r\n", 0, 0);
    }
}
