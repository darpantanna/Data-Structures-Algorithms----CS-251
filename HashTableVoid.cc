
// CS251
// Implementation of a HashTable that stores void *
//
#include "HashTableVoid.h"
#include <string.h>

// Obtain the hash code of a key
int HashTableVoid::hash(const char * key)
{
  // Add implementation here
 	int length = strlen(key);
	int h = 0;
	for(int i = 0; i < length; i++) {
		h += (i+1)*key[i];
	}
	h = h % TableSize;
	return h;
}

// Constructor for hash table. Initializes hash table
HashTableVoid::HashTableVoid()
{
  // Add implementation here
    _buckets = new HashTableVoidEntry * [TableSize]();
}

// Find a key in the dictionary and place in "data" the corresponding record
// Returns false if key is does not exist
bool HashTableVoid::find( const char * key, void ** data)
{
  // Add implementation here
 	int h = hash(key);
	HashTableVoidEntry * e = _buckets[h];

	if(e == NULL || e->_key == NULL) {
		return false;
	}

	while(e != NULL) {
		if(strcmp(key, e->_key) == 0) {
			* data = e->_data;
			return true;
		}
		e = e->_next;
	}
	return false;
}

// Add a record to the hash table. Returns true if key already exists.
// Substitute content if key already exists.
bool HashTableVoid::insertItem( const char * key, void * data)
{
  // Add implementation here
  	int h = hash(key);
	HashTableVoidEntry * e = _buckets[h];

	while(e != NULL) {
		if(strcmp(key, e->_key) == 0) {
			e->_data = data;
			return true;
		}
		e = e->_next;
	}
		e = new HashTableVoidEntry();
		e->_key = strdup(key);
		e->_data = data;
		e->_next = _buckets[h];
		_buckets[h] = e;
		return false;
}

// Removes an element in the hash table. Return false if key does not exist.
bool HashTableVoid::removeElement(const char * key)
{
  // Add implementation here
  	int h = hash(key);
	HashTableVoidEntry * e = _buckets[h];
	HashTableVoidEntry * temp = NULL;

	if(e == NULL) {
		return false;
	}
	if(strcmp(key, e->_key) == 0 && e->_next != NULL) {
		_buckets[h] = e->_next;
		free((char*)(e->_key));
		delete e;
		return true;
	}
	if(strcmp(key, e->_key) == 0 && e->_next == NULL) {
		free((char*)(e->_key));
		delete e;
		_buckets[h] = NULL;
		return true;
	}
	while(e->_next != NULL) {
		if(strcmp(key, e->_next->_key) == 0) {
			temp = e->_next->_next;
			free ((char*)(e->_next->_key));
			delete e->_next;
			e->_next = temp;
			return true;
		}
		e = e->_next;
	}
	return false;
}

//HashTable Destructor

HashTableVoid::~HashTableVoid() {
	for(int i = 0; i < TableSize; i++) {
		if(_buckets[i] != NULL) {
			HashTableVoidEntry * e = _buckets[i];
			HashTableVoidEntry * temp = NULL;
			while(e!=NULL) {
				temp = e->_next;
				free((char*)(e->_key));
				delete e;
				e = temp;
			}
		}
	}
		delete [] _buckets;
}
// Creates an iterator object for this hash table
HashTableVoidIterator::HashTableVoidIterator(HashTableVoid * hashTable)
{
  // Add implementation here
	_currentEntry = NULL;
	_currentBucket = 0;
	_hashTable = hashTable;
}

// Returns true if there is a next element. Stores data value in data.
bool HashTableVoidIterator::next(const char * & key, void * & data)
{
  // Add implementation here
 	if(_currentEntry != NULL && _currentEntry ->_next != NULL) {
		key = _currentEntry -> _next -> _key;
		data = _currentEntry -> _next -> _data;
		_currentEntry = _currentEntry -> _next;
		return true;
	}
	int i;
	for(i = _currentBucket+1; i < 2039 && _hashTable -> _buckets[i] == NULL; i++){
		;
	}

	if(i < 2039 && _hashTable -> _buckets[i] != NULL) {
		key = _hashTable -> _buckets[i] -> _key;
		data = _hashTable -> _buckets[i] -> _data;
		_currentBucket = i;
		_currentEntry = _hashTable -> _buckets[i];
		return true;

	}
	return false;
}
