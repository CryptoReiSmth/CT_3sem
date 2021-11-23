#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

int main(void)
{

    // credentials - process identifiers
    printf ("Process ID: %d, Parent Pricess ID: %d, Session ID: %d\n", getpid(), getppid(), getsid(0));

    struct passwd *pswd = getpwuid(getuid());
    if (pswd == NULL)
    {
        perror("Error in reading /etc/passwd");
        return 1;
    }
    printf("User ID: %d, User Name: %s\n", getuid(), pswd->pw_name);

    struct group *grp = getgrgid(pswd->pw_gid);
    if (grp == NULL)
    {
        perror("Error in reading /etc/group");
        return 2;
    }
    printf("Group ID: %d, Group Name: %s\n", getgid(), grp->gr_name);


    long ngroups_max = sysconf(_SC_NGROUPS_MAX);
    gid_t *groups = (gid_t *)malloc(ngroups_max *sizeof(gid_t));
    int ngroups = getgroups(ngroups_max, groups);
    if (ngroups == -1)
    {
        perror("Error in getting groups");
        return 3;
    }
    printf("Groups: ");
    struct group *supgrp = getgrgid(pswd->pw_gid);
    int i = 0;
    for (i = 0; i < ngroups - 1; i++)
    {
        printf("%d ", groups[i]);
        supgrp = getgrgid(groups[i]);
        printf("(%s); ", supgrp->gr_name);
    }
    printf("\n");

    return 0;
}
