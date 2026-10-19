# `std::vector` ใน C++

ตัวอย่างในไดเรกทอรีนี้สาธิตการสร้าง แก้ไข เดินข้อมูล และส่ง `std::vector<int>` เข้า function ตามโค้ดจริงในแต่ละไฟล์ แต่ละไฟล์ที่ไม่ว่างมี `main()` ของตนเอง จึงควร compile แยกกัน

## เป้าหมาย

เมื่อเรียนจบตัวอย่างชุดนี้ ควรสามารถ:

- อธิบายว่า `std::vector` เป็น dynamic contiguous array ได้
- แยกความหมายของ `size()` และ `capacity()`
- ใช้ `push_back()`, `pop_back()`, `resize()`, `data()`, `begin()` และ `end()`
- เลือกใช้ `operator[]` หรือ `at()` ตามระดับความปลอดภัยที่ต้องการ
- ส่ง vector ด้วย reference และ `const` reference เพื่อลดการ copy
- เข้าใจ complexity ของ operation หลัก
- ระบุได้ว่า operation ใดทำให้ iterator, pointer และ reference invalid
- หลีกเลี่ยง signed/unsigned warning เมื่อนำ index ไปเทียบกับ `size()`

## Mental model

`std::vector<T>` เป็นเจ้าของชุด object ชนิด `T` ที่เรียงต่อกันในหน่วยความจำ และสามารถขยายหรือลดจำนวนสมาชิกได้

```text
size      = จำนวนสมาชิกที่ใช้งานอยู่
capacity  = จำนวนสมาชิกที่ storage ปัจจุบันรองรับได้ก่อนต้องจัดสรรใหม่
```

เงื่อนไขสำคัญคือ:

```text
0 <= size <= capacity
```

ตัวอย่าง:

```cpp
std::vector<int> values;
values.reserve(10);  // capacity อย่างน้อย 10 แต่ size ยังเป็น 0
values.push_back(7); // size เป็น 1
```

เมื่อเพิ่มข้อมูลจนเกิน `capacity()` vector ต้องจัดสรร storage ใหม่และย้ายหรือ copy สมาชิกเดิม การย้าย storage นี้ทำให้ pointer, reference และ iterator ที่ชี้ storage เดิมใช้ต่อไม่ได้

สมาชิกของ vector อยู่ต่อเนื่องกัน จึงเข้าถึง raw storage ได้ด้วย `data()`:

```cpp
int* first = values.data();
```

Pointer นี้ไม่ได้เป็นเจ้าของข้อมูลและมีอายุการใช้งานขึ้นกับ vector

## Syntax และ core APIs

| API | ความหมาย | Complexity โดยทั่วไป |
|---|---|---:|
| `vec.size()` | จำนวนสมาชิกปัจจุบัน | O(1) |
| `vec.capacity()` | จำนวนสมาชิกที่รองรับก่อน reallocate | O(1) |
| `vec.empty()` | ตรวจว่าไม่มีสมาชิก | O(1) |
| `vec[i]` | เข้าถึงสมาชิกโดยไม่ตรวจ bounds | O(1) |
| `vec.at(i)` | เข้าถึงสมาชิกพร้อมตรวจ bounds | O(1) |
| `vec.front()`, `vec.back()` | สมาชิกแรกและสุดท้าย | O(1) |
| `vec.data()` | pointer ไปยัง contiguous storage | O(1) |
| `vec.push_back(x)` | เพิ่มสมาชิกท้าย vector | amortized O(1), worst O(n) |
| `vec.pop_back()` | ลบสมาชิกสุดท้าย | O(1) |
| `vec.resize(n)` | เปลี่ยนจำนวนสมาชิก | O(สมาชิกที่เพิ่ม/ลบ), อาจ O(n) เมื่อ reallocate |
| `vec.reserve(n)` | ขอ capacity อย่างน้อย `n` | O(n) เมื่อมีการจัดสรรใหม่ |
| `vec.insert(pos, x)` | แทรกก่อน `pos` | O(n) |
| `vec.erase(pos)` | ลบที่ `pos` และเลื่อนสมาชิกด้านหลัง | O(n) |
| `vec.begin()`, `vec.end()` | iterator ต้นทางและ one-past-the-end | O(1) |

### `at()` กับ `operator[]`

ทั้งสองแบบเข้าถึงสมาชิกได้ใน O(1) แต่พฤติกรรมเมื่อ index ผิดต่างกัน:

```cpp
std::vector<int> values = {10, 20};

std::cout << values[1] << '\n';    // ไม่ตรวจ bounds
std::cout << values.at(1) << '\n'; // ตรวจ bounds
```

