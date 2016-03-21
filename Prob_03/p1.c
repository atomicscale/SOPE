// PROGRAMA p1.c
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int global=1;
int main(void) {
	int local = 2;
	int p = fork();
	if(p > 0) {
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		global++;
		local--;
	} else {
		printf("PID = %d; PPID = %d\n", getpid(), getppid());
		global--;
		local++;
	}
	printf("PID = %d - global = %d ; local = %d\n", getpid(), global, local);
	return 0;
} 