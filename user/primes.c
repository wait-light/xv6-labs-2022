#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int leftpipe[2];
    int begin = 2, end = 35, index = 0, buffer = 0, p = 0;
    pipe(leftpipe);
    for (index = begin; index <= end; index++)
    {
        write(leftpipe[1], &index, sizeof(index));
    }
    while (read(leftpipe[0], &p, sizeof(p)) > 0)
    {
        printf("prime %d\n", p);
        int rightpipe[2];
        pipe(rightpipe);
        if (fork() != 0)
        {
            close(leftpipe[1]);
            close(rightpipe[0]);
            while (read(leftpipe[0], &buffer, sizeof(buffer)) > 0)
            {
                if (buffer % p != 0)
                {
                    write(rightpipe[1], &buffer, sizeof(buffer));
                }
            }
            close(leftpipe[0]);
            close(rightpipe[1]);
            wait((int*)0);
        }
        else
        {
            close(leftpipe[1]);
            leftpipe[0] = rightpipe[0];
            leftpipe[1] = rightpipe[1];
        }
    }
    exit(0);
}
