#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signo)
{
	printf("I'm SIGINT handler ...\n");
}

int main(void)
{
	struct sigaction action;

	if (signal(SIGINT, sigint_handler) < 0)
	{
		fprintf(stderr, "I'm unable to install SIGINT handler ...\n");
		exit(1);
	}

	action.sa_handler = sigint_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);

	printf("I'm sleeping for 30 seconds ...\n");
	sleep(30);
	printf("I'm waking up ...\n");

	return 0;
}
