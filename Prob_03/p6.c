// PROGRAMA p6b.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(void)
{
	pid_t pid;
	int i, j;
	int status;

	printf("Im process %d. My parent is %d.\n", getpid(),getppid());
	for (i=1; i<=3; i++) {
		pid = fork();
		if ( pid < 0) {
			printf("fork error");
			exit(1);
		}
		else if (pid == 0) {
			printf("Im process %d. My parent is %d. Im going to work for 1 second ...\n",getpid(),getppid());
 			sleep(1); // simulando o trabalho do filho
 			printf("Im process %d. My parent is %d. I finished my work\n", getpid(),getppid());
 			exit(0); // a eliminar na alinea c)
}
 else{ // simulando o trabalho do pai
 	for (j=1; j<=10; j++) {
 		waitpid(pid, &status, WNOHANG);
 		sleep(1);
 		printf("father working ...\n");
 		}
 	}
}
 exit(0);
} 


// P6B.C