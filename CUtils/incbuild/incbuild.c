//A little proggie to automatically increment a build number in an included header file. Written June 2011.
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	int thechar = 0, buildpos;
	char buildstr[5];

	FILE *header;
	header = fopen("bversion.h", "r+");	//Open header for updating.
	if(header == NULL) {
		printf("Header could not be found!");
		return 1;
	}

	while(thechar != 46) {	//(char) 46 == "."; This seeks to the first period in the version number.
		thechar = fgetc(header);
	};

	thechar = 0;
	while(thechar != 46) {	//This seeks to the second period.
		thechar = fgetc(header);
	};

	thechar = 0;
	while(thechar != 46) {	//This seeks to the third period.
		thechar = fgetc(header);
	};

	buildpos = ftell(header); //This saves the starting position of the build number.
	for(thechar = 0; thechar < 4; thechar++) { //We're just reusing thechar as a loop counter here.
		buildstr[thechar] = fgetc(header); //Grab the build number string. We expect it to be 4 characters long.
	}

	thechar = atoi(buildstr);	//Now we're using thechar to hold the build number integer.
	thechar++;			//Increment it.

	if(thechar < 1000)		//These are to make sure we take leading zeros into account.
		buildpos++;
	if(thechar < 100)
		buildpos++;
	if(thechar < 10)
		buildpos++;

	fseek(header, buildpos, 0);	//Seek to where the number starts.
	fprintf(header, "%i", thechar);	//Write the new number.

	fseek(header, buildpos + 24, 0);//Seek to where the number starts.
	fprintf(header, "%i", thechar);	//Write the new number.

	fclose(header);
	return 0;
}
