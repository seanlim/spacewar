template <class T> class Stack
{
  struct Node {
    Node* next;
    T item;
  };
  Node* top;
  int size;

public:
  Stack()
  {
    top = nullptr;
    size = 0;
  }
  ~Stack()
  {
    Node* tNode;

    while (top) {
      tNode = top;
      delete top;
      top = tNode->next;
    }
  }

  void push(T value)
  {
    Node* n = new Node;
    n->item = value;
    n->next = top;

    top = n;

    size++;
  }

  T pop()
  {
    if (!isEmpty()) {
      T val = top->item;
      Node* tNode = top;
      delete top;
      top = tNode->next;
      size -= 1;
      return val;
    } else {
      return 0;
    }
  }

  bool isEmpty() { return size == 0; }
};