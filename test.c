#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv)
{
	int x = (int) drand48() * 4;
	printf("%d", x);
	return 0;
}
