#include <iostream>
#include <vector>
using namespace std;

// Function to add elements to the vector starting from its current size
void addElements(vector<int> &vec, int numElementsToAdd) {
  int initialSize = vec.size();
  // Resize the vector to accommodate the new elements
  vec.resize(initialSize + numElementsToAdd);

  // Get a pointer to the first element to be added
  int *ptr = vec.data() + initialSize;

  // Use the pointer to add new elements
  for (int i = 0; i < numElementsToAdd; ++i) {
    *(ptr + i) = initialSize + i; // Example value: start from the initial size
  }
}

// Function to print the elements of the vector
void printVector(const vector<int> &vec) {
  for (int i = 0; i < vec.size(); ++i) {
    cout << vec[i] << " ";
  }
  cout << endl;
}

int main() {
  vector<int> myVector;

  // Initially add some elements
  for (int i = 0; i < 5; ++i) {
    myVector.push_back(i);
  }

  cout << "Original vector: ";
  printVector(myVector);

  // Add more elements using the addElements function
  addElements(myVector, 5); // Add 5 more elements

  cout << "After adding elements: ";
  printVector(myVector);

  return 0;
}