
#ifndef _BYTETREE_H_
#define _BYTETREE_H_

#ifdef __cplusplus
extern "C" {
#endif

//Requires C99
#include <stdbool.h>

typedef unsigned long UInt;
typedef char Byte;
static const UInt default_data_char_space = 126; //Enough to index the whole basic ASCII table except DEL
static const UInt default_max_key_length = 1000;
static const UInt default_max_string_size = 10000;

typedef enum TreeStatus {
	ERR_UNKNOWN,
	STAT_SUCCESS,
	ERR_ALREADY_INITIALIZED,
	ERR_WRONG_TYPE,		//Eventually ERR_CANT_CONVERT once we support type conversions
	ERR_KEY_HAS_NO_DATA,
	ERR_INVALID_CHAR_SPACE,
	ERR_KEY_NOT_FOUND,
	ERR_MAX_KEY_LENGTH_EXCEEDED,
	ERR_MAX_STRING_LENGTH_EXCEEDED,
	ERR_MEMALLOC_FAILED
} TreeStatus;

typedef enum DataType {
	DT_INVALID,
	DT_BOOL,
	DT_STRING,
	DT_INT,
	DT_UINT,
	DT_FLOAT,
	DT_DOUBLE
} DataType;


bool initializeTree(UInt data_char_space, UInt max_key_length, UInt max_string_size, TreeStatus* status);
//bool initializeFlatDB();
//Implement both and benchmark
void destroyTree();

//Utility
DataType getType(const Byte* key, UInt key_length, TreeStatus* status);
bool hasData(const Byte* key, UInt key_length, TreeStatus* status);
void clearData(const Byte* key, UInt key_length, TreeStatus* status);

//POD Setters
bool setBool(const Byte* key, UInt key_length, bool data, TreeStatus* status);
bool setString(const Byte* key, UInt key_length, const char* data, UInt data_length, TreeStatus* status);
bool setInt(const Byte* key, UInt key_length, int data, TreeStatus* status);
bool setUInt(const Byte* key, UInt key_length, UInt data, TreeStatus* status);
bool setFloat(const Byte* key, UInt key_length, float data, TreeStatus* status);
bool setDouble(const Byte* key, UInt key_length, double data, TreeStatus* status);

//POD Getters
bool   getBool(const Byte* key, UInt key_length, bool default_data, TreeStatus* status);
UInt   getString(const Byte* key, UInt key_length, char* target_string, TreeStatus* status);
char*  getStringCopy(const Byte* key, UInt key_length, const char* default_data, TreeStatus* status);
int    getInt(const Byte* key, UInt key_length, int default_data, TreeStatus* status);
UInt   getUInt(const Byte* key, UInt key_length, UInt default_data, TreeStatus* status);
float  getFloat(const Byte* key, UInt key_length, float default_data, TreeStatus* status);
double getDouble(const Byte* key, UInt key_length, double default_data, TreeStatus* status);
//DataType getData(const Byte* key, UInt key_length, void* data, TreeStatus* status);

#ifdef __cplusplus
}
#endif

#endif //_BYTETREE_H_
