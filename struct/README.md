# เรียนรู้ `struct` และกลุ่มข้อมูลใน C++ จาก `Student`

ตัวอย่างหลัก: [`example.cpp`](./example.cpp)

## เป้าหมาย

หลังอ่านบทนี้ คุณควรสามารถ:

- สร้าง type แบบ `struct` และเข้าถึงสมาชิกด้วย `.` ได้
- เข้าใจ aggregate, array ของ object และการใช้ index
- แยก default initialization, value initialization และ aggregate initialization ได้
- ระบุได้ว่าสมาชิกใดในตัวอย่างถูก initialize และสมาชิกใดยังมีค่าไม่แน่นอน
- เลือกระหว่าง raw array, `std::array` และ `std::vector` ตามโจทย์จริง
- ใช้ bounds checking, `const` และ reference เพื่อลดข้อผิดพลาด
- compile และ run โปรแกรมตัวอย่างโดยเปิด warning ที่สำคัญ

## Mental model

ให้คิดว่า `struct Student` คือ **ระเบียนข้อมูลหนึ่งรายการ** ที่รวมข้อมูลซึ่งเป็นของนักเรียนคนเดียวกัน:

- `name`: ชื่อ
- `age`: อายุ
- `gpa`: เกรดเฉลี่ย

ส่วน `Student s[10];` คือกล่องที่มี `Student` ต่อเนื่องกัน 10 ช่อง ใช้ index `0` ถึง `9`:

```text
s[0]  s[1]  s[2]  ...  s[9]
```

สมาชิกของ `struct` เป็น `public` โดยปริยาย จึงอ่านหรือเขียนตรง ๆ ได้ เช่น:

```cpp
s[0].age = 20;
```

ความเรียบง่ายนี้เหมาะกับ data record ที่ไม่มี invariant ซับซ้อน แต่หมายความว่าโค้ดทุกส่วนสามารถใส่ค่าที่ไม่สมเหตุสมผลได้ เช่น อายุเป็นลบหรือ GPA เกินช่วง หากต้องควบคุมกฎเหล่านี้ ควรเพิ่มฟังก์ชันตรวจสอบหรือพิจารณาใช้ class ที่ซ่อนสถานะ

## Syntax สำคัญ

โครงสร้างจาก [`example.cpp`](./example.cpp):

```cpp
struct Student {
  string name;
  int age;
  float gpa;
};
```

จุดที่ควรรู้:

- `struct` สร้าง user-defined type
- สมาชิกเป็น `public` โดยปริยาย
- ต้องมี `;` หลังปีกกาปิดของ struct
- แต่ละ object มี `name`, `age` และ `gpa` ของตัวเอง

การสร้าง raw array:

```cpp
Student s[10];
```

การเข้าถึงสมาชิกของ element แรก:

```cpp
s[0].name = "";
s[0].age = 0;
s[0].gpa = 0.0;
```

`[]` เลือก element ใน array ก่อน แล้ว `.` เลือก member ภายใน `Student`

## อธิบายโค้ดจริง

### Header และ namespace

```cpp
#include <iostream>
using namespace std;
```

ตัวอย่างใช้ `string` แต่ไม่ได้ include `<string>` โดยตรง แม้ compiler บางตัว compile ผ่านเพราะ header ภายใน include ต่อกันมา โค้ดที่ portable ควร include สิ่งที่ใช้โดยตรง:

```cpp
#include <iostream>
#include <string>
```

สำหรับโค้ดที่นำไปใช้จริง ควรเขียน `std::string` และ `std::cout` แทน `using namespace std;` เพื่อลดโอกาสชื่อชนกัน

### นิยาม `Student`

```cpp
struct Student {
  string name;
  int age;
  float gpa;
};
```

`Student` เป็น aggregate ตามรูปแบบปัจจุบัน จึงสามารถ aggregate-initialize ได้ เช่น:

```cpp
Student alice{"Alice", 20, 3.75f};
```

