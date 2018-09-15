#ifndef _PROPERTYTREE_HXX_
#define _PROPERTYTREE_HXX_

#include <string>
using std::string;

typedef unsigned long UInt;
typedef unsigned long long UInt64;

class PropertyTree {
	public:
		virtual bool initialize() = 0;
		virtual void destroy() = 0;

		virtual bool hasData(string key) = 0;
		virtual void clearData(string key) = 0;

		//POD Setters
		virtual bool setBool(string key, bool data) = 0;
		virtual bool setString(string key, string data) = 0;
		virtual bool setInt(string key, int data) = 0;
		virtual bool setUInt(string key, UInt data) = 0;
		virtual bool setFloat(string key, float data) = 0;
		virtual bool setDouble(string key, double data) = 0;

		//POD Getters
		virtual bool getBool(string key, bool default_data) = 0;
		virtual string getString(string key, string default_string) = 0;
		virtual int getInt(string key, int default_data) = 0;
		virtual UInt getUInt(string key, UInt default_data) = 0;
		virtual float getFloat(string key, float default_data) = 0;
		virtual double getDouble(string key, double default_data) = 0;
};

#endif //_PROPERTYTREE_HXX_
