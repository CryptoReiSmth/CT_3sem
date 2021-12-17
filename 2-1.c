#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


ssize_t write_all(int fd, const void *buffer, size_t lenght)
{
    size_t bytes_written = 0;
    const uint8_t *buf_addr = buffer;
    while (bytes_written < lenght) 
    {
		ssize_t res = write(fd, buf_addr + bytes_written, lenght - bytes_written);
		if (res < 0) 
        {
			return res;
		}
		bytes_written += res;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
		fprintf(stderr, "Usage: %s path text\n", argv[0]);
		return 1;
	}

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1)
    {
        perror("Erorr in opening file");
        return 2;
    }

    if(write_all(fd, argv[2], strlen(argv[2])) < 0)
    {
        perror("Error in writing");
        return 3;
    }
    
    if(close(fd) == -1)
    {
        perror("Error in closing file descriptor");
        return 4;
    }

    return 0;
}