ค่าถูกจับคู่ตามลำดับที่สมาชิกประกาศ: `name`, `age`, `gpa`

### การสร้าง array

```cpp
Student s[10];
```

คำสั่งนี้ default-initialize `Student` จำนวน 10 ตัว ผลที่สำคัญคือ:

- `name` ของแต่ละ element เป็น `std::string` จึงถูกสร้างอย่างถูกต้องและเริ่มเป็น string ว่าง
- `age` และ `gpa` เป็น fundamental types จึงไม่มีค่าเริ่มต้นที่แน่นอนจาก declaration นี้
- โค้ดกำหนดค่าครบเฉพาะ `s[0]`
- `age` และ `gpa` ของ `s[1]` ถึง `s[9]` ยังไม่ควรถูกอ่าน

ถ้าต้องการ value-initialize ทุก element ให้ใช้:

```cpp
Student s[10]{};
```

รูปแบบนี้ทำให้ string ว่าง, `age` เป็น `0` และ `gpa` เป็น `0.0f` สำหรับทุก element

### การกำหนดค่า element แรก

```cpp
s[0].name = "";
s[0].age = 0;
s[0].gpa = 0.0;
```

- ชื่อถูกกำหนดเป็น empty string จึงไม่มีข้อความตามหลัง `Name:`
- `0.0` มีชนิดเป็น `double` แล้วแปลงเป็น `float`; ใช้ `0.0f` ได้หากต้องการ literal ชนิด `float` โดยตรง
- การ assign ทั้งสามสมาชิกทำให้ `s[0]` พร้อมอ่าน

### การแสดงผล

โปรแกรมแสดงเฉพาะ `s[0]`:

```text
Name: 
Age: 0
GPA: 0
```

แม้กำหนด GPA เป็น `0.0` แต่ default formatting ของ `cout` แสดงเป็น `0` หากต้องการจำนวนตำแหน่งทศนิยมคงที่ สามารถใช้ `<iomanip>`, `std::fixed` และ `std::setprecision`

## เปรียบเทียบ `class` และ `struct`

ใน C++ ทั้งสองชนิดมีความสามารถเกือบเหมือนกัน:

| ประเด็น | `struct` | `class` |
|---|---|---|
| Member access โดยปริยาย | `public` | `private` |
| Base-class inheritance โดยปริยาย | `public` | `private` |
| มี methods/constructors/destructor ได้หรือไม่ | ได้ | ได้ |
| กำหนด `public`, `private`, `protected` ได้หรือไม่ | ได้ | ได้ |

ดังนั้น `struct` ไม่ได้หมายถึง “ข้อมูลอย่างเดียวเสมอ” และ `class` ไม่ได้หมายถึง “มี method เท่านั้น” ความแตกต่างที่ใช้ตัดสินใจในงานจริงมักเป็น convention:

- ใช้ `struct` กับข้อมูลเรียบง่ายที่ต้องการเปิดสมาชิกให้เข้าถึงตรง ๆ
- ใช้ `class` เมื่อจำเป็นต้องซ่อนสถานะและรักษา invariant ผ่าน public interface

หาก `Student` ต้องรับประกันว่า `age >= 0` และ GPA อยู่ในช่วงที่กำหนด การปล่อยสมาชิกเป็น public ทำให้รักษากฎได้ยากขึ้น อาจเพิ่ม validation หรือเปลี่ยนการออกแบบเป็น class

## Practical best practices

### Include สิ่งที่ใช้โดยตรง

```cpp
#include <iostream>
#include <string>
```

หากใช้ `std::array` หรือ `std::vector` ให้ include `<array>` หรือ `<vector>` โดยตรงเช่นกัน

### Initialize object ให้ชัดเจน

สร้างหนึ่งรายการด้วย aggregate initialization:

```cpp
Student student{"Mali", 19, 3.50f};
```

สร้าง raw array ที่ทุก element ถูก value-initialize:

```cpp
Student students[10]{};
```

