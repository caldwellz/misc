#include <stdlib.h>
//#include <stdarg.h>
#include <string.h>
#include "bytetree.h"

//Maybe someday add entity tag for root node, so we don't have to distinguish entities by a number string
//TODO: Will probably need to redo all string handling; according to the internet our sizeof kludge won't work

#define SETSTATUS(x) if (status != NULL) (*status) = (x)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tree_node {
	void* data;
	DataType data_type;

// Don't know if this will be needed yet	
//	tree_node* parent;
	struct tree_node** children;
} tree_node;


static tree_node* root = NULL;
static UInt char_space = 0;
static UInt key_max = 0;
static UInt string_max = 0;

// Forward declarations of internal functions
tree_node* createNode(TreeStatus* status);
void destroyNode(tree_node* node);


bool initializeTree(UInt data_char_space, UInt max_key_length, UInt max_string_size, TreeStatus* status) {
	if(root != NULL) {
		SETSTATUS(ERR_ALREADY_INITIALIZED);
		return false;
	}

	char_space = data_char_space;
	key_max = max_key_length;
	string_max = max_string_size;
	root = createNode(status);
	if(root == NULL) return false;

	SETSTATUS(STAT_SUCCESS);
	return true;
}


tree_node* createNode(TreeStatus* status) {
	tree_node* new_node = calloc(1, sizeof(struct tree_node));

	if(new_node == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return NULL;
	} else {
		new_node->children = calloc(char_space, sizeof(struct tree_node*));
		if(new_node->children == NULL) {
			SETSTATUS(ERR_MEMALLOC_FAILED);
			free(new_node);
			return NULL;
		}
	}

	return new_node;
}


void destroyNode(tree_node* node) {
	UInt i;

	if(node != NULL) {
		//Recursively destroy children
		for(i = 0; i < char_space; ++i) {
			destroyNode(node->children[i]);
		}

		//The free call does a NULL check for us. And yes leaving it as a void ptr is fine.
		free(node->children);
		free(node->data);
		free(node);
		//Hopefully this will propagate upwards / work as expected. Test!
		//Probably don't need it though
		//node = NULL;
	}
}


void destroyTree() {
/*
	UInt i;
	for(i = 0; i < char_space; ++i) {
		destroyNode(root->children[i]);
	}
	free(root);
*/
	destroyNode(root);

	root = NULL;
	char_space = 0;
	key_max = 0;
	string_max = 0;
}


DataType getType(const Byte* key, UInt key_length, TreeStatus* status) {
	UInt i;
	tree_node* node = root;

	if((key != NULL) && (node != NULL)) {
		///TODO: Make sure this sizeof test works as expected
		if(key_length > key_max) {
			SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
			return DT_INVALID;
		}

		//Walk the tree
		for(i = 0; i < key_length; ++i) {
			if(key[i] == '\0') break;
			if(key[i] >= char_space) {
				SETSTATUS(ERR_INVALID_CHAR_SPACE);
				return DT_INVALID;
			}
			if(node->children[key[i]] == NULL) {
				SETSTATUS(ERR_KEY_NOT_FOUND);
				return DT_INVALID;
			}
			node = node->children[key[i]];
		}

		SETSTATUS(STAT_SUCCESS);
		return node->data_type;
	}

	return DT_INVALID;
}


bool hasData(const Byte* key, UInt key_length, TreeStatus* status) {
	if(getType(key, key_length, status) == DT_INVALID)
		return false;
	return true;
}


void clearData(const Byte* key, UInt key_length, TreeStatus* status) {
	UInt i;
	tree_node* node = root;

	if((key != NULL) && (node != NULL)) {
		///TODO: Make sure this sizeof test works as expected
		if(key_length > key_max) {
			SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
			return;
		}

		//Walk the tree
		for(i = 0; i < key_length; ++i) {
			if(key[i] == '\0') break;
			if(key[i] >= char_space) {
				SETSTATUS(ERR_INVALID_CHAR_SPACE);
				return;
			}
			if(node->children[key[i]] == NULL)
				return;
			node = node->children[key[i]];
		}

		free(node->data);
		node->data_type = DT_INVALID;
		SETSTATUS(STAT_SUCCESS);
	}
}


