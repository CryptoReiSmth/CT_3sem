#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

typedef struct {
    atomic_ullong count;
    int term1;
    int term2;
} work_area_t;

void *thr_body(void *arg) 
{
    work_area_t *w = arg;
    for (int i = 0; i < w->term1; i++) 
    {
        atomic_fetch_add(&w->count, 1);
    }
    return NULL;
}

int main(int argc, char * argv[]) 
{
    // Summarizes 2 numbers
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s term-1 term-2\n", argv[0]);
        return 1;
    }
    work_area_t data;
    atomic_store(&data.count, 0);
    data.term1 = atoi(argv[1]);
    data.term2 = atoi(argv[2]);

    pthread_t secondary_tid;
    if(errno = pthread_create(&secondary_tid, NULL, thr_body, &data))
    {
        perror("Error in creating thread");
        return 1;
    }
    for (int i = 0; i < data.term2; i++)
    {
        atomic_fetch_add(&data.count, 1);
    }
    pthread_join(secondary_tid, NULL);
    printf("counter = %llu\n", atomic_load(&data.count));
    return 0;
}

