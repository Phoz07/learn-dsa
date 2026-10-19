# เรียนรู้ `class` ใน C++ จาก `Person`

ตัวอย่างหลัก: [`example.cpp`](./example.cpp)

## เป้าหมาย

หลังอ่านบทนี้ คุณควรสามารถ:

- อธิบายความสัมพันธ์ระหว่าง class, object, data member และ member function ได้
- ใช้ `private` และ `public` เพื่อทำ encapsulation
- เขียน constructor และเข้าใจบทบาทของ `this`
- เลือกใช้ constructor initializer list, reference และ `const` ได้เหมาะสม
- เข้าใจ lifetime และลำดับการ initialize สมาชิกของ object
- compile และ run โปรแกรมตัวอย่างโดยเปิด warning ที่สำคัญ

## Mental model

ให้คิดว่า `class` คือ **แม่แบบที่กำหนดทั้งข้อมูลและพฤติกรรม** ส่วน object คือค่าจริงที่สร้างจากแม่แบบนั้น

ใน [`example.cpp`](./example.cpp) class `Person` กำหนดว่า Person หนึ่งคนมี:

- สถานะ: `name` และ `age`
- วิธีสร้าง: constructor `Person(string name, int age)`
- วิธีอ่านสถานะ: `getName()` และ `getAge()`
- พฤติกรรม: `print()`

`name` และ `age` เป็น `private` จึงเข้าถึงตรง ๆ จาก `main()` ไม่ได้ ผู้ใช้ class ต้องทำงานผ่าน public interface เช่น `print()` การซ่อนรายละเอียดเช่นนี้เรียกว่า **encapsulation** ซึ่งช่วยควบคุม invariant ของ object และลดการผูกติดกับรายละเอียดภายใน

เมื่อ `main()` ทำงาน:

1. `Person p("John", 30);` สร้าง object `p`
2. constructor กำหนด `name` และ `age`
3. `p.print();` อ่านสถานะภายในและแสดงผล
4. เมื่อออกจาก scope ของ `main()` object `p` ถูกทำลายโดยอัตโนมัติ

## Syntax สำคัญ

โครงสร้างย่อจากตัวอย่าง:

```cpp
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
  void print() { /* ... */ }
};
```

จุดที่ควรรู้:

- สมาชิกของ `class` เป็น `private` โดยปริยาย แต่โค้ดระบุ `private:` ไว้ชัดเจน
- `public:` คือ interface ที่โค้ดภายนอกเรียกใช้ได้
- constructor มีชื่อเดียวกับ class และไม่มี return type
- `this` เป็น pointer ไปยัง object ปัจจุบัน
- ใน `this->name = name` ด้านซ้ายคือ data member ส่วนด้านขวาคือ parameter
- ต้องมี `;` หลังปีกกาปิดของ class

ตัวอย่างใช้งาน:

```cpp
Person p("John", 30);
p.print();
```

โค้ดนี้สร้าง object บน stack ไม่ต้องเรียก `delete` เอง

## อธิบายโค้ดจริง

[`example.cpp`](./example.cpp) มีองค์ประกอบสำคัญดังนี้:

### Header และ namespace

```cpp
#include <iostream>
using namespace std;
```

`<iostream>` ให้ `cout` และ `endl` ส่วนโค้ดใช้ `string` ด้วย แต่ไม่ได้ include `<string>` โดยตรง แม้ compiler บางตัว compile ผ่านเพราะ standard-library headers include ต่อกันภายใน การพึ่งพา transitive include ไม่ portable ควรเขียนให้ชัดเจนว่า:

```cpp
#include <iostream>
#include <string>
```

สำหรับโค้ดที่นำไปใช้จริง ควรใช้ `std::string`, `std::cout` และ `std::endl` แทน `using namespace std;` เพื่อลดโอกาสชื่อชนกัน

### ข้อมูลแบบ private

```cpp
string name;
int age;
```

โค้ดภายนอกแก้ `p.name` หรือ `p.age` โดยตรงไม่ได้ การเปลี่ยนสถานะจึงควรผ่าน public member function ที่ class ควบคุมได้

### Constructor

```cpp
Person(string name, int age) {
  this->name = name;
  this->age = age;
}
```

โค้ดนี้ทำงานได้ แต่ data member ถูก initialize ก่อนเข้า constructor body แล้วจึงถูก assign ซ้ำ โดยเฉพาะ `name` จะถูกสร้างเป็น string ว่างก่อนรับค่าใหม่ แนวทางที่เหมาะกว่าคือ constructor initializer list:

