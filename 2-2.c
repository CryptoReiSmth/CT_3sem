#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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

    if (dprintf(fd, "%s", argv[2]) != (int)strlen(argv[2]))
    {
        perror("Error in dprintf");
        return 3;
    }

        if(close(fd) == -1)
    {
        perror("Error in closing file descriptor");
        return 4;
    }
}
