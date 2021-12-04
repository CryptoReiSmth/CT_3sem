#include <sys/types.h>
#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void my_handler()
{
    int status;
    pid_t pid = waitpid(-1, &status, 0);
        if (pid < 0)
        {
            perror ("Error in waitpid");
        }
        else if ((status & 0xff) == 0)
        {
            printf("Process %d exited with status %d\n", pid, status >>8);
        }
        else if ((status & 0xff00) == 0)
        {
            printf("Process %d killed by signal %d %s\n", pid, status &0x7f, (status & 0x80) ?
			       "with core file" : "without core file");
        }
}

int main(void)
{
    pid_t pid = fork();
    signal (SIGCHLD, my_handler);
    if (pid < 0)
    {
        printf("Error in fork\n");
        return 1;
    }
    if (pid == 0)
    {
        printf("I'm working");
        return 2;
    }
    printf("First we see this\n");
    //sleep(5);
    return 0;
}