```cpp
Person(const std::string& name, int age)
    : name(name), age(age) {}
```

รูปแบบนี้ initialize สมาชิกโดยตรง และ `const std::string&` หลีกเลี่ยงการ copy parameter ที่ไม่จำเป็น สำหรับกรณีที่ต้องการรับ ownership ของ string อาจรับ by value แล้ว `std::move` แต่ควรเลือกตามรูปแบบการใช้งานจริง

### Getter และ `print()`

```cpp
string getName() { return name; }
int getAge() { return age; }
void print() { /* ... */ }
```

getter ปัจจุบันเรียกได้เฉพาะ object ที่ไม่เป็น `const` และ `getName()` คืนสำเนา string รูปแบบที่เหมาะกับ read-only interface มากกว่าคือ:

```cpp
const std::string& getName() const noexcept { return name; }
int getAge() const noexcept { return age; }
void print() const {
  std::cout << "Name: " << name << '\n';
  std::cout << "Age: " << age << '\n';
}
```

`const` หลังวงเล็บหมายความว่า function ไม่แก้สถานะที่สังเกตได้ของ object ส่วนการคืน `const std::string&` หลีกเลี่ยงสำเนา แต่ reference นั้นใช้ได้ไม่เกิน lifetime ของ `Person` ต้นทาง

### `main()` และผลลัพธ์

```cpp
Person p("John", 30);
p.print();
```

ผลลัพธ์:

```text
Name: John
Age: 30
```

getter ถูกประกาศไว้แต่ยังไม่ได้ใช้ในตัวอย่างนี้

## เปรียบเทียบ `class` และ `struct`

ใน C++ ทั้ง `class` และ `struct` สามารถมี data members, methods, constructors, destructors, access specifiers, inheritance และ templates ได้เหมือนกัน ความต่างทางภาษาหลักคือค่าเริ่มต้นของ access:

| ประเด็น | `class` | `struct` |
|---|---|---|
| Member access โดยปริยาย | `private` | `public` |
| Base-class inheritance โดยปริยาย | `private` | `public` |
| มี constructor/method ได้หรือไม่ | ได้ | ได้ |
| มี `private`/`protected` ได้หรือไม่ | ได้ | ได้ |

แนวปฏิบัติทั่วไป:

- ใช้ `class` เมื่อ type มี invariant หรือพฤติกรรมที่ต้องควบคุมผ่าน interface
- ใช้ `struct` เมื่อเป็นกลุ่มข้อมูลที่สมาชิกควรเข้าถึงได้ตรง ๆ และไม่มี invariant ซับซ้อน

นี่เป็น convention ไม่ใช่ข้อจำกัดของภาษา `struct` ก็ทำ encapsulation ได้ และ `class` ก็มี public data ได้

## Practical best practices

### Include สิ่งที่ใช้โดยตรง

```cpp
#include <iostream>
#include <string>
```

อย่าพึ่งว่า `<iostream>` จะทำให้ `std::string` พร้อมใช้เสมอ

### ใช้ initializer list

```cpp
Person(const std::string& name, int age)
    : name(name), age(age) {}
```

นอกจากมีประสิทธิภาพกว่าแล้ว สมาชิกบางชนิด เช่น reference, `const` member หรือ type ที่ไม่มี default constructor จำเป็นต้อง initialize ด้วยวิธีนี้

### รักษา invariant

โค้ดปัจจุบันยอมรับอายุติดลบ หาก domain ไม่อนุญาต ควรตรวจใน constructor หรือใช้ factory ที่สามารถรายงานข้อผิดพลาดได้ เป้าหมายคือ object ที่สร้างสำเร็จควรอยู่ในสถานะที่ถูกต้องเสมอ

### ใช้ const correctness

- method ที่ไม่แก้ object ควรเป็น `const`
- parameter ที่อ่านอย่างเดียวและมีขนาดไม่เล็กควรพิจารณา `const T&`
- ระวังการคืน reference เพราะ lifetime ของ reference ผูกกับ object ต้นทาง

### หลีกเลี่ยง `using namespace std;` ในโค้ดที่ขยายต่อ

การระบุ `std::` ทำให้รู้ว่า symbol มาจาก standard library และลด name collision โดยเฉพาะใน header ไม่ควรใช้ `using namespace std;`

### ใช้ newline ให้เหมาะสม

`std::endl` ขึ้นบรรทัดใหม่และ flush stream ทุกครั้ง หากไม่จำเป็นต้อง flush ให้ใช้ `'\n'` ซึ่งมักเหมาะกว่า

