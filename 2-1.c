#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


ssize_t write_all(int fd, const void *buffer, size_t length)
{
    size_t bytes_written = 0;
    const uint8_t *buf_addr = buffer;
    while (bytes_written < length) 
    {
        ssize_t res = write(fd, buf_addr + bytes_written, length - bytes_written);
        if (res < 0) 
        {
            return res;
        }
        bytes_written += res;
    }
    return (ssize_t)bytes_written; 
}

int main(int argc, char *argv[])
{
    // Write text to file
    int res = 0;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s path text\n", argv[0]);
        res = 1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Erorr in opening file");
        res =  2;
    }

    if(write_all(fd, argv[2], strlen(argv[2])) < 0)
    {
        perror("Error in writing");
        res = 3;
    }

    if(close(fd) == -1)
    {
        perror("Error in closing file descriptor");
        res = 4;
    }

    return res;
}
