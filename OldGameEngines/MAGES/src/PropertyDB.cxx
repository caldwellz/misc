/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include <fstream>
#include <iostream>
#include <stack>
#include <typeinfo>
#include <vector>
using namespace std;

#include <Poco/DeflatingStream.h>
#include <Poco/InflatingStream.h>
#include <Poco/File.h>
#include <Poco/Path.h>
//#include <Poco/Dynamic/Var.h>
using namespace Poco;

#include "MAGES/SerializerJSON.hxx"
//#include "MAGES/SerializerXML.hxx"
#include "MAGES/PropertyDB.hxx"
using namespace MAGES;

PropertyDB::PropertyDB(UInt32 charSize):
	_log(Logger::get("MAGES.PropertyDB")) {

	Mutex::ScopedLock lock(_dbm);
	UInt32 n;
	_currentUpdate = 0;
	_charSize = charSize;

	_root = new TrieNode*[_charSize];
	for(n = 0; n < _charSize; ++n) _root[n] = NULL;
}


PropertyDB::~PropertyDB() {
	Mutex::ScopedLock lock(_dbm);

	deleteTrie(_root);
}


template<typename T> void PropertyDB::setProperty(const String& prop, const T& val, bool serialize) {
	UInt32 n;
	TrieNode** p;
	TrieNode* endNode;
	T* newVal;

	try {
		newVal = new T;
		(*newVal) = val;
		if(!newVal) throw std::exception();
	} catch(...) {
		throw TypeException();
	}

	//Traverse trie
	_dbm.lock();
	p = _root;
	for(n = 0; n < prop.length(); ++n) {
		if(prop[n] > _charSize) {
			poco_error_f2(_log, "Property name '%s' contains out-of-range character '%c', aborting set operation.", prop, prop[n]);
//			_dbm.unlock();
//			return;
		}
		if(p[prop[n]] == NULL) {
			p[prop[n]] = new TrieNode(_charSize);
		}
		if(n < prop.length() - 1) p = p[prop[n]]->_branches;
	}

	endNode = p[prop[prop.length() - 1]];
	Mutex::ScopedLock lock(endNode->_tnm);
	_dbm.unlock();

	//Store the value
	try {
		TrieNode::deleteValue(endNode);
		endNode->_value = static_cast<void*>(newVal);
		endNode->_valueType = const_cast<type_info*>(&typeid(val));
		endNode->_serializeValue = serialize;
	} catch(...) {
		endNode->_value = NULL;
		endNode->_valueType = NULL;
		throw TypeException();
	}

	++_currentUpdate;
	///@todo Notify listeners
}
//Instantiate for supported types
template void PropertyDB::setProperty(const String& prop, const Int32& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const Int64& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const UInt32& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const UInt64& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const bool& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const float& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const double& val, bool serialize);
template void PropertyDB::setProperty(const String& prop, const String& val, bool serialize);


template<typename T> T PropertyDB::getProperty(const String& prop) {
	UInt32 n;
	TrieNode** p;
	TrieNode* endNode;
	T tmp;

	//Traverse trie
	_dbm.lock();
	p = _root;
	for(n = 0; n < prop.length(); ++n) {
		if(prop[n] > _charSize) {
			poco_error_f2(_log, "Property name '%s' contains out-of-range character '%c', aborting get operation.", prop, prop[n]);
			_dbm.unlock();
			return tmp;
		}
		if(p[prop[n]] == NULL) {
			poco_warning(_log, String("PropertyDB: Tried to access non-existent property: ") + prop);
			throw NotFoundException();
		}
		if(n < prop.length() - 1) p = p[prop[n]]->_branches;
	}

	endNode = p[prop[prop.length() - 1]];
	Mutex::ScopedLock lock(endNode->_tnm);
	_dbm.unlock();

	if(endNode->_value != NULL) {
		try {
			if(typeid(tmp) == *(endNode->_valueType)) {
				tmp = *(static_cast<T*>(endNode->_value));
			} else {
				///@todo Attempt a type conversion.
				//Maybe something like this? It didn't work right in my early
				// tests but I found later that that copy of Poco was broken anyway...
				//tmp = Dynamic::Var(*(endNode->_value)).convert<T>();
				throw TypeException();
			}
		} catch(...) {
			throw TypeException();
		}
	} else throw NotFoundException();

	return tmp;
}
//Instantiate for supported types
template Int32 PropertyDB::getProperty(const String& prop);
template Int64 PropertyDB::getProperty(const String& prop);
template UInt32 PropertyDB::getProperty(const String& prop);
template UInt64 PropertyDB::getProperty(const String& prop);
template bool PropertyDB::getProperty(const String& prop);
template float PropertyDB::getProperty(const String& prop);
template double PropertyDB::getProperty(const String& prop);
template String PropertyDB::getProperty(const String& prop);