- `values[i]`: หาก `i >= values.size()` จะเกิด undefined behavior
- `values.at(i)`: หาก index ผิดจะ throw `std::out_of_range`

ใช้ `at()` เมื่อ index มาจาก input หรือแหล่งที่ยังไม่เชื่อถือ และใช้ `operator[]` เมื่อโค้ดได้พิสูจน์ bounds แล้วและต้องการ syntax ที่กระชับ

## Complexity

ให้ `n` เป็นจำนวนสมาชิกเดิมและ `k` เป็นจำนวนสมาชิกที่เพิ่ม:

- เดินข้อมูลหรือพิมพ์สมาชิกทั้งหมด: **O(n)**
- เพิ่มท้ายด้วย `push_back()`: amortized **O(1)** ต่อครั้ง แต่ครั้งที่ reallocate เป็น **O(n)**
- `resize(n + k)`: **O(k)** หาก storage เพียงพอ และอาจเป็น **O(n + k)** เมื่อ reallocate
- แทรกกลาง vector: **O(n)** เพราะสมาชิกด้านหลังต้องเลื่อน และอาจมี reallocation
- ลบสมาชิกแรกหรือกลาง: **O(n)** เพราะสมาชิกด้านหลังต้องเลื่อน
- พื้นที่ที่ vector ใช้สัมพันธ์กับ `capacity()` ไม่ใช่เฉพาะ `size()` จึงเป็น **O(capacity)**

## อธิบายแต่ละไฟล์

### [`funcargs.cpp`](./funcargs.cpp)

สาธิตการส่ง vector เข้า function สองรูปแบบ:

```cpp
void addElements(std::vector<int>& vec, int numElementsToAdd);
void printVector(const std::vector<int>& vec);
```

- `addElements` รับ non-const reference จึงแก้ vector ต้นทางได้โดยไม่ copy
- `printVector` รับ `const` reference เพื่ออ่านโดยไม่ copyและไม่อนุญาตให้แก้สมาชิก
- `addElements` เรียก `resize()` ก่อน แล้วจึงเรียก `data()` เพื่อหา pointer ไปยังสมาชิกใหม่
- `main()` เพิ่ม `0..4` ด้วย `push_back()` แล้วเพิ่มอีกห้าสมาชิกเป็น `5..9`

Functions สำคัญ: `addElements(...)`, `printVector(...)`, `main()`

ผลลัพธ์:

```text
Original vector: 0 1 2 3 4
After adding elements: 0 1 2 3 4 5 6 7 8 9
```

ข้อจำกัดสำคัญ: `numElementsToAdd` เป็น `int` และไม่มีการปฏิเสธค่าติดลบ ค่าติดลบอาจทำให้ `resize()` ได้ขนาดไม่ถูกต้องหรือทำให้การคำนวณ pointer ออกนอกขอบเขต ฟังก์ชันนี้จึงปลอดภัยเฉพาะ contract ที่จำนวนเพิ่มไม่ติดลบและขนาดรวมอยู่ในช่วงที่รองรับ

### [`iterator.cpp`](./iterator.cpp)

สาธิต iterator ของ vector:

- `vec.begin()` คืน iterator ไปยังสมาชิกแรก
- `*it` อ่านหรือแก้สมาชิกที่ iterator ชี้อยู่
- `++iter` เลื่อนไปสมาชิกถัดไป
- `vec.begin() + 2` ใช้ได้เพราะ vector iterator เป็น random-access iterator
- `insert()` แทรกก่อนตำแหน่งที่กำหนด
- `erase()` ลบสมาชิกและเลื่อนสมาชิกด้านหลัง

Function สำคัญ: `main()`

หลังแก้สมาชิกแรก แทรก `20` และลบสมาชิกแรก vector สุดท้ายคือ:

```text
2 20 3 4 5
```

โค้ดกำหนด `it = vec.begin()` ใหม่หลัง `insert()` ซึ่งถูกต้อง เพราะ iterator เดิมอาจ invalid แล้ว หลัง `erase(it)` โค้ดไม่ใช้ `it` ต่อ

### [`loop.cpp`](./loop.cpp)

ไฟล์นี้ **ว่าง ขนาด 0 bytes** ไม่มี include, function หรือ `main()`

- Compile เป็น object file ได้ เพราะ empty translation unit ถูกต้อง
- Link เป็น executable ไม่ได้ เพราะไม่มี `main()`
- ไม่มีพฤติกรรมให้ run หรือผลลัพธ์ให้แสดง

### [`vector.cpp`](./vector.cpp)

สาธิต operation พื้นฐาน:

