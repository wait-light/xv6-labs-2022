#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/string.h"
#include "kernel/param.h"

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
