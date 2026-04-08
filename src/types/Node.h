
#ifndef NODE_H
#define NODE_H

struct Node {

  int data;

  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;

  Node() {}

  Node(int in_data) : data(in_data) {} 

};

#endif 
