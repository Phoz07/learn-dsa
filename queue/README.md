# Queue

เอกสารนี้อธิบาย queue โดยอิงจากตัวอย่าง C++ ในไดเรกทอรีนี้ ครอบคลุม linear queue, circular queue, deque, `std::queue`, คิวงานพิมพ์ และ priority queue พร้อมวิเคราะห์ complexity, edge case และข้อจำกัดของ implementation ปัจจุบัน

## เป้าหมายการเรียนรู้

เมื่ออ่านและทดลองตัวอย่างครบแล้ว ควรสามารถ:

1. อธิบายหลัก **FIFO — First In, First Out** (เข้าก่อนออกก่อน) ได้
2. เข้าใจบทบาทของ `front` และ `rear`
3. แยกความแตกต่างระหว่าง linear queue กับ circular queue ได้
4. เข้าใจ **overflow** (คิวเต็ม) และ **underflow** (นำข้อมูลออกจากคิวว่าง)
5. ใช้ deque ซึ่งเพิ่มและลบได้ทั้งสองด้านได้
6. อธิบาย priority queue และลำดับเมื่อ priority เท่ากันได้
7. ใช้ `std::queue` อย่างปลอดภัย โดยตรวจสถานะก่อนเรียก operation ที่ต้องมีสมาชิก
8. วิเคราะห์เวลาและพื้นที่ของ array-based และ linked-list-based implementation ได้
9. เห็นผลของ memory ownership, destructor และ copy semantics ในโครงสร้างที่ใช้ raw pointer
10. เลือกชนิดของ queue ให้เหมาะกับงานจริงได้

## Mental model และหลักการ

### FIFO queue

Queue ปกติทำงานแบบ FIFO: สมาชิกที่เข้าแถวก่อนถูกนำออกก่อน

```text
dequeue/front                              enqueue/rear
      |                                           |
      v                                           v
    [10] <- [20] <- [30] <- [40]
```

Operation หลัก:

- **enqueue**: เพิ่มข้อมูลที่ท้าย (`rear`)
- **dequeue**: นำข้อมูลออกจากหน้า (`front`)
- **peek/front**: อ่านข้อมูลหน้าโดยไม่ลบ
- **isEmpty**: ตรวจว่าคิวว่างหรือไม่
- **isFull**: ตรวจว่าคิวเต็มหรือไม่ สำหรับคิวที่ความจุจำกัด

### Linear queue แบบ array

Linear queue ใน [`linear-queue.cpp`](./linear-queue.cpp) ให้ `front` และ `rear` เคลื่อนไปทางขวา:

```text
index:  0    1    2    3    4
       [ ]  [ ]  [A]  [B]  [C]
                 ^         ^
               front      rear
```

แม้ช่อง 0 และ 1 ว่าง แต่ถ้า `rear` ถึง index สุดท้ายแล้ว implementation นี้ถือว่าคิวเต็ม จึงเกิด **false overflow** หรือภาวะที่รายงานว่าเต็มทั้งที่มีพื้นที่ว่างด้านหน้า

### Circular queue

Circular queue มอง array เป็นวงแหวนและเลื่อน index ด้วย:

```cpp
(index + 1) % SIZE
```

เมื่อถึงท้าย array index ถัดไปจะกลับเป็น 0 ทำให้ reuse ช่องว่างด้านหน้าได้

Implementation ใน [`circular-queue.cpp`](./circular-queue.cpp) ใช้:

```cpp
(rear + 1) % SIZE == front
```

เป็นเงื่อนไข full วิธีนี้สงวนหนึ่งช่องไว้เพื่อแยกสถานะ full กับ empty ดังนั้นเมื่อ `SIZE == 5` จะเก็บข้อมูลได้จริงสูงสุด 4 ค่า

### Deque

**Deque — Double-Ended Queue** คือคิวที่เพิ่มและลบได้ทั้งด้านหน้าและด้านหลัง:

```text
insert/delete front <-> [A] [B] [C] <-> insert/delete rear
```

