#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/times.h>
#include <time.h>

#define BUFFER_SIZE 512

int main(int argc, char** argv)
{
    int src, dst;

    char buf[BUFFER_SIZE];

    if ((src = open(argv[1], O_RDONLY)) == -1)
    {	
    	perror("error in fopen");
        exit(1);
    }
    
    if ((dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_SYNC)) == -1)
    {
    	perror("error in fopen");
    	exit(2);
    }

	clock_t programStart, programEnd;

	struct tms t;

	int clockTicks = sysconf(_SC_CLK_TCK);


    int bytesRead = 0;
    int bytesWritten = 0;

    programStart = times(&t);

    while ((bytesRead = read(src, buf, BUFFER_SIZE)) > 0) 
    {
        if ((bytesWritten = write(dst, buf, bytesRead)) <= 0 || bytesRead != bytesWritten) 
        {
            perror(argv[2]);
            close(src);
            close(dst);
            exit(1);
        }
    }

    close(src);
    close(dst);

    programEnd = times(&t);

    printf("Real time: %4.2f s\n", (double) (programEnd - programStart) / clockTicks);
	printf("User time: %4.2f s\n", (double) t.tms_utime / clockTicks);
	printf("System time: %4.2f s\n", (double) t.tms_stime / clockTicks);

    exit(0);
}