การใช้ `{}` ช่วยป้องกันการอ่านค่า `int` หรือ `float` ที่ยังไม่ถูกกำหนด

### ใช้ `std::array` เมื่อขนาดคงที่

```cpp
#include <array>

std::array<Student, 10> students{};
students[0] = {"Mali", 19, 3.50f};
```

ข้อดีเหนือ raw array:

- มี `.size()`
- ใช้ `.at(index)` เมื่อต้องการ bounds checking
- copy/assign ทั้ง container ได้
- ทำงานกับ standard algorithms และ range-based `for` ได้สะดวก

`operator[]` ของ `std::array` ยังไม่ตรวจ bounds เช่นเดียวกับ raw array ส่วน `.at()` จะรายงาน index ที่เกินขอบเขตด้วย exception

### ใช้ `std::vector` เมื่อขนาดเปลี่ยนได้

```cpp
#include <vector>

std::vector<Student> students;
students.push_back({"Mali", 19, 3.50f});
students.push_back({"Niran", 20, 3.25f});
```

เลือก `std::vector` เมื่อจำนวนข้อมูลทราบตอน runtime หรือต้องเพิ่มและลบรายการ ระวังว่าเมื่อ vector ขยายและ reallocate pointer/reference/iterator ที่ชี้ element เดิมอาจใช้ต่อไม่ได้

### ส่ง object ด้วย const reference

ฟังก์ชันที่อ่าน Student โดยไม่แก้ไขควรรับ `const Student&`:

```cpp
void printStudent(const Student& student) {
  std::cout << student.name << '\n';
}
```

รูปแบบนี้ไม่ copy ทั้ง object และสื่อว่า function จะไม่แก้ค่า หากเพิ่ม member function ที่อ่านอย่างเดียว ก็ควรประกาศ `const` เช่นกัน

### ใช้ชื่อที่บอกความหมาย

ในตัวอย่าง `s` เหมาะกับ demo สั้น ๆ แต่โค้ดจริงควรใช้ชื่ออย่าง `students` หรือ `studentRecords` เพื่อให้ผู้อ่านเข้าใจเจตนา

## Lifetime, initialization และ const correctness

### Lifetime ของ raw array

`Student s[10];` เป็น automatic array ภายใน `main()`:

- element ทั้งสิบเริ่ม lifetime เมื่อ execution มาถึง declaration
- element ถูกสร้างตามลำดับ index
- เมื่อออกจาก scope จะถูกทำลายในลำดับย้อนกลับ
- `std::string` ภายในแต่ละ element จัดการหน่วยความจำของตัวเอง ไม่ต้องเรียก `delete`

ห้ามคืน pointer หรือ reference ไปยัง element ของ local array แล้วนำไปใช้หลัง function จบ เพราะ array หมด lifetime แล้ว

### Default initialization กับ value initialization

```cpp
Student a[10];   // age และ gpa ยังไม่มีค่าที่แน่นอน
Student b[10]{}; // age == 0 และ gpa == 0.0f ทุก element
```

นี่เป็นความต่างสำคัญของตัวอย่าง การที่ `s[0]` ปลอดภัยเกิดจากมีการ assign ครบก่อนอ่าน ไม่ได้หมายความว่า element อื่นถูกตั้งเป็นศูนย์โดยอัตโนมัติ

### Const correctness

ใช้ `const` เมื่อข้อมูลไม่ควรถูกแก้:

```cpp
const Student student{"Mali", 19, 3.50f};
// student.age = 20; // ทำไม่ได้
```

เมื่อวนอ่าน container ให้หลีกเลี่ยงการ copy:

```cpp
for (const auto& student : students) {
  // อ่าน student โดยไม่แก้และไม่ copy
}
```

หากต้องแก้ element ให้ใช้ `auto&` โดยไม่ใส่ `const`

## Bounds และข้อควรระวัง

### ขอบเขตของ array

