# LeetCode: Stack Patterns ใน C++

ไดเรกทอรีนี้มีคำตอบโจทย์ LeetCode ที่ใช้ stack สองโจทย์:

- [20.cpp](./20.cpp) — **Valid Parentheses**
- [1047.cpp](./1047.cpp) — **Remove All Adjacent Duplicates in String**

ทั้งสองไฟล์อยู่ในรูปแบบ submission ของ LeetCode: ประกาศ `class Solution` และ method ที่ระบบเรียกใช้ แต่ไม่มี `main()`

## เป้าหมายการเรียนรู้

เมื่อศึกษาโค้ดชุดนี้แล้ว ควรสามารถ:

1. มองเห็น pattern ว่าโจทย์แบบใดเหมาะกับ stack
2. อธิบาย invariant ของ stack ระหว่างประมวลผล input ได้
3. ใช้ stack จับคู่วงเล็บที่ซ้อนกันได้
4. ใช้ stack ทำ cancellation ของสมาชิกที่ติดกันได้
5. วิเคราะห์ complexity จาก implementation จริง ไม่ใช่เฉพาะแนวคิดระดับ algorithm
6. แยกแยะ constraint ของโจทย์ออกจากพฤติกรรมสำหรับ input ทั่วไป
7. เข้าใจวิธี compile ไฟล์ LeetCode ที่ไม่มี `main()` และวิธีสร้าง test harness ชั่วคราว

## เมื่อไรควรมองหา stack pattern

ลองพิจารณา stack เมื่อโจทย์มีลักษณะต่อไปนี้:

### 1. ต้องจับคู่สิ่งที่เปิดล่าสุดก่อน

ตัวอย่าง:

- วงเล็บเปิด/ปิด
- HTML/XML tags ที่ซ้อนกัน
- scope หรือ nested structure

เมื่อพบตัวปิด ตัวที่ต้องจับคู่คือ **ตัวเปิดล่าสุดที่ยังไม่มีคู่** ซึ่งตรงกับ LIFO

### 2. สมาชิกใหม่อาจยกเลิกสมาชิกก่อนหน้าทันที

ตัวอย่าง:

- ลบตัวอักษรซ้ำที่ติดกัน
- path simplification
- backspace processing
- collision หรือ cancellation บางประเภท

stack top ทำหน้าที่เป็น “สมาชิกก่อนหน้าที่ยังมีผลอยู่”

### 3. ต้องย้อนกลับไปยัง state ล่าสุดที่ยังไม่เสร็จ

ตัวอย่าง:

- DFS แบบ iterative
- expression evaluation
- backtracking
- undo/redo

### 4. ต้องหา nearest unresolved item ทางซ้าย

เช่น monotonic stack สำหรับ next greater/smaller element แม้สองไฟล์ในไดเรกทอรีนี้ยังไม่ได้สาธิต monotonic stack โดยตรง

### คำถามช่วยตัดสินใจ

ระหว่างอ่าน input จากซ้ายไปขวา ให้ถามว่า:

- สิ่งที่พบใหม่ต้องเทียบกับ “สิ่งล่าสุดที่ยังไม่ถูกจัดการ” หรือไม่?
- เมื่อจับคู่หรือยกเลิกแล้ว สามารถทิ้งทั้งคู่ได้หรือไม่?
- nesting ต้องปิดย้อนลำดับจากด้านในออกด้านนอกหรือไม่?

ถ้าคำตอบคือใช่ stack มักเป็นตัวเลือกที่เหมาะสม

## Mental model และ invariant

ทั้งสองโจทย์ใช้แนวคิดเดียวกัน:

> stack เก็บเฉพาะข้อมูลจาก prefix ที่อ่านแล้ว แต่ยังต้องมีผลต่อการตัดสินใจในอนาคต

เมื่ออ่านอักขระใหม่:

1. ดู `stack.top()` หากจำเป็น
2. ตัดสินใจว่าจะ push อักขระใหม่ หรือ pop สิ่งเดิม
3. รักษา invariant ก่อนอ่านอักขระถัดไป

### Invariant ของ Valid Parentheses

หลังอ่าน prefix ใด ๆ stack เก็บวงเล็บเปิดที่ยังไม่มีวงเล็บปิดจับคู่ โดยวงเล็บเปิดที่ต้องปิดก่อนอยู่บนสุด

### Invariant ของ Remove Adjacent Duplicates

หลังอ่าน prefix ใด ๆ stack แทนผลลัพธ์ของ prefix นั้นหลังลบ adjacent duplicates ที่เกิดขึ้นแล้วทั้งหมด ตัวบนสุดคืออักขระล่าสุดที่ยังอยู่ในผลลัพธ์

## Operations และ complexity

Operation ของ `std::stack` ที่ใช้ในโค้ด:

| Operation | หน้าที่ | Complexity โดยทั่วไป |
|---|---|---:|
| `push(c)` | เพิ่มอักขระด้านบน | O(1) |
| `pop()` | ลบอักขระด้านบน | O(1) |
| `top()` | อ่านอักขระด้านบน | O(1) |
| `empty()` | ตรวจว่าว่างหรือไม่ | O(1) |

Complexity ระดับฟังก์ชัน:

| ไฟล์ | Time ตามโค้ดจริง | Auxiliary space |
|---|---:|---:|
| [20.cpp](./20.cpp) | O(n) | O(n) |
| [1047.cpp](./1047.cpp) | **O(n²) worst case** | O(n) |

สาเหตุที่ `1047.cpp` ไม่ใช่ O(n) โดยรวมอธิบายไว้ในหัวข้อของไฟล์นั้น

## อธิบายไฟล์

### [20.cpp](./20.cpp) — Valid Parentheses

#### เป้าหมายของฟังก์ชัน

```cpp
bool isValid(string s)
```

ตรวจว่า string ที่ประกอบด้วย `(`, `)`, `{`, `}`, `[` และ `]` มีวงเล็บครบคู่และซ้อนกันถูกลำดับหรือไม่

#### ขั้นตอน

1. สร้าง `stack<char> stk`
2. เมื่อพบ `(`, `{`, `[` ให้ push
3. เมื่อพบอักขระอื่น โค้ดถือว่าเป็นฝั่งปิด:
   - ถ้า stack ว่าง คืน `false`
   - อ่านและ pop วงเล็บเปิดด้านบน
   - ตรวจว่าชนิดตรงกับวงเล็บปิดหรือไม่
4. เมื่ออ่านครบ คืน `stk.empty()`

การคืน `stk.empty()` สำคัญ เพราะ input เช่น `"(("` ไม่มี mismatch ระหว่างทาง แต่ยังมีวงเล็บเปิดที่ไม่ได้ปิด

#### Complexity

สมาชิกแต่ละตัวถูก push และ pop ไม่เกินหนึ่งครั้ง:

- Time: O(n)
- Auxiliary space: O(n) เมื่อ input เป็นวงเล็บเปิดทั้งหมด
- ฟังก์ชันอาจหยุดก่อนครบ input เมื่อพบ mismatch

### [1047.cpp](./1047.cpp) — Remove All Adjacent Duplicates in String

#### เป้าหมายของฟังก์ชัน

```cpp
string removeDuplicates(string s)
```

ลบตัวอักษรที่เหมือนกันและอยู่ติดกันเป็นคู่ซ้ำ ๆ จนไม่เหลือคู่ที่ลบได้

#### ขั้นตอน stack

สำหรับอักขระ `c` แต่ละตัว:

- ถ้า stack ว่าง หรือ `stk.top() != c` ให้ push `c`
- หาก `stk.top() == c` ให้ pop เพื่อยกเลิกคู่

วิธีนี้รองรับ cancellation แบบต่อเนื่อง เช่น เมื่อลบคู่ตรงกลางแล้วอักขระด้านนอกกลายมาอยู่ติดกัน

#### Complexity จริงตาม implementation

ช่วงประมวลผล input เป็น O(n) เพราะแต่ละอักขระถูก push/pop ไม่เกินหนึ่งครั้ง แต่โค้ดสร้างผลลัพธ์ด้วย:

```cpp
result = stk.top() + result;
```

นี่คือการเติมอักขระด้านหน้า string ทุกครั้ง โดยทั่วไปต้อง:

1. สร้าง string ใหม่
2. copy เนื้อหาเดิมของ `result`
3. เพิ่มอักขระใหม่ด้านหน้า

หาก stack เหลือ `m` ตัว จำนวนข้อมูลที่ copy โดยรวมมีลักษณะ:

```text
1 + 2 + 3 + ... + m = O(m²)
```

ดังนั้น:

- stack scan: O(n)
- reconstruction: O(n²) worst case
- **เวลารวมตามโค้ดจริง: O(n²) worst case**
- auxiliary space: O(n)

แนวทาง O(n) ที่นิยมกว่าคือ append อักขระจาก stack ลงท้าย string แล้วเรียก `reverse()` หนึ่งครั้ง หรือใช้ `std::string` เป็น stack ตั้งแต่ต้น

parameter เป็น `string s` แบบ by-value จึงอาจมีการ copy input เมื่อเรียกด้วย lvalue ด้วย แม้ไม่เปลี่ยน Big-O หลัก

## Dry run

### Valid Parentheses: `"([])"`

| อักขระ | การตัดสินใจ | stack จากล่างขึ้นบน |
|---|---|---|
| `(` | push | `(` |
| `[` | push | `( [` |
| `]` | top คือ `[`, จับคู่แล้ว pop | `(` |
| `)` | top คือ `(`, จับคู่แล้ว pop | ว่าง |