ตัวอย่างใช้ doubly linked list และเก็บทั้ง `front` กับ `rear` จึงทำ operation ที่ปลายทั้งสองด้านได้ `O(1)`

### Priority queue

Priority queue นำสมาชิกออกตาม priority แทนลำดับเข้าอย่างเดียว ตัวอย่างใน [`priority-queue.cpp`](./priority-queue.cpp) กำหนดว่า:

- ตัวเลข priority **น้อยกว่า** มีลำดับสูงกว่าและออกก่อน
- สมาชิกที่ priority เท่ากันรักษาลำดับ FIFO เพราะสมาชิกใหม่ถูกแทรกหลังสมาชิกเดิมในระดับเดียวกัน

## Operations และ complexity

### เปรียบเทียบ implementation ในไดเรกทอรีนี้

| Implementation | Enqueue | Dequeue | Peek | แสดงทั้งหมด | พื้นที่ | ความจุ |
|---|---:|---:|---:|---:|---:|---|
| Linear array queue | `O(1)` | `O(1)` | ไม่มีเมธอด | `O(n)` | `O(SIZE)` | คงที่ 5 |
| Circular array queue | `O(1)` | `O(1)` | ไม่มีเมธอด | `O(n)` | `O(SIZE)` | ใช้จริงสูงสุด 4 จาก array 5 ช่อง |
| Linked-list deque | `O(1)` ทั้งสองด้าน | `O(1)` ทั้งสองด้าน | `O(1)` | ไม่มีเมธอด | `O(n)` | ตามหน่วยความจำ |
| `std::queue` | โดยทั่วไป `O(1)` | โดยทั่วไป `O(1)` | `O(1)` | ไม่มี iterator โดยตรง | `O(n)` | dynamic |
| Printer queue | โดยทั่วไป `O(1)` ต่อการเพิ่ม | `processJobs` รวม `O(n)` | ไม่มีเมธอด | พิมพ์ระหว่าง process | `O(n)` | dynamic |
| Sorted-list priority queue | `O(n)` | `O(1)` | ไม่มีเมธอด | `O(n)` | `O(n)` | ตามหน่วยความจำ |

> ต้นทุนของ `std::string`, console output และการจัดสรรหน่วยความจำอาจเพิ่มจาก complexity เชิงโครงสร้างที่แสดงในตาราง

### Trade-off สำคัญ

- Linear queue ง่าย แต่ไม่ reuse ช่องที่ dequeue แล้วจนกว่าคิวจะว่างทั้งหมด
- Circular queue reuse พื้นที่ได้ แต่ต้องจัดการ wrap-around และ full/empty state ให้ถูกต้อง
- Linked structure ขยายตามข้อมูลได้ แต่มี allocation และ pointer overhead
- Sorted-list priority queue จ่ายต้นทุน `O(n)` ตอน enqueue เพื่อให้ dequeue เป็น `O(1)`
- Heap-based priority queue เช่น `std::priority_queue` มักมี enqueue/dequeue `O(log n)` และอ่านค่าสูงสุด `O(1)`

## ไฟล์ตัวอย่าง

### [`linear-queue.cpp`](./linear-queue.cpp)

สาธิตคิวแบบ fixed-size array ผ่านคลาส `Queue`:

- `items[SIZE]` เก็บข้อมูล โดย `SIZE` เท่ากับ 5
- เริ่มด้วย `front = rear = -1`
- `isEmpty()` ตรวจ `front == -1`
- `isFull()` ตรวจ `rear == SIZE - 1`
- `enqueue(int element)` เพิ่มที่ `++rear`
- `dequeue()` อ่านที่ `front` แล้วเลื่อน `front`
- เมื่อเอาสมาชิกสุดท้ายออก จะ reset ทั้ง `front` และ `rear` เป็น `-1`
- `display()` แสดงตั้งแต่ `front` ถึง `rear`

ทุก enqueue/dequeue เป็น `O(1)` แต่ช่องว่างก่อน `front` ไม่ถูก reuse หากคิวยังไม่ว่างทั้งหมด

