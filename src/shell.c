
#define BUFFERLEN 200

void syscall(void*, void*, void*, void*);

void handleCommand(char* buffer);
int strcmp(char* s1, char* s2, int n);
void reverse(char* s);
int imod(int value, int modulus);

void main()
{
    char* prompt = "\r\nA:> ";
    char linebuffer[BUFFERLEN];

    while (1)
    {
        syscall(0, prompt, 0, 0);
        syscall(1, linebuffer, 0, 0);
        handleCommand(linebuffer);
    }

    syscall(5, 0, 0, 0);
    while (1);

}

int strcmp(char* s1, char* s2, int n)
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

void itoa(int val, char* buffer)
{
	int i, sign;
	
	if ((sign = val) < 0) val = -val;

	i = 0;
	do {
		buffer[i++] = imod(val, 10) + '0';
	} while ((val /= 10) > 0);

	if (sign < 0) buffer[i++] = '-';
	buffer[i] = '\0';
	reverse(buffer);
}

int imod(int val, int modulus)
{
	return val - (val / modulus);
}

void reverse(char* buffer)
{
	int i, j;
	char c;
	for (i = 0, j = strlen(buffer) - 1; i < j; i++, j--)
	{
		c = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = c;
	}
}

int strlen(char* s)
{
	char* s2 = s;
	int len = 0;
	while (*s2 != '\0') {
		len++;
		s2++;
	}
	return len;
}

void getFileName(char* dir, char* fname, int entry)
{
	int i;
	for (i = 0; i < 6; i++)
	{
		fname[i] = dir[(entry * 32) + i];
		if (fname[i] == '\0') fname[i] = ' ';
	} 
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

void builtin_exec(char* buffer)
{
    char filename[7];


    filename[0] = buffer[5];
    filename[1] = buffer[6];
    filename[2] = buffer[7];
    filename[3] = buffer[8];
    filename[4] = buffer[9];
    filename[5] = buffer[10];
    filename[6] = '\0';

    syscall(4, filename, 0, 0);
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

	syscall(2, dir, 2);

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

		syscall(0, fname);
		syscall(0, " ");
		itoa(numSectors * 512, printbuf);
		syscall(0, printbuf);
		syscall(0, "bytes\r\n");
	}

	itoa(filecount, printbuf);
	syscall(0, printbuf);
	syscall(0, " files found\r\n");
}

void sanitizeCommand(char* buffer)
{
    int i;
    for (i = 0; i < BUFFERLEN; i++)
    {
        if (buffer[i] == '\0' && i > 0)
        {
            buffer[i - 1] = '\0';
        }
    }
}

void handleCommand(char* buffer)
{
    sanitizeCommand(buffer);

    if (!strcmp(buffer, "type", 4))
    {
        builtin_type(buffer);
    }
    else if (!strcmp(buffer, "exec", 4))
    {
        builtin_exec(buffer);
    }
	else if (!strcmp(buffer, "dir", 3))
	{
		builtin_dir(buffer);
	}
    else
    {
        syscall(0, "Unrecognized command: \"", 0, 0);
        syscall(0, buffer, 0, 0);
        syscall(0, "\"\r\n", 0, 0);
    }
}
