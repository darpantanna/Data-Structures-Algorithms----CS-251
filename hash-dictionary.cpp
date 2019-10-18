
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  // Add your code here
   for(int i = 0; i < MaxBuckets; i++) {
      buckets[i] = NULL;
   }

}

int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += i*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  // Add your code here
  int h = hash(key);
  HashNode * e = buckets[h];
  while(e != NULL) {
    if(strcmp(e->key, key) == 0) {
      e->data = record;
      return false;
    }
    e = e->next;
  }
  e = new HashNode();
  e->key = strdup(key);
  e->data = record;
  e->next = buckets[h];
  buckets[h] = e;
  nElements++;
  return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
  int h = hash(key);
  HashNode * e = buckets[h];

  while(e != NULL) {
    if(strcmp(e->key, key) == 0) {
      return (DataType) e -> data;
    }
    e = e -> next;
  }
  return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
  // Add your code here
  int h = hash(key);
  HashNode * e = buckets[h];
  HashNode * prev = NULL;

  while(e != NULL) {
    if(strcmp(e->key, key) == 0) {
      nElements--;
      break;
    }
    prev = e;
    e = e->next;
  }

  if(e == NULL) {
    return false;
  }
  if(prev == NULL){
    buckets[h] = e->next;
  }
  else {
    prev->next = e->next;
  }
  //char * k = strdup(key);
  //free(e->key);
  delete e;

  return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}
