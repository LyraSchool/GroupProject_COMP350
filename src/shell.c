
#define BUFFERLEN 200

void syscall(void*, void*, void*, void*);

void handleCommand(char* buffer);
int strcmp(char* s1, char* s2, int n);

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
    else
    {
        syscall(0, "Unrecognized command: \"", 0, 0);
        syscall(0, buffer, 0, 0);
        syscall(0, "\"\r\n", 0, 0);
    }
}
