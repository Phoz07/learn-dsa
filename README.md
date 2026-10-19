# Data Structures & Algorithms ด้วย C++

Repository นี้รวบรวมตัวอย่างพื้นฐานของ C++, โครงสร้างข้อมูล (Data Structures) และการนำโครงสร้างข้อมูลไปแก้โจทย์ Algorithm โดยเอกสารแต่ละบทอธิบายทั้ง **แนวคิด**, **การทำงานของโค้ดจริง**, **Big-O**, **ข้อควรระวัง** และ **แนวทางนำไปใช้ต่อ**

> เป้าหมายไม่ใช่แค่จำ syntax แต่คือการตอบให้ได้ว่า “ข้อมูลควรถูกจัดเก็บอย่างไร”, “operation ใดเกิดบ่อย” และ “trade-off แบบไหนเหมาะกับโจทย์”

## สารบัญและลำดับการเรียนที่แนะนำ

| ลำดับ | บท | สิ่งที่จะได้เรียนรู้ |
|---:|---|---|
| 1 | [`struct`](./struct/README.md) | การรวมข้อมูลหลายค่าเป็นชนิดข้อมูลเดียว, initialization และ array ของ object |
| 2 | [`class`](./class/README.md) | Object, encapsulation, constructor, interface และ invariant |
| 3 | [`pointer`](./pointer/README.md) | Address, dereference, lifetime, array pointer และ function pointer |
| 4 | [`vector`](./vector/README.md) | Dynamic array, iterator, contiguous storage และ invalidation |
| 5 | [`linkedlist`](./linkedlist/README.md) | Node, singly/doubly/circular list, pointer linking และ ownership |
| 6 | [`stack`](./stack/README.md) | LIFO, array stack, linked-list stack และ `std::stack` |
| 7 | [`queue`](./queue/README.md) | FIFO, circular queue, deque, priority queue และ `std::queue` |
| 8 | [`leetcode`](./leetcode/README.md) | การสังเกต stack pattern, invariant และการวิเคราะห์โจทย์จริง |

หากมีพื้นฐาน C++ อยู่แล้ว สามารถเริ่มจาก `vector` แล้วเรียน `linkedlist` → `stack` → `queue` → `leetcode` ได้

## วิธีคิดก่อนเลือก Data Structure

อย่าเลือกโครงสร้างข้อมูลจากชื่อที่คุ้นเคย ให้เริ่มจากคำถามต่อไปนี้:

1. **ข้อมูลมีจำนวนคงที่หรือเปลี่ยนตลอดเวลา?**
   - คงที่และขนาดเล็ก: array หรือ `std::array`
   - เปลี่ยนขนาดได้: `std::vector` หรือ container อื่น
2. **ต้องเข้าถึงตาม index บ่อยหรือไม่?**
   - ถ้าต้องการ random access O(1): array/`std::vector`
   - linked list ต้องเดินจาก node เริ่มต้น จึงเป็น O(n)
3. **เพิ่มหรือลบตรงตำแหน่งใดบ่อยที่สุด?**
   - ท้ายลำดับ: `std::vector` มักเหมาะ
   - บนสุดเท่านั้น: stack
   - เข้าท้ายและออกหน้า: queue
   - ทั้งหน้าและหลัง: deque
4. **ลำดับการนำข้อมูลออกเป็นแบบใด?**
   - ล่าสุดออกก่อน (LIFO): stack
   - มาก่อนออกก่อน (FIFO): queue
   - สำคัญกว่าถูกนำออกก่อน: priority queue
5. **ต้องรักษา address/reference ของสมาชิกหรือไม่?**
   - การขยาย `std::vector` อาจทำให้ pointer, reference และ iterator เดิมใช้ไม่ได้
   - linked node มี address แยกกัน แต่เพิ่มภาระเรื่อง allocation และ ownership
6. **ขอบเขตข้อมูลและความผิดพลาดจัดการอย่างไร?**
   - ระบุพฤติกรรมเมื่อว่าง, เต็ม, index ผิด หรือ allocation ล้มเหลว
   - หลีกเลี่ยง sentinel เช่น `-1` หากค่านั้นเป็นข้อมูลจริงได้

## Big-O ที่ควรรู้

Big-O อธิบายแนวโน้มของเวลาและพื้นที่เมื่อจำนวนข้อมูล `n` เพิ่มขึ้น โดยไม่ผูกกับความเร็วของเครื่องใดเครื่องหนึ่ง

