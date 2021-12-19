#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <poll.h>
#include <sys/wait.h>

int proc_info(const char *procces)
{
    printf ("%s: PID %d, Parent PID %d, SID %d\n", procces, getpid(), getppid(), getsid(0));
    return 0;
}

int main(void)
{
    pid_t parent_pid = getpid(),child_id = fork();
    if (child_id == -1)
    {
        perror("Error in fork");
        return -1;
    }

    if (child_id == 0)
    {
        struct pollfd poll_fd;
        poll_fd.fd = parent_pid; 
        poll_fd.events = POLLIN;
        printf("Waiting for parent \n");

        int state = poll(&poll_fd, 1, -1);
        if (state < 0)
        {
            perror("Error in poll");
            return 3;
        }
        close(parent_pid);
    } 
    else 
    {
        proc_info("Parent");
    }


    return 0;
}
