#include <stdio.h>
#include <stdlib.h>
#include "primesft.h"

int main(int argc, char *argv[]) {
	int nums[argc - 1], num2[argc - 1], i, n;

	for(i = 0; i < argc - 1; i++) {
		nums[i] = atoi(argv[i + 1]);
		num2[i] = atoi(argv[i + 1]);
	}
/*
	SiftPrimes(nums, 0);

	while(n < i) {
		if(nums[i] != 0)
		printf("%i ", nums[n]);
		n++;
	};
*/
	SiftPrimes(num2, i, 1);

	for(n = 0; n < i; n++) {
		if(num2[n] != 0)
			printf("%i ", num2[n]);
	};
}
