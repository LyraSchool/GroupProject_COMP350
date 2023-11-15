void syscall(void*, void*, void*, void*);

void handleCommand(char* buffer);
int strcmp(char* s1, char* s2, int n);

void main()
{
    char* prompt = "A:> ";
    char linebuffer[200];

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

void handleCommand(char* buffer)
{
    if (!strcmp(buffer, "type", 4))
    {

    } else if (!strcmp(buffer, "exec", 4))
    {

    } else {
        syscall(0, "Unrecognized command: \"", 0, 0);
        syscall(0, buffer, 0, 0);
        syscall(0, "\"\r\n", 0, 0);
    }
}

void builtin_type(char* filename)
{
    char buffer[11312];
    int sectorsRead;

    syscall(3, filename, buffer, &sectorsRead);
    syscall(0, buffer, 0, 0);
}
