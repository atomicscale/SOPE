#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	pid_t parentPID = getpid();

	if (fork() != 0)
	{
		printf("Hello\n");
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
	}
	else
	{
		while (getppid() == parentPID)
		{
		}
		
		printf("world!\n");
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
	}

	return 0;
}