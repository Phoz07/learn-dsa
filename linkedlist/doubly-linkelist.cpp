#include <iostream>
using namespace std;

class Node {
public:
  int data;
  Node *prev;
  Node *next;

  Node(int data) {
    this->data = data;
    this->prev = nullptr;
    this->next = nullptr;
  }
};

void printList(Node *node) {
  while (node != nullptr) {
    cout << node->data << " ";
    node = node->next;
  }
}

int main() {
  Node *head = new Node(1);
  Node *second = new Node(2);
  Node *third = new Node(3);

  head->next = second;
  second->prev = head;
  second->next = third;
  third->prev = second;

  printList(head);

  return 0;
}
