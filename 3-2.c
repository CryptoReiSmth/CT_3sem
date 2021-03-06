#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h> 

ssize_t pwrite_all(int fd, const void *buffer, size_t length, off_t offset)
{
    size_t bytes_written = 0;
    const uint8_t *buf_addr = buffer;
    while (bytes_written < length) 
    {
        ssize_t res = pwrite(fd, buf_addr + bytes_written, length - bytes_written, offset);
        if (res < 0) 
        {
            return res;
        }
        bytes_written += res;
        offset +=(off_t)res;
    }
    return (ssize_t)bytes_written;
}

int close_all(int sour_fd, int dest_fd)
{
    int res = 0;
    if (close(sour_fd) == -1)
    {
        perror("Error in closing sourse file");
        res = 8;
    }
    if (close(dest_fd) == -1)
    {
        perror("Error in closing destination file");
        res = 9;
    }
    return res;
}

int main(int argc, char *argv[])
{
    // Copy data from source file to destination file    

    int res = 0;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s source-file path destination-file path\n", argv[0]);
        res = 1;
    }
    if (strcmp(argv[1], argv[2]) == 0) 
    {
        perror("Source and destination files should be different");
        res = 2;
    }
    int sour_fd = open(argv[1], O_RDONLY);
    if (sour_fd == -1)
    {
        perror("Erorr in opening source file");
        res = 3;
    }
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (dest_fd == -1)
    {
        perror("Erorr in opening destination file");
        res = 4;
    }

    char *data = (char *)malloc(INT_MAX);
    if (data == NULL)
    {
        perror("Error in allocating memory");
        res = 5;
    }

    int check = -1;
    off_t offset = 0;
    while((check = pread(sour_fd, data, INT_MAX, offset)) != 0)
    {
        if (check == -1)
        {
            perror("Error in reading source file");
            return 6;
        }
        if (pwrite_all(dest_fd, data, strlen(data), offset) < 0)
        {
            perror("Error in writing"); 
            return 7;
        }
        offset += (off_t)check;
    }
    free(data);
    close_all(sour_fd, dest_fd);
    return res;
}
