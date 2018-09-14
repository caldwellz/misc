#include <stdio.h>
#include <stdlib.h>
#include "primesft.h"

int main(int argc, char *argv[]) {
	int nums[argc - 1], num2[argc - 1], i, n;

	for(i = 0; i < argc - 1; i++) {
		nums[i] = atoi(argv[i + 1]);
		num2[i] = atoi(argv[i + 1]);
	}

	for(n = 0; n < i; n++) {
		if(testPrime(num2[n]))
			printf("%i ", num2[n]);
	};
}
