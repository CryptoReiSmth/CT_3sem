#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BUF_SIZE 1048576 // aka 2^20

int close_all(int sour_fd, int dest_fd)
{
    int res = 0;
    if (close(sour_fd) == -1)
    {
        perror("Error in closing sourse file");
        res = 7;
    }
    if (close(dest_fd) == -1)
    {
        perror("Error in closing destination file");
        res = 8;
    }
    return res;
}

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

int copy_data(int sour_fd, int dest_fd)
{
    int res = 0;
    char *data = (char *)malloc(BUF_SIZE);
    if (data == NULL)
    {
        perror("Error in allocating memory");
        res = 9;
    }

    int check = -1;
    while((check = read(sour_fd, data, BUF_SIZE)) != 0)
    {
        if (check == -1)
        {
            perror("Error in reading source file");
            res = 10;
        }

        if (write_all(dest_fd, data, strlen(data)) < 0)
        {
            perror("Error in writing");
            res = 11;
        }
    }
    free(data);
    close_all(sour_fd, dest_fd);
    return res;
}

int copy_metadata(int sour_fd, int dest_fd)
{
    int res = 0;
    struct stat sour_new;
    if (fstat(sour_fd, &sour_new) == 0)
    {
        if (fchmod(dest_fd, sour_new.st_mode) == -1)
        {
            perror("Error in copying  mode");
            res = 12;
        }
        const struct timespec times_copy[2] = {sour_new.st_atim, sour_new.st_mtim};
        if (futimens(dest_fd, times_copy) == -1)
        {
            perror("Error in copying time");
            res = 13;
        }
    }
    close_all(sour_fd, dest_fd);
    return res;
}

int main(int argc, char *argv[])
{
    // Copy data + metadata from source file to destination file
    
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

    if(copy_data(sour_fd, dest_fd) != 0)
    {
        perror("Error in copying data");
        res = 5;
    }
    if(copy_metadata(sour_fd, dest_fd) != 0)
    {
        perror("Error in copying metadata");
        res = 6;
    }
    close_all(sour_fd, dest_fd);
    return res;
}