std::vector<String> PropertyDB::getPropertyKeys() {
	Mutex::ScopedLock lock(_dbm);
	stack<TrieNode**> stkBranches;
	stack<UInt32> stkPos;
	String prop;
	TrieNode** branchp = _root;
	UInt32 n = 0;
	vector<String> v;

process_node:
	for(; n < _charSize; ++n) {
		if(branchp[n] != NULL) {
			prop.push_back((char) n);
			if(branchp[n]->_value != NULL) {
				v.push_back(prop);
			}

			stkBranches.push(branchp);
			stkPos.push(n + 1);
			branchp = branchp[n]->_branches;
			n = 0;
			goto process_node;
		}
	}

	//When we get here it means we've reached the end of a line,
	// so if there's still stored positions we pop one and move on
	if(!stkBranches.empty()) {
		branchp = stkBranches.top();
		stkBranches.pop();
		n = stkPos.top();
		stkPos.pop();
		prop.pop_back(); //C++11 feature
		goto process_node;
	}

	return v;
}


std::vector<String> PropertyDB::getSerializablePropertyKeys() {
	Mutex::ScopedLock lock(_dbm);
	stack<TrieNode**> stkBranches;
	stack<UInt32> stkPos;
	String prop;
	TrieNode** branchp = _root;
	UInt32 n = 0;
	vector<String> v;

process_node:
	for(; n < _charSize; ++n) {
		if(branchp[n] != NULL) {
			prop.push_back((char) n);
			if((branchp[n]->_value != NULL) && (branchp[n]->_serializeValue)) {
				v.push_back(prop);
			}

			stkBranches.push(branchp);
			stkPos.push(n + 1);
			branchp = branchp[n]->_branches;
			n = 0;
			goto process_node;
		}
	}

	//When we get here it means we've reached the end of a line,
	// so if there's still stored positions we pop one and move on
	if(!stkBranches.empty()) {
		branchp = stkBranches.top();
		stkBranches.pop();
		n = stkPos.top();
		stkPos.pop();
		prop.pop_back(); //C++11 feature
		goto process_node;
	}

	return v;
}


RecognizedDataType PropertyDB::getPropertyType(const String& prop) {
	RecognizedDataType rdt;
	TrieNode** p;
	TrieNode* endNode;
	UInt32 n;

	//Traverse trie
	_dbm.lock();
	p = _root;
	for(n = 0; n < prop.length(); ++n) {
		if(p[prop[n]] == NULL) {
			poco_warning(_log, String("PropertyDB: Tried to access non-existent property: ") + prop);
			throw NotFoundException();
			//return RDT_UNKNOWN;
		}

		if(n < prop.length() - 1) p = p[prop[n]]->_branches;
	}

	endNode = p[prop[prop.length() - 1]];
	endNode->_tnm.lock();
	_dbm.unlock();

	rdt = checkType(endNode->_valueType);
	endNode->_tnm.unlock();
	return rdt;
}


void PropertyDB::unsetProperty(const String& prop) {
	UInt32 n;
	TrieNode** p;
	TrieNode* endNode;

	//Traverse trie
	_dbm.lock();
	p = _root;
	for(n = 0; n < prop.length(); ++n) {
		if(p[prop[n]] == NULL) {
			return;
		}

		if(n < prop.length() - 1) p = p[prop[n]]->_branches;
	}

	endNode = p[(UInt8)prop[prop.length() - 1]];
	Mutex::ScopedLock lock(endNode->_tnm);
	_dbm.unlock();

	TrieNode::deleteValue(endNode);
}


void PropertyDB::clear() {
	Mutex::ScopedLock lock(_dbm);
	UInt32 n;

	deleteTrie(_root);
	_root = new TrieNode*[_charSize];
	for(n = 0; n < _charSize; ++n) _root[n] = NULL;
}


bool PropertyDB::unserializeFromFile(const String& fn, FileFormat format) {
	Path path(fn);
	File f(path);
	PropertySerializer* ps;
	bool success, compress;

	poco_debug(_log, String("Attempting to load file: ") + fn);
	if(f.exists() && f.isFile() && f.canRead()) {
		///@todo Make file type detection sane/robust. Also implement things like XML and maybe INI support.
		if(format == FMT_AUTO) {
			String ext = path.getExtension();
			if(ext == "json" || ext == "JSON") ps = new SerializerJSON;
			if(ext == "jsonz" || ext == "JSONZ") {
				ps = new SerializerJSON;
				compress = true;
			}
//			else if(ext == "xml" || ext == "XML") ps = new SerializerXML;
			else return false;
		} else {
			switch(format) {
				case FMT_JSON:
					ps = new SerializerJSON;
					break;

				case FMT_JSON_COMPRESSED:
					ps = new SerializerJSON;
					compress = true;
					break;

//				case FMT_XML:
//					ps = new SerializerXML;
//					break;

				default:
					return false;
			}
		}

		fstream ifile(fn.c_str(), ifstream::in | ifstream::binary);
		if(compress) {
			InflatingInputStream zstr(ifile);
			success = ps->read(zstr, this);
			//zstr.close();
		} else {
			success = ps->read(ifile, this);
		}

		ifile.close();
		delete ps;
		if(!success) {
			poco_error(_log, String("Could not load file: ") + fn);
		}

		return success;
	} else {
		poco_error(_log, String("Could not open file: ") + fn);
	}

	///@todo Decide whether throwing Read/Write exceptions would be better than returning bools here.
	return false;
}


