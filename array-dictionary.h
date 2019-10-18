

#if !defined ARRAY_DICTIONARY_H
#define ARRAY_DICTIONARY_H

// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include "dictionary.h"

struct ArrayDictionaryNode {
	char * key;
	void * data;
};

class ArrayDictionary : public Dictionary {

 protected:
	ArrayDictionaryNode *array;
	int maxNumber;
	int currentNumber;
public:
  // Constructor
  ArrayDictionary();
  
  // Add a record to the dictionary. Returns false if key already exists
  bool addRecord( KeyType key, DataType record);
  
  // Find a key in the dictionary and return corresponding record or NULL
  // Use sequential search
  DataType findRecord( KeyType key);

  // Removes one element from the table
  bool removeElement(KeyType key);

  // Returns all the elements in the table as an array of strings.
  // *n is the size of the table and it is returned by reference
  KeyType * keys(int * n);

  // Add other methods you may need
};

#endif