bool setData(const Byte* key, UInt key_length, void* data, DataType data_type, TreeStatus* status) {
	UInt i;
	tree_node* node = root;

	if(key != NULL) {
		///TODO: Make sure this and other sizeof tests work as expected
		if(key_length > key_max) {
			SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
			return false;
		}

		//Walk the tree
		///BUG: Currently, if the tree-walk bails out, any successfully created intermediate nodes are left behind.
		//It's not a HUGE deal since the final tree destruction will find them, and since they may also get used again, but still...
		for(i = 0; i < key_length; ++i) {
			if(key[i] == '\0') break;
			if(key[i] >= char_space) {
				SETSTATUS(ERR_INVALID_CHAR_SPACE);
				return false;
			}
			if(node->children[key[i]] == NULL)
				node->children[key[i]] = createNode(status);
			if(node->children[key[i]] == NULL)
				return false;
			node = node->children[key[i]];
		}
	}

	free(node->data);
	node->data = data;
	node->data_type = data_type;

	SETSTATUS(STAT_SUCCESS);
	return true;
}


DataType getData(const Byte* key, UInt key_length, void** data, TreeStatus* status) {
	UInt i;
	tree_node* node = root;

	if((key != NULL) && (node != NULL)) {
		if(key_length > key_max) {
			SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
			return DT_INVALID;
		}

		//Walk the tree
		for(i = 0; i < key_length; ++i) {
			if(key[i] == '\0') break;
			if(key[i] >= char_space) {
				SETSTATUS(ERR_INVALID_CHAR_SPACE);
				return DT_INVALID;
			}
			if(node->children[key[i]] == NULL) {
				SETSTATUS(ERR_KEY_NOT_FOUND);
				return DT_INVALID;
			}
			node = node->children[key[i]];
		}

		if(node->data == NULL) {
			SETSTATUS(ERR_KEY_HAS_NO_DATA);
		} else {
			SETSTATUS(STAT_SUCCESS);
		}

		//TODO: Make sure this assignment-to-outside-void works as expected
		(*data) = node->data;
		return node->data_type;
	}

	return DT_INVALID;
}


bool setBool(const Byte* key, UInt key_length, bool data, TreeStatus* status) {
	bool setter_status;
	bool* data_store;

	if(key_length > key_max) {
		SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
		return false;
	}

	data_store = malloc(sizeof(data));
	if(data_store == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return false;
	}

	(*data_store) = data;
	setter_status = setData(key, key_length, (void*) data_store, DT_BOOL, status);
	if(!setter_status) {
		free(data_store);
		return false;
	}

	return true;
}


bool setString(const Byte* key, UInt key_length, const char* data, UInt data_length, TreeStatus* status) {
	bool setter_status;
	char* data_store;

	if(key_length > key_max) {
		SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
		return false;
	}
	if(data_length > string_max) {
		SETSTATUS(ERR_MAX_STRING_LENGTH_EXCEEDED);
		return false;
	}

	data_store = malloc(data_length);
	if(data_store == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return false;
	}
	strncpy(data_store, data, data_length);
	data_store[data_length - 1] = '\0';

	setter_status = setData(key, key_length, (void*) data_store, DT_STRING, status);
	if(!setter_status) {
		free(data_store);
		return false;
	}

	return true;
}


bool setInt(const Byte* key, UInt key_length, int data, TreeStatus* status) {
	bool setter_status;
	int* data_store;

	if(key_length > key_max) {
		SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
		return false;
	}

	data_store = malloc(sizeof(data));
	if(data_store == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return false;
	}

	(*data_store) = data;
	setter_status = setData(key, key_length, (void*) data_store, DT_INT, status);
	if(!setter_status) {
		free(data_store);
		return false;
	}

	return true;
}


bool setUInt(const Byte* key, UInt key_length, UInt data, TreeStatus* status) {
	bool setter_status;
	UInt* data_store;

	if(key_length > key_max) {
		SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
		return false;
	}

	data_store = malloc(sizeof(data));
	if(data_store == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return false;
	}

	(*data_store) = data;
	setter_status = setData(key, key_length, (void*) data_store, DT_UINT, status);
	if(!setter_status) {
		free(data_store);
		return false;
	}

	return true;
}


bool setFloat(const Byte* key, UInt key_length, float data, TreeStatus* status) {
	bool setter_status;
	float* data_store;

	if(key_length > key_max) {
		SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
		return false;
	}

	data_store = malloc(sizeof(data));
	if(data_store == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return false;
	}

	(*data_store) = data;
	setter_status = setData(key, key_length, (void*) data_store, DT_FLOAT, status);
	if(!setter_status) {
		free(data_store);
		return false;
	}

	return true;
}


