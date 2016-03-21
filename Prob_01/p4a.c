#include <stdio.h>

int main(int argc, char** argv)
{


    if (argc != 2) {
    printf("Note: Use \"name\" for multiple names\n");
	return 1;
    }

    if (argv[1] == NULL) {
	return 1;
    }


    
	printf("Hello %s!\n", argv[1]);
    return 0;
} 



/*

	p4a.c para um nome.
	Adicionar "" no nome para adicionar varios nomes.

*/
