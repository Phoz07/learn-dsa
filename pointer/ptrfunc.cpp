#include <iostream>
using namespace std;

void myFunction(int a) {
  // Function implementation
  cout << a << endl;
}

int main() {

  void (*functionPtr)(int) =
      myFunction; // Pointer to a function that takes an int and returns void
  functionPtr(42);
  return 0;
}
