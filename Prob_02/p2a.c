#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 512

int main(int argc, char** argv)
{
    FILE *src, *dst;

    char buf[BUF_SIZE];

    if ((src = fopen(argv[1], "r")) == NULL)
    {	
    	perror(argv[1]);
        printf("error in fopen: %s\n", strerror(errno));
        exit(1);
    }
    
    if ((dst = fopen(argv[2], "w")) == NULL)
    {
    	perror(argv[2]);
        printf("error in fopen: %s\n", strerror(errno));
		exit(2);
    }

    while ((fgets(buf, BUF_SIZE, src)) != NULL)
    {
        fputs(buf, dst);
    }

    fclose(src);
    fclose(dst);
    exit(0);
}
