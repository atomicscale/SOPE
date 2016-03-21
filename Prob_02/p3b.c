#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char** argv)
{
    int src,dst;

    char buf[BUFFER_SIZE];

    if ((argc != 2) && (argc != 3)) 
    {
        printf("%s: invalid number of arguments\n", argv[0]);
        exit(1);
    }

    if ((src = open(argv[1], O_RDONLY)) == -1)
    {	
    	perror(argv[1]);
        exit(1);
    }

    if (argc == 3)
    {
        if ((dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            perror(argv[2]);
            close(src);
            exit(1);
        }
        
        dup2(dst, STDOUT_FILENO);

    }

    int bytesRead = 0;
    int bytesWritten = 0;

    while ((bytesRead = read(src, buf, BUFFER_SIZE)) > 0) 
    {
        if ((bytesWritten = write(STDOUT_FILENO, buf, bytesRead)) <= 0 || bytesRead != bytesWritten) 
        {
            if(argc == 3)
            {
                perror(argv[2]);
                close(dst);
            }
            else
            {
                perror(argv[0]);
            }

            close(src);
            exit(1);
        }
    }

    close(src);

    if (argc == 3)
    {
        close(dst);
    }

    exit(0);
}