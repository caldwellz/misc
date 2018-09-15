//Takes an array of integers and eliminates the nonprimes.
//TODO: Set up for double

#include <math.h>
#include <stdio.h>

void SiftPrimes(int *intarry, const int nums, int verbose) {
	int n, i, sqrts[nums];

	if(verbose) printf("Eliminating invalids (even or <3)...\n");
	for(i = 0; i < nums; i++) {
		if(((intarry[i] % 2) != 1) || (intarry[i] < 3)) {
			if(verbose) printf("%i, ", intarry[i]);
			intarry[i] = 0;
		}
	}

	if(verbose) printf("\nGetting square roots (to upper odd)...\n");
	for(i = 0; i < nums; i++) {
		if(intarry[i] > 3) { //This is so we don't try to root anything under 3.
			if(verbose) printf("%i->", intarry[i]); //Print the number we're rooting.

			n = (int) sqrt(intarry[i]); //Get the root.
			if((n % 2) != 1) n++; //Make sure it's an odd number.
			sqrts[i] = n;

			if(verbose) printf("%i, ", n); //Print the number's root.
		}
	}

	if(verbose) printf("\nEliminating remaining nonprimes...\n");
	i = 0;
	while(i < nums) {
		n = sqrts[i];
		while(n >= 3) {
			if(intarry[i]) { //This is so we don't try to test nulls.
				if(intarry[i] % n == 0) {
					if(verbose) printf("%i/%i, ", intarry[i], n);
					intarry[i] = 0;
				}
			}
			n -= 2;
		};
		i++;
	};
	if(verbose) printf("\n\n");
}
