#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int last_sig;

void sig_handler(int val)
{
    last_sig = val;
}

int main(void)
{
    printf("Hint: kill -s SIGKILL %d\n", getpid());
    int res = 0;
    struct sigaction sigact_struct = 
    {
        .sa_handler = sig_handler,
        .sa_flags = SA_RESTART
    };
    if (sigaction(SIGINT, &sigact_struct, NULL) == -1)
    {
        perror("Error in SIGINT");
        res = 1;
    }
    if (sigaction(SIGQUIT, &sigact_struct, NULL) == -1)
    {
        perror("Error in SIGQUIT");
        res = 2;
    }
    if (sigaction(SIGTERM, &sigact_struct, NULL) == -1)
    {
        perror("Error in SIGTERM");
        res = 3;
    }
    if (sigaction(SIGSTOP, &sigact_struct, NULL) == -1)
    {
        perror("Error in SIGSTOP");
        res = 4;
    }

    while(1)
    {
        printf("Still running, try smth better\n");
        pause();
        switch (last_sig)
        {
            case SIGINT:
                printf("\n^C doesnt't work\n"); break;
            case SIGQUIT:
                printf("\n^\\ doesn't work\n"); break;
            case SIGTERM:
                printf("\nSIGTERM doesn't work\n"); break;
            case SIGSTOP:
                printf("\nSIGSTOP doesn't work\n"); break;
            default:
                printf("unknown signal"); break;
        }
    }

    return res;
}
