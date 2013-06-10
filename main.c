#include <stdio.h>
#include <stdlib.h>

/* Returns 2 to the n-th. */
int pow2(int n)
{
    if (n == 0) return 1;
    else return 2 << (n - 1);
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);

	printf("2^%d = %d\n", n, pow2(n));

	return 0;
}
