#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv, char** envp)
{
    if (argc != 1) {
	return 1;
    }

    int i = 0;

    while (envp[i] != NULL) {
        printf("%s\n", envp[i]);
	i++;
    }

    return 0;
}

