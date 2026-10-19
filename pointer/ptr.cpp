#include <iostream>
using namespace std;

int main() {
  int num = 10;
  int *ptr = &num;      // ptr points to the address of num
  cout << *ptr << endl; // prints the value of num

  *ptr = 20;            // modifies the value of num through the pointer
  cout << *ptr << endl; // prints the value of num after modification

  return 0;
}
