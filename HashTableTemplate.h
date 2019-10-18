
//
// CS251 Data Structures
// Hash Table
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Each hash entry stores a key, object pair
template <typename Data>
struct HashTableTemplateEntry {
  const char * _key;
  Data _data;
  HashTableTemplateEntry * _next;
};

// This is a Hash table that maps string keys to objects of type Data
template <typename Data>
class HashTableTemplate {
 public:
  // Number of buckets
  enum { TableSize = 2039};
  
  // Array of the hash buckets.
  HashTableTemplateEntry<Data> **_buckets;
  
  // Obtain the hash code of a key
  int hash(const char * key);
  
 public:
  HashTableTemplate();
  ~HashTableTemplate();  
  // Add a record to the hash table. Returns true if key already exists.
  // Substitute content if key already exists.
  bool insertItem( const char * key, Data data);

  // Find a key in the dictionary and place in "data" the corresponding record
  // Returns false if key is does not exist
  bool find( const char * key, Data * data);

  // Removes an element in the hash table. Return false if key does not exist.
  bool removeElement(const char * key);

  // Returns the data that corresponds to this index.
  // Data is initialized to 0s if the entry does not exist
  Data operator[] (const char * &key);
};

template <typename Data>
int HashTableTemplate<Data>::hash(const char * key)
{
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

template <typename Data>
HashTableTemplate<Data>::HashTableTemplate()
{
  // Add implementation here
	_buckets = new HashTableTemplateEntry<Data> * [TableSize]();
}

template <typename Data>
bool HashTableTemplate<Data>::insertItem( const char * key, Data data)
{
  // Add implementation here
	int h = hash(key);
	HashTableTemplateEntry<Data> * e = _buckets[h];
	
	while(e != NULL) {
		if(strcmp(key, e->_key) == 0) {
			e->_data = data;
			return true;
		}
		e = e->_next;
	}
		HashTableTemplateEntry<Data> * temp = new HashTableTemplateEntry<Data>;
		temp->_key = strdup(key);
		temp->_data = data;
		temp->_next = _buckets[h];
		_buckets[h] = temp;
		return false;
}

template <typename Data>
bool HashTableTemplate<Data>::find( const char * key, Data * data)
{
  // Add implementation here
	int h = hash(key);
	HashTableTemplateEntry<Data> * e = _buckets[h]; 
	
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

template <typename Data>
Data HashTableTemplate<Data>::operator[] (const char * &key) {
  Data d;
  int h = hash(key);
  HashTableTemplateEntry<Data> * e = _buckets[h];
  while(e) {
  	if(strcmp(key, e->_key) == 0) {
		d = e->_data;
		return d;
	}
	e = e->_next;
  }
  	_buckets[h]->_data = d;
	return d;
}

template <typename Data>
bool HashTableTemplate<Data>::removeElement(const char * key)
{
  // Add implementation here
    int h = hash(key);
	HashTableTemplateEntry<Data> * e = _buckets[h]; 
	HashTableTemplateEntry<Data> * temp = NULL; 

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
		_buckets[h] = NULL;
		free((char*)(e->_key));
		delete e;
		return true;
	}
	while(e->_next != NULL) {
		if(strcmp(key, e->_next->_key) == 0) {
			temp = e->_next->_next;
			free((char*)(e->_next->_key));
			delete e->_next;
			e->_next = temp;
			return true;
		}
		e = e->_next;
	}
	return false;
}

template <typename Data>
HashTableTemplate<Data>::~HashTableTemplate() {
	for(int i = 0; i < TableSize; i++) {
		if(_buckets[i] != NULL) {
			HashTableTemplateEntry<Data> * e = _buckets[i];
			HashTableTemplateEntry<Data> * temp = NULL;
			while(e != NULL) {
				temp = e;
				free((char*)(e->_key));
				delete e;
				e = temp->_next;
			}
		}
	}
		delete [] _buckets;
}


template <typename Data>
class HashTableTemplateIterator {
  int _currentBucket;
  HashTableTemplateEntry<Data> *_currentEntry;
  HashTableTemplate<Data> * _hashTable;
 public:
  HashTableTemplateIterator(HashTableTemplate<Data> * hashTable);
  bool next(const char * & key, Data & data);
};

template <typename Data>
HashTableTemplateIterator<Data>::HashTableTemplateIterator(HashTableTemplate<Data> * hashTable)
{
  // Add implementation here
	_currentEntry = NULL;
	_currentBucket = 0;
	_hashTable = hashTable;

}

template <typename Data>
bool HashTableTemplateIterator<Data>::next(const char * & key, Data & data)
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
