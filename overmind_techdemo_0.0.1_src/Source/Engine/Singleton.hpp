/*
 *	Overmind: A 3D RPG game.
 *	This file is part of Overmind.
 *
 *	Copyright (C) 2012 by authors.
 *
 * Overmind is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * Overmind is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *	along with Overmind.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Original version Copyright (C) Scott Bilas, 2000.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Bilas, 2000"
 */

#ifndef _Singleton_hpp_
#define _Singleton_hpp_

#include <cassert>

namespace OvermindEngine {
	/// \brief Template class for creating single-instance global classes.
	template <typename T> class Singleton {
		public:
			/// \brief Constructor
			Singleton(void) {
				assert( !msSingleton );
				#if defined( _MSC_VER ) && _MSC_VER < 1200	 
					int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
					msSingleton = (T*)((int)this + offset);
				#else
					msSingleton = static_cast< T* >( this );
				#endif
			}

			/// \brief Destructor
			virtual ~Singleton(void) { assert( msSingleton ); msSingleton = 0; }

			/// \brief Get a Singleton reference
			static T& getSingleton(void) { assert(msSingleton); return (*msSingleton); }
			/// \brief Get a Singleton pointer
			static T* getSingletonPtr(void) { return msSingleton; }

		protected:
			static T* msSingleton;

		private:
			/// \brief Explicit private copy constructor. This is a forbidden operation.
			Singleton(const Singleton<T> &);

			/// \brief Private operator= . This is a forbidden operation.
			Singleton& operator=(const Singleton<T> &);
	};
} //namespace OvermindEngine

#endif //_Singleton_hpp_
