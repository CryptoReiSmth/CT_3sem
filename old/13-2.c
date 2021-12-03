#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>

int proc_info(const char *procces)
{
    printf ("%s: PID %d, Parent PID %d, SID %d, ", procces, getpid(), getppid(), getsid(0));
    struct passwd *pswd = getpwuid(getuid());
    if (pswd == NULL)
    {
        perror("Error in reading /etc/passwd");
        return 1;
    }
    printf("UID %d, User Name -- %s, ", getuid(), pswd->pw_name);
    struct group *grp = getgrgid(pswd->pw_gid);
    if (grp == NULL)
    {
        perror("Error in reading /etc/group");
        return 2;
    }
    printf("GID %d, Group Name -- %s\n", getgid(), grp->gr_name);
    return 0;
}

int main(void)
{
    pid_t proc_id = fork();
    if (proc_id == -1)
    {
        perror("Error in fork");
        return -1;
    }    
    if (proc_id == 0)
    {
        proc_info("Child ");
    }
    else
    {
        proc_info("Parent");
    }

    return 0;
}