### [`circular-queue.cpp`](./circular-queue.cpp)

สาธิต circular queue ผ่านคลาส `CircularQueueArray`:

- ใช้ modulo เลื่อน `front` และ `rear`
- `enqueue` และ `dequeue` เป็น `O(1)`
- `display()` แยกสองกรณี:
  - `rear >= front`: ข้อมูลอยู่ช่วงเดียว
  - `rear < front`: ข้อมูลคร่อมปลายและต้น array

**ไฟล์นี้ไม่มี `main()`** จึงยังรันเป็น executable เดี่ยว ๆ ไม่ได้ สามารถ compile เป็น object file เพื่อตรวจ syntax/implementation หรือเขียน driver ที่สร้าง `CircularQueueArray` และเรียกเมธอดต่าง ๆ

ข้อสำคัญคือ array มี 5 ช่อง แต่เงื่อนไข full แบบสงวนหนึ่งช่องทำให้เก็บได้สูงสุด 4 ค่า

### [`double-end-queue.cpp`](./double-end-queue.cpp)

สาธิต deque ด้วย doubly linked list:

- คลาส `Node` เก็บ `value`, `next`, `prev`
- คลาส `Deque` เก็บ `front`, `rear`, `size`
- `insertFront` และ `insertRear` เพิ่มข้อมูลใน `O(1)`
- `deleteFront` และ `deleteRear` ลบข้อมูลใน `O(1)`
- `getFront` และ `getRear` อ่านค่าปลายแต่ละด้านใน `O(1)`
- destructor `~Deque()` เรียก `deleteFront()` จนคิวว่าง จึงใช้เวลา `O(n)`

ตัวอย่างจัดการกรณีโหนดสุดท้ายถูกลบโดยตั้ง pointer อีกด้านเป็น `nullptr` อย่างถูกต้อง

### [`stl-queue.cpp`](./stl-queue.cpp)

สาธิต adapter `std::queue<int>` จาก C++ Standard Library:

- `push` เพิ่มท้าย
- `pop` ลบหน้า แต่ไม่คืนค่าที่ลบ
- `front` อ่านค่าหน้า
- `back` อ่านค่าท้าย
- `empty` ตรวจคิวว่าง
- `size` คืนจำนวนสมาชิก

`std::queue` ใช้ `std::deque` เป็น underlying container โดย default operation หลักจึงโดยทั่วไปเป็น `O(1)` ก่อนเรียก `front`, `back` หรือ `pop` ต้องแน่ใจว่าคิวไม่ว่าง มิฉะนั้นเกิด undefined behavior

### [`printjob.cpp`](./printjob.cpp)

ประยุกต์ `std::queue` เป็นคิวงานพิมพ์:

- `PrintJob` เก็บ `documentName` และ `numberOfPages`
- `PrinterQueue::addJob(const PrintJob &job)` เพิ่มงานแบบ FIFO
- `PrinterQueue::processJobs()` วนจนคิวว่าง โดยอ่าน `front` แล้ว `pop`

งาน `Document1.pdf`, `Document2.pdf`, `Document3.pdf` จึงถูกพิมพ์ตามลำดับที่เพิ่ม หลัง `processJobs()` ทำงาน คิวจะว่าง

คอมเมนต์ระบุการจำลอง delay แต่โค้ดไม่มี delay จริง และจำนวนหน้าไม่ได้ส่งผลต่อเวลาประมวลผล ไม่มี validation สำหรับชื่อว่างหรือจำนวนหน้าติดลบ

### [`priority-queue.cpp`](./priority-queue.cpp)

สาธิต priority queue ด้วย sorted singly linked list:

- `Node` เก็บ `data`, `priority`, `next`
- `PriorityQueue::enqueue(int d, int p)` เดินหาระดับ priority ที่เหมาะสมใน `O(n)`
- `PriorityQueue::dequeue()` ลบ `head` ใน `O(1)`
- `PriorityQueue::display()` เดินแสดงข้อมูลและ priority ใน `O(n)`

