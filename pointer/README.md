# Pointer ใน C++

ตัวอย่างในไดเรกทอรีนี้สาธิตพื้นฐานของ pointer ผ่านโปรแกรมสั้น ๆ ที่แต่ละไฟล์มี `main()` ของตนเอง ควร compile และ run ทีละไฟล์ ไม่ควร compile ทุกไฟล์รวมเป็น executable เดียว เพราะจะมี `main()` ซ้ำกัน

## เป้าหมาย

เมื่อเรียนจบตัวอย่างชุดนี้ ควรสามารถ:

- อธิบายความต่างระหว่าง “ค่า”, “address” และ “pointer” ได้
- ใช้ `&` เพื่อหา address และใช้ `*` เพื่อ dereference pointer
- แก้ค่าของตัวแปรต้นทางผ่าน pointer
- เข้าใจความสัมพันธ์ระหว่าง array กับ pointer arithmetic
- อ่าน syntax ของ function pointer และ pointer-to-pointer ได้
- ระบุความเสี่ยงเรื่อง lifetime, null pointer, dangling pointer และการเข้าถึงนอกขอบเขตได้

## Mental model

ให้คิดว่าตัวแปรเป็นกล่องข้อมูลที่มีตำแหน่งในหน่วยความจำ และ pointer เป็นอีกกล่องหนึ่งที่เก็บตำแหน่งนั้น

```cpp
int value = 10;
int* ptr = &value;
```

- `value` คือค่าจำนวนเต็ม `10`
- `&value` คือ address ของ `value`
- `ptr` เก็บ address ของ `value`
- `*ptr` คือค่าที่อยู่ ณ address ที่ `ptr` ชี้ไป

ดังนั้นการเขียนผ่าน pointer จะเปลี่ยนตัวแปรต้นทาง:

```cpp
*ptr = 20;  // value กลายเป็น 20
```

Pointer ไม่ได้เป็นเจ้าของ object ที่ชี้อยู่โดยอัตโนมัติ ผู้เขียนโปรแกรมต้องมั่นใจว่า object นั้นยังมีชีวิตอยู่และ pointer ชี้ไปยังตำแหน่งที่ถูกต้องก่อน dereference

## Syntax และ core operations

| Syntax | ความหมาย |
|---|---|
| `int* ptr;` | ประกาศ pointer ที่ชี้ไปยัง `int` |
| `&value` | หา address ของ `value` |
| `*ptr` | อ่านหรือเขียนค่าที่ `ptr` ชี้อยู่ |
| `ptr + i` | เลื่อนไปยังสมาชิกที่ห่างออกไป `i` ตำแหน่งภายใน array เดียวกัน |
| `int** ptr2` | pointer ที่ชี้ไปยัง `int*` อีกที |
| `void (*fn)(int)` | function pointer ที่รับ `int` และคืน `void` |
| `nullptr` | ค่า null pointer มาตรฐานของ C++ |

ตัวอย่างการตรวจ null ก่อนใช้งาน:

```cpp
int* ptr = nullptr;

if (ptr != nullptr) {
  std::cout << *ptr << '\n';
}
```

Function pointer ต้องมี signature ที่เข้ากันได้กับ function:

```cpp
void printNumber(int number) {
  std::cout << number << '\n';
}

void (*callback)(int) = printNumber;
callback(42);
```

## Complexity

ตัวอย่างทั้งหมดทำงานกับข้อมูลจำนวนคงที่ จึงมี time complexity **O(1)** และใช้พื้นที่เพิ่มเติม **O(1)**

| Operation | Time |
|---|---:|
| อ่านหรือเขียนผ่าน pointer | O(1) |
| หา address ด้วย `&` | O(1) |
| pointer arithmetic หนึ่งครั้ง | O(1) |
| เรียก function ผ่าน function pointer | O(1) บวกเวลาของ function ที่ถูกเรียก |

Pointer arithmetic ไม่ได้ตรวจขอบเขตให้ แม้ operation จะเป็น O(1) ก็อาจเกิด undefined behavior หากตำแหน่งไม่ถูกต้อง

## อธิบายแต่ละไฟล์

