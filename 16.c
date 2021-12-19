#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s /queuename\n", argv[0]);
        return 1;
    }

    // create a new queue or open existing one:
    mqd_t queue = mq_open(argv[1], O_RDWR | O_CREAT | O_NONBLOCK, 0600, NULL);
    if(queue == (mqd_t) -1)
    {
        perror("Error in opening message queue");
        return 1;
    }

    // send some text message:
    if (mq_send(queue, "hello", strlen("hello"), 0) == -1)
    {
        perror("mq_send");
    }
    // get and show queue info:
    struct mq_attr queue_info = {};
    mq_getattr(queue, &queue_info);
    printf("Flags: %ld\n", queue_info.mq_flags);
    printf("Max. # of messages on queue: %ld\n", queue_info.mq_maxmsg);
    printf("Max. message size (bytes): %ld\n", queue_info.mq_msgsize);
    printf("# of messages currently in queue: %ld\n", queue_info.mq_curmsgs);

    // cleanup:
    mq_unlink(argv[1]);
    mq_close(queue);
    return 0;
}

