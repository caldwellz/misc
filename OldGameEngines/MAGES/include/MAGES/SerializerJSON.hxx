/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MAGES_SERIALIZERJSON_HXX_
#define _MAGES_SERIALIZERJSON_HXX_

#include "Globals.hxx"
#include "PropertySerializer.hxx"

namespace MAGES {
	class PropertyDB;

	/// Implements a PropertySerializer using the JSON format
	class SerializerJSON: public PropertySerializer {
		public:
			SerializerJSON();
			bool read(std::istream& input, PropertyDB* db);
			bool write(PropertyDB* db, std::ostream& output, UInt32 maxKeySize);

		protected:
			Logger& _log;
	};
}
#endif //_MAGES_SERIALIZERJSON_HXX_
