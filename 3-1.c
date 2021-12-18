#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>


#define BUF_SIZE 1048576 // aka 2^20


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
    return (ssize_t)bytes_written;
}

int copy_all(int sour_fd, int dest_fd)
{
    char *data = (char *)malloc(BUF_SIZE);
    if (data == NULL)
    {
        perror("Error in allocating memory");
        return 7;          
    }
    
    int check = -1;
    while((check = read(sour_fd, data, BUF_SIZE)) != 0)
    {
        if (check == -1)
        {
            perror("Error in reading source file");
            return 8;
        }
        
        if (write_all(dest_fd, data, strlen(data)) < 0)
        {
             perror("Error in writing");
            return 9;
        }
    }
    free(data);
    return 0;
}


int main(int argc, char *argv[])
{
    // Copy data from source file to destination file
    
    if (argc != 3)
    {
		fprintf(stderr, "Usage: %s source-file path destination-file path\n", argv[0]);
		return 1;
	}    
    if (strcmp(argv[1], argv[2]) == 0) 
    {
        perror("Source and destination files should be different");
        return 2;
    }
    int sour_fd = open(argv[1], O_RDONLY);
    if (sour_fd == -1)
    {
        perror("Erorr in opening source file");
        return 3;
    }
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (dest_fd == -1)
    {
        perror("Erorr in opening destination file");
        return 4;
    }

    int result = copy_all(sour_fd, dest_fd);


    if (close(sour_fd) == -1)
    {
        perror("Error in closing sourse file");
        return 5;
    }
    if (close(dest_fd) == -1)
    {
        perror("Error in closing destination file");
        return 6;
    }
    return result;
}