เงื่อนไขการแทรกใช้ `temp->next->priority <= p` ทำให้สมาชิกใหม่ที่ priority เท่ากันอยู่หลังสมาชิกเดิม จึง stable แบบ FIFO ภายใน priority เดียวกัน

**ไฟล์นี้ไม่มี `main()`** จึง compile เป็น executable เดี่ยว ๆ ไม่ได้ ต้อง compile เป็น object file หรือมี driver สำหรับทดลองใช้งาน

## Dry run

### Linear queue และ false overflow

เริ่มต้น:

```text
front = -1, rear = -1
```

หลัง enqueue `1`, `2`, `3`, `4`, `5`:

```text
index:   0   1   2   3   4
items:  [1] [2] [3] [4] [5]
         ^               ^
       front            rear
```

หลัง dequeue สองครั้ง:

```text
index:   0   1   2   3   4
items:  [x] [x] [3] [4] [5]
                 ^       ^
               front    rear
```

แม้ index 0 และ 1 ว่าง แต่ `rear == SIZE - 1` ทำให้ `isFull()` คืน `true` และ enqueue เพิ่มไม่ได้จนกว่าคิวจะถูก dequeue จนว่างและ indices ถูก reset

### Circular queue และ wrap-around

เพราะ implementation สงวนหนึ่งช่อง สมมติ enqueue `10`, `20`, `30`, `40` จะเต็มเมื่อมีสมาชิก 4 ตัว จากนั้น dequeue `10`, `20` ทำให้ `front` เลื่อนไปข้างหน้า เมื่อ enqueue `50`, `60`, ค่า `rear` จะ wrap กลับไปต้น array:

```text
physical array: [50] [60] [30] [40] [ว่าง]
logical order:   30 -> 40 -> 50 -> 60
```

`display()` ต้องพิมพ์จาก `front` ถึงท้าย array แล้วต่อจาก index 0 ถึง `rear`

### Stable priority queue

enqueue ตามลำดับ:

```text
(ข้อมูล A, priority 2)
(ข้อมูล B, priority 1)
(ข้อมูล C, priority 2)
```

รายการที่จัดเรียงแล้วเป็น:

```text
B(1) -> A(2) -> C(2)
```

`B` ออกก่อนเพราะเลข priority ต่ำกว่า ส่วน `A` ออกก่อน `C` เพราะเข้าก่อนใน priority 2

### Deque ใน `main()`

ลำดับ operation:

```text
insertFront(1)  => [1]
insertRear(2)   => [1, 2]
insertFront(3)  => [3, 1, 2]
insertRear(4)   => [3, 1, 2, 4]
deleteFront()   => [1, 2, 4]
deleteRear()    => [1, 2]
```

ดังนั้นค่าหน้าและท้ายสุดท้ายคือ `1` และ `2`

## การเลือกใช้ในงานจริง

| ความต้องการ | ตัวเลือกที่ควรพิจารณา | เหตุผล/ข้อแลกเปลี่ยน |
|---|---|---|
| FIFO ทั่วไปและไม่ต้องควบคุม layout | `std::queue` | API มาตรฐานและจัดการหน่วยความจำให้ |
| ความจุคงที่และไม่ต้อง allocate ระหว่างทำงาน | Circular array queue | เวลา operation คงที่และ reuse ช่องได้ |
| ต้องเพิ่ม/ลบทั้งสองด้าน | `std::deque` หรือ deque | รองรับปลายทั้งสองด้าน `O(1)` |
| งานมีระดับเร่งด่วน | `std::priority_queue` หรือ priority queue | ลำดับออกขึ้นกับ priority |
| ต้องรักษา FIFO ภายใน priority เดียวกัน | โครงสร้างที่กำหนด tie-breaking ชัดเจน | sorted-list ตัวอย่างนี้รักษาลำดับดังกล่าว |
| ระบบ producer/consumer หลาย thread | synchronized queue | container ในตัวอย่างไม่มี thread safety |
| ระบบงานพิมพ์อย่างง่าย | FIFO queue | งานมาก่อนถูกประมวลผลก่อน |

