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
	std::cout << "====================" << std::endl;
	printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
	std::cout << "====================" << std::endl;

}

template<typename Key, typename Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* root, const std::pair<Key, Value>& keyValuePair, AVLNode<Key, Value>* parent) {
	
	if (root == NULL) {
		AVLNode<Key,Value>* nnode = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, parent);
		nnode->setHeight(nnode->getHeight()+1);
		//std::cout << "inserting " << nnode->getKey() <<std::endl;
		return nnode;		
	}
	
	if (keyValuePair.first > root->getKey()) {
		//std::cout << " if1" << std::endl;

		root->setRight(static_cast<AVLNode<Key,Value>*>(insertHelper(root->getRight(), keyValuePair, root))); 
	}

	else if (keyValuePair.first < root->getKey()) {
		//std::cout << " if2" << std::endl;

		root->setLeft(static_cast<AVLNode<Key,Value>*>(insertHelper(root->getLeft(), keyValuePair, root))); 
	}

	else {
		root->setValue(keyValuePair.second);
		//delete nnode;
	}

	if (bettergetheight(root->getLeft()) > bettergetheight(root->getRight()))
		root->setHeight(1+bettergetheight(root->getLeft()));
	else
		root->setHeight(1+bettergetheight(root->getRight()));
	
	//std::cout << root->getKey() << " height set to " <<root->getHeight() << std::endl;

	int bf = calcbf(root);
/*	std::cout << "bf for " << root->getKey() << " is ";
	if (root ->getLeft() != NULL && root ->getRight() != NULL)	
		std::cout << root->getLeft()->getHeight() << " - " << root->getRight()->getHeight() << " = ";
	std::cout << bf << std::endl;*/
/*
	std::cout << "====================" << std::endl;
	printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
	std::cout << "====================" << std::endl;
*/
	//right right
	if (root->getRight() != NULL) {
		if (keyValuePair.second > root->getRight()->getKey() && bf < -1) {
			std::cout << root->getKey() << "procL" << std::endl;
			return leftrotate(root);
		}
	}

	//left left
	if (root->getLeft() != NULL) {
		if (keyValuePair.second < root->getLeft()->getKey() && bf > 1) {
			//std::cout << root->getKey() << "procR" << std::endl;
			return rightrotate(root);
		}
	}

	//right left
	if (root->getRight() != NULL) {
		if (keyValuePair.second < root->getRight()->getKey() && bf < -1) {
			//std::cout << root->getKey() << "procRL" << std::endl;
			root->setRight(rightrotate(root->getRight()));
			return leftrotate(root);
		}
	}

	//left right
	if (root->getLeft() != NULL) {
		if (keyValuePair.second > root->getLeft()->getKey() && bf > 1) {
			//std::cout << root->getKey() << "procLR" << std::endl;
			root->setLeft(leftrotate(root->getLeft()));
			return rightrotate(root);
		}
	}

