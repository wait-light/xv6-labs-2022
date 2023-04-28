#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pipes[2];
    pipe(pipes);
    int pid = fork();
    char c;
    if (pid == 0)
    {
        read(pipes[1], &c, 1);
        close(pipes[1]);
        printf("%d: received ping\n",getpid());
        write(pipes[0], &c, 1);
        close(pipes[0]);
    }else{
        write(pipes[1], &c, 1);
        close(pipes[1]);
        wait(0);
        read(pipes[0], &c, 1);
        printf("%d: received pong\n",getpid());
        close(pipes[0]);
    }
    
    exit(0);
}