สำหรับ `Student s[10]` index ที่ถูกต้องคือ `0` ถึง `9` เท่านั้น:

```cpp
s[9].age = 21;  // ถูกต้อง
// s[10].age = 21; // เกินขอบเขต: undefined behavior
```

Raw array ไม่ตรวจ bounds และการเข้าถึงเกินขอบเขตอาจทำให้ข้อมูลเสียหาย โปรแกรม crash หรือดูเหมือนทำงานได้แต่ผิด การตรวจด้วยตาไม่เพียงพอเมื่อ index มาจาก input ควรตรวจเงื่อนไขหรือใช้ `.at()` ของ `std::array`/`std::vector`

### สมาชิกที่ยังไม่ initialize

อย่าอ่าน `s[1].age` หรือ `s[1].gpa` จากโค้ดปัจจุบัน เพราะยังไม่ได้กำหนดค่า การเปิด `-Wall -Wextra` มีประโยชน์แต่ compiler อาจไม่ตรวจพบทุกเส้นทางที่อ่านค่าไม่พร้อมใช้ จึงต้องออกแบบ initialization ให้ชัดเจน

### ช่วงค่าของข้อมูล

โค้ดไม่มี validation จึงสามารถเก็บอายุติดลบหรือ GPA ที่อยู่นอกช่วงจริงได้ หากข้อมูลมาจากผู้ใช้หรือไฟล์ ควรตรวจสอบก่อนสร้างหรือแก้ Student

### Precision และการแสดง `float`

`float` มี precision จำกัด หากต้องคำนวณสะสมหรือจำเป็นต้องแม่นยำมากขึ้น ให้พิจารณา `double` ส่วนการกำหนดจำนวนหลักที่แสดงเป็นเรื่อง formatting ไม่ได้เพิ่ม precision ของค่าที่เก็บอยู่

## Compile และ run จาก root `dsa`

ให้รันคำสั่งต่อไปนี้ขณะ working directory คือ root `dsa`:

```sh
mkdir -p build/struct
g++ -std=c++17 -Wall -Wextra -Wpedantic struct/example.cpp -o build/struct/example
./build/struct/example
```

ผลลัพธ์ที่คาดหวัง:

```text
Name: 
Age: 0
GPA: 0
```

บรรทัด `Name:` ว่างตามค่าจริงในโค้ด และ `GPA` แสดงเป็น `0` ตาม default stream formatting การตั้ง output เป็น `build/struct/example` ช่วยแยก binary ออกจาก source

## แบบฝึกหัด

1. เพิ่ม `#include <string>` และเปลี่ยนโค้ดให้ใช้ `std::` โดยไม่ใช้ `using namespace std;`
2. เปลี่ยน `Student s[10];` เป็น `Student s[10]{};` แล้วอธิบายว่าสมาชิกทุกตัวเริ่มต้นต่างจากเดิมอย่างไร
3. สร้าง Student สามคนด้วย aggregate initialization และพิมพ์ด้วย range-based `for`
4. เปลี่ยน raw array เป็น `std::array<Student, 10>` แล้วทดลอง `.size()`, `operator[]` และ `.at()`
5. เปลี่ยนเป็น `std::vector<Student>` รับจำนวนและข้อมูลนักเรียนจากผู้ใช้ แล้วเพิ่มข้อมูลด้วย `push_back`
6. เขียน `printStudent(const Student&)` และอธิบายว่าเหตุใดจึงใช้ `const&`
7. ตรวจว่า `age` ไม่ติดลบและ GPA อยู่ในช่วงที่กำหนดก่อนเก็บข้อมูล
8. ใช้ `std::fixed << std::setprecision(2)` เพื่อแสดง GPA สองตำแหน่ง และสังเกตว่าการจัดรูปแบบต่างจาก precision ของชนิดข้อมูลอย่างไร
9. ทดลองเข้าถึง index จาก input โดยตรวจ `index < students.size()` ก่อน และเปรียบเทียบกับการใช้ `.at(index)`