### [`ptr.cpp`](./ptr.cpp)

สาธิต pointer พื้นฐาน:

1. สร้าง `num` ที่มีค่า `10`
2. ให้ `ptr` ชี้ไปยัง `num`
3. อ่านค่าผ่าน `*ptr`
4. เขียน `20` ผ่าน `*ptr` ทำให้ `num` เปลี่ยนตาม

Function สำคัญ: `main()`

ผลลัพธ์:

```text
10
20
```

### [`ptrarray.cpp`](./ptrarray.cpp)

สาธิตว่า array จะ decay เป็น pointer ไปยังสมาชิกแรกใน expression ส่วนใหญ่:

```cpp
int* arrPtr = arr;
```

- `*arrPtr` อ่านสมาชิกตำแหน่ง `0`
- `*(arrPtr + 1)` อ่านสมาชิกตำแหน่ง `1`

Function สำคัญ: `main()`

ผลลัพธ์:

```text
1
2
```

Pointer ไม่ได้เก็บขนาดของ array ไว้ จึงต้องจัดการ bounds แยกต่างหาก

### [`ptrfunc.cpp`](./ptrfunc.cpp)

สาธิต function pointer:

```cpp
void (*functionPtr)(int) = myFunction;
functionPtr(42);
```

Functions สำคัญ:

- `myFunction(int)` รับจำนวนเต็มและพิมพ์ออกมา
- `main()` สร้างและเรียก function pointer

ผลลัพธ์:

```text
42
```

ชื่อ function สามารถแปลงเป็น function pointer ได้โดยไม่จำเป็นต้องเขียน `&myFunction`

### [`ptrtoptr.cpp`](./ptrtoptr.cpp)

สาธิต pointer สองระดับ:

```cpp
int* ptr = &var;
int** ptr2 = &ptr;
```

- `ptr2` ชี้ไปยัง `ptr`
- `*ptr2` คือ `ptr`
- `**ptr2` คือค่าของ `var`

Function สำคัญ: `main()`

ผลลัพธ์:

```text
10
```

## Practical patterns

### ใช้ reference เมื่อไม่ต้องการความหมายแบบ nullable

หาก parameter จำเป็นต้องอ้างถึง object ที่มีอยู่เสมอ reference มักสื่อเจตนาได้ชัดกว่า pointer:

```cpp
void increment(int& value) {
  ++value;
}
```

ใช้ pointer เมื่อ `nullptr` มีความหมาย หรือเมื่อต้องทำงานกับ API/โครงสร้างข้อมูลที่ต้องใช้ address โดยตรง

### ส่งขนาดคู่กับ raw array

```cpp
void printAll(const int* data, std::size_t size) {
  for (std::size_t i = 0; i < size; ++i) {
    std::cout << data[i] << '\n';
  }
}
```

`data` เพียงอย่างเดียวไม่สามารถบอกจำนวนสมาชิกได้ ในโค้ดสมัยใหม่อาจใช้ `std::array`, `std::vector` หรือ `std::span` (C++20) เพื่อเก็บข้อมูลขอบเขตให้ชัดเจนขึ้น

### เริ่มต้น pointer ด้วยค่าที่ทราบแน่นอน

```cpp
int* ptr = nullptr;
```

หลีกเลี่ยง pointer ที่ไม่ได้ initialize เพราะค่าภายในไม่สามารถนำไป dereference อย่างปลอดภัยได้

## Lifetime, bounds และ undefined behavior

### Lifetime และ dangling pointer

```cpp
int* ptr = nullptr;
{
  int local = 10;
  ptr = &local;
}
// local หมด lifetime แล้ว; ห้ามใช้ *ptr
```

หลังออกจาก block `ptr` ยังเก็บ address เดิม แต่ object ที่ address นั้นไม่มีชีวิตอยู่แล้ว การ dereference เป็น undefined behavior

### Null pointer

การ dereference `nullptr` เป็น undefined behavior:

```cpp
int* ptr = nullptr;
// std::cout << *ptr;  // ห้ามทำ
```

### Array bounds

