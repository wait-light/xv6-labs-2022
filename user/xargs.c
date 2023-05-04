#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int readLine(int fd, char *buffer, int maxSize)
{
    int index = 0;
    while (index < maxSize)
    {
        if (read(fd, buffer + index, 1) == 0)
        {
            buffer[index] = '\0';
            return index;
        }
        if (buffer[index] == '\n')
        {
            buffer[index] = '\0';
            return index;
        }
        index++;
    }
    buffer[index] = '\0';
    return index;
}

int slipLine(char *line, char **words)
{
    int wordCount = 0;
    int index = 0, wordIndex = 0;
    while (line[index] != '\0')
    {
        while (line[index] != ' ' && line[index] != '\n' && line[index] != '\0')
        {
            words[wordCount][wordIndex++] = line[index++];
        }
        words[wordCount][wordIndex] = '\0';
        if (line[index] == '\0')
        {
            break;
        }
        wordCount++;
        index++;
        wordIndex = 0;
    }
    return wordCount + 1;
}
void xargs(int argc, char *argv[])
{
    char *execArgv[MAXARG];
    char execArgvBuffer[MAXARG][64];
    char buffer[512];
    char **appendExecArgvPointer;
    char *command = argv[1];
    memset(execArgvBuffer, 0, MAXARG * 64);
    for (int i = 0; i < MAXARG; i++)
    {
        execArgv[i] = execArgvBuffer[i];
    }
    for (int i = 1; i < argc; i++)
    {
        execArgv[i - 1] = argv[i];
    }
    appendExecArgvPointer = execArgv + argc - 1;
    int count = 0;
    while ((count = readLine(0, buffer, 512)) != 0)
    {
        int wordCount = slipLine(buffer, appendExecArgvPointer);
        if (fork() == 0)
        {
            // exec params need end with 0
            *(appendExecArgvPointer + wordCount) = 0;
            exec(command, execArgv);
            exit(1);
        }
        wait((int *)0);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "usage: xargs <params...>\n");
        exit(1);
    }
    xargs(argc, argv);
    exit(0);
}
