# Linked List

เอกสารนี้อธิบายแนวคิด linked list โดยอิงจากตัวอย่าง C++ ในไดเรกทอรีนี้ เนื้อหาครอบคลุม singly linked list, doubly linked list, circular linked list และการประยุกต์เป็นรายการเพลง พร้อมชี้ข้อจำกัดด้านหน่วยความจำและ ownership ของโค้ดปัจจุบัน

## เป้าหมายการเรียนรู้

เมื่ออ่านและทดลองตัวอย่างครบแล้ว ควรสามารถ:

1. อธิบายความแตกต่างระหว่าง array กับ linked list ได้
2. เข้าใจหน้าที่ของ **โหนด (node)**, `head`, `tail`, `next` และ `prev`
3. เดินรายการ (**traversal**) แบบทางเดียว สองทาง และแบบวงกลมได้
4. อธิบาย **invariant** หรือเงื่อนไขที่ต้องเป็นจริงเสมอของแต่ละโครงสร้างได้
5. วิเคราะห์เวลาและพื้นที่ด้วย Big-O สำหรับการเดินรายการ ค้นหา เพิ่ม และลบข้อมูลได้
6. จัดการ edge case เช่น รายการว่าง โหนดเดียว การลบหัว/ท้าย และข้อมูลซ้ำได้
7. เห็นความสำคัญของ **ownership** (ใครเป็นเจ้าของและต้องคืนหน่วยความจำ) เมื่อใช้ raw pointer กับ `new`/`delete`
8. เลือกชนิดของ linked list ให้เหมาะกับลักษณะงานได้

## Mental model และหลักการ

### โหนดและการเชื่อมโยง

Linked list ไม่ได้บังคับให้ข้อมูลอยู่ติดกันในหน่วยความจำเหมือน array แต่ละ **โหนด (node)** เก็บข้อมูลและ pointer ที่ชี้ไปยังโหนดอื่น

Singly linked list:

```text
head
  |
  v
[data|next] -> [data|next] -> [data|null]
```

Doubly linked list:

```text
null <- [prev|data|next] <-> [prev|data|next] <-> [prev|data|next] -> null
         ^                                                   ^
        head                                                tail
```

Circular singly linked list:

```text
head -> [data|next] -> [data|next] -> [data|next]
  ^                                           |
  +-------------------------------------------+
```

### Pointer สำคัญ

- `head`: ชี้โหนดแรก ถ้ารายการว่างควรเป็น `nullptr`
- `tail`: ชี้โหนดสุดท้าย ช่วยให้เพิ่มท้ายได้ `O(1)`
- `next`: ชี้โหนดถัดไป
- `prev`: ชี้โหนดก่อนหน้า ใช้ใน doubly linked list
- `nullptr`: แสดงว่าไม่มีโหนดให้ชี้ต่อ สำหรับ non-circular list

### Invariant ที่ควรรักษา

- **Singly linked list:** `next` ของโหนดสุดท้ายต้องเป็น `nullptr`
- **Doubly linked list:** ถ้า `x->next == y` แล้วควรมี `y->prev == x`
- **รายการว่างที่มี head/tail:** ทั้ง `head` และ `tail` ควรเป็น `nullptr`
- **รายการโหนดเดียว:** `head == tail`; ใน non-circular list ค่า `next` และ `prev` ที่ไม่มีเพื่อนบ้านควรเป็น `nullptr`
- **Circular list:** การเดินผ่าน `next` จาก `head` ต้องกลับถึง `head` และไม่ควรพบ `nullptr`

การแก้ pointer เพียงด้านเดียวอาจทำให้โครงสร้างเสียหาย (**corrupted structure**) แม้โปรแกรมยังคอมไพล์ผ่าน

## Operations และ complexity

ตารางนี้เป็น complexity ทั่วไป โดยสมมติว่ามี pointer ตามที่ระบุและทราบตำแหน่งโหนดที่จะดำเนินการแล้ว

