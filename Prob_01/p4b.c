#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc != 3 || argv[1] == NULL || argv[2] == NULL) {
	printf("usage: <name> <number of times>\n");
	return 1;
    }

    int numberTimes = atoi(argv[2]);

    if (!numberTimes) {
	printf("please enter an integer larger than zero\n");
        return 1;
    }

    while (numberTimes) {
        printf("Hello %s!\n", argv[1]);
	numberTimes--;
    }

    return 0;
} 



/*
	Apena funciona para um nome (sem uso de aspas)

	Verificação do numero de argumentos - check
	Conversão de string para inteiro (atoi) - check
	Ciclo de escrita - check

	argv[0] --> contém SEMPRE o nome do programa que mandamos executar

		> prog abc cde

		argc = 3
		argv[0] = "prog\0"
		argv[1] = "abc\0"
		argv[2] = "cde\0"
		argv[3] = '\0'

		> prog "abc cde"

		argc = 2
		argv[0] = "prog\0"
		argv[1] = "abc cde\0"
		argv[2] = '\0'
*/

