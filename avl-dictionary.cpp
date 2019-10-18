
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printf("", root, 0);
	}

	// Add your implementation here

	//Find n to insert into
	//n does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .
	AVLNode * current = root;
	AVLNode * previous = NULL;

	while(current != NULL) {
		previous = current;
		if(strcmp(key,current -> key) == 0) {
			current -> data = record;
			return false;
		}
		else if(strcmp(key,current -> key) < 0) {
			current = current -> left;
		}
		else {
			current = current -> right;
		}
	}

	AVLNode * n = new AVLNode();
	n -> key = key;
	n -> data = record;
	n -> left = NULL;
	n -> right = NULL;
	n -> parent = NULL;
	n -> height = 1;

	if(previous == NULL) {
		root = n;
		nElements++;
		return true;
	}

	if(strcmp(key, previous -> key) < 0 ){
		previous -> left = n;
	}
	else {
		previous -> right = n;
	}
	n->parent = previous;

	AVLNode * m = n -> parent;

	while(m != NULL) {
		int maxHeight = 0;
		if(m -> left != NULL) {
			maxHeight = m->left->height;
		}
		if(m -> right != NULL && m->right->height > maxHeight) {
			maxHeight = m->right->height;
		}
		m->height = maxHeight + 1;
		m = m->parent;
	}

	if (debug) {
		printf("---------- Before Restructure -----------------\n");
		printf("", root, 0);
	}

	// Call restructure
	restructure(n);
	if (debug) {
		checkRecursive(root);

		printf("---------- After Restructure -----------------\n");
		printf("", root, 0);
	}
	nElements++;
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n

	//Go upwards until root is found

        // See class notes

	// Add your implementation here
	AVLNode *z;
	z = n->parent;

    while(z != NULL)	{
        int hleft = 0;
        if(z->left != NULL) {
            hleft = z->left->height;
        }

        int hright = 0;
        if(z->right != NULL) {
            hright = z->right->height;
        }

				if(hleft > hright) {
						z->height = hleft + 1;
				}

        else {
            z->height = hright + 1;
				}

        int hdiff = hleft - hright;
        if(hdiff < 0) {
						hdiff = - hdiff;
				}

        if(hdiff <= 1) {
            z = z->parent;
            continue;
        }

        AVLNode *y = NULL;
        int maxheight = 0;
        if(z->left != NULL)
				{
            y = z->left;
            maxheight = y->height;
        }
        if(z->right != NULL && z->right->height > maxheight) {
            y = z->right;
            //maxheight = y->height;
        }

        AVLNode *x = NULL;
        maxheight = 0;
        if(y->left != NULL) {
            x = y->left;
            maxheight = x->height;
        }
        if(y->right != NULL && y->right->height > maxheight) {
            x = y->right;
            //maxheight = x->height;
        }
        //assert(x != NULL);

        AVLNode * a, * b, * c, * t0, * t1, * t2, * t3;
        if(z->right == y) {
            if(y->right == x) {
                a = z;
                b = y;
                c = x;
                t0 = z->left;
                t1 = y->left;
                t2 = x->left;
                t3 = x->right;
            }
            else {
                a = z;
                b = x;
                c = y;
                t0 = z->left;
                t1 = x->left;
                t2 = y->left;
                t3 = y->right;
            }
        }
        else {
            if(y->left == x) {
                a = x;
                b = y;
                c = z;
                t0 = x->left;
                t1 = x->right;
                t2 = y->right;
                t3 = z->right;
            }
            else {
                a = y;
                b = x;
                c = z;
                t0 = y->left;
                t1 = x->left;
                t2 = x->right;
                t3 = z->right;
            }
        }

        AVLNode *p;
				p = z->parent;
        if(p != NULL) {
            if(p->left == z) {
							p->left = b;
						}
            else {
                p->right = b;
						}
        }

				else {
					root = b;
				}

        b->parent = p;
        b->left = a;
        b->right = c;
        a->parent = b;
        a->left = t0;
        a->right = t1;
        c->parent = b;
        c->left = t2;
        c->right = t3;

        if(t0 != NULL) {
            t0->parent = a;
        }
				if(t1 != NULL) {
            t1->parent = a;
				}
        if(t2 != NULL) {
            t2->parent = c;
				}
				if(t3 != NULL) {
            t3->parent = c;
				}

        maxheight = 0;
        if(a->left != NULL) {
            maxheight = a->left->height;
				}
        if(a->right != NULL && a->right->height > maxheight) {
            maxheight = a->right->height;
				}
				a->height = maxheight + 1;

        maxheight = 0;
        if(c->left != NULL) {
            maxheight = c->left->height;
				}
        if(c->right != NULL && c->right->height > maxheight) {
            maxheight = c->right->height;
				}
        c->height = maxheight + 1;

        maxheight = 0;
        if(b->left != NULL) {
            maxheight = b->left->height;
				}
        if(b->right != NULL && b->right->height > maxheight) {
            maxheight = b->right->height;
				}
        b->height = maxheight + 1;

        z = z->parent;
    }
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
        // Add your implementation here
	AVLNode * current = root;
	while(current != NULL) {
		if (strcmp(key, current->key) < 0) {
			current = current->left;
		}
		else if (strcmp(key, current->key) > 0) {
			current = current->right;
		}
		else{
			return (DataType)current->data;
		}
	}
	return NULL;
}

