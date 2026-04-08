
#ifndef NODE_H
#define NODE_H

enum Color{
  BLACK,
  RED
};

struct Node {

  int data;

  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;

  Color color;

  Node() {}

  Node(int in_data, Color in_color) : data(in_data), color(in_color) {} 

};

#endif 
