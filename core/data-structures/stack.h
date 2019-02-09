#pragma once
template <class T> class Stack
{
  struct Node {
    Node* next;
    T item;
  };
  Node* topNode;
  int size;

public:
  Stack()
  { }
  ~Stack()
  {
    Node* tNode;

    while (topNode) {
      tNode = topNode;
      delete topNode;
      topNode = tNode->next;
    }
  }

  void push(T value)
  {
    Node* n = new Node;
    n->item = value;
    n->next = topNode;

    topNode = n;

    size++;
  }

  T pop()
  {
    if (!isEmpty()) {
      T val = topNode->item;
      Node* tNode = topNode;
      delete topNode;
      topNode = tNode->next;
      size -= 1;
      return val;
    } else {
      return 0;
    }
  }

  T top()
  {
    if (!isEmpty()) {
      return topNode->item;
    } else {
      return 0;
    }
  }

  bool isEmpty() { return size == 0; }
};