bool PropertyDB::serializeToFile(const String& fn, FileFormat format, UInt32 maxKeySize) {
	Path path(fn);
	File f(path);
	PropertySerializer* ps;
	bool success, compress;

	try {
		poco_debug(_log, String("Attempting to save file: ") + fn);
		if(!(f.exists() && f.isFile() && f.canWrite())) {
			if(!(f.createFile() && f.canWrite())) {
				//File functions should throw on error, but just in case...
				throw NotFoundException();
			}
		}

		///@todo Make file type detection sane/robust. Also implement things like XML and maybe INI support.
		if(format == FMT_AUTO) {
			String ext = path.getExtension();
			if(ext == "json" || ext == "JSON") ps = new SerializerJSON;
			if(ext == "jsonz" || ext == "JSONZ") {
				ps = new SerializerJSON;
				compress = true;
			}
//			else if(ext == "xml" || ext == "XML") ps = new SerializerXML;
			else return false;
		} else switch(format) {
				case FMT_JSON:
					ps = new SerializerJSON;
					break;

				case FMT_JSON_COMPRESSED:
					compress = true;
					ps = new SerializerJSON;
					break;

//				case FMT_XML:
//					ps = new SerializerXML;
//					break;

				default:
					return false;
			}

		fstream ofile(fn.c_str(), ofstream::out | ofstream::binary | ofstream::trunc);
		if(compress) {
			DeflatingOutputStream zstr(ofile);
			success = ps->write(this, zstr, maxKeySize);
			zstr.close();
		} else {
			success = ps->write(this, ofile, maxKeySize);
		}

		ofile.close();
		delete ps;
		if(!success) throw WriteException();

		return true;
	} catch(...) {
		poco_error(_log, String("Could not load file: ") + fn);
	}

	///@todo Decide whether throwing Read/Write exceptions would be better than returning bools here.
	return false;
}


void PropertyDB::deleteTrie(TrieNode** root) {
	UInt32 n;

	if(root != NULL) {
		for(n = 0; n < _charSize; ++n) {
			if(root[n] != NULL) delete root[n];
		}

		delete root;
		root = NULL;
	}
}


inline void deleteVoid(RecognizedDataType t, void* v) {
	switch(t) {
		case RDT_INT32:
			delete static_cast<Int32*>(v);
			break;

		case RDT_INT64:
			delete static_cast<Int64*>(v);
			break;

		case RDT_UINT32:
			delete static_cast<UInt32*>(v);
			break;

		case RDT_UINT64:
			delete static_cast<UInt64*>(v);
			break;

		case RDT_BOOL:
			delete static_cast<bool*>(v);
			break;

//		case RDT_CHAR:
//			delete static_cast<char*>(v);
//			break;

		case RDT_FLOAT:
			delete static_cast<float*>(v);
			break;

		case RDT_DOUBLE:
			delete static_cast<double*>(v);
			break;

		case RDT_STRING:
			delete static_cast<String*>(v);
			break;

		default:
			break;
	}
}


TrieNode::TrieNode(UInt32 charSize) {
	Mutex::ScopedLock lock(_tnm);
	UInt32 n;
	_charSize = charSize;
	_value = NULL;
	_valueType = NULL;
	_serializeValue = false;
	_branches = new TrieNode*[_charSize];

	for(n = 0; n < _charSize; ++n) {
		_branches[n] = NULL;
	}
}


TrieNode::~TrieNode() {
	Mutex::ScopedLock lock(_tnm);
	UInt32 n;

	for(n = 0; n < _charSize; ++n) {
		if(_branches[n] != NULL) delete _branches[n];
	}

	delete _branches;
	deleteValue(this);
}


void TrieNode::deleteValue(TrieNode* n) {
	if((n != NULL) && (n->_value != NULL) && (n->_valueType != NULL)) {
		deleteVoid(checkType(n->_valueType), n->_value);
		n->_value = NULL;
//		Can't delete because of the hackery we use to get the pointer in the first place
//		delete n->_valueType;
		n->_valueType = NULL;
		n->_serializeValue = false;
	}
}
