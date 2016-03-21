#include <stdio.h>
#include <stdlib.h>

#define BUF_LENGTH 256

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("error in main() function: invalid number of arguments.\n");
        exit(1);
    }

    if (argv[1] == NULL || argv[2] == NULL)
    {
        printf("error in main() function: filenames must not be empty.\n");
        exit(1);
    }

    FILE *src;
    FILE *dst;

    char buf[BUF_LENGTH];

    if ((src = fopen(argv[1], "r")) == NULL)
    {
        perror("error in fopen() function");
        exit(1);
    }

    if ((dst = fopen(argv[2], "w")) == NULL)
    {
        perror("error in fopen() function");
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