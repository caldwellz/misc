/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MAGES_PROPERTYSERIALIZER_HXX_
#define _MAGES_PROPERTYSERIALIZER_HXX_

#include <iostream>

#include <Poco/Exception.h>

#include "Globals.hxx"

namespace MAGES {
	class PropertyDB;

	/** Basic PropertySerializer interface.
	*   Custom property serializers should inherit this class. */
	class PropertySerializer {
		public:
			/** Read properties from a stream.
			*   The format of the stream depends on the implementing subclass.
			* \param input An input stream. Will usually be a file stream that
			*        the PropertyDB has opened for us via unserializeFromFile().
			* \param db Pointer to the PropertyDB that you want to add the properties to. */
			virtual bool read(std::istream& input, PropertyDB* db) = 0;

			/** Write properties to a stream.
			*   The format of the stream depends on the implementing subclass.
			* \param db Pointer to the PropertyDB that you want to get properties from. Note
			*        that this will only write properties that have been marked for serialization.
			* \param input An input stream. Will usually be a file stream that
			*        the PropertyDB has opened for us via serializeToFile().
			* \param maxKeySize Don't use. Will eventually be replaced by internally smarter code. */
			virtual bool write(PropertyDB* db, std::ostream& output, UInt32 maxKeySize) = 0;
	};

	class ReadException: public Poco::Exception {
		public:
			ReadException() throw();
			ReadException(const String path) throw();
			~ReadException() throw();
			const char* what() const throw();
		protected:
			String msg;
	};

	class WriteException: public Poco::Exception {
		public:
			WriteException() throw();
			WriteException(const String path) throw();
			~WriteException() throw();
			const char* what() const throw();
		protected:
			String msg;
	};
}
#endif //_MAGES_PROPERTYSERIALIZER_HXX_
