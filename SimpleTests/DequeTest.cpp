//Simple test for my own custom Deque implementation. Written March 2013.
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <iostream>

#include "Types/Deque.hpp"

int main() {
	unsigned int *i = new unsigned int;
	Deque<unsigned int> empty, test;

	//Begin/Begin
	for((*i) = 0; (*i) < 5; (*i)++) {
		test.pushBegin(new unsigned int(*i));
		std::cout << " " << (*i) << " ";
	}
	std::cout << "\t\t\t\t\t\t\tSize:" << test.size() << std::endl;
	while(i != 0) {
		std::cout << " " << (*i) << " ";
		i = test.popBegin();
	}
	std::cout << "\t\t\t\t\t\tSize:" << test.size() << std::endl;

	test.clear();

	//End/End
	i = new unsigned int;
	for((*i) = 10; (*i) < 20; (*i)++) {
		test.pushEnd(new unsigned int(*i));
		std::cout << (*i) << " ";
	}
	std::cout << "\t\t\t\t\tSize:" << test.size() << std::endl;
	while(i != 0) {
		std::cout << (*i) << " ";
		i = test.popEnd();
	}
	i = test.popEnd();
	i = test.popEnd();
	std::cout << "\t\t\t\tSize:" << test.size() << std::endl;

	test.pushEnd(new unsigned int(10));
	test.clear();

	//Begin/End
	i = new unsigned int;
	for((*i) = 25; (*i) < 40; (*i)++) {
		test.pushBegin(new unsigned int(*i));
		std::cout << (*i) << " ";
	}
	std::cout << "\t\t\tSize:" << test.size() << std::endl;
	while(i != 0) {
		std::cout << (*i) << " ";
		i = test.popEnd();
	}
	std::cout << "\t\tSize:" << test.size() << std::endl;

	test.pushBegin(new unsigned int(11));
	test.pushBegin(new unsigned int(12));
	test.clear();

	//End/Begin
	i = new unsigned int;
	for((*i) = 45; (*i) < 65; (*i)++) {
		test.pushEnd(new unsigned int(*i));
		std::cout << (*i) << " ";
	}
	std::cout << "\tSize:" << test.size() << std::endl;
	while(i != 0) {
		std::cout << (*i) << " ";
		i = test.popBegin();
	}
	std::cout << "\tSize:" << test.size() << std::endl;


	return 0;
}
