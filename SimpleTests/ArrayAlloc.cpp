//Simple test program that allocates a bunch of big arrays on a timer, to test memory usage. Written March 2013.

#include <iostream>
#include <string>

#include <Poco/Timestamp.h>

int main() {
	Poco::Timestamp timestamp;
	Poco::Int64 timediff = 0;
	int arraytotal = 0, cleartotal;
	std::string* array[30];
	for(cleartotal = 0; cleartotal < 30; ++cleartotal) array[cleartotal] = 0;

	//unsigned int test[950243];
	//std::string** array;
	//array = new std::string*[47483646];

	while(timediff < 15000000) {
		if((timestamp.elapsed() - timediff) >= 500000) {
			timediff = timestamp.elapsed();
			std::cout << timediff << std::endl;
			array[arraytotal] = new std::string[32767];
			for(cleartotal = 0; cleartotal < 32767; ++cleartotal) array[arraytotal][cleartotal] = "";
			++arraytotal;
		}
	}

	return 0;
}
