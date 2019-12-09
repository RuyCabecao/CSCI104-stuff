#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "../bst/bst.h"

/**
* A templated binary search tree implemented as a Splay tree.
*/
template <class Key, class Value>
class SplayTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods.
	SplayTree();
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);
	int report() const;

private:
	/* You'll need this for problem 5. Stores the total number of inserts where the
	   node was added at level strictly worse than 2*log n (n is the number of nodes
	   including the added node. The root is at level 0). */
	int badInserts;
	Node<Key, Value>* insertHelper(Node<Key, Value>* root, const std::pair<Key, Value>& keyValuePair, Node<Key, Value>* parent);
	Node<Key, Value>* rotations(Node<Key, Value>* root);
	Node<Key, Value>* leftrotate(Node<Key, Value>* node);
	Node<Key, Value>* rightrotate(Node<Key, Value>* node);
	Node<Key, Value>* search(const Key& key, Node<Key, Value>* curr);
	void printit(Node<Key, Value>* curr);

	/* Helper functions are encouraged. */
};

/*
--------------------------------------------
Begin implementations for the SplayTree class.
--------------------------------------------
*/

template<typename Key, typename Value>
SplayTree<Key, Value>::SplayTree() : badInserts(0) { }

template<typename Key, typename Value>
int SplayTree<Key, Value>::report() const {
	return badInserts;
}

