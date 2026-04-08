#include <iostream>
#include "BinarySearchTree.h"
#include "Node.h"

BinarySearchTree::BinarySearchTree(int* intList, int listSize) {

  head = nullptr;

  if (!intList) { return; }

  for (int i = 0; i < listSize; ++i) {
    insert(intList[i]);
  }
}

BinarySearchTree::~BinarySearchTree() {
  deallocateRecursive(head);
}

int BinarySearchTree::remove(int data) {
  Node* node = searchNode(data);
  if (!node) { return 1; }

  return removeNode(node);
}

int BinarySearchTree::insert(int data) {
  if (!head) { 
    head = new Node(data, BLACK); 
    return 0;
  }

  return insertRecursive(data, head);
}

bool BinarySearchTree::search(int data) {
  return (bool)searchNode(data);
}

int BinarySearchTree::removeNode(Node* node) {

  Node** parentsPtr = nullptr;
  if (node == head) { parentsPtr = &head; }
  if (node->parent && node->parent->left == node) { parentsPtr = &(node->parent->left); }
  if (node->parent && node->parent->right == node) { parentsPtr = &(node->parent->right); }

  if (!parentsPtr) { return 1; }

  if (!node->left && !node->right) {
    *parentsPtr = nullptr;  
    delete node;
    return 0;
  }

  if (!node->left) {
    *parentsPtr = node->right;
    node->right->parent = node->parent;
    delete node;
    return 0;
  }

  if (!node->right) {
    *parentsPtr = node->left;
    node->left->parent = node->parent;
    delete node;
    return 0;
  }

  Node* leaf = node->left;

  while (leaf->right) {
    leaf = leaf->right;
  }

  int dataCache = leaf->data;
  removeNode(leaf);
  node->data = dataCache;
  return 0;
}

int BinarySearchTree::insertRecursive(int data, Node* node) {
  // should not be possible for this to occur
  if (!node) { return 1; }

  Node** nextNode;

  if (node->data > data) {
    nextNode = &(node->left);
  } else {
    nextNode = &(node->right);
  }

  
  if (!*nextNode) {
    *nextNode = new Node(data, RED);
    (*nextNode)->parent = node;
  } else {
    insertRecursive(data, *nextNode);
  }

  return 0;
}

Node* BinarySearchTree::searchNode(int data) {
  Node* node = head;

  if (!node) { return nullptr; }

  while (node->data != data) {
    if (node->data > data) {
      node = node->left;
    } else {
      node = node->right;
    }

    if (!node) { return nullptr; }
  }

  return node;
}

void BinarySearchTree::deallocateRecursive(Node* node) {
  if (!node) { return; }

  deallocateRecursive(node->left);
  deallocateRecursive(node->right);

  delete node;
}

Node* BinarySearchTree::getSibling(Node* node) {
  if (!node) { return nullptr; }
  if (!node->parent) { return nullptr; }

  if (node->parent->left == node ) { return node->parent->right; }
  else { return node->parent->left; }
}

Node* BinarySearchTree::rightRotation(Node* node) {
  Node** parentsPtr = nullptr;
  if (node == head) { parentsPtr = &head; }
  if (node->parent && node->parent->left == node) { parentsPtr = &(node->parent->left); }
  { parentsPtr = &(node->parent->right); }

  Node* child = node->left;

  node->left = child->right;
  child->parent = node->parent;
  node->parent = child;
  *parentsPtr = child;
  child->right->parent = node;
  child->right = node;

  return child;
}

Node* BinarySearchTree::leftRotation(Node* node) {
  Node** parentsPtr = nullptr;
  if (node == head) { parentsPtr = &head; }
  if (node->parent && node->parent->left == node) { parentsPtr = &(node->parent->left); }
  if (node->parent && node->parent->right == node) { parentsPtr = &(node->parent->right); }

  Node* child = node->right;

  node->right = child->left;
  child->parent = node->parent;
  node->parent = child;
  *parentsPtr = child;
  child->left->parent = node;
  child->left = node;

  return child;
}

int BinarySearchTree::organizeTreeInsertion(Node* node) {
  return 0;
}

int BinarySearchTree::swapColor(Node* node) {
  if (!node) { return 1; }
  if (!node->color) { return 1; }

  if (node->color == BLACK) { node->color = RED; }
  else { node->color = BLACK; }

  return 0;
}

std::ostream& operator<<(std::ostream& os, BinarySearchTree& tree) {
  if (!tree.head) { os << "Tree is empty." << std::endl;}

  tree.reprRecursive(os, tree.head, 0);

  return os;
}

std::ostream& BinarySearchTree::reprRecursive(std::ostream& os, Node* node, int depth) {
  if (!node) { return os; }

  reprRecursive(os, node->right, depth + 1);

  for (int i = 0; i < depth; ++i) { os << "  "; }
  os << node->data << std::endl;

  reprRecursive(os, node->left, depth + 1);

  return os;
}
