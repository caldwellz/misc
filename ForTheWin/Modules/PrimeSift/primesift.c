/*
 *	ForTheWin: A modular windoze utility program.
 *	This file is part of ForTheWin.
 *
 *	Copyright (C) 2012 Zach Caldwell
 *
 * ForTheWin is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * ForTheWin is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *	along with ForTheWin.  If not, see <http://www.gnu.org/licenses/>.
*/

/// The rest of this file was from one of my old console C programs, so it's a little archaic ;)

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
