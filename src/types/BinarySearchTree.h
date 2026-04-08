#include <iostream>
#include "Node.h"

#ifndef BIN_SEARCH_TREE_H
#define BIN_SEARCH_TREE_H

class BinarySearchTree {
  public:

    // Constructor
    BinarySearchTree(int* intList, int listSize);

    // Destructor
    ~BinarySearchTree();

    // Remove an element from the tree, returns 1 if data isnt in tree
    int remove(int data);

    // Insert an element into the tree, returns 1 if an error occured
    int insert(int data);

    // Returns true if supplied element exists in the tree, false if else
    bool search(int data);

  private:

    // Head of the tree
    Node* head;

    // Removes the node supplied from the tree
    int removeNode(Node* node);

    // Recursive algorithm for inserting element into the tree
    int insertRecursive(int data, Node* node);

    // Returns a pointer to the node with the supplied data
    Node* searchNode(int data);

    // Recursive algoroithm for deallocating this object
    void deallocateRecursive(Node* node);

    // Helper function for getting the sibling of a node
    Node* getSibling(Node* node);

    // Perform a right rotation on the supplied node
    Node* rightRotation(Node* node);

    // Perform a left rotation on the supplied node
    Node* leftRotation(Node* node);

    // Organizes the tree in the insertion case starting from the supplied node
    int organizeTreeInsertion(Node* node);

    // Swap the color of the supplied node (RED->BLACK, BLACK->RED)
    int swapColor(Node* node);

    // Overload << operator for printing
    friend std::ostream& operator<<(std::ostream& os, BinarySearchTree& tree);

    // Recursive algorithm for printing
    std::ostream& reprRecursive(std::ostream& os, Node* node, int depth);
};

#endif
