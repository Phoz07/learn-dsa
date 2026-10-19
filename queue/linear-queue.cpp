#include <iostream>
#define SIZE 5

class Queue {
  int items[SIZE], front, rear;

public:
  Queue() : front(-1), rear(-1) {}

  bool isFull() { return rear == SIZE - 1; }

  bool isEmpty() { return front == -1; }

  void enqueue(int element) {
    if (isFull()) {
      std::cout << "Queue is full" << std::endl;
    } else {
      if (front == -1)
        front = 0;
      items[++rear] = element;
      std::cout << element << " inserted" << std::endl;
    }
  }

  int dequeue() {
    if (isEmpty()) {
      std::cout << "Queue is empty" << std::endl;
      return -1;
    } else {
      int element = items[front];
      if (front >= rear) {
        front = -1;
        rear = -1;
      } else {
        front++;
      }
      return element;
    }
  }

  void display() {
    if (isEmpty()) {
      std::cout << "Queue is empty" << std::endl;
    } else {
      for (int i = front; i <= rear; i++)
        std::cout << items[i] << " ";
      std::cout << std::endl;
    }
  }
};

int main() {
  Queue q;

  q.enqueue(1);
  q.enqueue(2);
  q.enqueue(3);

  q.display();

  q.dequeue();
  q.display();

  return 0;
}