1. สร้าง vector ว่าง
2. เพิ่ม `10`, `20`, `30` ด้วย `push_back()`
3. พิมพ์สมาชิกด้วย index-based loop
4. ลบสมาชิกท้ายด้วย `pop_back()`
5. อ่าน `vec[1]`
6. พิมพ์ `size()`

Function สำคัญ: `main()`

ผลลัพธ์:

```text
10 20 30
Element at index 1: 20
Size of vector: 2
```

Comment ในไฟล์ระบุ “range-based for loop” แต่โค้ดจริงใช้ index-based loop

## Practical patterns

### ส่งแบบ `const` reference เมื่ออ่านอย่างเดียว

```cpp
void printValues(const std::vector<int>& values) {
  for (int value : values) {
    std::cout << value << ' ';
  }
}
```

วิธีนี้ไม่ copy vector และ range-based loop หลีกเลี่ยงปัญหาชนิด index

### ใช้ `std::size_t` เมื่อต้องใช้ index

```cpp
for (std::size_t i = 0; i < values.size(); ++i) {
  std::cout << values[i] << '\n';
}
```

`size()` คืน `std::vector<T>::size_type` ซึ่งโดยทั่วไปคือ `std::size_t` และเป็น unsigned การใช้ `int i` เปรียบเทียบกับ `size()` ทำให้ compiler เตือนเรื่อง signed/unsigned

### ใช้ `reserve()` เมื่อทราบจำนวนโดยประมาณ

```cpp
std::vector<int> values;
values.reserve(100);

for (int i = 0; i < 100; ++i) {
  values.push_back(i);
}
```

`reserve()` ลดจำนวนครั้งที่ต้อง reallocate แต่ไม่เพิ่ม `size()` และยังไม่สร้างสมาชิกให้เข้าถึงได้ ห้ามใช้ `values[0]` จนกว่าจะมีการเพิ่มหรือ `resize()` สมาชิกจริง

### ใช้ iterator ที่ `insert()` คืนกลับมา

```cpp
std::vector<int> values = {1, 3};
auto inserted = values.insert(values.begin() + 1, 2);
std::cout << *inserted << '\n'; // 2
```

อย่าคาดว่า iterator ที่สร้างก่อน `insert()` จะยัง valid ให้ใช้ iterator ที่ operation คืนกลับมาเมื่อเหมาะสม

### เก็บ pointer จาก `data()` ให้น้อยที่สุด

```cpp
std::vector<int> values = {1, 2, 3};
int* data = values.data();
std::cout << data[0] << '\n';
```

ระหว่างที่ใช้ `data` ไม่ควรทำ operation ที่อาจเปลี่ยน storage เช่น `push_back()`, `resize()`, `insert()` หรือ `reserve()` หากยังต้องใช้ pointer เดิมต่อ

## Lifetime, invalidation และ undefined behavior

Vector เป็นเจ้าของสมาชิก เมื่อ vector ถูกทำลาย pointer, reference และ iterator ทั้งหมดที่ชี้สมาชิกภายในจะ dangling

### Invalidation โดยสรุป

| Operation | ผลต่อ iterator/reference/pointer เดิม |
|---|---|
| อ่านข้อมูล, `size()`, `capacity()` | ไม่ invalidate |
| `push_back()` | หาก reallocate: invalid ทั้งหมด; หากไม่ reallocate: สมาชิกเดิมยัง valid แต่ `end()` เดิม invalid |
| `reserve(n)` | หาก capacity เพิ่ม: invalid ทั้งหมด; หากไม่เพิ่ม: ไม่ invalidate |
| `resize()` | หาก reallocate: invalid ทั้งหมด; การลดขนาดทำให้ตัวที่ชี้สมาชิกที่ถูกลบและตำแหน่งหลังจากนั้น invalid |
| `insert()` | หาก reallocate: invalid ทั้งหมด; หากไม่ reallocate: ตำแหน่งที่แทรกและหลังจากนั้น invalid |
| `erase()` | ตัวที่ชี้ตำแหน่งที่ลบและหลังจากนั้น invalid |
| `pop_back()` | ตัวที่ชี้สมาชิกที่ถูกลบและ `end()` เดิม invalid |

Pointer จาก `data()` ปฏิบัติตามกฎ storage เดียวกับ iterator และ reference

### Undefined behavior ที่ต้องหลีกเลี่ยง

```cpp
std::vector<int> values;
// values[0] = 10; // ผิด: size ยังเป็น 0
// values.pop_back(); // ผิด: vector ว่าง
// *values.begin(); // ผิด: begin() เท่ากับ end()
```

อีกตัวอย่างหนึ่งคือใช้ iterator หลัง reallocation:

