#ifndef _PRIMESFT_H_
#define _PRIMESFT_H_
//extern "C" {

/* Our prime-sifting function.
 *
 * Params: An array of integers to sift and a variable to tell it whether to be verbose or not.
 * Returns: Nothing, but nulls nonprimes in the array.
*/
void SiftPrimes(int *intarry, const int nums, int verbose);

//}
#endif //_PRIMESFT_H_