| Operation | Singly linked list | Doubly linked list | หมายเหตุ |
|---|---:|---:|---|
| อ่าน/แก้โหนดแรก | `O(1)` | `O(1)` | ผ่าน `head` |
| เข้าถึงสมาชิกตำแหน่งที่ `i` | `O(n)` | `O(n)` | ไม่มี random access แบบ array |
| ค้นหาตามค่า | `O(n)` | `O(n)` | ต้องเดินทีละโหนด |
| เพิ่มที่หัว | `O(1)` | `O(1)` | ปรับ `head` และ pointer ที่เกี่ยวข้อง |
| เพิ่มที่ท้ายโดยไม่มี `tail` | `O(n)` | `O(n)` | ต้องหาโหนดสุดท้าย |
| เพิ่มที่ท้ายเมื่อมี `tail` | `O(1)` | `O(1)` | playlist ในตัวอย่างใช้แนวทางนี้ |
| ลบหัว | `O(1)` | `O(1)` | ต้องคืนหน่วยความจำโหนดเดิม |
| ลบท้ายเมื่อมีเพียง `head` | `O(n)` | `O(n)` | ต้องหาโหนดก่อนท้าย |
| ลบท้ายเมื่อมี `tail` | `O(n)` | `O(1)` | singly ยังต้องหาโหนดก่อนท้าย; doubly ใช้ `tail->prev` ได้ |
| ลบโหนดที่ทราบ pointer แล้ว | `O(n)` หรือ `O(1)` | `O(1)` | singly อาจต้องหา predecessor; doubly มี `prev` |
| เดินแสดงข้อมูลทั้งหมด | `O(n)` | `O(n)` | ใช้พื้นที่เสริม `O(1)` แบบ iterative |
| พื้นที่เก็บ `n` โหนด | `O(n)` | `O(n)` | doubly ใช้ pointer ต่อโหนดมากกว่า |

> การลบ “ตามค่า” ยังคงเป็น `O(n)` เพราะต้องค้นหาโหนดก่อน แม้ขั้นตอน unlink หลังพบจะเป็น `O(1)`

## ไฟล์ตัวอย่าง

### [`singly-linkedlist.cpp`](./singly-linkedlist.cpp)

สาธิต singly linked list ขั้นพื้นฐานผ่านคลาส `Node`:

- `Node::data` เก็บจำนวนเต็ม
- `Node::next` ชี้โหนดถัดไป
- `printList(Node *n)` เดินผ่าน `next` จนถึง `nullptr`
- `main()` สร้างโหนด `1`, `2`, `3` ด้วย `new` และเชื่อม pointer ด้วยตนเอง

โครงสร้างที่ได้:

```text
head -> 1 -> 2 -> 3 -> null
```

`printList` ใช้เวลา `O(n)` และพื้นที่เสริม `O(1)` ไม่มีเมธอด insert, remove หรือ search และไม่มี object ที่รับผิดชอบรายการทั้งชุด

### [`doubly-linkelist.cpp`](./doubly-linkelist.cpp)

> ชื่อไฟล์จริงคือ `doubly-linkelist.cpp` และคงการสะกดนี้ไว้ในคำสั่ง compile

สาธิต doubly linked list ผ่านคลาส `Node` ซึ่งมี `prev` และ `next`:

```text
null <- 1 <-> 2 <-> 3 -> null
```

`main()` ต้องตั้ง pointer ทั้งสองทิศทางด้วยตนเอง เช่น:

```cpp
head->next = second;
second->prev = head;
```

ฟังก์ชัน `printList(Node *node)` ในไฟล์นี้เดินไปข้างหน้าผ่าน `next` เท่านั้น จึงยังไม่ได้สาธิต traversal ย้อนกลับผ่าน `prev` และไม่ได้เก็บ `tail`

### [`circular-linkedlist.cpp`](./circular-linkedlist.cpp)

สาธิต circular singly linked list โดยเชื่อม:

```cpp
third->next = head;
```

โครงสร้างจึงไม่มี `nullptr` ที่ปลายรายการ ฟังก์ชัน `printList(Node *head)` ใช้ `do-while` เพื่อ:

1. แสดงโหนดปัจจุบัน
2. เลื่อนไป `next`
3. หยุดเมื่อ pointer กลับมาเท่ากับ `head`

การเดินใช้เวลา `O(n)` หากวงถูกสร้างอย่างถูกต้อง เงื่อนไขหยุดต่างจาก singly linked list ปกติอย่างสำคัญ: ห้ามรอพบ `nullptr` เพราะวงที่ถูกต้องจะไม่พบค่านั้น

### [`music-playlist-manager.cpp`](./music-playlist-manager.cpp)

ประยุกต์ doubly linked list เป็น playlist:

- คลาส `Song` เก็บ `title`, `next` และ `prev`
- คลาส `Playlist` เก็บ `head` และ `tail`
- `addSong(std::string title)` เพิ่มเพลงท้ายรายการใน `O(1)`
- `removeSong(std::string title)` ค้นหาเพลงใน `O(n)` และ unlink ใน `O(1)` หลังพบ
- `display()` เดินจาก `head` ไป `tail` ใน `O(n)`

