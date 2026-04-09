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
    organizeTreeDeletion(node, nullptr);
    delete node;
    return 0;
  }

  if (!node->left) {
    *parentsPtr = node->right;
    node->right->parent = node->parent;
    organizeTreeDeletion(node, node->right);
    delete node;
    return 0;
  }

  if (!node->right) {
    *parentsPtr = node->left;
    node->left->parent = node->parent;
    organizeTreeDeletion(node, node->left);
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
    organizeTreeInsertion(*nextNode);
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
  if (node->parent && node->parent->right == node) { parentsPtr = &(node->parent->right); }

  Node* child = node->left;

  node->left = child->right;
  child->parent = node->parent;
  node->parent = child;
  *parentsPtr = child;
  if (child->right) { child->right->parent = node; }
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
  if (child->left) { child->left->parent = node; }
  child->left = node;

  return child;
}

int BinarySearchTree::organizeTreeInsertion(Node* node) {
  if (!node) { return 1; }

  if (node == head) {
    node->color = BLACK;
    return 0;
  }

  if (node->parent->color == BLACK) {
    return 0;
  }

  Node* parent = node->parent;
  Node* unc = getSibling(parent);
  Node* grandfather = parent->parent;

  if (unc && unc->color == RED) {
    parent->color = BLACK;
    unc->color = BLACK; 
    grandfather->color = RED;
    if (grandfather == head) { grandfather->color = RED; }

    organizeTreeInsertion(grandfather);
  } else {
    if (grandfather->left == node->parent) {
      if (parent->right == node) {
        swapColor(node);
        swapColor(parent);
        leftRotation(parent);
      }
      rightRotation(grandfather);
    } else {
      if (parent->left == node) {
        swapColor(node);
        swapColor(parent);
        rightRotation(parent);
      }
      leftRotation(grandfather);
    }

    swapColor(parent);
    swapColor(grandfather);
  }

  return 0;
}

int BinarySearchTree::organizeTreeDeletion(Node* deletedNode, Node* replacingNode) {

  // Simple case
  if (deletedNode->color == RED 
      || (replacingNode && replacingNode->color == RED) ) {
    if (replacingNode) { replacingNode->color = BLACK; }
    return 0;
  }

  return doubleBlack(replacingNode, deletedNode->parent);

}

int BinarySearchTree::doubleBlack(Node* node, Node* parent) {

  if (node == head) { 
    node->color = BLACK;
    return 0;
  }

  Node* sibling = nullptr;
  if (parent->left == node) { sibling = parent->right; }
  else if (parent->right == node) { sibling = parent->left; }

  Node* redNeice = nullptr;
  if (sibling && sibling->right && sibling->right->color == RED) { redNeice = sibling->right; }
  if (sibling && sibling->left && sibling->left->color == RED) { redNeice = sibling->left; }

  // sibling is black and has red child
  if (sibling && sibling->color == BLACK && redNeice) {
    if (parent->left == sibling) {
      // Left Right case
      if (sibling->right == redNeice && 
          (!getSibling(redNeice) || getSibling(redNeice)->color == BLACK)) {
        redNeice->color = parent->color;
        leftRotation(sibling);
      // Left Left case
      } else {
        redNeice->color = sibling->color;
        sibling->color = parent->color;
      }
      rightRotation(parent);
    } else {
      // Right Left case
      if (sibling->left == redNeice && 
          (!getSibling(redNeice) || getSibling(redNeice)->color == BLACK)) {
        redNeice->color = parent->color;
        rightRotation(sibling);
      // Right Right case
      } else {
        // Have to use sibling->right here in the case that sibling has two red children
        sibling->right->color = sibling->color;
        sibling->color = parent->color;
      }
      leftRotation(parent);
    }

    parent->color = BLACK;
    return 0;

  // sibling is red
  } else if (sibling && sibling->color == RED){

    parent->color = RED;
    sibling->color = BLACK;

    // sibling is left child of parent
    if (parent->left == sibling) {
      rightRotation(parent);
    // sibling is right child of parent
    } else {
      leftRotation(parent);
    }
    swapColor(sibling);
    swapColor(parent);

    doubleBlack(node, parent);
    return 0;

  // sibling is black and has no red child
  } else {
    if (sibling) { sibling->color = RED; }

    if (parent->color == BLACK) {
      doubleBlack(parent, parent->parent);
    } else {
      parent->color = BLACK;
    }

    return 0;
  }

  return 1;
}

int BinarySearchTree::swapColor(Node* node) {
  if (!node) { return 1; }

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

  const char* nodeCol;

  if (node->color == RED) { nodeCol = "\033[41m"; }
  else { nodeCol = "\033[40m"; }

  os << nodeCol << node->data << "\033[0m" << std::endl;

  reprRecursive(os, node->left, depth + 1);

  return os;
}