| Complexity | ความหมายโดยย่อ | ตัวอย่าง |
|---|---|---|
| O(1) | จำนวนงานไม่โตตาม `n` | อ่าน `vector[i]`, stack `push()` |
| O(log n) | ลดพื้นที่ค้นหาลงเป็นสัดส่วนทุกขั้น | Binary search บนข้อมูลเรียงแล้ว |
| O(n) | ตรวจข้อมูลประมาณหนึ่งรอบ | เดิน linked list, ค้นหาแบบเชิงเส้น |
| O(n log n) | มักพบใน sorting ที่มีประสิทธิภาพ | `std::sort` โดยเฉลี่ย/ที่รับประกันตามมาตรฐานในเชิง upper bound |
| O(n²) | เปรียบเทียบหรือทำงานซ้อนกันตาม `n` | loop ซ้อนสองชั้น, เติมหน้าของ string ซ้ำ ๆ |

ข้อควรจำ:

- วิเคราะห์ **worst case**, **average case** และ **amortized cost** แยกกันเมื่อมีผลต่อการเลือกใช้
- O(1) ไม่ได้แปลว่าเร็วที่สุดเสมอ เพราะ allocation, cache locality และค่าคงที่ยังมีผล
- ต้องวิเคราะห์ทั้ง operation ที่สนใจและ workflow รวม เช่น algorithm หลักเป็น O(n) แต่ขั้นประกอบผลลัพธ์อาจทำให้รวมเป็น O(n²)
- Space complexity ควรรวมพื้นที่เพิ่มเติม (auxiliary space) และ ownership ของข้อมูลที่สร้างขึ้น

## สรุปโครงสร้างข้อมูลในโปรเจกต์

| โครงสร้าง | จุดเด่น | Operation เด่น | ข้อแลกเปลี่ยน |
|---|---|---|---|
| `std::vector` | ข้อมูลต่อเนื่อง, cache-friendly, เข้าถึงตาม index | อ่าน index O(1), เพิ่มท้าย amortized O(1) | แทรก/ลบกลาง O(n), reallocation ทำให้ iterator invalid |
| Singly linked list | ต่อ node ได้โดยไม่เลื่อนข้อมูลทั้งหมด | เพิ่ม/ลบที่หัว O(1) | ค้นหา O(n), ไม่มี backward traversal |
| Doubly linked list | เดินหน้าและย้อนกลับได้ | unlink node ที่ทราบตำแหน่ง O(1) | ใช้ pointer เพิ่มและต้องรักษาความสอดคล้องสองทิศ |
| Stack | จำกัดการเข้าถึงไว้ที่ด้านบน | `push`/`pop`/`top` O(1) | เข้าถึงสมาชิกกลางโดยตรงไม่ได้ |
| Queue | รักษาลำดับ FIFO | `enqueue`/`dequeue` O(1) ใน implementation ที่เหมาะสม | ไม่เหมาะกับการค้นหาสมาชิกทั่วไป |
| Deque | เพิ่มและลบได้ทั้งสองด้าน | operation ที่ปลายทั้งสอง O(1) | API และ implementation ซับซ้อนกว่า queue |
| Priority queue | เลือกข้อมูลตาม priority | ขึ้นกับ implementation; heap มัก O(log n) ตอนเพิ่ม/ลบ | ไม่รักษาลำดับทั้งหมดแบบ sorted container |

## แนวทางเรียนแต่ละบทให้ได้ผล

ใช้วงจรต่อไปนี้กับทุกตัวอย่าง:

1. **อ่านโจทย์ของโครงสร้างข้อมูล** — มันรับข้อมูลอะไรและรับประกันพฤติกรรมใด
2. **วาดสถานะก่อนรัน** — วาด array, node และลูกศร หรือเขียนรายการสมาชิก
3. **ทำ dry run** — ติดตาม index, `top`, `front`, `rear`, `head` และ `tail` ทีละ operation
4. **ทำนาย output** — เขียนผลลัพธ์ก่อน compile
5. **เปิด compiler warnings** — warning เป็นข้อมูลสำคัญสำหรับเรียน C++
6. **ทดลอง edge cases** — ว่าง, เต็ม, หนึ่งสมาชิก, ข้อมูลซ้ำ และ input ผิดรูปแบบ
7. **วิเคราะห์ Big-O** — แยกแต่ละ operation และภาพรวมของโปรแกรม
8. **ปรับ implementation** — ทำแบบฝึกหัดท้าย README ของแต่ละบท
9. **ใช้ sanitizer เมื่อต้องทำงานกับ pointer** — ช่วยค้นหา out-of-bounds, use-after-free และ undefined behavior บางประเภท

## การ Compile และ Run

คำสั่งใน README แต่ละบทสมมติว่า terminal อยู่ที่ root ของ repository นี้ (`dsa`)

ตรวจสอบ compiler:

```bash
g++ --version
```

ทดลองโปรแกรมพื้นฐาน [`hello.cpp`](./hello.cpp):