## Lifetime, initialization และ const correctness

### Lifetime ของ `p`

`p` เป็น automatic object ซึ่งเริ่ม lifetime เมื่อ execution มาถึง declaration และสิ้นสุดเมื่อออกจาก scope ของ `main()` สมาชิกถูกทำลายอัตโนมัติ จึงไม่ควรใช้ pointer หรือ reference ไปยัง `p` หลังจากนั้น

### ลำดับ initialization

สมาชิกถูก initialize ตาม **ลำดับที่ประกาศใน class** ไม่ใช่ลำดับที่เขียนใน initializer list ในตัวอย่างคือ `name` ก่อน `age` การเขียน initializer list ตามลำดับประกาศช่วยหลีกเลี่ยง warning และความสับสน

### Copy และ destruction

`Person` ไม่มี resource ดิบและ `std::string` จัดการหน่วยความจำเอง compiler-generated copy/move/destructor จึงเพียงพอในตัวอย่างนี้ แนวคิดนี้สอดคล้องกับ Rule of Zero: หากใช้ resource-managing types อยู่แล้ว มักไม่ต้องเขียน destructor หรือ copy operations เอง

### Const object

ด้วย method ปัจจุบัน โค้ดต่อไปนี้เรียก getter หรือ `print()` ไม่ได้:

```cpp
const Person p("John", 30);
// p.print(); // method ปัจจุบันไม่ได้ประกาศ const
```

การเพิ่ม `const` ให้ read-only methods ทำให้ API ใช้ได้กว้างขึ้นและสื่อเจตนาได้ชัดเจน

## ข้อควรระวัง

- ตัวอย่างใช้ `std::string` โดยไม่ได้ include `<string>` โดยตรง
- constructor ปัจจุบันใช้ assignment ใน body แทน initializer list
- ไม่มีการตรวจว่า `age` สมเหตุสมผลหรือไม่
- `getName()` คืนสำเนา ซึ่งอาจไม่จำเป็นสำหรับ string ขนาดใหญ่
- methods ที่อ่านอย่างเดียวยังไม่ได้ประกาศ `const`
- อย่าคืน reference ไปยัง local variable และอย่าเก็บ reference ไปยัง object ที่หมด lifetime แล้ว
- ถ้าเพิ่ม raw pointer หรือ resource ที่ต้อง release เอง ต้องพิจารณา ownership และ Rule of Five; ทางที่ดีกว่าคือใช้ RAII types เช่น `std::string`, containers และ smart pointers

## Compile และ run จาก root `dsa`

ให้รันคำสั่งต่อไปนี้ขณะ working directory คือ root `dsa`:

```sh
mkdir -p build/class
g++ -std=c++17 -Wall -Wextra -Wpedantic class/example.cpp -o build/class/example
./build/class/example
```

ผลลัพธ์ที่คาดหวัง:

```text
Name: John
Age: 30
```

การตั้ง output เป็น `build/class/example` ช่วยไม่ให้ binary ปะปนกับ source และหลีกเลี่ยงการใช้ชื่อเริ่มต้น `a.out`

## แบบฝึกหัด

1. เพิ่ม `#include <string>` และเปลี่ยนชื่อทั้งหมดให้ใช้ `std::` โดยไม่ใช้ `using namespace std;`
2. เปลี่ยน constructor ให้ใช้ initializer list และรับชื่อเป็น `const std::string&`
3. ทำ `getName()`, `getAge()` และ `print()` ให้เป็น `const` แล้วทดลองสร้าง `const Person`
4. เพิ่ม validation ที่ไม่อนุญาตให้อายุติดลบ และตัดสินใจว่าจะรายงานข้อผิดพลาดด้วย exception, optional factory หรือวิธีอื่น
5. เพิ่ม `setAge()` ที่รักษา invariant เดียวกับ constructor แทนการเปิด `age` เป็น public
6. ทดลองพิมพ์ชื่อผ่าน `getName()` และอธิบายความต่างระหว่างคืนค่าแบบ value กับ `const&`
7. เพิ่ม Person หลายคนใน `std::vector<Person>` แล้วใช้ range-based `for` แบบ `const auto&` เพื่อแสดงข้อมูล
8. ทดลองเปลี่ยนลำดับสมาชิกและ initializer list เปิด warning แล้วสังเกตผล เพื่อยืนยันว่าลำดับสร้างจริงขึ้นกับลำดับประกาศ
