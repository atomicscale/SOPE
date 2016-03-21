#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_LENGTH 256

int main(int argc, char** argv)
{
    FILE *src, *dst;

    char buf[BUF_LENGTH];

    if ((src = fopen("infile.txt", "r")) == NULL)
    {	
    	perror("error in fopen");
        printf("error in fopen: %s\n", strerror(errno));
        exit(1);
    }
    
    if ((dst = fopen("outfile.txt", "w")) == NULL)
    {
    	perror("error in fopen");
        printf("error in fopen: %s\n", strerror(errno));
	exit(2);
    }

    while ((fgets(buf, BUF_LENGTH, src)) != NULL)
    {
        fputs(buf, dst);
    }

    fclose(src);
    fclose(dst);
    exit(0);
}

/*

p6a.c and p6d.c

*/