การลบรองรับโหนดกลาง, `head`, `tail` และกรณีมีเพลงเดียว เนื่องจากมีการปรับ pointer ที่เกี่ยวข้องและ `delete temp` แต่ถ้ามีชื่อซ้ำจะลบเฉพาะรายการแรกที่พบ

## Dry run

### การเดิน singly linked list

ให้รายการเป็น `1 -> 2 -> 3 -> nullptr` และเริ่ม `n = head`

| รอบ | `n->data` | หลังสั่ง `n = n->next` |
|---:|---:|---|
| 1 | 1 | ชี้โหนด 2 |
| 2 | 2 | ชี้โหนด 3 |
| 3 | 3 | `nullptr` |
| จบ | — | เงื่อนไข `n != nullptr` เป็นเท็จ |

### การลบ `Song 2` จาก playlist

สถานะก่อนลบ:

```text
head                                      tail
  |                                         |
Song 1 <-> Song 2 <-> Song 3
```

เมื่อ `temp` ชี้ `Song 2`:

1. ตั้ง `Song 1->next` ให้ชี้ `Song 3`
2. ตั้ง `Song 3->prev` ให้ชี้ `Song 1`
3. `temp` ไม่ใช่ `head` หรือ `tail` จึงไม่ต้องเปลี่ยนสอง pointer นี้
4. เรียก `delete temp`

สถานะหลังลบ:

```text
head                         tail
  |                            |
Song 1 <-----------------> Song 3
```

### การเดิน circular list

สำหรับ `1 -> 2 -> 3 -> กลับไป 1` การตรวจ `temp != head` ต้องเกิดหลังเลื่อน pointer มิฉะนั้นหากตรวจก่อนประมวลผล โหนดแรกอาจไม่ถูกแสดงเลย นี่คือเหตุผลที่ตัวอย่างใช้ `do-while`

## การเลือกใช้ในงานจริง

| ความต้องการ | โครงสร้างที่ควรพิจารณา | เหตุผล/ข้อแลกเปลี่ยน |
|---|---|---|
| เพิ่ม/ลบด้านหน้าเป็นหลัก | Singly linked list | pointer น้อยและ operation ที่หัวเป็น `O(1)` |
| เดินหน้าและย้อนกลับ | Doubly linked list | มี `prev` แต่ใช้หน่วยความจำต่อโหนดมากขึ้น |
| เพิ่มท้ายบ่อย | List ที่เก็บ `tail` | ไม่ต้องเดินหาท้ายทุกครั้ง |
| หมุนวนซ้ำ เช่น round-robin | Circular linked list | โหนดสุดท้ายกลับไปเริ่มต้นได้โดยตรง |
| รายการเพลงที่ต้อง next/previous และลบกลางรายการ | Doubly linked list | unlink โหนดที่ทราบตำแหน่งได้สะดวก |
| ต้องเข้าถึงด้วย index บ่อย | `std::vector` มักเหมาะกว่า | linked list เข้าถึงตำแหน่งที่ `i` เป็น `O(n)` |
| ต้องการ container มาตรฐาน | `std::list` หรือ `std::forward_list` | มีการจัดการ lifetime และ iterator จาก standard library |

ควรตัดสินใจจาก operation ที่เกิดบ่อยที่สุด ไม่ใช่เลือก linked list เพียงเพราะ insert/delete มีชื่อเสียงว่าเร็ว เพราะการค้นหาตำแหน่งอาจยังใช้ `O(n)` และ locality ของหน่วยความจำมักด้อยกว่า array

## Edge cases และข้อควรระวังตามโค้ดปัจจุบัน

### Memory และ ownership

- [`singly-linkedlist.cpp`](./singly-linkedlist.cpp), [`doubly-linkelist.cpp`](./doubly-linkelist.cpp) และ [`circular-linkedlist.cpp`](./circular-linkedlist.cpp) สร้างโหนดด้วย `new` แต่ไม่เรียก `delete` จึงเกิด **memory leak**
- [`music-playlist-manager.cpp`](./music-playlist-manager.cpp) ลบโหนดที่เลือกผ่าน `removeSong` แต่ไม่มี destructor สำหรับล้างเพลงที่ยังเหลือเมื่อ `Playlist` หมดอายุ
- `Playlist` ใช้ raw pointer แต่ไม่ได้กำหนด copy constructor หรือ copy assignment การ copy object จะเป็น **shallow copy** ทำให้หลาย object ชี้โหนดชุดเดียวกัน และอาจนำไปสู่ dangling pointer หรือการจัดการ ownership ที่ไม่ชัดเจน
- แนวทางต่อยอดคือกำหนด destructor และ Rule of Five, ห้าม copy แล้วรองรับ move หรือออกแบบ ownership ด้วย smart pointer ตามรูปแบบการเชื่อมโยงที่เหมาะสม