/**
* Insert function for a key value pair. Finds location to insert the node and then splays it to the top.
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	// TODO
	this->mRoot = insertHelper(this->mRoot, keyValuePair, NULL);
	
	Node<Key, Value>* temp = NULL;

	//finds latest added node
	temp = search(keyValuePair.first, this->mRoot);
	std::cout <<temp->getKey() <<std::endl;

	//splay node to top
	rotations(temp);

	//printit(this->mRoot);


}

template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::insertHelper(Node<Key, Value>* root, const std::pair<Key, Value>& keyValuePair, Node<Key, Value>* parent)
{
	//checks wther node should go to left or right subtree
	if (!root) {
		Node<Key,Value>* nnode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, parent);
		return nnode;
	}
	//if key has already been inserted, update value
	if (keyValuePair.first == root->getKey()) {
		root->setValue(keyValuePair.second);
	}

	else if (keyValuePair.first > root->getKey()) {
		root->setRight(insertHelper(root->getRight(), keyValuePair, root)); 
	}

	else if (keyValuePair.first < root->getKey()) {
		root->setLeft(insertHelper(root->getLeft(), keyValuePair, root)); 
	}

	return root;
}

template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::rotations(Node<Key, Value>* root) {

	//stores nodes for easier reading
	Node<Key, Value>* x = root; //node to be splayed
	Node<Key, Value>* p = root->getParent(); //parent node
	Node<Key, Value>* g = NULL; //grandparent node

	//stop only when node has been splayed to top
	while (x!=this->mRoot) {
		p = x->getParent();
		if (p->getParent()) {
			g = p->getParent();
			if (g->getLeft() == p) {
				if (p->getLeft() == x) {
					std::cout << "x left of p" << std::endl;
					//left left (zig-zig)
					p = rightrotate(g);
					x = rightrotate(p);
				}
				else if (p->getRight() == x) {
					std::cout << "x right of p" << std::endl;
					//right left (zag-zig)
					x = leftrotate(p);
					p = rightrotate(g);
				}
			}

			else if (g->getRight() == p) {
				if (p->getLeft() == x) {
					std::cout << "x left of p" << std::endl;
					//right left (zig-zag)
					x = rightrotate(p);
					p = leftrotate(g);
					
				}
				else if (p->getRight() == x) {
					std::cout << "x right of p" << std::endl;
					//left left (zag-zag)
					p = leftrotate(g);
					x = leftrotate(p);
				}
			}
		}
		else if (!p->getParent()) {
			std::cout << "no g" <<std::endl;
			if (p->getLeft() == x)  {
				//simple right rotation when x is child of mRoot
				x = rightrotate(p);
			}
			else if (p->getRight() == x)  {
				//simple left rotation when x is child of mRoot
				x = leftrotate(p);
			}
		}
	}

	return x;

}


template<typename Key, typename Value>
Node<Key, Value>*  SplayTree<Key, Value>::leftrotate(Node<Key, Value>* a) {

	Node<Key,Value>* b = a->getRight();
	Node<Key,Value>* T2 = b->getLeft();
	

	//following ifs move pointers around to perform rotations
	//depending on what the parent of a is.

	if (a->getParent() == NULL) { //if a is mRoot
		b->setParent(NULL);
		this->mRoot = b;
		b->setLeft(a);
		a->setRight(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}

	else if (a->getParent()->getRight() == a) { //if a is right child
		b->setParent(a->getParent());
		a->getParent()->setRight(b);
		b->setLeft(a);
		a->setRight(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}

	else if (a->getParent()->getLeft() == a) { //if a is left child
		b->setParent(a->getParent());
		a->getParent()->setLeft(b);
		b->setLeft(a);
		a->setRight(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}
	
	return b;

}


template<typename Key, typename Value>
Node<Key, Value>*  SplayTree<Key, Value>::rightrotate(Node<Key, Value>* a) {

	Node<Key,Value>* b = a->getLeft();
	Node<Key,Value>* T2 = b->getRight();
	std::cout << a->getKey() << " and " << b->getKey() << std::endl;

	//following ifs move pointers around to perform rotations
	//depending on what the parent of a is.

	if (a->getParent() == NULL) { //if a is mRoot
		std::cout << "is root" << std::endl;
		b->setParent(NULL);
		this->mRoot = b;
		b->setRight(a);
		a->setLeft(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}

	else if (a->getParent()->getRight() == a) { //if a is right child
		std::cout << "is right" << std::endl;
		b->setParent(a->getParent());
		a->getParent()->setRight(b);
		b->setRight(a);
		a->setLeft(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}

	else if (a->getParent()->getLeft() == a) { //if a is left child
		b->setParent(a->getParent());
		a->getParent()->setLeft(b);
		b->setRight(a);
		a->setLeft(T2);
		if (T2)
			T2->setParent(a);
		a->setParent(b);
	}

	return b;

}

template<typename Key, typename Value>
Node<Key, Value>* SplayTree<Key, Value>::search(const Key& key, Node<Key, Value>* curr) {

	//finds node given its key and a starting position
	while (curr) {
		if (key > curr->getKey()) {
			curr = curr->getRight();
		}
		else if (key < curr->getKey()) {
			curr = curr->getLeft();
		}
		else if (key == curr->getKey()) return curr;
	}
	return NULL;

}

template<typename Key, typename Value>
void SplayTree<Key, Value>::printit(Node<Key, Value>* curr) {
	if (!curr) return;
	std::cout << curr->getKey() <<  " ";
	if (curr->getParent())
		 std::cout<< curr->getParent()->getKey();
	else std::cout << "0";
	std::cout << std::endl;
	printit(curr->getLeft());
	printit(curr->getRight());
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then splays the parent
* of the deleted node to the top.
*/
template<typename Key, typename Value>
void SplayTree<Key, Value>::remove(const Key& key)
{
	Node<Key, Value>* root = NULL;
	root = search(key, this->mRoot);
	BinarySearchTree<Key, Value>::remove(key);
	root = root->getParent();
	rotations(root);

/*	Node<Key, Value>* root = NULL;
	root = search(key, this->mRoot);

	if (!root->getRight() || !root->getLeft()) { //one or 0 child
		if (root->getRight()) { //if root has right child
			Node <Key, Value>* tnode = root->getRight();
			if (root->getParent()) { //if root has a parent, change pointers
				root->getRight()->setParent(root->getParent());
				if (root->getParent()->getLeft() == root) {
					root->getParent()->setLeft(root->getRight());
				}
				else if (root->getParent()->getRight() == root) {
					root->getParent()->setRight(root->getRight());
				}
			}	
			else {//if root doesn't have parent it is the  mRoot
				root->getRight()->setParent(root->getParent());
				this->mRoot = root->getRight();
			}

			delete root;
			//points deallocated root to its successor to preserve functionality
			root = tnode;
			std::cout << "OK" <<std::endl;
			rotations(tnode->getParent());
		}
		else if (root->getLeft()) {//if root has left child
			Node <Key, Value>* tnode = root->getLeft();
			if (root->getParent()) {//if root has a parent, change pointers
				root->getLeft()->setParent(root->getParent());
				if (root->getParent()->getLeft() == root) {//checks if root is right child
					root->getParent()->setLeft(root->getLeft());
				}
				else if (root->getParent()->getRight() == root) {//checks if root is left child
					root->getParent()->setRight(root->getLeft());
				}
			}
			else {//if root has no parent, it is mRoot
				root->getLeft()->setParent(root->getParent());
				this->mRoot = root->getLeft();
			}

			delete root;
			//points deallocated root to its successor to preserve functionality
			root = tnode;
			rotations(tnode->getParent());
		}
	}
	else if (!root->getRight() && !root->getLeft()) {//no children case
		if (root->getParent()) {//if root has a parent
			if (root->getParent()->getLeft() == root) {//if root is left
				root->getParent()->setLeft(NULL);
			}
			else if (root->getParent()->getRight() == root) { //if root is right
					root->getParent()->setRight(NULL);
			}
			else this->mRoot = NULL; //tree is empty, just set root to null
			delete root;
			root = NULL;
		}
	}

	*/
	printit(this->mRoot);
}

/*
------------------------------------------
End implementations for the SplayTree class.
------------------------------------------
*/

#endif
