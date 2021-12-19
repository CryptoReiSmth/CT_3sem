#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{

    // credentials - process identifiers
    printf ("Process ID: %d, Parent Process ID: %d, Process Group ID: %d, Session ID: %d\n", getpid(), getppid(), getpgrp(), getsid(0));

    uid_t uid = getuid();
    struct passwd *pswd = getpwuid(uid);
    printf("User ID: %d, User Name: %s\n", uid, pswd ? pswd->pw_name : "?");

    gid_t gid = getgid();
    struct group *grp = getgrgid(gid);
    printf("Group ID: %d, Group Name: %s\n", gid, grp ? grp->gr_name : "?");

    int ngroups_max = getgroups(0, NULL);
    gid_t *groups = (gid_t *)calloc(ngroups_max, sizeof(gid_t));
    int ngroups = getgroups(ngroups_max, groups);
    if (ngroups == -1)
    {
        perror("Error in getting groups");
        return 3;
    }
    printf("Groups: \n");
    for (int i = 0; i < ngroups; i++)
    {
        struct group *supgrp = getgrgid(groups[i]);
        printf("%d (%s)\n", groups[i], supgrp ? supgrp->gr_name : "?");
    }
    free(groups);
    return 0;
}
