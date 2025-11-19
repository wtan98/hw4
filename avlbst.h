#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
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
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode <Key, Value>* upperNode);
    void rotateRight(AVLNode <Key, Value>* upperNode);


};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode <Key, Value>* upperNode)
{
    
    AVLNode<Key, Value>* rightChild = upperNode->getRight(); 

    //end result: right child is the new parent 
    //right child's left tree to the right of upperNode
    //upperNode is to the left of right child 

    //first move the right child's left tree to the right of upperNode 
    AVLNode<Key, Value>* leftNodeOfRightChild = rightChild->getLeft(); 
    upperNode->setRight(leftNodeOfRightChild); 

    //check if it is a nullptr and set
    if(leftNodeOfRightChild != nullptr){
        leftNodeOfRightChild->setParent(upperNode); 
    }


    //now set right child node's parent to upper node's previous parent
    //iow. right child takes the place of upper node 
    AVLNode<Key, Value>* prevParentofUpperNode = upperNode->getParent(); 
    rightChild->setParent(prevParentofUpperNode);
    //case 1: upperNode was a left child to prevParentofUpperNode
    if (prevParentofUpperNode == nullptr){
        //upperNode was not right/left child (was a root)
        this->root_ = rightChild;
    }
    else if (upperNode == prevParentofUpperNode->getLeft()){
            prevParentofUpperNode->setLeft(rightChild); 
    }
    //case 2: upperNode was a right child to prevParentofUpperNode
    else if (upperNode == prevParentofUpperNode->getRight()){
        prevParentofUpperNode->setRight(rightChild); 
    }
        
    //set upper node to the left of right child and done
    rightChild->setLeft(upperNode);
    upperNode->setParent(rightChild);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight (AVLNode <Key, Value>* upperNode)
{
    AVLNode<Key, Value>* leftChild = upperNode->getLeft(); 

    //end result: left child is the new parent 
    //left child's right tree to the left of upperNode
    //upperNode is to the right of right child 

    //first move the left child's right tree to the left of upperNode 
    AVLNode<Key, Value>* rightNodeOfLeftChild = leftChild->getRight(); 
    upperNode->setLeft(rightNodeOfLeftChild); 

    if (rightNodeOfLeftChild != nullptr){
        rightNodeOfLeftChild->setParent(upperNode); 
    }

    //now set left child node's parent to upper node's previous parent
    //iow. left child takes the place of upper node 
    AVLNode<Key, Value>* prevParentofUpperNode = upperNode->getParent(); 
    leftChild->setParent(prevParentofUpperNode);
    //case 1: upperNode was a left child to prevParentofUpperNode
    if (prevParentofUpperNode == nullptr){
        this->root_ = leftChild; 
    }
    //case 2: upperNode was a right child to prevParentofUpperNode
    else if (upperNode == prevParentofUpperNode->getLeft()){
        prevParentofUpperNode->setLeft(leftChild); 
    }
    else{
        prevParentofUpperNode->setRight(leftChild); 
    }

    //set upper node to the right of left child and done
    leftChild->setRight(upperNode);
    upperNode->setParent(leftChild);
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    Key key = new_item.first; 
    Value val = new_item.second; 

    //Case 1: tree is empty 
    if (this->root_ == nullptr){
        //make a new node as the root  
        this->root_ = new AVLNode<Key, Value> (key, val, nullptr);
        return;
    }
    //Case 2: tree is not empty 
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* aboveNode = nullptr; 

    //use loop to find where to insert the item
    while (curr != nullptr){
        aboveNode = curr; 

        //go left if key < curr
        if (key < curr->getKey()){
            curr = curr->getLeft(); 
        }
        //go right if key > curr 
        else if (key > curr->getKey()){
            curr = curr->getRight(); 
        }
        //otherwise, the key exists; set it
        else{
            curr->setValue(val);
            return; //exit
        }
    }

    //now that we know where to insert the item, make the actual node to insert
    AVLNode<Key, Value>* nodeToInsert = new AVLNode<Key, Value>(key, val, aboveNode);
    //set it left if key < above node 
    if (key < aboveNode->getKey()){
        aboveNode->setLeft(nodeToInsert); 
    }
    //set it right if key > above node 
    else if (key > aboveNode->getKey()){
        aboveNode->setRight(nodeToInsert); 
    }

    //now check for balance (see if we need to rotate nodes)
    AVLNode<Key,Value>* newNode = nodeToInsert; 
    AVLNode<Key,Value>* parent = aboveNode; 


    //update the balance of parent node
    if (parent->getLeft() == newNode){
      parent->updateBalance(1);
    }
    else{
      parent->updateBalance(-1);
    }

    //if balanced, exit 
    if (parent->getBalance() == 0){
      return; 
    }

    //go up
    newNode = parent; 
    parent = parent->getParent(); 

    while (parent != nullptr){

        //update the balance: if there is new left node, increase balance by 1
        //if there is a new right node, increase balance by -1 
        //balance = left height - right height 
        if(parent->getLeft() == newNode){
            parent->updateBalance(1);
        }
        else if(parent->getRight() == newNode){
            parent->updateBalance(-1); 
        }

        //rotation needed if balance is not -1, 0, or 1
        if (parent->getBalance() == 2){  //right rotation needed
            AVLNode<Key, Value>* leftChild = parent->getLeft(); 

            //Case 1: 1 right rotation needed for left to left 
            if (leftChild->getBalance() >= 1){
                rotateRight(parent);
                parent->setBalance(0);
                leftChild->setBalance(0);
            }
                //Case 2: 2 rotations needed for left to right 
                //first rotate left on left child
                //then rotate right on parent 
                //result: newNode on top, left child is left and parent is right child
            else{

                AVLNode<Key, Value>* rightChildOfLeftChild = leftChild->getRight(); 
                rotateLeft(leftChild);
                rotateRight(parent); 

                //update the balance 
                if(rightChildOfLeftChild->getBalance() == 1){
                    leftChild->setBalance(0);
                    parent->setBalance(-1); 
                }
                else if(rightChildOfLeftChild->getBalance() == 0){
                    leftChild->setBalance(0);
                    parent->setBalance(0); 
                }
                else{
                    leftChild->setBalance(1);
                    parent->setBalance(0);
                }
                rightChildOfLeftChild->setBalance(0);
            }
            break; 
        }
        else if(parent->getBalance() == -2){    //left rotation needed
            AVLNode<Key, Value>* rightChild = parent->getRight(); 

            //Case 1: 1 left rotation needed for right to right 
            if (rightChild->getBalance() <= -1){
                rotateLeft(parent);
                parent->setBalance(0);
                rightChild->setBalance(0);
            }
                //Case 2: 2 rotations needed for right to left  
                //first rotate right on left child
                //then rotate left on parent 
                //result: newNode on top, right child is right and parent is left child
            else{
                AVLNode<Key, Value>* leftChildOfRightChild = rightChild->getLeft(); 
                rotateRight(rightChild);
                rotateLeft(parent); 

                //update the balance 
                if(leftChildOfRightChild->getBalance() == -1){
                    rightChild->setBalance(0);
                    parent->setBalance(1); 
                }
                else if(leftChildOfRightChild->getBalance() == 0){
                    rightChild->setBalance(0);
                    parent->setBalance(0); 
                }
                else{
                    rightChild->setBalance(-1);
                    parent->setBalance(0);
                }
                leftChildOfRightChild->setBalance(0);
            }
            break; 
            
        }
            
        if(parent->getBalance() == 0){
            break; 
        }

        //keep going to the next upper nodes to repeat
        newNode = parent; 
        parent = parent->getParent(); 
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    
    //use loop to find where to remove the item
    while (curr != nullptr && curr->getKey() != key){
        //go left if key < curr
        if (key < curr->getKey()){
            curr = curr->getLeft(); 
        }
        //go right if key > curr 
        else if (key > curr->getKey()){
            curr = curr->getRight(); 
        }
    }

    //exit if the item to remove is not found 
    if(curr==nullptr){
        return; 
    }

    //Otherwise, item exists
    //Case 1: node has 2 children
    if (curr->getRight() != nullptr && curr->getLeft() != nullptr){
        //predecessor is the max of left tree (right most node)
        AVLNode<Key, Value>* predecessor = curr->getLeft(); 
        //find the max node in the left tree 
        while(predecessor->getRight() != nullptr){
            predecessor= predecessor->getRight(); 
        }

        nodeSwap(curr, predecessor);
    }

    //Case 2: node has 1 child at most
    AVLNode<Key, Value>* aboveNode = curr->getParent(); 
    int balanceFactor = 0; 
    
    //balance factor = left height - right height 
    //starting at 0, if left child is removed, it'll be -1
    //if right child is removed, it'll be 1
    if (aboveNode != nullptr){
        if (curr == aboveNode->getLeft()){
            balanceFactor = -1; 
        }
        else if (curr == aboveNode->getRight()){
            balanceFactor = 1; 
        }
    }
    
    //now that we swapped the child node with the pred, remove curr
    AVLNode<Key, Value>* childNode = nullptr; 
    if(curr->getLeft() != nullptr){
        childNode = curr->getLeft(); 
    }
    else if (curr->getRight() != nullptr){
        childNode = curr->getRight(); 
    }

    //set the child node to the above node 
    if (aboveNode != nullptr){
        if (aboveNode->getRight() == curr){
            aboveNode->setRight(childNode); 
        }
        else if (aboveNode->getLeft() == curr){
            aboveNode->setLeft(childNode); 
        }
        //set the child nodes parent to aboveNode
        if (childNode != nullptr){
            childNode->setParent(aboveNode);
        }
    }
    //else curr is not a root  
    else{
        this->root_ = childNode; 
        //update child node's parent to null 
        if (childNode != nullptr){
            childNode->setParent(nullptr);
        }
    }
        
    delete curr; 
    
    //update the balance; start on the parent of deleted node and go up till at root 
    AVLNode<Key, Value>* node = aboveNode;
    int sideRemoved = balanceFactor; 

    while (node != nullptr){
        AVLNode<Key, Value>* nodesParent = node->getParent(); 
        int nextSideRemoved = 0;

        if (nodesParent != nullptr){
            if (node == nodesParent->getLeft()){
                nextSideRemoved = -1; 
            }
            else if(node == nodesParent->getRight()){
                nextSideRemoved = 1; 
            }
        }

        //keep track if we need to go to next upper node
        bool goUp = false; 

        //Case 1: the node was removed on the left side
        if (sideRemoved == -1){
            int newBF = node->getBalance() + sideRemoved; 

            //Case 1a: BF of -2 means there are 2 nodes on the right 
            //which means we need to rotate left to balance 
            if (newBF == -2){
                AVLNode<Key,Value>* rightChild = node->getRight(); 

                //if right child's bf is -1, that means there is a right child node
                if (rightChild->getBalance() == -1){
                  //simply rotate left 
                  rotateLeft(node);
                  node->setBalance(0);
                  rightChild->setBalance(0);
                  goUp = true; 
                }
                //but if bf is 0, don't go up
                else if (rightChild->getBalance() == 0){
                  //simply rotate left 
                  rotateLeft(node);
                  node->setBalance(-1);
                  rightChild->setBalance(1);
                  goUp = false; 
                }
                //otherwise, the balance is >0, which means there is a left child node 
                else if (rightChild->getBalance() > 0){
                  //we need to rotate left node to the right 
                  AVLNode<Key, Value>* leftChildToRightChild = rightChild->getLeft(); 
                  rotateRight(rightChild); 

                  //now that the left child is to the right now, rotate to the left 
                  rotateLeft(node); 

                  //update the balance factor 
                  if (leftChildToRightChild->getBalance() == 0){
                    node->setBalance(0);
                    rightChild->setBalance(0);
                  }
                  else if (leftChildToRightChild->getBalance() == -1){
                    node->setBalance(1);
                    rightChild->setBalance(0);
                  }
                  else if (leftChildToRightChild->getBalance() == 1){
                    node->setBalance(0);
                    rightChild->setBalance(-1);
                  }
                  goUp = true; 
                  //its balance now so leftChildToRightChild BF = 0
                  leftChildToRightChild->setBalance(0);
                }
            }
            //Case 1b: newBF = -1 is still balanced (dont need to go up)
               
            else if (newBF == -1){
              node->setBalance(-1); 
              goUp = false; 
            } 
            //Case 2b: newBF = 0 
            //keep going up because it went from -1/1 to 0
            //height decreased so keep going up
            else if (newBF == 0){
              node->setBalance(0);
              goUp = true; 
            }
        }

        //Case 2: the node was removed on the right side
        else if (sideRemoved == 1){
            int newBF = node->getBalance() + sideRemoved; 

            //Case 1a: BF of 2 means there are 2 nodes on the left 
            //which means we need to rotate right to balance 
            if (newBF == 2){
                AVLNode<Key,Value>* leftChild = node->getLeft(); 

                //if left child's bf is 1, that means there is a left child node
                if (leftChild->getBalance() == 1){
                  //simply rotate right 
                  rotateRight(node);
                  node->setBalance(0);
                  leftChild->setBalance(0);
                  goUp = true; 
                }
                //if bf is 0, dont go up 
                else if (leftChild->getBalance() == 0){
                  //simply rotate right 
                  rotateRight(node);
                  node->setBalance(1);
                  leftChild->setBalance(-1);
                  goUp = false; 
                }
                //otherwise, the balance is <0, which means there is a right child node 
                else if (leftChild->getBalance() < 0){
                  //we need to rotate right node to the left 
                  AVLNode<Key, Value>* rightChildToLeftChild = leftChild->getRight(); 
                  rotateLeft(leftChild); 

                  //now that the right child is to the left now, rotate to the right 
                  rotateRight(node); 

                  //update the balance factor 
                  if (rightChildToLeftChild->getBalance() == 0){
                    node->setBalance(0);
                    leftChild->setBalance(0);
                  }
                  else if (rightChildToLeftChild->getBalance() == 1){
                    node->setBalance(-1);
                    leftChild->setBalance(0);
                  }
                  else if (rightChildToLeftChild->getBalance() == -1){
                    node->setBalance(0);
                    leftChild->setBalance(1);
                  }
                  goUp = true; 
                  //its balance now so leftChildToRightChild BF = 0
                  rightChildToLeftChild->setBalance(0);
                }
            }
            //Case 1b: newBF = -1 is still balanced (dont need to go up)
            else if (newBF == 1){
              node->setBalance(1); 
              goUp = false; 
            }
            //Case 2b: newBF = 0 
            //keep going up because it went from -1/1 to 0
            //height decreased so keep going up
            else if (newBF == 0){
              node->setBalance(0);
              goUp = true; 
            }
        }

        //go up if needed 
        if (goUp == true){
          node = nodesParent; 
          sideRemoved = nextSideRemoved; 
        }
        else{
          //dont need to go up; exit 
          break; 
        }
    }

    return; 
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif