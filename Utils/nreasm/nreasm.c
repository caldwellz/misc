//Makes ndisasm dumps reassemble-able by stripping the first 28 characters of each line and outputting the result to an asm file.
//Need to set up space-finding loops to fix long-hex dump bug.
//Base copied from a cprogramming.com tut.
#include <stdio.h>
#include <string.h>
int main ( int argc, char *argv[] )
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s <dump filename>", argv[0] );
	return 0;
    }

        // We assume argv[1] is a filename to open
        FILE *dump = fopen(argv[1], "r");
	FILE *reasm = fopen(strcat(argv[1], ".asm"), "w");

        /* fopen returns 0, the NULL pointer, on failure */
        if ((dump || reasm) == 0)
        {
		printf( "Could not open file!\n" );
		return 1;
        }

            int x, i;
            /* read one character at a time from file, stopping at EOF, which
               indicates the end of the file.  Note that the idiom of "assign
               to a variable, check the value" used below works because
               the assignment statement evaluates to the value assigned. */
		fprintf(reasm, ";Start\n");
            while(fgetc(dump) != EOF)
            {
		for(i = 1; i < 27; i++) {
			fgetc(dump); }

		while((x = fgetc(dump)) != '\n') {
			fprintf(reasm, "%c", (int) x);
		}
		fprintf(reasm, "\n");
            }
		fclose(dump);
		fclose(reasm);
}
