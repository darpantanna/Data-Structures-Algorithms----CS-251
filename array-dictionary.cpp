
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
	// Add your code here
 	maxNumber = 250;
  currentNumber = 0;
  array = new ArrayDictionaryNode[maxNumber];

}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
	// Add your code here
  if(currentNumber == maxNumber) {
    maxNumber = maxNumber * 2;
    ArrayDictionaryNode * newArray = new ArrayDictionaryNode[maxNumber];

    for(int i = 0; i < currentNumber; i++) {
        newArray[i].key = strdup(array[i].key);
        newArray[i].data = new DataType;
        newArray[i].data = (DataType *) array[i].data;
    }
    delete [] array;
    array = newArray;
  }

  //char * k = (char *) key;
  for(int i = 0; i < currentNumber; i++) {
      if(strcmp(array[i].key, key) == 0) {
        array[i].data = (DataType)record;
        return false;
      }
  }

  array[currentNumber].key = strdup(key);
  array[currentNumber].data = record;
  currentNumber++;
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
        // add your code here
  //char * k = (char *) key;
  for(int i = 0; i < currentNumber; i++) {
    if(strcmp(key, array[i].key) == 0) {
      return array[i].data;
    }
  }

	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
        // Add your code here
  char * k = (char *) key;
  int flag = 0;
  int count = 0;
  for(int i = 0; i < currentNumber; i++) {
    if(strcmp(array[i].key, key) == 0) {
      free(array[i].key);
      //delete array[i].data;
      count = i;
      flag = 1;
      break;
    }
  }

  if(flag == 0) {
    return false;
  }

  for(int i = count; i < currentNumber - 1; i++) {
    array[i].data = array[i+1].data;
    array[i].key = array[i+1].key;
  }
	return true;

}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
        // Add your code here

  *n = currentNumber;
  char ** t = new char* [currentNumber];
  for(int i = 0; i  < currentNumber; i++) {
     t[i] = strdup(array[i].key);
  }

  return (const char **) t;
}