อ่านครบและ stack ว่าง จึงคืน `true`

### Valid Parentheses: `"([)]"`

| อักขระ | การตัดสินใจ | stack จากล่างขึ้นบน |
|---|---|---|
| `(` | push | `(` |
| `[` | push | `( [` |
| `)` | top คือ `[`, ไม่ตรงกับ `(` ที่ต้องการ | คืน `false` |

แม้จำนวนวงเล็บเปิดและปิดเท่ากัน แต่ nesting ผิดลำดับ

### Remove Adjacent Duplicates: `"abbaca"`

| อักขระ | การตัดสินใจ | stack/ผลลัพธ์ชั่วคราว |
|---|---|---|
| `a` | push | `a` |
| `b` | top ไม่ใช่ `b`, push | `ab` |
| `b` | top เป็น `b`, pop | `a` |
| `a` | top เป็น `a`, pop | ว่าง |
| `c` | push | `c` |
| `a` | push | `ca` |

ผลลัพธ์คือ `"ca"`

ตัวอย่างนี้แสดง cascading effect: เมื่อลบ `bb` แล้ว `aa` กลายมาอยู่ติดกันและถูกลบต่อ

## การนำไปใช้จริง

Pattern จาก [20.cpp](./20.cpp) ใช้ต่อยอดกับ:

- parser สำหรับวงเล็บและ delimiter
- ตรวจ nested tags
- ตรวจ syntax เบื้องต้นของ expression
- ตรวจ scope ที่เปิดและปิดเป็นคู่

Pattern จาก [1047.cpp](./1047.cpp) ใช้ต่อยอดกับ:

- ลบ backspace หรือ undo token
- normalize ข้อมูลที่มี cancellation rule
- remove adjacent runs ตามเงื่อนไข
- collision simulation ที่สมาชิกใหม่โต้ตอบกับสมาชิกก่อนหน้าที่ยังเหลือ

เมื่อนำไปใช้จริง ควรกำหนดให้ชัดเจนว่า:

- input alphabet มีอะไรบ้าง
- อักขระที่ไม่รู้จักควรถูก reject หรือ ignore
- ต้องรายงานเพียง `true/false` หรือต้องบอกตำแหน่ง error
- ขนาด input สูงสุดเท่าไร
- reconstruction ของ output มีต้นทุนแฝงหรือไม่

## Edge cases และข้อจำกัดตามโค้ดจริง

### [20.cpp](./20.cpp)

ภายใต้ constraint ของ LeetCode ที่ string มีเฉพาะ `()[]{}` โค้ดทำงานตามโจทย์ แต่โค้ดนี้ **ไม่ได้ validate ว่าอักขระเป็นวงเล็บปิดจริง**

ทุกอักขระที่ไม่ใช่ `(`, `{`, `[` จะเข้าสู่ branch เดียวกับวงเล็บปิด:

```cpp
if (c == '(' || c == '{' || c == '[') {
    // opening bracket
} else {
    // อักขระอื่นทั้งหมดเข้าที่นี่
}
```

ผลที่ควรระวัง:

- ถ้า stack ว่าง อักขระทั่วไป เช่น `"a"` ทำให้คืน `false`
- ถ้า stack ไม่ว่าง อักขระทั่วไปจะ pop วงเล็บเปิดหนึ่งตัว
- ตัวอย่าง `"(a"` คืน `true` ตามโค้ดจริง เพราะ `a` ทำให้ `(` ถูก pop และเงื่อนไข mismatch ตรวจเฉพาะ `)`, `}` และ `]`

ดังนั้นต้องระบุ precondition ว่า input มีเฉพาะวงเล็บหกชนิด หรือเพิ่ม validation สำหรับอักขระอื่น

กรณีสำคัญอื่น:

| Input | ผลตามที่ควรเป็นในโจทย์ |
|---|---|
| `""` | `true` |
| `"("` | `false` |
| `")"` | `false` |
| `"()[]{}"` | `true` |
| `"([)]"` | `false` |
| `"{[]}"` | `true` |

### [1047.cpp](./1047.cpp)

- empty string คืน empty string
- string ยาว 1 ตัวคงเดิม
- `"aa"` ถูกลบหมด
- `"aaa"` เหลือ `"a"` เพราะลบเป็นคู่
- input ที่ไม่มี adjacent duplicate ทำให้ stack โตถึง O(n) และ reconstruction ปัจจุบันเข้าใกล้กรณี O(n²)
- โค้ดทำงานกับ `char` ทั่วไป แต่เป็น byte-oriented; ไม่ได้จัดการ Unicode code point แบบเต็มรูปแบบ

