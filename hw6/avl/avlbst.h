#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "../bst/bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
	AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
	virtual ~AVLNode();

	// Getter/setter for the node's height.
	int getHeight() const;
	void setHeight(int height);

	// Getters for parent, left, and right. These need to be redefined since they 
	// return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
	// for more information.
	virtual AVLNode<Key, Value>* getParent() const override;
	virtual AVLNode<Key, Value>* getLeft() const override;
	virtual AVLNode<Key, Value>* getRight() const override;

protected:
	int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
	: Node<Key, Value>(key, value, parent)
	, mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
	mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);

private:
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
	int bettergetheight(AVLNode<Key, Value>* node);
	int calcbf(AVLNode<Key, Value>* node);
	AVLNode<Key, Value>* insertHelper(AVLNode<Key, Value>* root, const std::pair<Key, Value>& keyValuePair, AVLNode<Key, Value>* parent);
	void printheights(AVLNode<Key, Value>* root);
	AVLNode<Key, Value>* leftrotate(AVLNode<Key, Value>* node);
	AVLNode<Key, Value>* rightrotate(AVLNode<Key, Value>* node);
	AVLNode<Key, Value>* removeHelper(AVLNode<Key, Value>* root, const Key& key);
	AVLNode<Key, Value>* getMinChild(AVLNode<Key, Value>* parent);
	void swap(AVLNode<Key, Value>* onode, AVLNode<Key, Value>* succ);
	AVLNode<Key, Value>* rotations(AVLNode<Key, Value>* root);



};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{

	this->mRoot = insertHelper(static_cast<AVLNode<Key,Value>*>(this->mRoot), keyValuePair, NULL);

}

template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* root, const std::pair<Key, Value>& keyValuePair, AVLNode<Key, Value>* parent) {
	
	if (root == NULL) {
		//found empty child, insert new node
		AVLNode<Key,Value>* nnode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, parent);
		
		//start with node height+1 to make things easier
		nnode->setHeight(nnode->getHeight()+1);
		return nnode;		
	}
	
	//if node already exits, update value
	if (keyValuePair.first == root->getKey()) {
		root->setValue(keyValuePair.second);
	}

	//check if new node should be right 
	else if (keyValuePair.first > root->getKey()) {
		root->setRight(static_cast<AVLNode<Key,Value>*>(insertHelper(root->getRight(), keyValuePair, root))); 
	}

	//check if new node should be left 
	else if (keyValuePair.first < root->getKey()) {
		root->setLeft(static_cast<AVLNode<Key,Value>*>(insertHelper(root->getLeft(), keyValuePair, root))); 
	}

	//update heights depending on wether left subtree or right subtree is larger
	if (bettergetheight(root->getLeft()) > bettergetheight(root->getRight()))
		root->setHeight(1+bettergetheight(root->getLeft()));
	else
		root->setHeight(1+bettergetheight(root->getRight()));

	root = rotations(root);

	return root;
}


/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
	// TODO
	this->mRoot = removeHelper(static_cast<AVLNode<Key,Value>*>(this->mRoot), key);
	
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removeHelper(AVLNode<Key, Value>* root, const Key& key)
{
 	// TODO
	if (root == NULL) {
		return root;
	}

	if (key == root->getKey())  {
		if (root->getRight() && root->getLeft()) { //2 children 

			//finds successor
			AVLNode <Key, Value>* tnode = getMinChild(root->getRight());

			swap(root, tnode);//swaps node with successor

			//deletes node by calling remove on the now swapped node
			tnode->setRight(removeHelper(tnode->getRight(), tnode->getKey()));

			delete root;
			//points deallocated root to its successor to preserve functionality
			root = tnode;
		}

		else if (!root->getRight() || !root->getLeft()) { //one or 0 child
			if (root->getRight()) { //if root has right child
				AVLNode <Key, Value>* tnode = root->getRight();
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
			}
			else if (root->getLeft()) {//if root has left child
				AVLNode <Key, Value>* tnode = root->getLeft();
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
			}
			else if (!root->getRight() && !root->getLeft()) {//no children case
				if (root->getParent()) {//if root has a parent
					if (root->getParent()->getLeft() == root) {//if root is left
						root->getParent()->setLeft(NULL);
					}
					else if (root->getParent()->getRight() == root) { //if root is right
						root->getParent()->setRight(NULL);
					}
				}
				else this->mRoot = NULL; //tree is empty, just set root to null
				delete root;
				root = NULL;
			}
		}
	}


	else if (key < root->getKey()) { //if given key is to the left of root
		root->setLeft(removeHelper(root->getLeft(), key));
	}

	else if (key > root->getKey()) { //if given key is to the right of root
		root->setRight(removeHelper(root->getRight(), key));
	}


	if(!root) { //if root just got deleted, return
		return NULL;
	} 

	//updates heights
	if (bettergetheight(root->getLeft()) > bettergetheight(root->getRight()))
		root->setHeight(1+bettergetheight(root->getLeft()));
	else
		root->setHeight(1+bettergetheight(root->getRight()));

	//performs rotations
	root = rotations(root);

	return root;
}


template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotations(AVLNode<Key, Value>* root) {

	//right cases
	if (root->getRight()) { //checks for out of bounds
		//right right
		if (0 >= calcbf(root->getRight()) && calcbf(root) < -1) {
			return leftrotate(root);
		}
		//right left
		if (0 < calcbf(root->getRight()) && calcbf(root) < -1) {
			//right
			root->setRight(rightrotate(root->getRight()));
			//left
			return leftrotate(root);
		}
	}

	//left cases
	if (root->getLeft()) { //checks for out of bounds
		//left left
		if (0 <= calcbf(root->getLeft()) && calcbf(root) > 1) {
			return rightrotate(root);
		}
		//left right
		if (0 > calcbf(root->getLeft()) && calcbf(root) > 1) {
			//left
			root->setLeft(leftrotate(root->getLeft()));
			//right
			return rightrotate(root);
		}
	}
	return root;
}

