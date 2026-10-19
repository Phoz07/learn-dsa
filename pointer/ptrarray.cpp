#include <iostream>
using namespace std;

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    int* arrPtr = arr; // Pointer to the first element of the array

    cout << *arrPtr << endl; // prints the value of the first element of the array
    cout << *(arrPtr + 1) << endl; // prints the value of the second element of the array
    
  return 0;
}
