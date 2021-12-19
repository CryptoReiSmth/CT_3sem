#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


int main(int argc, char *argv[])
{
    int res = 0;
    if (argc != 2)
    {
        printf("Usage: %s /queuename\n", argv[0]);
        return 1;
    }
    const char * queue_name = argv[1];

    // create a new queue or open existing one:
    mqd_t queue = mq_open(queue_name, O_RDWR | O_CREAT | O_NONBLOCK, 0600, NULL);
    if(queue == (mqd_t) -1)
    {
        perror("Error in opening message queue");
        res = 2;
    }

    // send some text message:
    if (mq_send(queue, "hello", strlen("hello"), 0) == -1)
    {
        perror("Error in senging message");
        res = 3;
    }

    // get and show queue info:
    struct mq_attr queue_info = {};
    if (mq_getattr(queue, &queue_info) == -1)
    {
        perror("Error in exacting attributes");
        res = 4;
    }

    printf("Flags: %ld\n", queue_info.mq_flags);
    printf("Max. # of messages on queue: %ld\n", queue_info.mq_maxmsg);
    printf("Max. message size (bytes): %ld\n", queue_info.mq_msgsize);
    printf("# of messages currently in queue: %ld\n", queue_info.mq_curmsgs);

    // cleanup:
    if(mq_unlink(queue_name) == -1)
    {
        perror("Error in closing message queue");
        res = 5;
    }
    if(mq_close(queue) == -1)
    {
        perror("Error in unlinking message queue");
        res = 6;
    }
    return res;
}

