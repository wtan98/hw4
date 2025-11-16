#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

// find the height of left and right paths
int findHeight(Node* root)
{
    int leftHeight; 
    int rightHeight; 

    //base case: root is null (height = 0)
    if (root == nullptr){
        return 0; 
    }

    leftHeight = findHeight(root -> left);
    rightHeight = findHeight(root -> right);

    //return -1 if error 
    if(leftHeight < 0){
        return -1; 
    }

    if(rightHeight < 0){
        return -1; 
    }

    //case 1: left and right heights don't match
    if (root->left != nullptr && root->right != nullptr && leftHeight != rightHeight){
        return -1;  //error
    }

    //case 2: there is left child, no right child
    else if (root->left != nullptr && root->right == nullptr){
        return leftHeight + 1;  
    }

    //case 3: there is right child, no left child 
    else if (root->left == nullptr && root->right != nullptr){
        return rightHeight + 1;  
    }

    //if above conditions not satisfied, left and right are the same 
    return rightHeight + 1; 
}

bool equalPaths(Node * root)
{
    // Add your code below
    //base case: tree is empty (height = same)
    if (root == nullptr){
        return true; 
    }

    //if findHeight() returns -1; there is error (return false)
    //but if it returns a height, return true (same length)
    int height = findHeight(root);
    
    if (height == -1) {
        return false;
    }

    return true; 

}

