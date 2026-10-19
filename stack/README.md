# Stack ใน C++

ไดเรกทอรีนี้สาธิตการสร้างและใช้งาน **stack** 3 แบบจากโค้ดจริงในโปรเจกต์:

- [arr-stack.cpp](./arr-stack.cpp) — สร้าง stack ด้วย array ขนาดคงที่
- [linkedlist-stack.cpp](./linkedlist-stack.cpp) — สร้าง stack ด้วย singly linked list
- [stl-stack.cpp](./stl-stack.cpp) — ใช้ `std::stack` จาก C++ Standard Library

## เป้าหมายการเรียนรู้

เมื่อศึกษาและทดลองโค้ดในไดเรกทอรีนี้แล้ว ควรสามารถ:

1. อธิบายหลัก **LIFO (Last In, First Out)** ได้
2. ติดตามการเปลี่ยนแปลงของตำแหน่ง `top` หลัง `push` และ `pop` ได้
3. เปรียบเทียบ stack แบบ array, linked list และ `std::stack` ได้
4. วิเคราะห์ time/space complexity ของ operation หลักได้
5. เข้าใจ stack overflow, stack underflow และ precondition ของ `top()`/`pop()`
6. เห็นความสำคัญของ memory ownership, destructor และ RAII ใน C++
7. เลือก implementation ให้เหมาะกับข้อจำกัดของงานจริงได้

## Mental model และ invariant

ให้นึกถึง stack เป็นกองจาน:

- วางจานใหม่บนสุดด้วย `push`
- ดูจานบนสุดด้วย `peek` หรือ `top`
- หยิบได้เฉพาะจานบนสุดด้วย `pop`
- จานที่ถูกวางล่าสุดจะถูกหยิบออกก่อนเสมอ

ลำดับนี้เรียกว่า **LIFO**

```text
push(10)       push(20)       pop()

|    |         | 20 |         |    |
| 10 |         | 10 |         | 10 |
+----+         +----+         +----+
 top             top            top
```

Invariant ที่ควรรักษาหลังทุก operation คือ:

- `top` ต้องอ้างถึงสมาชิกที่ถูกเพิ่มล่าสุดและยังไม่ถูกนำออก
- สมาชิกที่อยู่ใต้ `top` ต้องคงลำดับเดิม
- stack ว่างต้องไม่มีสมาชิกที่สามารถอ่านหรือ pop ได้
- `push(x)` แล้ว `pop()` ทันทีควรได้ `x` หากไม่มี operation อื่นคั่นกลาง

Invariant เฉพาะแต่ละ implementation:

- **Array:** `top == -1` หมายถึงว่าง และสมาชิกที่ใช้งานอยู่คือ `arr[0..top]`
- **Linked list:** `top == nullptr` หมายถึงว่าง และ node แรกคือสมาชิกบนสุด
- **`std::stack`:** container ภายในเป็นผู้รักษาโครงสร้างให้ แต่ผู้เรียกยังต้องไม่ใช้ `top()` หรือ `pop()` ขณะว่าง

## Operations และ complexity

| Operation | ความหมาย | Array | Linked list | `std::stack` (container เริ่มต้น) |
|---|---|---:|---:|---:|
| `push(x)` | เพิ่ม `x` บนสุด | O(1) | O(1) | O(1) โดยทั่วไป |
| `pop()` | นำสมาชิกบนสุดออก | O(1) | O(1) | O(1) |
| `peek()` / `top()` | อ่านสมาชิกบนสุด | O(1) | O(1) | O(1) |
| `isEmpty()` / `empty()` | ตรวจว่าว่างหรือไม่ | O(1) | O(1) | O(1) |
| `size()` | จำนวนสมาชิก | ไม่มีใน class นี้ | ไม่มีใน class นี้ | O(1) |
| พื้นที่เก็บ `n` สมาชิก | — | O(`MAX_SIZE`) | O(n) | O(n) |

> `std::stack` ใช้ `std::deque` เป็น underlying container โดยปริยายในโค้ดนี้ การ push/pop ที่ปลายจึงเป็น O(1) โดยทั่วไป

## อธิบายไฟล์

### 1. [arr-stack.cpp](./arr-stack.cpp)

ไฟล์นี้ประกาศ `class Stack` ซึ่งเก็บข้อมูลใน:

```cpp
int top;
int arr[MAX_SIZE];
```

โดย `MAX_SIZE` ถูกกำหนดเป็น `100`

#### ส่วนสำคัญ

- `Stack()` ตั้ง `top = -1` เพื่อแทน stack ว่าง
- `push(int x)` ตรวจ overflow แล้วบันทึกที่ `arr[++top]`
- `pop()` อ่าน `arr[top]` แล้วลด `top`
- `peek()` อ่าน `arr[top]` โดยไม่ลบ
- `isEmpty()` คืนค่า `top < 0`
- `main()` ทดลอง push `10`, `20`, ดู top และ pop หนึ่งครั้ง

