#include <iostream>
#include <vector>
using namespace std;

int main() {
  vector<int> vec = {1, 2, 3, 4, 5};

  // Iterator initialization, similar to obtaining a pointer to the first
  // element
  vector<int>::iterator it = vec.begin();

  // Dereferencing an iterator to access the first element, similar to using a
  // pointer
  cout << "First element (using iterator): " << *it << endl;

  // Modify the first element through the iterator
  *it = 10;
  cout << "Modified first element: " << vec[0] << endl;

  // Using iterators in a loop, similar to pointer arithmetic
  cout << "Elements in vector: ";
  for (vector<int>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
    cout << *iter << " "; // Dereference iterator to access element
  }
  cout << endl;

  // Using iterators to insert elements at a specific position
  it = vec.begin() + 2; // Points to the third element
  vec.insert(it, 20);   // Insert '20' before the third element

  // Using iterators to erase elements
  it = vec.begin(); // Reset iterator to the first element
  vec.erase(it);    // Erase the first element

  // Print the modified vector
  cout << "Modified vector: ";
  for (int elem : vec) {
    cout << elem << " ";
  }
  cout << endl;

  return 0;
}