สำหรับ C++ ทั่วไปควรเริ่มพิจารณา standard containers ก่อน แล้วสร้าง implementation เองเมื่อมีข้อกำหนดเฉพาะ เช่น fixed capacity, memory layout, real-time constraint หรือเพื่อการเรียนรู้ algorithm

## Edge cases และข้อควรระวังตามโค้ดปัจจุบัน

### Linear queue

- ความจุคงที่ 5
- ไม่ reuse ช่องด้านหน้าจนกว่าคิวจะว่างทั้งหมด
- `dequeue()` คืน `-1` เมื่อว่าง จึงแยกไม่ได้ว่าเป็น error หรือข้อมูลจริงค่า `-1`
- ไม่มี `peek()` หรือ public `size()`
- `SIZE` เป็น macro ระดับ global

### Circular queue

- ความจุใช้งานจริงเป็น 4 แม้ `SIZE` เท่ากับ 5
- `dequeue()` แสดงค่าที่ลบ แต่ไม่คืนค่าให้ caller
- ไม่มี `main()`
- ไม่มี `peek()` หรือ size counter
- ค่าเก่าในช่อง array ไม่ถูกล้าง แต่ไม่ส่งผลต่อลำดับตราบใดที่ indices ถูกต้อง

### Linked-list deque

- `getFront()` และ `getRear()` คืน `-1` เมื่อว่าง จึงกำกวมกับข้อมูล `-1`
- destructor คืนโหนดที่เหลือทั้งหมด แต่คลาสไม่ได้กำหนด copy constructor หรือ copy assignment
- การ copy `Deque` ด้วยพฤติกรรม default จะเป็น **shallow copy**: object หลายตัวชี้โหนดเดียวกัน และ destructor อาจทำให้เกิด double-free หรือ use-after-free
- ไม่มีเมธอด public สำหรับอ่าน `size` หรือแสดงข้อมูลทั้งหมด

### `std::queue` และ print queue

- ห้ามเรียก `front`, `back` หรือ `pop` เมื่อคิวว่าง
- `pop()` ไม่คืนค่าที่ลบ
- `processJobs()` ล้างคิวทั้งหมด
- print queue ไม่มี validation, retry, cancellation, error state หรือการทำงานแบบ asynchronous
- จำนวนหน้าไม่ส่งผลต่อเวลาจำลอง และไม่มี printing delay จริง

### Linked-list priority queue

- ไม่มี destructor ดังนั้นโหนดที่ยังไม่ถูก dequeue เมื่อ object หมดอายุจะเกิด **memory leak**
- ใช้ raw pointer และไม่มี copy-control; default copy เป็น shallow copy และ ownership ไม่ชัดเจน
- ตัวเลข priority ต่ำกว่าจะออกก่อน ผู้ใช้คลาสต้องทราบ convention นี้
- ไม่มี `peek`, `isEmpty` หรือ `size`
- ไม่มี `main()`

### Ownership โดยรวม

- Array queue เก็บข้อมูลภายใน object จึงไม่มี dynamic node ที่ต้อง `delete`
- `std::queue` จัดการ lifetime ของสมาชิกภายใน container ให้
- `Deque` ล้างโหนดใน destructor แต่ไม่ปลอดภัยต่อการ copy แบบ default
- `PriorityQueue` ลบเฉพาะโหนดที่ dequeue แล้วและไม่มี destructor สำหรับโหนดที่เหลือ
- หากพัฒนาโครงสร้าง raw-pointer ต่อ ควรกำหนด Rule of Five, ปิดการ copy เมื่อไม่รองรับ หรือออกแบบ ownership ใหม่ให้ชัดเจน

## วิธี compile และ run

ให้รันจาก project root `dsa`:

