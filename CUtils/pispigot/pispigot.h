#ifndef _PISPIGOT_H_
#define _PISPIGOT_H_

// Uncomment the following line to use 'long long' integers.
// #define HAS_LONG_LONG
#ifdef HAS_LONG_LONG
#define mulMod(a,b,m) (( (long long) (a) * (long long) (b) ) % (m))
#else
#define mulMod(a,b,m) fmod( (double) a * (double) b, m)
#endif

// Our pi-spigot function.
int piSpigot(int position);

// Returns the inverse of x mod y.
inline int invMod(int x,int y);

// Returns (a^b) mod m.
inline int powMod(int a,int b,int m);

// Returns true if n is prime.
inline int isPrime(int n);

// Returns the prime number immediatly after n.
inline int nextPrime(int n);

#endif //_PISPIGOT_H_
