#include <iostream>
#include <stack>
using namespace std;

int main() {
  stack<int> myStack;

  // Push elements into the stack
  myStack.push(10);
  myStack.push(20);
  myStack.push(30);

  cout << "The top element is: " << myStack.top() << endl;

  // Pop an element from the stack
  myStack.pop();
  cout << "After popping, the top element is: " << myStack.top() << endl;

  // Check if the stack is empty
  if (!myStack.empty()) {
    cout << "The stack is not empty" << endl;
  }

  // Print the size of the stack
  cout << "The stack size is: " << myStack.size() << endl;

  return 0;
}
