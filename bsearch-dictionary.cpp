
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
	//maxNumber = 250;
	//currentNumber = 0;
}


bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}

	// Use binary search
	int low = 0;
	int high = currentNumber - 1;
	while(high >= low) {
		int mid = (high + low)/2;
		int result = strcmp(key, array[mid].key);
		if(result > 0) {
			low = mid+1;
		}
		else if(result < 0) {
			high = mid - 1;
		}
		else {
			return (DataType) array[mid].data;
		}
		
	}



	
	// Add your implementation here

	return NULL;
}

// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	printf("Before\n");
	for (int i = 0; i < currentNumber; i++) {
		printf("%s\n", array[i].key);
	}
	printf("Carrot\n");
        // Add your code here
				HeapInt heap(maxNumber);
				for(int j = 0; j < currentNumber; j++) {
					heap.insert(array[j].key, array[j].data);

				}
	printf("Potato\n");
				for(int k = 0; k < currentNumber; k++) {
					ArrayDictionaryNode * node = heap.removeMin(); 
					array[k].key = node->key;
					array[k].data = node->data;
				}

	printf("After\n");
	for (int l = 0; l < currentNumber; l++) {
		printf("%s\n", array[l].key);
	}
}


HeapInt::HeapInt(int maxNumber) {
	currentNumber = 0;
	nMax = maxNumber;
	array = new ArrayDictionaryNode [maxNumber];
}

void
HeapInt::insert(KeyType key, DataType data) {
	//assert(currentNumber < nMax);
	array[currentNumber].key = (char*) key;
	array[currentNumber].data = data;
	currentNumber++;
	//printf("INSERTED\n");
	int child = currentNumber - 1;
	int parent = parent(child);

	while(child > 0) {
		if(strcmp((char*) array[child].key, (char *) array[parent].key) > 0) {
			break;
		}

		void * temp = array[child].data;
		array[child].data = array[parent].data;
		array[parent].data = temp;

		char * temp1 = array[child].key;
		array[child].key = array[parent].key;
		array[parent].key = temp1;

		child = parent;
		parent = parent(child);

	}
}

ArrayDictionaryNode *
HeapInt :: removeMin() {
	//assert(currentNumber > 0);
	char * minKey = array[0].key;
	void * minData = array[0].data;
	currentNumber--;
	if(currentNumber == 0) {
	
		ArrayDictionaryNode * node = new ArrayDictionaryNode();
		node->key =  minKey;
		node->data = minData;
		return node;
	}

	array[0].key = array[currentNumber].key;
	array[0].data = array[currentNumber].data;
	//array[0].data = array[currentNumber].data;
	int parent = 0;
	int left = leftChild(parent);
	int right = rightChild(parent);

	while(left < currentNumber) {
		int minChild = left;

		if(right < currentNumber && strcmp((char*) array[right].key, (char *) array[left].key) < 0) {
			minChild = right;
		}
		if(strcmp((char *) array[parent].key, (char *)array[minChild].key) < 0) {
			break;
		}
	 	void * temp = array[minChild].data;
		array[minChild].data = array[parent].data;
		array[parent].data = temp;

		char *  temp1 = array[minChild].key;
		array[minChild].key = array[parent].key;
		array[parent].key = temp1;

		parent = minChild;
		left = leftChild(parent);
		right = rightChild(parent);
	}
	ArrayDictionaryNode * node = new ArrayDictionaryNode();
	node->key =  minKey;
	node->data = minData;
	return node;
}