สำหรับ array ขนาด 5 สามารถคำนวณ pointer ตั้งแต่สมาชิกแรกจนถึง one-past-the-end ได้ แต่ one-past ใช้เปรียบเทียบได้เท่านั้น ห้าม dereference:

```cpp
int values[5]{};
int* end = values + 5;  // คำนวณได้
// int x = *end;        // undefined behavior
```

### หลายระดับของ pointer

ก่อนใช้ `**ptr2` ต้องมั่นใจทั้งสองระดับว่า valid: `ptr2` ต้องไม่เป็น null และ `*ptr2` ต้องเป็น pointer ที่ชี้ object ซึ่งยังมีชีวิตอยู่

### ข้อจำกัดของโค้ดชุดนี้

- ตัวอย่างทั้งหมดใช้ค่าคงที่และ happy path ไม่มี input จากผู้ใช้
- ไม่มีตัวอย่าง `nullptr`, dangling pointer หรือ dynamic allocation
- [`ptrarray.cpp`](./ptrarray.cpp) ไม่ตรวจ bounds หากเพิ่ม offset ผิดจะเกิด undefined behavior
- [`ptrfunc.cpp`](./ptrfunc.cpp) สมมติว่า function pointer ชี้ function ที่ valid ก่อนเรียก
- [`ptrtoptr.cpp`](./ptrtoptr.cpp) สมมติว่า pointer ทั้งสองระดับ valid
- ตัวอย่างใช้ `using namespace std;` เพื่อความสั้น แต่ควรหลีกเลี่ยงใน header และโค้ดขนาดใหญ่เพื่อลดปัญหาชื่อชนกัน

## Compile และ run จาก root `dsa`

คำสั่งต่อไปนี้ต้องรันขณะที่ current directory คือ root `dsa`:

```bash
mkdir -p build/pointer

g++ -std=c++17 -Wall -Wextra -Wpedantic pointer/ptr.cpp -o build/pointer/ptr
./build/pointer/ptr

g++ -std=c++17 -Wall -Wextra -Wpedantic pointer/ptrarray.cpp -o build/pointer/ptrarray
./build/pointer/ptrarray

g++ -std=c++17 -Wall -Wextra -Wpedantic pointer/ptrfunc.cpp -o build/pointer/ptrfunc
./build/pointer/ptrfunc

g++ -std=c++17 -Wall -Wextra -Wpedantic pointer/ptrtoptr.cpp -o build/pointer/ptrtoptr
./build/pointer/ptrtoptr
```

Compile แต่ละไฟล์แยกกัน เพราะทุกไฟล์มี `main()` การใช้ `g++ pointer/*.cpp` เพื่อสร้าง executable เดียวจะเกิด linker error จากการมี `main()` หลายตัว

เมื่อต้องการตรวจปัญหาหน่วยความจำเพิ่มเติม สามารถใช้ sanitizer ได้:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g pointer/ptrarray.cpp -o build/pointer/ptrarray-sanitized
./build/pointer/ptrarray-sanitized
```

## แบบฝึกหัด

1. แก้ [`ptr.cpp`](./ptr.cpp) ให้พิมพ์ทั้ง `num`, `&num`, `ptr` และ `&ptr` แล้วอธิบายว่าค่าใดควรเท่ากัน
2. เพิ่ม loop ใน [`ptrarray.cpp`](./ptrarray.cpp) เพื่อพิมพ์สมาชิกทั้งห้าด้วย pointer arithmetic โดยไม่ออกนอก bounds
3. สร้าง function `swapValues(int* left, int* right)` พร้อมตรวจ `nullptr` ก่อนสลับค่า
4. เพิ่ม function อีกตัวที่มี signature `void(int)` แล้วเลือก callback ที่จะเรียกจาก function pointer
5. ทดลองเปลี่ยนค่า `ptr` ผ่าน `ptr2` ใน [`ptrtoptr.cpp`](./ptrtoptr.cpp) ให้ไปชี้ตัวแปร `int` อีกตัว
6. สร้างตัวอย่าง dangling pointer ในโปรแกรมทดลอง รันด้วย `-fsanitize=address,undefined` และสังเกตข้อความผิดพลาด โดยอย่านำรูปแบบนั้นไปใช้ในโค้ดจริง
