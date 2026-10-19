#include <iostream>
using namespace std;

int main() {
  int var = 10;
  int *ptr = &var;
  int **ptr2 = &ptr;

  cout << **ptr2 << endl; // prints the value of var  

  return 0;
}