#### Trade-off

ข้อดี:

- implementation ตรงไปตรงมา
- ทุก operation หลักเป็น O(1)
- ไม่ต้อง allocate node ทีละตัว

ข้อจำกัด:

- เก็บได้สูงสุด 100 ค่า
- ใช้พื้นที่สำหรับ array ทั้ง 100 ช่องแม้มีข้อมูลน้อย
- การเปลี่ยนความจุต้องแก้และ compile โปรแกรมใหม่

### 2. [linkedlist-stack.cpp](./linkedlist-stack.cpp)

ไฟล์นี้ใช้ `class Node` และ `class Stack` โดย `Stack::top` ชี้ไปยัง node แรกของ linked list

#### ส่วนสำคัญ

- `Node::data` เก็บค่า
- `Node::next` ชี้ไปยังสมาชิกถัดลงไปใน stack
- `push(int x)` สร้าง node ด้วย `new` แล้วต่อไว้ด้านหน้า
- `pop()` เลื่อน `top` ไป node ถัดไปและ `delete` node เดิม
- `peek()` อ่าน `top->data`
- `isEmpty()` ตรวจ `top == nullptr`
- `main()` แสดงว่าเมื่อ pop `20` แล้ว top กลับเป็น `10`

#### Trade-off

ข้อดี:

- ไม่มีความจุสูงสุดแบบค่าคงที่
- ใช้พื้นที่ตามจำนวนสมาชิกจริง
- push/pop ที่หัว list เป็น O(1)

ข้อจำกัด:

- ทุก node มี overhead ของ pointer
- ต้อง allocate/deallocate หน่วยความจำ
- ต้องจัดการ ownership และอายุของ node ให้ถูกต้อง

### 3. [stl-stack.cpp](./stl-stack.cpp)

ไฟล์นี้ใช้ `std::stack<int>` โดยตรง และสาธิต API มาตรฐาน:

- `push(value)` เพิ่มสมาชิก
- `top()` อ่านสมาชิกบนสุด
- `pop()` ลบสมาชิกบนสุด แต่ **ไม่คืนค่า**
- `empty()` ตรวจว่าว่างหรือไม่
- `size()` คืนจำนวนสมาชิก

หากต้องการอ่านค่าก่อนลบด้วย STL ต้องทำสองขั้นตอน:

```cpp
int value = myStack.top();
myStack.pop();
```

`std::stack` เหมาะกับโค้ดใช้งานจริงมากกว่าการเขียน stack เอง เมื่อไม่จำเป็นต้องควบคุม representation ภายใน

## Dry run

### Array stack

เริ่มต้น `top = -1`

| คำสั่ง | การเปลี่ยนแปลง | เนื้อหา stack จากล่างขึ้นบน | `top` |
|---|---|---|---:|
| `push(10)` | เก็บ `10` ที่ `arr[0]` | `[10]` | 0 |
| `push(20)` | เก็บ `20` ที่ `arr[1]` | `[10, 20]` | 1 |
| `peek()` | คืน `20`, ไม่เปลี่ยน stack | `[10, 20]` | 1 |
| `pop()` | คืน `20`, ลด `top` | `[10]` | 0 |

### Linked-list stack

```text
เริ่มต้น: top -> nullptr

push(10): top -> [10 | nullptr]

push(20): top -> [20 | *] -> [10 | nullptr]

pop():    ลบ node 20
          top -> [10 | nullptr]
```

### STL stack

โค้ด push `10`, `20`, `30` ตามลำดับ:

```text
ล่าง [10, 20, 30] บน
```

- `top()` ได้ `30`
- `pop()` ลบ `30`
- `top()` ถัดไปได้ `20`
- `size()` ได้ `2`

## การนำไปใช้จริง

Stack เหมาะกับงานที่ต้องย้อนกลับตามลำดับล่าสุด เช่น:

- undo/redo ใน editor
- browser back history
- call stack และ recursion
- ตรวจวงเล็บหรือ tag ที่ซ้อนกัน
- ประเมิน expression และแปลง infix/postfix
- depth-first search แบบ iterative
- backtracking
- เก็บ state ล่าสุดที่ยังจัดการไม่เสร็จ

แนวทางเลือก implementation:

- ใช้ **array แบบคงที่** เมื่อทราบจำนวนสูงสุดแน่นอน ต้องการโครงสร้างง่าย และยอมรับ overflow ได้
- ใช้ **linked list** เพื่อเรียนรู้ pointer/ownership หรือเมื่อจำเป็นต้องควบคุม node เอง
- ใช้ **`std::stack`** เป็นตัวเลือกเริ่มต้นสำหรับงาน C++ ทั่วไป

## Edge cases และข้อจำกัดตามโค้ดจริง

### `arr-stack.cpp`

