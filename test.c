#include <stdlib.h>
#include <stdio.h>
int main()
{
	int *t = malloc(4);
	*t = 42;
	printf("%p\n", t);
	free(t);
	t = NULL;
	printf("%p\n", t);
}