#include <iostream>

class Node {
public:
  int data;
  Node *next;
};

class Stack {
private:
  Node *top;

public:
  Stack() { top = nullptr; }

  // Function to add an item to the stack. It changes the top pointer
  void push(int x) {
    Node *newNode = new Node();
    newNode->data = x;
    newNode->next = top;
    top = newNode;
    std::cout << x << " pushed into stack\n";
  }

  // Function to remove an item from the stack. It changes the top pointer
  int pop() {
    if (top == nullptr) {
      std::cout << "Stack Underflow" << std::endl;
      return 0;
    }
    Node *temp = top;
    int poppedValue = top->data;
    top = top->next;
    delete temp;
    return poppedValue;
  }

  // Function to check if the stack is empty
  bool isEmpty() { return top == nullptr; }

  // Function to return the top element of the stack without removing it
  int peek() {
    if (!isEmpty()) {
      return top->data;
    } else {
      std::cout << "Stack is empty" << std::endl;
      return -1;
    }
  }
};

int main() {
  Stack s;
  s.push(10);
  s.push(20);
  std::cout << "Top element is " << s.peek() << std::endl;
  std::cout << "Popped from stack: " << s.pop() << std::endl;
  std::cout << "Top element is " << s.peek() << std::endl;
  return 0;
}