template<typename Key, typename Value>
AVLNode<Key, Value>*  AVLTree<Key, Value>::leftrotate(AVLNode<Key, Value>* a) {

	AVLNode<Key,Value>* b = a->getRight();
	AVLNode<Key,Value>* T2 = b->getLeft();

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

	//update a height
	if (bettergetheight(a->getLeft()) > bettergetheight(a->getRight()))
		a->setHeight(1+bettergetheight(a->getLeft()));
	else
		a->setHeight(1+bettergetheight(a->getRight()));

	//update b height
	if (bettergetheight(b->getLeft()) > bettergetheight(b->getRight()))
		b->setHeight(1+bettergetheight(b->getLeft()));
	else
		b->setHeight(1+bettergetheight(b->getRight()));
	
	return b;

}


template<typename Key, typename Value>
AVLNode<Key, Value>*  AVLTree<Key, Value>::rightrotate(AVLNode<Key, Value>* a) {

	AVLNode<Key,Value>* b = a->getLeft();
	AVLNode<Key,Value>* T2 = b->getRight();

	//following ifs move pointers around to perform rotations
	//depending on what the parent of a is.

	if (a->getParent() == NULL) { //if a is mRoot
		b->setParent(NULL);
		this->mRoot = b;
		b->setRight(a);
		a->setLeft(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}

	else if (a->getParent()->getRight() == a) { //if a is right child
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

	//update a height
	if (bettergetheight(a->getLeft()) > bettergetheight(a->getRight()))
		a->setHeight(1+bettergetheight(a->getLeft()));
	else
		a->setHeight(1+bettergetheight(a->getRight()));

	//update b height
	if (bettergetheight(b->getLeft()) > bettergetheight(b->getRight()))
		b->setHeight(1+bettergetheight(b->getLeft()));
	else
		b->setHeight(1+bettergetheight(b->getRight()));

	return b;

}


//function for calculating balance factor
template<typename Key, typename Value>
int AVLTree<Key, Value>::calcbf(AVLNode<Key, Value>* node)
{
	//if node is null, bf is 0
	if (!node) return 0;
	//uses formula height of left tree - height of right tree
	return bettergetheight(node->getLeft()) - bettergetheight(node->getRight());
}


//function to get height without the problems of accessing out of bonds
//when using given getHeight() when a node is null
template<typename Key, typename Value>
int AVLTree<Key, Value>::bettergetheight(AVLNode<Key, Value>* node) {
	//if node is null, height is 0
	if (!node) return 0;
	//if not, just return height
	else return node->getHeight();
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::printheights(AVLNode<Key, Value>* root) {
	if (root) {
		if (root->getParent())
			std::cout << root->getKey() << " height " << root->getHeight() << " parent " << root->getParent()->getKey() << std::endl; 
		else std::cout << root->getKey() <<" height " << root->getHeight() << " parent " << 0 << std::endl; 
		printheights(root->getLeft());
		printheights(root->getRight());
	}
}


//function to traverse to leftmost node of a given node
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getMinChild(AVLNode<Key, Value>* parent)
{
	AVLNode<Key, Value>* min = parent;

	while(min && min->getLeft()) {
		//traverses until reaching NULL
		min = min->getLeft();
	}

	return min;

}


//function for swapping values when deleting
template<typename Key, typename Value>
void AVLTree<Key, Value>::swap(AVLNode<Key, Value>* onode, AVLNode<Key, Value>* succ) {

	if (onode == succ) {
		return;//edge case if nodes are the same
	}

	//defines some bools to make less cluttered lines
	bool oisroot = false; 
	bool oisleft = false;
	bool oisright = false;
	
	//if no parent, is mRoot
	if (onode->getParent() == NULL)
		oisroot = true;

	//if it has parents, check if it is right or left child
	if(!oisroot) {
		if (onode->getParent()->getRight() == onode) 
			oisright = true;
		else if (onode->getParent()->getLeft() == onode)
			oisleft = true;
	}

	//no successor case
	if (succ==NULL) {
		//sets right of parent to  NULL
		if (oisright) {
			onode->getParent()->setRight(NULL);
		}

		//sets left of parent to  NULL
		if (oisleft) {
			onode->getParent()->setLeft(NULL);
		}

		onode->setRight(NULL);
		onode->setLeft(NULL);
		onode->setParent(NULL);
		onode = NULL;
		return;
	}

	//removes succ's parent's pointers to it
	if (succ->getParent()->getRight() == succ)
		succ->getParent()->setRight(NULL);
	if (succ->getParent()->getLeft() == succ)
		succ->getParent()->setLeft(NULL);

	//sets succ's parent to onode's  parent
	succ->setParent(onode->getParent());
	succ->setLeft(onode->getLeft());
	succ->setRight(onode->getRight());
	
	//sets onode's children's parent to succ 
	if (onode->getLeft())
		onode->getLeft()->setParent(succ);
	if (onode->getRight())
		onode->getRight()->setParent(succ);
	if (oisright)
		onode->getParent()->setRight(succ);
	if (oisleft)
		onode->getParent()->setLeft(succ);
	if (oisroot)
		this->mRoot = succ;

	//swap heights
	succ->setHeight(onode->getHeight());

	onode = NULL;
}


/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
