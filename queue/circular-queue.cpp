#include <iostream>
#define SIZE 5

class CircularQueueArray {
  int items[SIZE], front, rear;

public:
  CircularQueueArray() : front(-1), rear(-1) {}

  bool isFull() { return (rear + 1) % SIZE == front; }

  bool isEmpty() { return front == -1; }

  void enqueue(int element) {
    if (isFull()) {
      std::cout << "Queue is full" << std::endl;
    } else {
      if (front == -1)
        front = 0;
      rear = (rear + 1) % SIZE;
      items[rear] = element;
      std::cout << element << " inserted into queue" << std::endl;
    }
  }

  void dequeue() {
    if (isEmpty()) {
      std::cout << "Queue is empty" << std::endl;
    } else {
      std::cout << items[front] << " removed from queue" << std::endl;
      if (front == rear) {
        front = -1;
        rear = -1; // Queue is empty
      } else {
        front = (front + 1) % SIZE;
      }
    }
  }

  void display() {
    if (isEmpty()) {
      std::cout << "Queue is empty" << std::endl;
      return;
    }
    std::cout << "Queue elements are: ";
    if (rear >= front) {
      for (int i = front; i <= rear; i++)
        std::cout << items[i] << " ";
    } else {
      for (int i = front; i < SIZE; i++)
        std::cout << items[i] << " ";
      for (int i = 0; i <= rear; i++)
        std::cout << items[i] << " ";
    }
    std::cout << std::endl;
  }
};