```cpp
std::vector<int> values = {1, 2};
auto it = values.begin();
values.push_back(3); // อาจ reallocate
// std::cout << *it; // ห้ามสมมติว่า it ยัง valid
```

### Warning และข้อจำกัดตามโค้ดจริง

Compiler ด้วย `-Wall -Wextra -Wpedantic` แจ้ง signed/unsigned comparison ใน:

- [`funcargs.cpp`](./funcargs.cpp): `int i` เทียบกับ `vec.size()` ใน `printVector`
- [`vector.cpp`](./vector.cpp): `int i` เทียบกับ `vec.size()`

ข้อจำกัดอื่น:

- [`funcargs.cpp`](./funcargs.cpp) เก็บ `vec.size()` ลง `int` ซึ่งอาจเก็บขนาดของ vector ขนาดใหญ่มากไม่ได้
- การบวก `initialSize + numElementsToAdd` อาจ overflow สำหรับค่าขนาดใหญ่มาก
- `resize()` อาจ throw `std::length_error` หรือ `std::bad_alloc`
- [`iterator.cpp`](./iterator.cpp) dereference `begin()` และใช้ `begin() + 2` โดยอาศัยว่าข้อมูลเริ่มต้นมีห้าสมาชิก
- [`vector.cpp`](./vector.cpp) ใช้ `vec[1]` โดยไม่ตรวจ bounds และเรียก `pop_back()` โดยอาศัยว่า vector ไม่ว่าง
- ตัวอย่างใช้ `using namespace std;` เพื่อความสั้น แต่ควรหลีกเลี่ยงใน header และโค้ดขนาดใหญ่

## Compile และ run จาก root `dsa`

คำสั่งต่อไปนี้ต้องรันขณะที่ current directory คือ root `dsa`:

```bash
mkdir -p build/vector

g++ -std=c++17 -Wall -Wextra -Wpedantic vector/funcargs.cpp -o build/vector/funcargs
./build/vector/funcargs

g++ -std=c++17 -Wall -Wextra -Wpedantic vector/iterator.cpp -o build/vector/iterator
./build/vector/iterator

g++ -std=c++17 -Wall -Wextra -Wpedantic vector/vector.cpp -o build/vector/vector
./build/vector/vector
```

[`loop.cpp`](./loop.cpp) ไม่มี `main()` จึง compile ได้เฉพาะ object file:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -c vector/loop.cpp -o build/vector/loop.o
```

หากพยายามสร้าง executable:

```bash
g++ -std=c++17 vector/loop.cpp -o build/vector/loop
```

linker จะรายงาน `undefined reference to main` บน toolchain ที่ใช้อยู่

ไม่ควรใช้ `g++ vector/*.cpp` เพื่อสร้าง executable เดียว เพราะไฟล์ที่ไม่ว่างสามไฟล์ต่างมี `main()` ของตนเอง

เมื่อต้องการตรวจ invalid access เพิ่มเติม:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g vector/iterator.cpp -o build/vector/iterator-sanitized
./build/vector/iterator-sanitized
```

## แบบฝึกหัด

1. แก้ signed/unsigned warning ใน [`funcargs.cpp`](./funcargs.cpp) และ [`vector.cpp`](./vector.cpp) โดยใช้ `std::size_t` หรือ range-based loop
2. พิมพ์ `size()` และ `capacity()` ก่อนและหลัง `push_back()` แต่ละครั้ง แล้วสังเกตช่วงที่ capacity เปลี่ยน
3. เรียก `reserve(10)` ก่อนเพิ่มสมาชิกสิบตัว แล้วเปรียบเทียบการเปลี่ยนแปลงของ capacity กับกรณีไม่เรียก `reserve()`
4. เปลี่ยนการอ่าน `vec[1]` เป็น `vec.at(1)` แล้วทดลอง index ที่เกินขอบเขตพร้อม `try`/`catch`
5. ปรับ `addElements` ให้รับจำนวนแบบ unsigned ที่เหมาะกับ container และตรวจว่าไม่เกิน `max_size()`
6. ทดลองเก็บ iterator ก่อน `insert()` แล้วใช้ iterator ที่ `insert()` คืนกลับมาแทน iterator เดิม
7. เติม [`loop.cpp`](./loop.cpp) ด้วยตัวอย่าง index-based loop, iterator loop และ range-based loop แล้วเปรียบเทียบความอ่านง่าย
8. สร้างตัวอย่าง invalidated iterator ในโปรแกรมทดลองและใช้ AddressSanitizer ช่วยตรวจ โดยอย่า dereference iterator ที่ invalid ในโค้ดใช้งานจริง