### ข้อจำกัดระดับไฟล์

- ทั้งสองไฟล์ประกาศ `class Solution` ชื่อเดียวกัน
- แต่ละไฟล์ตั้งใจให้ compile/submit แยกกัน
- ห้าม `#include` ทั้งสองไฟล์ใน translation unit เดียว เพราะจะเกิด class redefinition
- การ link เป็นโปรแกรมเดียวที่มีนิยาม `Solution` ต่างกันยังขัดกับ One Definition Rule
- `#include <iostream>` ไม่จำเป็นต่อ method ของ LeetCode โดยตรง แต่มีประโยชน์เมื่อสร้าง harness ที่พิมพ์ผล

## `class Solution` และสาเหตุที่ไม่มี `main()`

LeetCode สร้าง `main()` และ test harness ให้เอง ระบบจะ:

1. สร้าง object ของ `Solution`
2. เรียก method ตาม signature ของโจทย์
3. เปรียบเทียบค่าที่คืนกับ expected result

ดังนั้นไฟล์เหล่านี้ compile เป็น source ได้ แต่ link เป็น executable เดี่ยว ๆ ไม่ได้ เพราะไม่มี entry point `main()`

## Compile, syntax check และ run จาก root `dsa`

คำสั่งต่อไปนี้สมมติว่า current directory คือ root `dsa`:

```bash
pwd
# /home/phos/Desktop/Projects/self-taught-developer/dsa
```

### Syntax check

ตรวจแต่ละโจทย์โดยไม่ต้องมี `main()` และไม่สร้าง object/executable:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsyntax-only leetcode/20.cpp
```

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsyntax-only leetcode/1047.cpp
```

ตรวจทั้งสอง source พร้อมกันในโหมด syntax-only ได้ เพราะ compiler ยังไม่รวมสองไฟล์เป็น translation unit เดียว:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -fsyntax-only \
  leetcode/20.cpp \
  leetcode/1047.cpp
```

คำสั่งต่อไปนี้ไม่เพียงพอสำหรับสร้าง executable:

```bash
# link ไม่ผ่านเพราะไม่มี main()
# g++ leetcode/20.cpp -o /tmp/leetcode-20
```

### Run `20.cpp` ด้วย temporary harness

คำสั่งนี้ส่ง driver ผ่าน standard input ให้ compiler และไม่แก้ source file:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Ileetcode -x c++ -o /tmp/dsa-lc20 - <<'CPP'
#include <iostream>
#include "20.cpp"

int main() {
    Solution solution;
    std::cout << std::boolalpha << solution.isValid("([])") << '\n';
}
CPP

/tmp/dsa-lc20
```

ผลที่คาดหวัง:

```text
true
```

### Run `1047.cpp` ด้วย temporary harness

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Ileetcode -x c++ -o /tmp/dsa-lc1047 - <<'CPP'
#include <iostream>
#include "1047.cpp"

int main() {
    Solution solution;
    std::cout << solution.removeDuplicates("abbaca") << '\n';
}
CPP

/tmp/dsa-lc1047
```

ผลที่คาดหวัง:

```text
ca
```

## แบบฝึกหัด

1. เขียน test cases ของ [20.cpp](./20.cpp) ให้ครอบคลุม empty string, unmatched opening, unmatched closing และ wrong nesting
2. แก้ `isValid` ให้ reject อักขระที่ไม่ใช่วงเล็บทั้งหกชนิดอย่างชัดเจน
3. เปลี่ยน policy ให้ ignore non-bracket characters แล้วเขียน README ระบุพฤติกรรมนั้น
4. ปรับ [1047.cpp](./1047.cpp) ให้ reconstruction เป็น O(n) ด้วย append แล้ว `std::reverse`
5. เขียน `removeDuplicates` โดยใช้ `std::string` เป็น stack แทน `std::stack<char>`
6. พิสูจน์ว่าแต่ละอักขระใน stack scan ถูก push และ pop ไม่เกินหนึ่งครั้ง
7. Dry run `"azxxzy"` และอธิบาย cascading cancellation
8. วัดเวลาของ implementation ปัจจุบันของ 1047 กับ input ที่ไม่มีตัวซ้ำหลายขนาด แล้วเปรียบเทียบกับเวอร์ชัน O(n)
9. ดัดแปลง 1047 ให้ลบเมื่อมีอักขระซ้ำติดกัน `k` ตัว แทนการลบเป็นคู่
10. เพิ่ม method ที่คืนตำแหน่งแรกของวงเล็บผิดคู่ แทนการคืนเพียง `bool`
11. เขียนโจทย์ Next Greater Element เพื่อเรียนรู้ monotonic stack pattern
12. อธิบายว่าเหตุใด queue จึงไม่เหมาะกับการจับคู่ nested parentheses เท่ากับ stack
