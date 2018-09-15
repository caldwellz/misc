#include <stdio.h>
#include <stdlib.h>

void oneptr(char *str) {
	printf("oneptr sizeof (char* ?): %i\n", sizeof(str));
	printf("oneptr deref sizeof (char ?): %i\n", sizeof(*str));
	printf("oneptr amp sizeof (char*& ?): %i\n", sizeof(&str));
	printf("oneptr deref amp sizeof (char& ?): %i\n", sizeof(&(*str)));
	printf("oneptr amp index math: %i\n", (&str)[1] - (short)&str);
	printf("oneptr deref amp index math: %i\n", (&(*str))[1] - (short)&str);
	printf("oneptr dual-deref amp index math: %i\n", (&(*str))[1] - (short)&(*str));
}

void dualptr(char **str) {
	printf("dualptr sizeof (char**): %i\n", sizeof(str));
	printf("dualptr deref sizeof (char* ?): %i\n", sizeof(*str));
	printf("dualptr dual-deref sizeof (char ?): %i\n", sizeof(**str));
	printf("dualptr amp index math: %i\n", (&str)[1] - (short)&str);
	printf("dualptr deref amp index math: %i\n", (&(*str))[1] - (short)&str);
	printf("dualptr dual-deref amp index math: %i\n", (&(*str))[1] - (short)&(*str));
}

int main(int argc, char** argv) {
	char** string;
	(*string) = calloc(20 + argc, sizeof(char));
	(*string)[0] = 'T';
	(*string)[1] = 'e';
	(*string)[2] = 's';
	(*string)[3] = 't';
	(*string)[4] = '\0';

	printf("Wanted output: %i\n", (20 + argc) * sizeof(char));
	oneptr(*string);
	dualptr(string);

	free(*string);
	return 0;
}