/*
AVLNode *
AVLDictionary::getNode(KeyType key)
{
	AVLNode *current = root;
	while(current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			return current;
		}
		else if (strcmp(current->key, key) > 0)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}

	return NULL;
}*/

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printf("", root, 0);
	}

/*
	// Add your implementation here
	AVLNode * n = getNode(key);
	n = getNode(key);
	if(n == NULL) {
		return false;
	}

	if (n->left == NULL && n->right == NULL)
	{
		if (n == n->parent->left)	{
			n->parent->left = NULL;
		}
		else	{
			n->parent->right = NULL;
		}

		AVLNode *m;
		m = n->parent;

		while(m != NULL)	{
			int maxheight = 0;
			if(m->left != NULL)	{
				maxheight = m->left->height;
			}
			if (m->right != NULL && maxheight < m->right->height)	{
				maxheight = m->right->height;
			}
			m->height = maxheight +1;
			m = m->parent;
		}

		restructure(n->parent);
		delete n;
    }

		else if (n->left == NULL)	{
	    AVLNode temp;

     	temp.height = n->height;
     	strcpy((char*)temp.key, n->key);
     	temp.data = n->data;

     	n->height = n->right->height;
     	strcpy((char*)n->key, n->right->key);
     	n->data = n->right->data;

     	n->right->height = temp.height;
     	strcpy((char*)n->right->key, temp.key);
     	n->right->data = temp.data;

     	delete n->right;
   		n->right = NULL;

     	AVLNode *m = n->parent;
     	while(m != NULL)	{
     		int maxheight = 0;
     		if(m->left != NULL)	{
     			maxheight = m->left->height;
     		}
     		if (m->right != NULL && maxheight < m->right->height)	{
     			maxheight = m->right->height;
     		}
     		m->height = maxheight +1;
     		m = m->parent;
     	}

     	restructure(n);
    }

		else if(n -> right == NULL) {
			AVLNode temp;
			temp.height = n -> height;

			strcpy((char*)temp.key, n->key);
     	temp.data = n->data;

     	n->height = n->left->height;
     	strcpy((char*)n->key, n->left->key);
     	n->data = n->left->data;

     	n->left->height = temp.height;
     	strcpy((char*)n->left->key, temp.key);
     	n->left->data = temp.data;

     	delete n->left;

     	n->left = NULL;

     	AVLNode *m;
			m = n->parent;

     	while(m != NULL)	{
     		int maxheight = 0;
     		if( m->left != NULL)	{
     			maxheight = m->left->height;
     		}
     		if (m->right != NULL && maxheight < m->right->height)	{
     			maxheight = m->right->height;
     		}
     		m->height = maxheight + 1;
     		m = m->parent;

     	}
     	restructure(n);
    }

		else {
			AVLNode * a = n -> left;
			if(a -> right == NULL) {
					a = n -> right;
					while(a -> left != NULL) {
						a = a->left;
					}
			}

			else {
				while(a -> right != NULL) {
					a = a->right;
				}
			}
		}

		AVLNode temp;
		temp.height = n->height;
    strcpy((char*)temp.key, n->key);
    temp.data = n->data;

    n->height = a->height;
    strcpy((char*)n->key, a->key);
    n->data = a->data;

    a->height = temp.height;
    strcpy((char*)a->key, temp.key);
    a->data = temp.data;

		AVLNode * e = a->parent;
		if (e != NULL)	{
			if (a == e->left)	{
				e->left = NULL;
				delete a;
			}
			else
			{
				e->right = NULL;
				delete a;
			}

			AVLNode *m = e;
			while(m != NULL)
			{
				int maxheight = 0;
				if(m->left != NULL)
				{
					maxheight = m->left->height;
				}
				if (m->right != NULL && maxheight < m->right->height)
				{
					maxheight = m->right->height;
				}
				m->height = maxheight + 1;
				m = m->parent;
			}

			restructure(e);
		}


	nElements--;


*/
	AVLNode * current = root;
	while(current != NULL) {
		if (strcmp(key, current->key) < 0) {
			current = current->left;
		}
		else if (strcmp(key, current->key) > 0) {
			current = current->right;
		}
		else{
			return true;
		}
	}

	if (debug) {
		printf("---------- After -----------------\n");
		printf("", root, 0);

		checkRecursive(root);
	}

	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * node)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*node = 0;
	addKeysRecursive(root, a, node);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node == NULL) {
		return;
	}

	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}
