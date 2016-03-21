#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int processSt = 0;

	if (fork() != 0)
	{
		wait(&processSt);
		printf("world!\n");
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		
	}
	else
	{
		printf("Hello\n");
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
	}

	return 0;
}