//Simple fixed-size LIFO queue algorithm, written March 2013.
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

//NOT WORKING

#include <iostream>

template <typename T>
class LIFO {
	public:
		LIFO(unsigned int queueSize = 65535) {
			_queueMax = queueSize;
			_queue = new T *[queueSize];
			for(_queueEnd = 0; _queueEnd < queueSize; ++_queueEnd) _queue[_queueEnd] = 0;
			_queueEnd = 0;
		}

		~LIFO() {
			delete[] _queue;
		}

		T* operator[](unsigned int pos) {
			if(pos <= _queueMax) {
				return _queue[pos];
			} else {
				return 0;
			}
		}

		bool push(T *item) {
			if(_queueEnd == _queueMax) return false;
			_queue[_queueEnd] = item;
			++_queueEnd;
			return true;
		}

		T* pop() {
			if(_queueEnd == 0) return 0;
			--_queueEnd;
			return _queue[_queueEnd];
			//_queue[_queueEnd] = 0;
		}

	private:
		unsigned int _queueEnd, _queueMax;
		T **_queue;
};

int main() {
	int i;
	LIFO<int> lifo(10);

	for(i = 1; i < 5; ++i) {
		std::cout << i << " ";
		lifo.push(&i);
	}
	i = *(lifo.pop());
	std::cout << i << " ";
	i = *(lifo.pop());
	std::cout << i << " ";
	i = *(lifo.pop());
	while(lifo.push(&i)) {
		std::cout << i << " ";
		++i;
	}
	std::cout << std::endl;
	for(i = 0; i < 10; ++i) {
		std::cout << *(lifo[i]) << " ";
	}

	return 0;
}
