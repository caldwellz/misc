/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MAGES_PROPERTYDB_HXX_
#define _MAGES_PROPERTYDB_HXX_

#include <exception>
#include <typeinfo>
#include <vector>

#include <Poco/Mutex.h>

#include "Globals.hxx"
#include "PropertySerializer.hxx"

namespace MAGES {
	struct TrieNode;
	class PropertyDB {
		public:
			/** Default constructor
			* \param charSize Don't use this. Eventually we'll switch to UTF-8 internally and get rid of this.
			* \todo Get rid of the whole charSize thing and replace with UTF-8 stuff. */
			PropertyDB(UInt32 charSize);

			/** Default destructor */
			~PropertyDB();

			/** Get a property.
			*   Pre-instantiated for all the recognized data types.
			*   Will eventually support type conversion, but not yet.
			* \param prop The key (name) of the property you want to retrieve.
			* \returns The data associated with the property. */
			template<typename T> T getProperty(const String& prop);

			/** Get a vector of all property keys.
			*   Try not to use this very often as it is a relatively expensive
			*   operation (it has to crawl through the entire database tree).
			* \returns A vector of property strings. */
			std::vector<String> getPropertyKeys();

			/** Get a vector of all property keys that are marked serializable.
			*   Try not to use this very often as it is a relatively expensive
			*   operation (it has to crawl through the entire database tree).
			*   This mostly exists for the benefit of property serializers and
			*   may eventually be removed.
			* \returns A vector of property strings. */
			std::vector<String> getSerializablePropertyKeys();

			/** Identify the data type of a property value.
			* \param prop The key (name) of the property you want to check.
			* \returns A type value from the RecognizedDataType enum. */
			RecognizedDataType getPropertyType(const String& prop);

			/** Set a property.
			*   Pre-instantiated for all the recognized data types.
			* \param prop The key (name) of the property you want to retrieve.
			* \param val The value/data you want to associate with the property.
			* \param serialize Whether to mark the property for serialization or not.
			* \todo Add function(s) to check / clear the serialization flag. */
			template<typename T> void setProperty(const String& prop, const T& val, bool serialize = false);

			/** Unset (clear) a property.
			* \param prop The key (name) of the property you want to retrieve. */
			void unsetProperty(const String& prop);

			/** Clear the database.
			*   Deletes all keys and values contained within. */
			void clear();

			/** Unserialize (read) properties from a file.
			*   See the individual Serializer* classes for the ability to read from a stream.
			* \param fn The filename you want to read.
			* \param format The serialized file format. See the FileFormat enum for details.
			* \todo Add an override where one can pass in a custom Serializer class.*/
			bool unserializeFromFile(const String& fn, FileFormat format = FMT_AUTO);

			/** Serialize (write) properties to a file.
			*   Only writes properties that have been marked for serialization.
			*   See the individual Serializer* classes for the ability to write to a stream.
			* \param fn The filename you want to read.
			* \param format The serialized file format. See the FileFormat enum for details.
			* \todo Add an override where one can pass in a custom Serializer class.*/
			bool serializeToFile(const String& fn, FileFormat format = FMT_AUTO, UInt32 maxKeySize = DEFAULT_CHAR_SIZE);

//			void registerStack(Id subsystem, vector<Poco::Dynamic::Var>* v, String propertyRoot = "");
//			void registerListener(void (*func)(Poco::Dynamic::Var val), String propertyRoot = "");

		protected:
			void deleteTrie(TrieNode** p);
			Poco::Mutex _dbm;
			Logger& _log;
			Id _currentUpdate;
			UInt32 _charSize;
			TrieNode** _root;
	};


//TODO: Give the exact property in these error messages
	class TypeException: public std::exception {
			const char* what() const throw() {
				return "TypeException: Either given a type that doesn't support copy semantics, or attempted a type conversion that is not yet supported.";
			}
	};

	class NotFoundException: public std::exception {
			const char* what() const throw() {
				return "NotFoundException: Value or file not found.";
			}
	};

	struct TrieNode {
		TrieNode(UInt32 charSize);
		~TrieNode();
		static void deleteValue(TrieNode* n);

		Mutex _tnm;
		UInt32 _charSize;
		void* _value;
		std::type_info* _valueType;
		bool _serializeValue;
		TrieNode** _branches;
	};
}
#endif //_MAGES_PROPERTYDB_HXX_
