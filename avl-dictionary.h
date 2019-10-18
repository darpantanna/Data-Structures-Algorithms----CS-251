
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees

#include "dictionary.h"

struct AVLNode {
	KeyType key;
	DataType data;
	AVLNode * parent;
	AVLNode * left;
	AVLNode * right;
	int height;
};

class AVLDictionary : public Dictionary {
  // Add any member variables you need
	AVLNode * root;
	int nElements;
public:
  // Constructor
  AVLDictionary();

  // Restrcture tree starting at "node"
  void restructure(AVLNode * node);

  // Add a record to the dictionary. Returns false if key already exists
  bool addRecord( KeyType key, DataType record);

  // Find a key in the dictionary and return corresponding record or NULL
  DataType findRecord( KeyType key);

  // Removes one element from the table
	//AVLNode * getNode(KeyType key);
	bool removeElement(KeyType key);

  // Returns all the elements in the table as an array of strings.
  // *n is the size of the table and it is returned by reference
  KeyType * keys(int * n);

  void addKeysRecursive(AVLNode * node, KeyType * a, int * n);

  void check();

  void checkRecursive(AVLNode * node);

  // Print a node and children
  void printNode(const char * s, AVLNode * node, int depth);

  void print();

};
