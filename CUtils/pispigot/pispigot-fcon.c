#include <stdlib.h>
#include <stdio.h>

#include "pispigot.h"

int main(int argc,char *argv[])
{
  int n, i, x;
  FILE *pitxt;

	if (argc<2) {
		printf("This program computes the decimal digits of pi up to the n'th\n"
			"position and writes them to pi.txt.\n"
			"usage: %s n, where n is the digit you want to calculate\n"
			"to (rounded up to a multiple of 9).\n", argv[0]);
		exit(1);
	}

  pitxt = fopen("pi.txt", "w");
  n = abs(atoi(argv[1]));
  if(n < 10) n = 10;

  fprintf(pitxt, "3.\n");
  for(i = 1, x = 1; i < n; i += 9, x++) {
	fprintf(pitxt, "%i ", piSpigot(i));
	if(x == 11) {
		fprintf(pitxt, "\n");
		x = 0;
	}
  }
  fclose(pitxt);
  return 0;
}
