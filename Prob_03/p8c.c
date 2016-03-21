// PROGRAMA p8.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *envp[])
{
	pid_t pid;
	if (argc != 2) {
		printf("usage: %s dirname\n",argv[0]);
		exit(1);
	}
	pid=fork();

	char* execVector[4];
	execVector[0] = "ls";
	execVector[1] = "-laR";
	execVector[2] = argv[1];
	execVector[3] = NULL;

	if (pid > 0)
		printf("My child is going to execute command ls -laR %s\"\n", argv[1]);
	else if (pid == 0){
		execvp("ls",execVector);
		printf("Command not executed !\n");
		exit(1);
	}
	exit(0);
} 