/*
	std::cout << "///////////" << std::endl;
	printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
	std::cout << "///////////" << std::endl;
	
	
	std::cout << root->getKey() <<" returning " << std::endl;*/
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
 
	std::cout << "in here" << std::endl;
	// TODO
	if (root == NULL) {
		std::cout << "root is null" << std::endl;
		return root;
	}


	if (key < root->getKey()) {
		std::cout << "in if1" << std::endl;
		root->setLeft(removeHelper(root->getLeft(), key));
		
	}

	else if (key > root->getKey()) {
		std::cout << "in if2" << std::endl;
		root->setRight(removeHelper(root->getRight(), key));
		std::cout << "back" <<std::endl;

	}

	else {
		std::cout << "found node ";
		std::cout << root->getKey() <<std::endl;

		AVLNode <Key, Value>* tnode = NULL;

		std::cout << root->getRight() << " and " << root->getLeft() << std::endl;
		if (!root->getRight() || !root->getLeft()) { //one or 0 child
			std::cout << "in else if1" << std::endl;

			if (root->getLeft()) {
				std::cout << "in else if1 if1" << std::endl;
				tnode = root->getLeft();
				std::cout << tnode->getKey() << std::endl; 

			}
			else if (root->getRight()) {
				std::cout << "in else if1 if2" << std::endl;
				tnode = root->getRight();
				std::cout << tnode->getKey() << std::endl; 

			}

			//std::cout << tnode->getParent()->getKey() << std::endl;
			//std::cout << root->getParent()->getKey() <<std::endl; 
			if (!tnode) {
				std::cout << "!tnode" << std::endl; 
				swap(root,NULL);
			}
			else {
				std::cout <<"swapping" <<std::endl;
				swap(root, tnode);
			}

			std::cout << "past !tnode" << std::endl;
			std::cout << "====================" << std::endl;
			printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
			std::cout << "====================" << std::endl;

			//std::cout << tnode->getKey() << std::endl;
			//std::cout << root->getKey() <<std::endl; 

			delete root;
			root = tnode;
			
		}
		else { //2 children 
			std::cout << "in swap else" << std::endl;
			
			AVLNode <Key, Value>* tnode = getMinChild(root->getRight());



			swap(root, tnode);
			std::cout << "past swap" << std::endl;
			

			tnode->setRight(removeHelper(tnode->getRight(), tnode->getKey()));
		}
	}

	std::cout << root << std::endl;
	if(!root) {
		return NULL;
	} 
	std::cout << root->getKey() << std::endl;


			
			printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));

			std::cout << "past return null" << std::endl;
		//std::cout << root->getLeft() << std::endl; 
		//std::cout << root->getRight() << std::endl; 

	if (bettergetheight(root->getLeft()) > bettergetheight(root->getRight()))
		root->setHeight(1+bettergetheight(root->getLeft()));
	else
		root->setHeight(1+bettergetheight(root->getRight()));

			std::cout << "past heights" << std::endl;
	

	int bf = calcbf(root);
	int bf2 = calcbf(root->getLeft());
	std::cout << bf << " " << bf2 <<std::endl;

		//right right
	if (root->getRight()) {
		if (0 >= calcbf(root->getRight()) && bf < -1) {
			std::cout << root->getKey() << "procL" << std::endl;
			return leftrotate(root);
		}
	}

	//left left
	if (root->getLeft()) {
		if (0 <= calcbf(root->getLeft()) && bf > 1) {
			//std::cout << root->getKey() << "procR" << std::endl;
			return rightrotate(root);
		}
	}

	//right left
	if (root->getRight()) {
		if (0 < calcbf(root->getRight()) && bf < -1) {
			//std::cout << root->getKey() << "procRL" << std::endl;
			root->setRight(rightrotate(root->getRight()));
			return leftrotate(root);
		}
	}

	//left right
	if (root->getLeft()) {
		if (0 > calcbf(root->getLeft()) && bf > 1) {
			//std::cout << root->getKey() << "procLR" << std::endl;
			root->setLeft(leftrotate(root->getLeft()));
			return rightrotate(root);
		}
	}

	return root;
}


