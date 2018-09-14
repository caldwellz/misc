//Simple object-id pool implementation, written March 2013.
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _IdPool_hpp_
#define _IdPool_hpp_

#include "Deque.hpp"

typedef unsigned int Id;

class IdPool {
	public:
		IdPool():
			topId(0)
		{ }

		Id& fetchId() {
			//ScopedLock(mutex) lock;
			if(deque.size()) {
				Id *i = deque.popEnd();
				return (*i);
			}
			++topId;
			return topId;
		}

		void returnId(Id &i) {
			//ScopedLock(mutex) lock;
			deque.pushBegin(new Id(i));
		}

	private:
		Id topId;
		Deque<Id> deque;
		//FastMutex mutex;
};

#endif //_IdPool_hpp_