```bash
mkdir -p build

g++ -std=c++17 -Wall -Wextra -Wpedantic hello.cpp -o build/hello
printf '42\n' | ./build/hello
```

ผลลัพธ์ที่คาดหวัง:

```text
42
```

Flags ที่ใช้:

- `-std=c++17` — ใช้มาตรฐาน C++17
- `-Wall -Wextra -Wpedantic` — เปิด warning ที่มีประโยชน์ต่อการเรียนรู้
- `-g` — เพิ่มข้อมูลสำหรับ debugger (เพิ่มเมื่อกำลัง debug)
- `-fsanitize=address,undefined` — ตรวจ memory error และ undefined behavior หลายชนิดระหว่างรัน

ตัวอย่างเปิด sanitizer:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -g \
  linkedlist/singly-linkedlist.cpp -o build/singly-linkedlist-sanitized

./build/singly-linkedlist-sanitized
```

> หลาย directory มี `.cpp` มากกว่าหนึ่งไฟล์และแต่ละไฟล์มี `main()` ของตัวเอง จึงควร compile แยก executable ตามคำสั่งใน README ประจำบท ไม่ควรใช้ `g++ directory/*.cpp` เพื่อ link รวมโดยไม่ตรวจโครงสร้างก่อน

## จากตัวอย่างเพื่อการเรียนรู้สู่งานจริง

ตัวอย่างใน repository เน้นให้เห็นกลไกภายใน จึงมีบางจุดที่ตั้งใจให้เรียบง่ายกว่าระบบจริง ก่อนนำแนวคิดไปใช้ใน production ควรพิจารณา:

- ใช้ Standard Library (`std::vector`, `std::stack`, `std::queue`, `std::deque`, `std::priority_queue`) เป็นค่าเริ่มต้น แทนการสร้าง container เองโดยไม่มีเหตุผลเฉพาะ
- ใช้ RAII และ smart pointer เพื่อให้ ownership ชัดเจน
- หลีกเลี่ยง raw `new`/`delete` ใน application code หาก standard container หรือ `std::unique_ptr` จัดการแทนได้
- เพิ่ม destructor/copy/move semantics ให้ถูกต้องเมื่อ class เป็นเจ้าของ resource หรือใช้ Rule of Zero ด้วย resource-managing types
- แยก data-structure logic ออกจากการพิมพ์ข้อความ เพื่อให้ทดสอบและ reuse ได้ง่าย
- กำหนด error handling ให้ชัด เช่น `std::optional`, exception, status result หรือ precondition
- เขียน tests ครอบคลุม edge cases และ invariant
- วัดประสิทธิภาพจาก workload จริงก่อน optimize

## คำศัพท์สำคัญ

| คำศัพท์ | ความหมาย |
|---|---|
| Data structure | รูปแบบการจัดเก็บและเชื่อมโยงข้อมูล |
| Algorithm | ลำดับขั้นตอนเพื่อแก้ปัญหา |
| Invariant | เงื่อนไขที่ต้องเป็นจริงก่อน/หลัง operation ตามที่กำหนด |
| Node | หน่วยข้อมูลในโครงสร้างแบบเชื่อมโยง |
| Pointer | ค่าที่เก็บ address ของ object/function |
| Traversal | การเดินเยี่ยมสมาชิกในโครงสร้าง |
| Ownership | ความรับผิดชอบว่าใครต้องดูแลและทำลาย resource |
| RAII | ผูกอายุ resource เข้ากับ lifetime ของ object |
| Overflow | พยายามเพิ่มข้อมูลเกินความจุ |
| Underflow | พยายามนำข้อมูลออกจากโครงสร้างที่ว่าง |
| Iterator invalidation | iterator/reference/pointer เดิมไม่สามารถใช้อย่างถูกต้องหลัง container เปลี่ยน |
| Undefined behavior | พฤติกรรมที่มาตรฐาน C++ ไม่กำหนดผลลัพธ์ไว้ |

## เป้าหมายหลังเรียนครบ

เมื่อเรียนและทำแบบฝึกหัดครบ ควรสามารถ:

- เลือกโครงสร้างข้อมูลจากรูปแบบ operation และข้อจำกัดของโจทย์
- อธิบาย time/space complexity พร้อมเหตุผล ไม่ใช่เพียงจำตาราง
- วาดและติดตามสถานะของ array, linked nodes, stack และ queue ได้
- ระบุ edge cases และ invariant ก่อนเริ่มเขียนโค้ด
- ใช้ Standard Library ได้อย่างปลอดภัย โดยเข้าใจ lifetime และ invalidation
- มองเห็น stack/queue patterns ในโจทย์ algorithm และทดสอบคำตอบด้วย dry run
- แยกความต่างระหว่าง implementation เพื่อการเรียนรู้กับ code ที่พร้อมใช้งานจริง
