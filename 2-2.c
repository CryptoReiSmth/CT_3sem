#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Write text to file
    int res = 0;
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s path text\n", argv[0]);
        res =  1;
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Erorr in opening file");
        res =  2;
    }

    if (dprintf(fd, "%s", argv[2]) != (int)strlen(argv[2]))
    {
        perror("Error in dprintf");
        res =  3;
    }

    if(close(fd) == -1)
    {
        perror("Error in closing file descriptor");
        res = 4;
    }
    return res;
}
