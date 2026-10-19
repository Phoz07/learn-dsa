#include <iostream>
using namespace std;

class Node {
public:
  int value;
  Node *next;
  Node *prev;
  Node(int value) : value(value), next(nullptr), prev(nullptr) {}
};

class Deque {
private:
  Node *front;
  Node *rear;
  int size;

public:
  Deque() : front(nullptr), rear(nullptr), size(0) {}

  // Check if deque is empty
  bool isEmpty() const { return size == 0; }

  // Insert at the front
  void insertFront(int value) {
    Node *newNode = new Node(value);
    if (isEmpty()) {
      front = rear = newNode;
    } else {
      newNode->next = front;
      front->prev = newNode;
      front = newNode;
    }
    size++;
  }

  // Insert at the rear
  void insertRear(int value) {
    Node *newNode = new Node(value);
    if (isEmpty()) {
      front = rear = newNode;
    } else {
      rear->next = newNode;
      newNode->prev = rear;
      rear = newNode;
    }
    size++;
  }

  // Delete from front
  void deleteFront() {
    if (isEmpty()) {
      cout << "Deque is empty" << endl;
      return;
    }
    Node *temp = front;
    front = front->next;
    if (front == nullptr) {
      rear = nullptr;
    } else {
      front->prev = nullptr;
    }
    delete temp;
    size--;
  }

  // Delete from rear
  void deleteRear() {
    if (isEmpty()) {
      cout << "Deque is empty" << endl;
      return;
    }
    Node *temp = rear;
    rear = rear->prev;
    if (rear == nullptr) {
      front = nullptr;
    } else {
      rear->next = nullptr;
    }
    delete temp;
    size--;
  }

  // Get front element
  int getFront() const {
    if (isEmpty()) {
      cout << "Deque is empty" << endl;
      return -1;
    }
    return front->value;
  }

  // Get rear element
  int getRear() const {
    if (isEmpty()) {
      cout << "Deque is empty" << endl;
      return -1;
    }
    return rear->value;
  }

  // Destructor to clean up the linked list
  ~Deque() {
    while (!isEmpty()) {
      deleteFront();
    }
  }
};

int main() {
  Deque dq;

  dq.insertFront(1);
  dq.insertRear(2);
  dq.insertFront(3);
  dq.insertRear(4);

  cout << "Front element: " << dq.getFront() << endl;
  cout << "Rear element: " << dq.getRear() << endl;

  dq.deleteFront();
  dq.deleteRear();

  cout << "Front element after deletion: " << dq.getFront() << endl;
  cout << "Rear element after deletion: " << dq.getRear() << endl;

  return 0;
}
