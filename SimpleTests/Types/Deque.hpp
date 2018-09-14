//Simple pointer deque (double-ended queue), written March 2013
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _Deque_hpp_
#define _Deque_hpp_

//Implementation needs to stay in header to make template work
template <typename T>
class Deque {
	public:
		Deque():
			_begin(new DequeNode),
			_end(_begin),
			_size(0)
		{ }

		~Deque() {
			clear();
			delete _begin;
		}

		void pushBegin(T *obj) {
			//ScopedLock(_mutex) lock;
			if((_begin == _end) && (_begin->object == 0)) {
				_begin->object = obj;
				_size = 1;
				return;
			}

			_begin->previous = new DequeNode;
			_begin->previous->next = _begin;
			_begin = _begin->previous;
			_begin->object = obj;

			++_size;
		}

		void pushEnd(T *obj) {
			//ScopedLock(_mutex) lock;
			if((_begin == _end) && (_end->object == 0)) {
				_end->object = obj;
				_size = 1;
				return;
			}

			_end->next = new DequeNode;
			_end->next->previous = _end;
			_end = _end->next;
			_end->object = obj;

			++_size;
		}

		T* popBegin() {
			//ScopedLock(_mutex) lock;
			T* ret = _begin->object;

			if(_begin == _end) {
				_begin->object = 0;
				_size = 0;
				return ret;
			}

			_begin = _begin->next;
			delete _begin->previous;
			_begin->previous = 0;

			--_size;
			return ret;
		}

		T* popEnd() {
			//ScopedLock(_mutex) lock;
			T* ret = _end->object;

			if(_begin == _end) {
				_end->object = 0;
				_size = 0;
				return ret;
			}

			_end = _end->previous;
			delete _end->next;
			_end->next = 0;

			--_size;
			return ret;
		}

		unsigned int size() {
			//ScopedLock(_mutex) lock;
			return _size;
		}

		void clear() {
			//ScopedLock(_mutex) lock;

			//Pop node list and delete objects
			while(_begin->next != 0) {
				_begin = _begin->next;
				delete _begin->previous->object;
				delete _begin->previous;
			}
			if(_begin->object != 0) {
				delete _begin->object;
				_begin->object = 0;
			}
			_end = _begin;

			_size = 0;
		}

	private:
		struct DequeNode {
			DequeNode():
				object(0),
				previous(0),
				next(0)
			{ }
			T *object;
			DequeNode *previous, *next;
		};

		Deque(const Deque& other);
		Deque& operator=(const Deque &other);

		DequeNode *_begin, *_end;
		unsigned int _size;
		//FastMutex _mutex;
};

#endif //_Deque_hpp_