### ความถูกต้องของ pointer

- `printList` ของ singly/doubly list จะวนไม่สิ้นสุดหาก `next` เกิด cycle โดยไม่ตั้งใจ
- `printList` ของ circular list สมมติว่าทุกทางจาก `head` จะกลับถึง `head`
  - หากพบ `nullptr` ระหว่างทาง รอบถัดไปอาจ dereference null pointer
  - หากเข้าสู่วงอื่นที่ไม่รวม `head` จะวนไม่สิ้นสุด
- การแก้ doubly linked list ต้องปรับทั้ง `next` และ `prev`
- ทุก operation ควรทดสอบ empty list, single-node list, head, tail และ middle node

### ข้อจำกัดเชิงข้อมูล

- Playlist อนุญาตชื่อเพลงซ้ำ และ `removeSong` ลบเฉพาะชื่อแรกที่พบ
- ตัวอย่างพื้นฐานไม่มี size counter, iterator, validation หรือ error reporting
- ฟังก์ชันแสดงผลในสามไฟล์พื้นฐานไม่พิมพ์ newline ตอนท้าย

## วิธี compile และ run

ให้รันจาก project root `dsa`:

```bash
cd /home/phos/Desktop/Projects/self-taught-developer/dsa
mkdir -p build/linkedlist
```

ทุกไฟล์ในไดเรกทอรีนี้มี `main()` และรันแยกกันได้:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic linkedlist/singly-linkedlist.cpp -o build/linkedlist/singly-linkedlist
./build/linkedlist/singly-linkedlist
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic linkedlist/doubly-linkelist.cpp -o build/linkedlist/doubly-linkelist
./build/linkedlist/doubly-linkelist
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic linkedlist/circular-linkedlist.cpp -o build/linkedlist/circular-linkedlist
./build/linkedlist/circular-linkedlist
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic linkedlist/music-playlist-manager.cpp -o build/linkedlist/music-playlist-manager
./build/linkedlist/music-playlist-manager
```

ตัวเลือก `-Wall -Wextra -pedantic` ช่วยเปิดคำเตือนจาก compiler แต่ไม่สามารถตรวจจับ memory leak ทุกกรณีได้ การทดลองเพิ่มเติมอาจใช้ AddressSanitizer:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -g linkedlist/singly-linkedlist.cpp -o build/linkedlist/singly-linkedlist-asan
./build/linkedlist/singly-linkedlist-asan
```

## แบบฝึกหัดต่อยอด

1. เพิ่ม `insertFront`, `insertBack`, `find` และ `remove` ให้ singly linked list
2. เพิ่ม `tail` ให้ singly linked list แล้วเปรียบเทียบเวลาของ `insertBack`
3. เพิ่มฟังก์ชันแสดง doubly linked list จาก `tail` ย้อนกลับมาที่ `head`
4. เพิ่ม destructor ที่คืนหน่วยความจำทุกโหนดให้แต่ละโครงสร้าง โดยระวัง circular list ซึ่งห้ามรอ `nullptr`
5. เขียน test สำหรับ empty list, single-node list, ลบ head, ลบ tail และไม่พบข้อมูล
6. ปรับ playlist ให้รองรับ `currentSong`, `nextSong()` และ `previousSong()`
7. กำหนดพฤติกรรมเมื่อชื่อเพลงซ้ำ: ลบรายการแรก ลบทุกรายการ หรือใช้ ID แยกเพลง
8. เพิ่ม `removeAllSongs(title)` และวิเคราะห์ complexity
9. ป้องกันการ copy ของ `Playlist` หรือ implement deep copy และ move semantics ตาม Rule of Five
10. ตรวจจับ malformed circular list ด้วยแนวคิด Floyd's cycle detection และแยกกรณี cycle ที่กลับ `head` กับ cycle ที่ผิดรูป
11. เปรียบเทียบเวลาและการใช้หน่วยความจำกับ `std::vector`, `std::list` และ `std::forward_list`
12. เพิ่ม unit tests และทดลองรันด้วย AddressSanitizer เพื่อสังเกต memory leak หรือ invalid access
