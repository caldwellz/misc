
#include <iostream>
#include "PropertyTree_bytetree.hxx"
#include "bytetree.h"
using std::cout;
using std::endl;

static TreeStatus status;

bool printStatus(string key) {
	if((status != STAT_SUCCESS) && (status != ERR_KEY_NOT_FOUND) && (status != ERR_KEY_HAS_NO_DATA)) {
		cout << "[PropertyTree_bytetree] Key: '" << key << "', Status: ";
		switch(status) {
			case ERR_UNKNOWN:
				cout << "ERR_UNKNOWN" << endl;
				break;
			case STAT_SUCCESS:
				cout << "STAT_SUCCESS" << endl;
				break;
			case ERR_ALREADY_INITIALIZED:
				cout << "ERR_ALREADY_INITIALIZED" << endl;
				break;
			case ERR_WRONG_TYPE:
				cout << "ERR_WRONG_TYPE" << endl;
				break;
			case ERR_KEY_HAS_NO_DATA:
				cout << "ERR_KEY_HAS_NO_DATA" << endl;
				break;
			case ERR_INVALID_CHAR_SPACE:
				cout << "ERR_INVALID_CHAR_SPACE" << endl;
				break;
			case ERR_KEY_NOT_FOUND:
				//cout << "ERR_KEY_NOT_FOUND" << endl;
				break;
			case ERR_MAX_KEY_LENGTH_EXCEEDED:
				cout << "ERR_MAX_KEY_LENGTH_EXCEEDED" << endl;
				break;
			case ERR_MAX_STRING_LENGTH_EXCEEDED:
				cout << "ERR_MAX_STRING_LENGTH_EXCEEDED" << endl;
				break;
			case ERR_MEMALLOC_FAILED:
				cout << "ERR_MEMALLOC_FAILED" << endl;
				break;
			default:
				cout << "Unhandled status!" << endl;
		}
	}

	return true;
}


bool PropertyTree_bytetree::initialize() {
	return ::initializeTree(default_data_char_space, default_max_key_length, default_max_string_size, &status) && printStatus("[initialize]");
}


void PropertyTree_bytetree::destroy() {
	::destroyTree();
}


bool PropertyTree_bytetree::hasData(string key) {
	return ::hasData(key.c_str(), key.length(), &status) && printStatus(key);
}


void PropertyTree_bytetree::clearData(string key) {
	::clearData(key.c_str(), key.length(), &status);
}


//POD Setters
bool PropertyTree_bytetree::setBool(string key, bool data) {
	return ::setBool(key.c_str(), key.length(), data, &status) && printStatus(key);
}


bool PropertyTree_bytetree::setString(string key, string data) {
	return ::setString(key.c_str(), key.length(), data.c_str(), data.length(), &status) && printStatus(key);
}


bool PropertyTree_bytetree::setInt(string key, int data) {
	return ::setInt(key.c_str(), key.length(), data, &status) && printStatus(key);
}


bool PropertyTree_bytetree::setUInt(string key, UInt data) {
	return ::setUInt(key.c_str(), key.length(), data, &status) && printStatus(key);
}


bool PropertyTree_bytetree::setFloat(string key, float data) {
	return ::setFloat(key.c_str(), key.length(), data, &status) && printStatus(key);
}


bool PropertyTree_bytetree::setDouble(string key, double data) {
	return ::setDouble(key.c_str(), key.length(), data, &status) && printStatus(key);
}


//POD Getters
bool PropertyTree_bytetree::getBool(string key, bool default_data) {
	bool temp = ::getBool(key.c_str(), key.length(), default_data, &status);
	printStatus(key);
	return temp;
}


string PropertyTree_bytetree::getString(string key, string default_data) {
	string temp(getStringCopy(key.c_str(), key.length(), default_data.c_str(), &status));
	printStatus(key);
	return temp;
}


int PropertyTree_bytetree::getInt(string key, int default_data) {
	int temp = ::getInt(key.c_str(), key.length(), default_data, &status);
	printStatus(key);
	return temp;
}


UInt PropertyTree_bytetree::getUInt(string key, UInt default_data) {
	UInt temp = ::getUInt(key.c_str(), key.length(), default_data, &status);
	printStatus(key);
	return temp;
}


float PropertyTree_bytetree::getFloat(string key, float default_data) {
	float temp = ::getFloat(key.c_str(), key.length(), default_data, &status);
	printStatus(key);
	return temp;
}


double PropertyTree_bytetree::getDouble(string key, double default_data) {
	double temp = ::getDouble(key.c_str(), key.length(), default_data, &status);
	printStatus(key);
	return temp;
}