```bash
cd /home/phos/Desktop/Projects/self-taught-developer/dsa
mkdir -p build/queue
```

### ไฟล์ที่มี `main()` และรันได้โดยตรง

```bash
g++ -std=c++17 -Wall -Wextra -pedantic queue/linear-queue.cpp -o build/queue/linear-queue
./build/queue/linear-queue
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic queue/double-end-queue.cpp -o build/queue/double-end-queue
./build/queue/double-end-queue
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic queue/stl-queue.cpp -o build/queue/stl-queue
./build/queue/stl-queue
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic queue/printjob.cpp -o build/queue/printjob
./build/queue/printjob
```

### ไฟล์ที่ไม่มี `main()`

[`circular-queue.cpp`](./circular-queue.cpp) และ [`priority-queue.cpp`](./priority-queue.cpp) ไม่มี `main()` จึงตรวจคอมไพล์เป็น object file ได้ดังนี้:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -c queue/circular-queue.cpp -o build/queue/circular-queue.o
g++ -std=c++17 -Wall -Wextra -pedantic -c queue/priority-queue.cpp -o build/queue/priority-queue.o
```

การสั่งสร้าง executable จากสองไฟล์นี้เพียงไฟล์เดียวจะเกิด linker error เพราะไม่มี entry point `main` ต้องเพิ่ม driver program ที่สร้าง object และเรียก operation ก่อน จากนั้นจึง compile/link ร่วมกัน

### ตรวจ memory error ระหว่างการทดลอง

ตัวอย่างสำหรับ deque:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -g queue/double-end-queue.cpp -o build/queue/double-end-queue-asan
./build/queue/double-end-queue-asan
```

AddressSanitizer ช่วยตรวจ invalid access, use-after-free และปัญหาหน่วยความจำหลายชนิด แต่ควรใช้ test case ที่ครอบคลุม operation และ edge case ด้วย

## แบบฝึกหัดต่อยอด

1. สาธิต false overflow ของ linear queue แล้วปรับเป็น circular queue
2. ปรับ circular queue ให้ใช้ array ได้ครบทั้ง 5 ช่อง โดยเพิ่ม `count` หรือ flag แยก full/empty
3. เพิ่ม `peek`, `size` และค่าที่คืนจาก `dequeue` โดยหลีกเลี่ยง sentinel `-1` เช่นใช้ `std::optional<int>`
4. เขียน driver สำหรับ `CircularQueueArray` เพื่อทดสอบ empty, full, wrap-around และ dequeue จนว่าง
5. เขียน driver สำหรับ `PriorityQueue` และตรวจว่าเลข priority ต่ำออกก่อน
6. ทดสอบ priority เท่ากันหลายรายการเพื่อยืนยัน FIFO stability
7. เพิ่ม destructor ให้ `PriorityQueue` และตรวจ memory leak ด้วย AddressSanitizer
8. ปิดการ copy ของ `Deque` หรือ implement deep copy/move semantics ตาม Rule of Five
9. เปลี่ยน `SIZE` macro เป็น `constexpr` หรือสร้าง queue แบบ template ที่รับ capacity
10. เพิ่ม `display()` และ public `size()` ให้ deque
11. เพิ่ม validation ให้ `PrintJob` เช่นชื่อเอกสารต้องไม่ว่างและจำนวนหน้าต้องเป็นบวก
12. เพิ่มสถานะ pending/printing/completed/failed และ retry policy ให้ระบบงานพิมพ์
13. เปรียบเทียบ sorted linked-list priority queue กับ `std::priority_queue` โดยวัด enqueue/dequeue หลายขนาดข้อมูล
14. สร้าง queue แบบ generic ด้วย template เพื่อรองรับข้อมูลชนิดอื่นนอกจาก `int`
15. เขียน unit tests สำหรับ underflow, overflow, single element, wrap-around และ duplicate priorities
16. ศึกษา thread-safe producer/consumer queue โดยใช้ mutex และ condition variable พร้อมกำหนด shutdown behavior ให้ชัดเจน
