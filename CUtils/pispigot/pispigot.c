//Original pi spigot code by Fabrice Bellard; modified by me to work with my programs.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "pispigot.h"

int piSpigot(int n)
{
  int av,a,vmax,N,num,den,k,kq,kq2,t,v,s,i;
  double sum;

//n = abs(position);
//if(!n) n = 1;
  
  N=(int)((n+20)*log(10)/log(2));

  sum=0;

  for(a=3;a<=(2*N);a=nextPrime(a)) {

    vmax=(int)(log(2*N)/log(a));
    av=1;
    for(i=0;i<vmax;i++) av=av*a;

    s=0;
    num=1;
    den=1;
    v=0;
    kq=1;
    kq2=1;

    for(k=1;k<=N;k++) {

      t=k;
      if (kq >= a) {
	 do {
	    t=t/a;
	    v--;
	 } while ((t % a) == 0);
	 kq=0;
      }
      kq++;
      num=mulMod(num,t,av);

      t=(2*k-1);
      if (kq2 >= a) {
	 if (kq2 == a) {
	    do {
	       t=t/a;
	       v++;
	    } while ((t % a) == 0);
	 }
	 kq2-=a;
      }
      den=mulMod(den,t,av);
      kq2+=2;
      
      if (v > 0) {
	t=invMod(den,av);
	t=mulMod(t,num,av);
	t=mulMod(t,k,av);
	for(i=v;i<vmax;i++) t=mulMod(t,a,av);
	s+=t;
	if (s>=av) s-=av;
      }
      
    }

    t=powMod(10,n-1,av);
    s=mulMod(s,t,av);
    sum=fmod(sum+(double) s/ (double) av,1.0);
  }

  return (int)(sum*1e9);
}

/* return the inverse of x mod y */
inline int invMod(int x,int y) {
  int q,u,v,a,c,t;

  u=x;
  v=y;
  c=1;
  a=0;
  do {
    q=v/u;
    
    t=c;
    c=a-q*c;
    a=t;
    
    t=u;
    u=v-q*u;
    v=t;
  } while (u!=0);
  a=a%y;
  if (a<0) a=y+a;
  return a;
}

/* return (a^b) mod m */
inline int powMod(int a,int b,int m)
{
  int r,aa;
   
  r=1;
  aa=a;
  while (1) {
    if (b&1) r=mulMod(r,aa,m);
    b=b>>1;
    if (b == 0) break;
    aa=mulMod(aa,aa,m);
  }
  return r;
}
      
/* return true if n is prime */
inline int isPrime(int n)
{
   int r,i;
   if ((n % 2) == 0) return 0;

   r=(int)(sqrt(n));
   for(i=3;i<=r;i+=2) if ((n % i) == 0) return 0;
   return 1;
}

/* return the prime number immediatly after n */
inline int nextPrime(int n)
{
   do {
      n++;
   } while (!isPrime(n));
   return n;
} 
