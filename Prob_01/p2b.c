#include <stdio.h>
int main(void)
{
	printf("Hello !\n");
	return 0;
}


/*
/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crt1.o: In function `_start':
/build/glibc-ryFjv0/glibc-2.21/csu/../sysdeps/x86_64/start.S:114: undefined reference to `main'
collect2: error: ld returned 1 exit status
*/
