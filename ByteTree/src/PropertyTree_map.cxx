#include "PropertyTree_map.hxx"

bool PropertyTree_map::initialize() {
	return true;
}


void PropertyTree_map::destroy() {
	propmap.clear();
}


bool PropertyTree_map::hasData(string key) {
	return (propmap.find(key) != propmap.end());
}


void PropertyTree_map::clearData(string key) {
	propmap.erase(key);
}


//POD Setters
bool PropertyTree_map::setBool(string key, bool data) {
	return true;
}


bool PropertyTree_map::setString(string key, string data) {
	return true;
}


bool PropertyTree_map::setInt(string key, int data) {
	return true;
}


bool PropertyTree_map::setUInt(string key, UInt data) {
	propmap[key] = data;
	return true;
}


bool PropertyTree_map::setFloat(string key, float data) {
	return true;
}


bool PropertyTree_map::setDouble(string key, double data) {
	return true;
}


//POD Getters
bool PropertyTree_map::getBool(string key, bool default_data) {
	return default_data;
}


string PropertyTree_map::getString(string key, string default_data) {
	return default_data;
}


int PropertyTree_map::getInt(string key, int default_data) {
	return default_data;
}


UInt PropertyTree_map::getUInt(string key, UInt default_data) {
	if(propmap.count(key)) {
		return propmap[key];
	}

	return default_data;
}


float PropertyTree_map::getFloat(string key, float default_data) {
	return default_data;
}


double PropertyTree_map::getDouble(string key, double default_data) {
	return default_data;
}
