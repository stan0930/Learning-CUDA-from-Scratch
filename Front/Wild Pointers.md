# 野指针 (Wild Pointer)

## 1. 什么是野指针？
野指针是指向**“垃圾”内存**（随机内存、已释放内存或不可访问内存）的指针。
与 `NULL` 指针不同，`NULL` 指针明确指向 0 地址（通常受保护，访问会直接报错），而野指针指向的位置是随机的，可能指向合法的内存地址，也可能指向非法的地址。

## 2. 野指针的成因

### 2.1 指针未初始化
在 C/C++ 中，局部变量（包括指针）默认不会被初始化。如果声明了一个指针但没有给它赋初值，它指向的就是一个随机地址。

```cpp
int *p; // p 是野指针，指向随机地址
*p = 10; // 危险操作！可能导致程序崩溃或数据篡改
```

### 2.2 指针越界访问
访问数组时超出了数组的范围，指针就指向了非法的内存区域。

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int *p = arr;
for (int i = 0; i <= 5; i++) {
    // 当 i = 5 时，p 指向了数组之后的内存，这是野指针
    *(p++) = i; 
}
```

### 2.3 指针释放后未置空（Dangling Pointer）
当使用 `free()` 或 `delete` 释放了指针指向的内存后，指针变量本身的值（地址）并没有改变，它仍然指向那块已经被系统回收的内存。这块内存可能已经被重新分配给其他用途。

```cpp
int *p = new int(10);
delete p; // 释放内存
// 此时 p 变成了“悬空指针”（Dangling Pointer），也是野指针的一种
// *p = 20; // 错误！访问已释放的内存
```

### 2.4 返回局部变量的地址
函数返回了栈上局部变量的地址。函数结束时，栈帧被销毁，该地址的内容变得无效。

```cpp
int* func() {
    int a = 10;
    return &a; // 错误！a 在函数结束后生命周期结束
}
```

## 3. 野指针的危害
1.  **程序崩溃 (Crash)**：如果野指针指向的内存是受保护的（如内核区），访问会导致段错误（Segmentation Fault）。
2.  **数据损坏**：如果野指针指向了程序中其他正在使用的有效数据区，写入数据会悄无声息地篡改 legitimate data，导致难以排查的逻辑错误。
3.  **不可预测的行为**：程序可能有时运行正常，有时崩溃，取决于野指针恰好指向了哪里。

## 4. 如何避免野指针？

1.  **初始化指针**：声明指针时，要么初始化为有效的地址，要么初始化为 `nullptr` (C++11) 或 `NULL`。
    ```cpp
    int *p = nullptr;
    ```
2.  **释放后置空**：调用 `free` 或 `delete` 后，立即将指针赋值为 `nullptr`。
    ```cpp
    delete p;
    p = nullptr;
    ```
    （注：`delete nullptr` 是安全的，不做任何操作）
3.  **注意数组边界**：循环遍历数组时小心 off-by-one 错误。
4.  **避免返回局部变量地址**：不要从函数返回局部栈变量的指针或引用。
5.  **使用智能指针 (Smart Pointers)**：在 C++ 中，尽量使用 `std::unique_ptr` 或 `std::shared_ptr`，它们会自动管理内存生命周期，极大减少野指针问题。

## 5. 进阶：黑客如何利用野指针？(Use-After-Free 漏洞)
黑客最常用的野指针利用方式称为 **Use-After-Free (UAF)**。
当程序释放了一块内存（free/delete），但保留了指向它的野指针，并在随后继续使用该指针时，黑客可以通过在这个“时间差”内申请大小相同的内存，来**占坑**（Heap Spraying 或简单的内存复用）。

当野指针再次被使用时，它实际上访问的是黑客精心构造的数据。

### 攻击场景模拟代码 (C++)
这个例子展示了如何通过“野指针”让一个普通用户通过内存复用变成管理员。

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class User {
public:
    virtual void whoAmI() {
        cout << "[User] I am a normal user." << endl;
    }
};

class Admin {
public:
    virtual void whoAmI() {
        cout << "[HACKER] I AM ROOT! System compromised." << endl;
    }
};

int main() {
    // 1. 正常逻辑：创建一个普通用户
    User* pUser = new User();
    cout << "1. Created normal user." << endl;
    pUser->whoAmI();

    // 2. 漏洞点：释放了对象，但没有把指针 pUser 置为 nullptr！
    delete pUser; 
    // pUser 现在成了“野指针” (Dangling Pointer)
    cout << "2. User deleted (but pointer is dangling)..." << endl;

    // --- 黑客的攻击时间 ---
    
    // 3. 黑客申请一块新的内存
    // 内存分配器（Allocator）通常为了效率，会复用刚刚释放的内存块。
    // 因为 Admin 对象通常和 User 对象大小相似（比如都有虚函数表指针），
    // 这一步申请的 new Admin() 很大概率会分配到刚才 pUser 所在的地址。
    Admin* pHackerObject = new Admin();
    cout << "3. Hacker allocated new object (occupied the freed slot)." << endl;

    // --- 系统继续运行 ---

    // 4. 系统并未察觉 pUser 已经失效，再次调用它的方法
    // 这就是 "Use-After-Free"
    cout << "4. System tries to use the old dangling pointer:" << endl;
    pUser->whoAmI(); 
    
    // 结果：虽然代码写的是 pUser->whoAmI()，
    // 但因为内存被 Admin 对象占据，虚函数表（vtable）指针被替换，
    // 实际上执行了 Admin::whoAmI()！

    return 0;
}
```

### 原理解析
1.  **悬空指针**：`delete pUser` 后，内存被标记为可用，但 `pUser` 变量依然保存着那个内存地址。
2.  **内存对其 (Memory Realignment)**：`new Admin()` 申请内存时，操作系统/运行时库发现刚才释放的那块内存大小合适，直接把那块地皮分给了 `Admin` 对象。
3.  **狸猫换太子**：此时 `pUser` 指向的地址里，住的已经是 `Admin` 对象了。
4.  **虚函数表劫持**：C++ 多态依靠对象头部的 `vptr` (虚表指针)。`pUser->whoAmI()` 实际上是去查 `vptr` 指向的函数表。因为内存里现在是 `Admin` 对象，`vptr` 指向的是 `Admin` 的函数表，所以执行了 hack 代码。

这就是为什么**释放后必须置空** (`pUser = nullptr`) 是一条黄金法则。如果置空了，第 4 步就会直接 Crash（空指针解引用），而不是不仅不报错，还悄悄执行了恶意代码。