bool setDouble(const Byte* key, UInt key_length, double data, TreeStatus* status) {
	bool setter_status;
	double* data_store;

	if(key_length > key_max) {
		SETSTATUS(ERR_MAX_KEY_LENGTH_EXCEEDED);
		return false;
	}

	data_store = malloc(sizeof(data));
	if(data_store == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return false;
	}

	(*data_store) = data;
	setter_status = setData(key, key_length, (void*) data_store, DT_DOUBLE, status);
	if(!setter_status) {
		free(data_store);
		return false;
	}

	return true;
}


bool getBool(const Byte* key, UInt key_length, bool default_data, TreeStatus* status) {
	bool* casted_data;
	void* data = NULL;
	DataType data_type;

	data_type = getData(key, key_length, &data, status);
	if(data == NULL)
		return default_data;
	if(data_type != DT_BOOL) {
		SETSTATUS(ERR_WRONG_TYPE);
		return default_data;
	}
	casted_data = (bool*) data;

	return (*casted_data);
}


UInt getString(const Byte* key, UInt key_length, char* target_string, TreeStatus* status) {
	void* data = NULL;
	DataType data_type;
	UInt str_length;

	if(target_string != NULL) {
		data_type = getData(key, key_length, &data, status);
		if(data == NULL)
			return 0;
		if(data_type != DT_STRING) {
			SETSTATUS(ERR_WRONG_TYPE);
			return 0;
		}

		//Setter function automatically adds null-terminator
		str_length = strlen((const char*) data);
		//BUG: Blindly trusts that the destination is big enough, because it ain't our problem...
		strncpy(target_string, (char*) data, str_length + 1); //+1 to get the null-terminator too
		//data_copy[str_length] = '\0';
		return str_length;
	}

	return 0;
}


char* getStringCopy(const Byte* key, UInt key_length, const char* default_data, TreeStatus* status) {
	void* data = NULL;
	char* data_copy = NULL;
	DataType data_type;
	UInt str_length;

	data_type = getData(key, key_length, &data, status);
	if(data == NULL)
		return default_data;
	if(data_type != DT_STRING) {
		SETSTATUS(ERR_WRONG_TYPE);
		return default_data;
	}

	//Setter function automatically adds null-terminator
	str_length = strlen((const char*) data) + 1;
	data_copy = malloc(str_length);
	if(data_copy == NULL) {
		SETSTATUS(ERR_MEMALLOC_FAILED);
		return default_data;
	}
	strncpy(data_copy, (char*) data, str_length);
	//data_copy[str_length - 1] = '\0';

	return data_copy;
}


int getInt(const Byte* key, UInt key_length, int default_data, TreeStatus* status) {
	int* casted_data;
	void* data = NULL;
	DataType data_type;

	data_type = getData(key, key_length, &data, status);
	if(data == NULL)
		return default_data;
	if(data_type != DT_INT) {
		SETSTATUS(ERR_WRONG_TYPE);
		return default_data;
	}
	casted_data = (int*) data;

	return (*casted_data);
}


UInt getUInt(const Byte* key, UInt key_length, UInt default_data, TreeStatus* status) {
	UInt* casted_data;
	void* data = NULL;
	DataType data_type;

	data_type = getData(key, key_length, &data, status);
	if(data == NULL)
		return default_data;
	if(data_type != DT_UINT) {
		SETSTATUS(ERR_WRONG_TYPE);
		return default_data;
	}
	casted_data = (UInt*) data;

	return (*casted_data);
}


float getFloat(const Byte* key, UInt key_length, float default_data, TreeStatus* status) {
	float* casted_data;
	void* data = NULL;
	DataType data_type;

	data_type = getData(key, key_length, &data, status);
	if(data == NULL)
		return default_data;
	if(data_type != DT_FLOAT) {
		SETSTATUS(ERR_WRONG_TYPE);
		return default_data;
	}
	casted_data = (float*) data;

	return (*casted_data);
}


double getDouble(const Byte* key, UInt key_length, double default_data, TreeStatus* status) {
	double* casted_data;
	void* data = NULL;
	DataType data_type;

	data_type = getData(key, key_length, &data, status);
	if(data == NULL)
		return default_data;
	if(data_type != DT_DOUBLE) {
		SETSTATUS(ERR_WRONG_TYPE);
		return default_data;
	}
	casted_data = (double*) data;

	return (*casted_data);
}

#ifdef __cplusplus
}
#endif
