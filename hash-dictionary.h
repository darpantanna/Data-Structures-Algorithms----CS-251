
// Implementation of a dictionary using a hash table

#include "dictionary.h"

struct HashNode {
	KeyType key;
	DataType data;
	HashNode * next;
};

#define MaxBuckets 1029

class HashDictionary : public Dictionary {
	// Add any member variables you need
	int nElements;
	HashNode * buckets[MaxBuckets];
  
public:
  // Constructor
  HashDictionary();

  // Hashing function
  int hash(KeyType key);
  
  // Add a record to the dictionary. Returns false if key already exists
  bool addRecord( KeyType key, DataType record);
  
  // Find a key in the dictionary and return corresponding record or NULL
  DataType findRecord( KeyType key);

  // Removes one element from the table
  bool removeElement(KeyType key);

  // Returns all the elements in the table as an array of strings.
  // *n is the size of the table and it is returned by reference
  KeyType * keys(int * n);

  // Add other methods you may need
};
