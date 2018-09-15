#include <stdlib.h>
#include <stdio.h>

#include "pispigot.h"

int main(int argc,char *argv[])
{
  int n;

	if (argc<2) {
		printf("This program computes decimal digits of pi at the n'th position.\n"
			"usage: %s n, where n is the digit you want.\n", argv[0]);
		exit(1);
	}

  n = abs(atoi(argv[1]));
  if(!n) n = 1;
  printf("Decimal digits of pi at position %i: %i\n", n, piSpigot(n));
  return 0;
}
