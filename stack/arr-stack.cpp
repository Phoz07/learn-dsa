#include <iostream>
#define MAX_SIZE 100 // Defining the maximum size of the stack

class Stack {
private:
  int top;
  int arr[MAX_SIZE];

public:
  Stack() { top = -1; }

  // Function to add an item to the stack. It increases top by 1
  void push(int x) {
    if (top >= (MAX_SIZE - 1)) {
      std::cout << "Stack Overflow" << std::endl;
    } else {
      arr[++top] = x;
      std::cout << x << " pushed into stack\n";
    }
  }

  // Function to remove an item from the stack. It decreases top by 1
  int pop() {
    if (top < 0) {
      std::cout << "Stack Underflow" << std::endl;
      return 0;
    } else {
      int x = arr[top--];
      return x;
    }
  }

  // Function to return the top element of the stack without removing it
  int peek() {
    if (top < 0) {
      std::cout << "Stack is Empty" << std::endl;
      return 0;
    } else {
      int x = arr[top];
      return x;
    }
  }

  // Function to check if the stack is empty
  bool isEmpty() { return (top < 0); }
};

int main() {
  Stack s;
  s.push(10);
  s.push(20);
  std::cout << "Top element is " << s.peek() << std::endl;
  std::cout << "Popped from stack: " << s.pop() << std::endl;
  return 0;
}
