//Simple object-id pool test, written March 2013.
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>

#include "Types/IdPool.hpp"

int main() {
	Deque<Id> deque;
	IdPool pool;
	Id i;

	for(i = pool.fetchId(); i <= 5; i = pool.fetchId()) {
		std::cout << i << " ";
		deque.pushEnd(new Id(i));
	}
	i = *(deque.popBegin());
	pool.returnId(i);
	std::cout << "\n" << i << " ";
	i = *(deque.popBegin());
	pool.returnId(i);
	std::cout << i << " ";
	i = *(deque.popBegin());
	pool.returnId(i);
	std::cout << i << std::endl;

	for(i = pool.fetchId(); i < 10; i = pool.fetchId()) {
		std::cout << i << " ";
		deque.pushEnd(new Id(i));
	}
	std::cout << std::endl;
	//i = 1;
	while(i < 9) {
		i = *(deque.popBegin());
		std::cout << i << " ";
	}

	return 0;
}
