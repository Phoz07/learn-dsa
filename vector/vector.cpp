#include <iostream>
#include <vector>
using namespace std;

int main() {
  // Create a vector of integers
  vector<int> vec;

  // Add elements to the vector
  vec.push_back(10);
  vec.push_back(20);
  vec.push_back(30);

  // Access and print elements using a range-based for loop
  for (int i = 0; i < vec.size(); i++) {
    cout << vec[i] << " ";
  }
  cout << endl;

  // Remove the last element
  vec.pop_back();

  // Access an element by index
  cout << "Element at index 1: " << vec[1] << endl;

  // Get the size of the vector
  cout << "Size of vector: " << vec.size() << endl;

  return 0;
}   