#include <iostream>
using namespace std;

// Define a struct
struct Student {
  string name;
  int age;
  float gpa;
};

int main() {
  Student s[10];
  s[0].name = "";
  s[0].age = 0;
  s[0].gpa = 0.0;

  cout << "Name: " << s[0].name << endl;
  cout << "Age: " << s[0].age << endl;
  cout << "GPA: " << s[0].gpa << endl;
  
  return 0;
}