template<typename Key, typename Value>
AVLNode<Key, Value>*  AVLTree<Key, Value>::leftrotate(AVLNode<Key, Value>* a) {

	//std::cout << "out here" <<std::endl;
	AVLNode<Key,Value>* b = a->getRight();
	AVLNode<Key,Value>* T2 = b->getLeft();
	//std::cout << "a is " << a->getKey() << std::endl;
	//std::cout << "b is " << b->getKey() << std::endl;
	//std::cout << "c is " << b->getRight()->getKey() << std::endl;


	if (a->getParent() == NULL) {
		//std::cout << "a is root" << std::endl;
		b->setParent(NULL);
		this->mRoot = b;
		b->setLeft(a);
		a->setRight(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);

	}

	else if (a->getParent()->getRight() == a) {
		//std::cout << "a is right" << std::endl;
		printheights(a);
		b->setParent(a->getParent());
		a->getParent()->setRight(b);
		b->setLeft(a);
		a->setRight(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
		printheights(b);
	}

	else if (a->getParent()->getLeft() == a) {
	//	std::cout << "a is left" << std::endl;
		b->setParent(a->getParent());
		a->getParent()->setLeft(b);
		b->setLeft(a);
		a->setRight(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
	}

	if (bettergetheight(a->getLeft()) > bettergetheight(a->getRight()))
		a->setHeight(1+bettergetheight(a->getLeft()));
	else
		a->setHeight(1+bettergetheight(a->getRight()));

	if (bettergetheight(b->getLeft()) > bettergetheight(b->getRight()))
		b->setHeight(1+bettergetheight(b->getLeft()));
	else
		b->setHeight(1+bettergetheight(b->getRight()));
	
	/*
	std::cout << "////////////////" << std::endl;
	printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
	std::cout << "////////////////" << std::endl;
*/
	return b;

}


template<typename Key, typename Value>
AVLNode<Key, Value>*  AVLTree<Key, Value>::rightrotate(AVLNode<Key, Value>* a) {

	//std::cout << "Rout here" <<std::endl;
	AVLNode<Key,Value>* b = a->getLeft();
	AVLNode<Key,Value>* T2 = b->getRight();
//	std::cout << "a is " << a->getKey() << std::endl;
//	std::cout << "b is " << b->getKey() << std::endl;
	//std::cout << "c is " << b->getLeft()->getKey() << std::endl;


	if (a->getParent() == NULL) {
	//	std::cout << "a is root" << std::endl;
		b->setParent(NULL);
		this->mRoot = b;
		b->setRight(a);
		a->setLeft(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);

	}

	else if (a->getParent()->getRight() == a) {
	//	std::cout << "a is right" << std::endl;
		printheights(a);
		b->setParent(a->getParent());
		a->getParent()->setRight(b);
		b->setRight(a);
		a->setLeft(T2);
		a->setParent(b);
		if (T2)
			T2->setParent(a);
		printheights(b);
	}

	else if (a->getParent()->getLeft() == a) {
	//	std::cout << "a is left" << std::endl;
		b->setParent(a->getParent());
		a->getParent()->setRight(b);
		b->setRight(a);
		a->setLeft(T2);
		if (T2)
			T2->setParent(a);
		a->setParent(b);
	}

	if (bettergetheight(a->getLeft()) > bettergetheight(a->getRight()))
		a->setHeight(1+bettergetheight(a->getLeft()));
	else
		a->setHeight(1+bettergetheight(a->getRight()));

	if (bettergetheight(b->getLeft()) > bettergetheight(b->getRight()))
		b->setHeight(1+bettergetheight(b->getLeft()));
	else
		b->setHeight(1+bettergetheight(b->getRight()));
	
	//std::cout << b->getRight()->getKey() << std::endl;
	/*
	std::cout << "~~~~~~~~~~~~~~" << std::endl;
	printheights(static_cast<AVLNode<Key,Value>*>(b));
	std::cout << "~~~~~~~~~~~~~~" << std::endl;
*/

	return b;

}



template<typename Key, typename Value>
int AVLTree<Key, Value>::calcbf(AVLNode<Key, Value>* node)
{
	if (node == NULL) return 0;

	return bettergetheight(node->getLeft()) - bettergetheight(node->getRight());

}

template<typename Key, typename Value>
int AVLTree<Key, Value>::bettergetheight(AVLNode<Key, Value>* node) {
	if (node == NULL) return 0;
	else return node->getHeight();
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::printheights(AVLNode<Key, Value>* root) {
	if (root != NULL) {
		//int bf = calcbf(root);
		if (root->getParent() != NULL)
			std::cout << root->getKey() << " height " << root->getHeight() << " parent " << root->getParent()->getKey() << std::endl; 
		else std::cout << root->getKey() <<" height " << root->getHeight() << " parent " << 0 << std::endl; 

		printheights(root->getLeft());
		printheights(root->getRight());
	}
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getMinChild(AVLNode<Key, Value>* parent)
{
	AVLNode<Key, Value>* curr = parent;

	while(curr && curr->getLeft() != NULL) {
		curr = curr->getLeft();
	}
	return curr;

}

template<typename Key, typename Value>
void AVLTree<Key, Value>::swap(AVLNode<Key, Value>* onode, AVLNode<Key, Value>* succ) {

	printheights(onode);

	if (onode == succ) {
		return;
	}
	std::cout << "first check" << std::endl;


	bool oisroot = false;
	bool oisleft = false;
	bool oisright = false;
	
	if (onode->getParent() == NULL)
		oisroot = true;

	if(!oisroot) {
		if (onode->getParent()->getRight() == onode) 
			oisright = true;
		else if (onode->getParent()->getLeft() == onode)
			oisleft = true;
	}

	if (succ==NULL) {
		if (oisright) {
			onode->getParent()->setRight(NULL);
		}
		if (oisleft) {
			onode->getParent()->setLeft(NULL);
		}
		onode->setRight(NULL);
		onode->setLeft(NULL);
		onode->setParent(NULL);
		onode = NULL;
		return;
	}

	if (succ->getParent()->getRight() == succ)
		succ->getParent()->setRight(NULL);
	if (succ->getParent()->getLeft() == succ)
		succ->getParent()->setLeft(NULL);

	succ->setParent(onode->getParent());
	succ->setLeft(onode->getLeft());
	succ->setRight(onode->getRight());

	printheights(onode);
	
	if (onode->getLeft())
		onode->getLeft()->setParent(succ);
	std::cout << onode->getRight() << std::endl;
	if (onode->getRight())
		onode->getRight()->setParent(succ);
	if (oisright)
		onode->getParent()->setRight(succ);
	if (oisleft)
		onode->getParent()->setLeft(succ);
	if (oisroot)
		this->mRoot = succ;

	succ->setHeight(onode->getHeight());

	onode = NULL;

	std::cout << "//////////////" << std::endl;
	printheights(static_cast<AVLNode<Key,Value>*>(this->mRoot));
	std::cout << "//////////////" << std::endl;
}


/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
