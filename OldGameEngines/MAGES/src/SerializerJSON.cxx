/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include <iostream>
#include <string>
using namespace std;

#include <Poco/NumberParser.h>
#include <Poco/Util/JSONConfiguration.h>
using namespace Poco;
using namespace Poco::Util;

#include "MAGES/PropertyDB.hxx"
#include "MAGES/SerializerJSON.hxx"
using namespace MAGES;

SerializerJSON::SerializerJSON():
	_log(Logger::get("MAGES.SerializerJSON")) {
}


bool SerializerJSON::read(istream& input, PropertyDB* db) {
	JSONConfiguration config;
	NumberParser nparser;
	String s("Property1"), prop;
	UInt32 n = 1, propType;

	try {
		poco_debug(_log, "Loading JSON stream...");
		config.load(input);

		///@todo Replace this with a more robust implementation?
		//For now we ignorantly assume that all properties are contained
		// within the root key range and follow the expected format.
		while(config.hasProperty(s)) {
			prop = config.getString(s);
			propType = config.getUInt(s + "-Type");

			switch(propType) {
				case RDT_INT32:
					db->setProperty<Int32>(prop, config.getInt(prop));
					break;

				case RDT_INT64:
					db->setProperty<Int64>(prop, config.getInt64(prop));
					break;

				case RDT_UINT32:
					db->setProperty<UInt32>(prop, config.getUInt(prop));
					break;

				case RDT_UINT64:
					db->setProperty<UInt64>(prop, config.getUInt64(prop));
					break;

				case RDT_BOOL:
					db->setProperty<bool>(prop, config.getBool(prop));
					break;

				case RDT_FLOAT:
					///@todo Just double-check that the float<->double thing actually converts properly...
					db->setProperty<float>(prop, (float) config.getDouble(prop));
					break;

				case RDT_DOUBLE:
					db->setProperty<double>(prop, config.getDouble(prop));
					break;

				case RDT_STRING:
					db->setProperty<String>(prop, config.getString(prop));
					break;

				default:
					poco_error(_log, String("Invalid data type listed, skipping property: ") + prop);
					break;
			}

//continue_after_syntax_error:
			++n;
			s.assign("Property");
			s.append(to_string(n)); //C++11 feature
		}
	} catch(SyntaxException e) {
		poco_error(_log, String("Encountered syntax error, aborting JSON load. Exception description and property: \'") + e.what() + String("\', ") + prop);
		//Wish we could just do this, but C++ is dumb sometimes...
		//goto continue_after_syntax_error;
		return false;
	} catch(std::exception& e) {
		poco_error(_log, String("Aborting property loading, caught unhandled exception:") + e.what());
		return false;
	} catch(...) {
		poco_error(_log, "Caught unhandled non-std exception! Aborting property loading.");
		return false;
	}

	return true;
}


bool SerializerJSON::write(PropertyDB* db, ostream& output, UInt32 maxKeySize) {
	JSONConfiguration config;
	RecognizedDataType rdt;
	String s;
	UInt32 n = 1;
	vector<String> v = db->getSerializablePropertyKeys();
	vector<String>::iterator itv;

	config.loadEmpty("");

	for(itv = v.begin(); itv != v.end(); ++itv) {
		try {
			rdt = db->getPropertyType(*itv);

			switch(rdt) {
				case RDT_INT32:
					config.setInt(*itv, db->getProperty<Int32>(*itv));
					break;

				case RDT_INT64:
					config.setInt64(*itv, db->getProperty<Int64>(*itv));
					break;

				case RDT_UINT32:
					config.setUInt(*itv, db->getProperty<UInt32>(*itv));
					break;

				case RDT_UINT64:
					config.setUInt64(*itv, db->getProperty<UInt64>(*itv));
					break;

				case RDT_BOOL:
					config.setBool(*itv, db->getProperty<bool>(*itv));
					break;

				case RDT_FLOAT:
					///@todo Just double-check that the float<->double thing actually converts properly...
					config.setDouble(*itv, (double) db->getProperty<float>(*itv));
					break;

				case RDT_DOUBLE:
					config.setDouble(*itv, db->getProperty<double>(*itv));
					break;

				case RDT_STRING:
					config.setString(*itv, db->getProperty<String>(*itv));
					break;

				default:
					poco_error(_log, String("Unknown data type, skipping property: ") + (*itv));
					break;
			}

			s.assign("Property");
			s.append(to_string(n)); //C++11 feature
			config.setString(s, *itv);
			s.append("-Type");
			config.setUInt(s, rdt);
			++n;
		} catch(...) {
			poco_error(_log, String("Caught exception while serializing, skipping property: ") + (*itv));
		}
	}

	config.save(output);

	return true;
}
