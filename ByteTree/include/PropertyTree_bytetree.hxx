#ifndef _PROPERTYTREE_BYTETREE_HXX_
#define _PROPERTYTREE_BYTETREE_HXX_

#include "PropertyTree.hxx"

class PropertyTree_bytetree: public PropertyTree {
	public:
		bool initialize();
		void destroy();

		bool hasData(string key);
		void clearData(string key);

		//POD Setters
		bool setBool(string key, bool data);
		bool setString(string key, string data);
		bool setInt(string key, int data);
		bool setUInt(string key, UInt data);
		bool setFloat(string key, float data);
		bool setDouble(string key, double data);

		//POD Getters
		bool getBool(string key, bool default_data);
		string getString(string key, string default_data);
		int getInt(string key, int default_data);
		UInt getUInt(string key, UInt default_data);
		float getFloat(string key, float default_data);
		double getDouble(string key, double default_data);
};

#endif //_PROPERTYTREE_BYTETREE_HXX_