- เมื่อมี 100 สมาชิกแล้ว `push()` จะพิมพ์ `Stack Overflow` และไม่เพิ่มค่า
- `pop()` ขณะว่างพิมพ์ `Stack Underflow` และคืน `0`
- `peek()` ขณะว่างพิมพ์ `Stack is Empty` และคืน `0`
- ค่า `0` เป็นข้อมูลจริงได้ จึงแยกไม่ได้ว่าเป็นข้อมูลหรือ error sentinel
- `push()` พิมพ์ข้อความเอง ทำให้ data structure ผูกกับ output
- `main()` ยังไม่ได้ทดสอบ overflow หรือ underflow

### `linkedlist-stack.cpp`

- `pop()` ขณะว่างคืน `0` แต่ `peek()` ขณะว่างคืน `-1`; sentinel ไม่สม่ำเสมอและอาจชนกับข้อมูลจริง
- ไม่มี destructor สำหรับลบ node ที่เหลือ ใน `main()` จริงยังมี node ค่า `10` เหลือเมื่อ object หมดอายุ
- ไม่มี copy constructor หรือ copy assignment ที่กำหนด ownership การ copy `Stack` จึงทำให้หลาย object ชี้ list เดียวกันและเสี่ยง dangling pointer
- ในงานจริงควรใช้ RAII เช่น `std::unique_ptr` หรือเพิ่ม destructor และจัดการ Rule of Three/Five
- `new Node()` อาจโยน `std::bad_alloc` เมื่อหน่วยความจำไม่พอ

### `stl-stack.cpp`

- ห้ามเรียก `top()` หรือ `pop()` เมื่อ stack ว่าง เพราะเป็น undefined behavior
- ต้องเรียก `empty()` ก่อนหากไม่แน่ใจ
- `pop()` ไม่คืนสมาชิกที่ถูกลบ
- `using namespace std;` สะดวกในตัวอย่างสั้น แต่ควรหลีกเลี่ยงใน header และโค้ดขนาดใหญ่

### Error handling ที่เหมาะกับงานจริง

แทนการใช้ `0` หรือ `-1` เป็น sentinel อาจเลือก:

- คืน `std::optional<int>`
- คืนสถานะผ่าน `bool` และเขียนค่าผ่าน output parameter
- โยน exception เมื่อ operation ผิดเงื่อนไข
- กำหนดให้ผู้เรียกตรวจ `isEmpty()` เป็น precondition ที่ชัดเจน

## Compile และ run จาก root `dsa`

คำสั่งต่อไปนี้สมมติว่า current directory คือ root `dsa`:

```bash
pwd
# /home/phos/Desktop/Projects/self-taught-developer/dsa
```

Compile และ run แต่ละไฟล์แยกกัน:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic stack/arr-stack.cpp -o /tmp/dsa-arr-stack
/tmp/dsa-arr-stack
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic stack/linkedlist-stack.cpp -o /tmp/dsa-linkedlist-stack
/tmp/dsa-linkedlist-stack
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic stack/stl-stack.cpp -o /tmp/dsa-stl-stack
/tmp/dsa-stl-stack
```

ใช้ `/tmp` เพื่อไม่สร้าง executable ปะปนกับ source code

ไม่ควร compile ทั้งสามไฟล์เป็น executable เดียว:

```bash
# ไม่ควรใช้: แต่ละไฟล์มี main() ของตัวเอง
# g++ stack/*.cpp -o stack-demo
```

linker จะพบ `main()` ซ้ำ แต่สามารถตรวจ syntax พร้อมกันโดยไม่ link ได้:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsyntax-only \
  stack/arr-stack.cpp \
  stack/linkedlist-stack.cpp \
  stack/stl-stack.cpp
```

## แบบฝึกหัด

1. เพิ่ม `size()` ให้ `Stack` ใน [arr-stack.cpp](./arr-stack.cpp)
2. เปลี่ยน `MAX_SIZE` ให้เป็น template parameter เช่น `Stack<int, 100>`
3. เปลี่ยน `pop()` และ `peek()` ให้คืน `std::optional<int>` แทน sentinel
4. เขียน test สำหรับกรณี array stack เต็มและว่าง
5. เพิ่ม destructor ให้ [linkedlist-stack.cpp](./linkedlist-stack.cpp) เพื่อลบทุก node ที่เหลือ
6. ปิดการ copy ของ linked-list stack หรือ implement deep copy ให้ถูกต้อง
7. เปลี่ยน `Node *next` เป็น `std::unique_ptr<Node>` เพื่อฝึก RAII
8. เพิ่มสมาชิก 1,000 ตัวแล้วเปรียบเทียบพฤติกรรมของทั้งสาม implementation
9. ใช้ `std::stack` เขียนฟังก์ชันกลับลำดับ string
10. เขียน DFS แบบ iterative โดยใช้ `std::stack`
11. แยกข้อความ output ออกจาก class เพื่อให้ class นำไปใช้และทดสอบได้ง่ายขึ้น
12. อธิบายว่าทำไม queue จึงให้ลำดับต่างจาก stack เมื่อใส่ข้อมูลชุดเดียวกัน
