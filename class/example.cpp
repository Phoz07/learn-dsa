#include <iostream>
using namespace std;

class Person {
private:
  string name;
  int age;

public:
  Person(string name, int age) {
    this->name = name;
    this->age = age;
  }

  string getName() { return name; }

  int getAge() { return age; }

  void print() {
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
  }
};

int main() {
  Person p("John", 30);
  p.print();
  return 0;
}
