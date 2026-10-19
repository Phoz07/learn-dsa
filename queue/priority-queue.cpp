#include <iostream>

class Node {
public:
  int data;
  int priority;
  Node *next;

  Node(int d, int p) : data(d), priority(p), next(nullptr) {}
};

class PriorityQueue {
  Node *head;

public:
  PriorityQueue() : head(nullptr) {}

  void enqueue(int d, int p) {
    Node *newNode = new Node(d, p);
    if (head == nullptr || head->priority > p) {
      newNode->next = head;
      head = newNode;
    } else {
      Node *temp = head;
      while (temp->next != nullptr && temp->next->priority <= p) {
        temp = temp->next;
      }
      newNode->next = temp->next;
      temp->next = newNode;
    }
  }

  void dequeue() {
    if (head == nullptr) {
      std::cout << "Queue is empty" << std::endl;
      return;
    }
    Node *temp = head;
    std::cout << temp->data << " dequeued" << std::endl;
    head = head->next;
    delete temp;
  }

  void display() {
    if (head == nullptr) {
      std::cout << "Queue is empty" << std::endl;
      return;
    }
    Node *temp = head;
    while (temp != nullptr) {
      std::cout << temp->data << "(" << temp->priority << ") ";
      temp = temp->next;
    }
    std::cout << std::endl;
  }
};
