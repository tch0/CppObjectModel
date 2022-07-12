## 关于对象

### C++对象模型

其他对象模型略。

虚函数的支持：
- 每个类对每个虚函数产出一个指针，然后将其放在虚表（virtual table，vtbl）中。这个表是类唯一的。
- 每个类对象安插一个虚指针（vptr）指向这个虚表，用以标识对象的动态类型。虚指针的设置与重置由构造函数、析构函数、拷贝赋值运算符自动完成。
- 虚表中第一项通常是`type_info`对象，然后是所有虚函数。

对象模型对程序的影响：
- 最主要的就是影响现有代码是否需要重新编译。
    - 比如基类添加了虚函数，那么虚表就会变，就会影响到所有派生类，所有使用了其派生类的代码都需要重新编译。

### 关键字差异

- class和struct没有本质差异，仅有的差异是默认访问限定。
- struct的存在可以说是为了兼容C，但C++的对象布局并不同于C，通常来说不应该在C++中搞柔性数组之类的这种小聪明。

### 对象差异

编程范式：
- 面向过程，Procedural，即是继承于C的编程范式。
- 抽象数据类型模型，ADT（也被称作基于对象，Object-based，OB）。
- 面向对象，Object Oriented，OO。
- OB和OO都是使用对象，单纯的封装不支持继承以扩展即是OB，而继承与多态则是OO。OB更具有效率（无虚调用的间接层次），OO更具有弹性（派生以扩展），具体如何使用取决于实际情况。

多态：
- 仅由引用和指针支持。
- 注意避免对象切片。
- 由虚表实现，引入一层间接层次，有运行时开销。

对象的大小：
- 非静态数据成员总大小。
- 对齐加入的padding（填充），位于成员间或者集合体边界。
- 为支持多态引入的内存开销，虚指针。

指针的类型：
- 指针的类型仅影响如何解释其中的地址处存储的内存，本质都是一个地址。
- 通常32位机器上为32位，64位机器上为64位长度。
- 普通的指针类型转换并不真正改变一个地址，仅仅改变编译器解释内存的方式。
- 但当涉及到多态时，指针的转换（使用`dynamic_cast`）就不一定能够保持地址不变了。

对象内存布局：
- 这里仅以本地环境（Windows，mingw-w64，g++ 8.1.0）为例，不同编译器完全可能是不同的布局，标准并未明确规定，这里仅展示可能性，描述多态情况下内存布局的基本实现逻辑。
- 测试代码见：[P29.ObjectLayout.cpp](P29.ObjectLayout.cpp)
- 仅考虑不需要对齐的情况，需要对齐的情况也是同理。
- 例子1：普通单继承。
```C++
class Buz
{
public:
    virtual ~Buz() {}
    double x;
};

class DBuz : public Buz
{
public:
    double y;
};
```
- 典型的内存布局是：
```
total size of DBuz: 24bytes
vptr    8bytes
x       8bytes
y       8bytes
```
- 多重继承并且菱形继承。
```C++
class Base
{
public:
    virtual ~Base() {}
    double xxx;
};

class DBase1 : public Base
{
public:
    double x;
};

class DBase2 : public Base
{
public:
    double y;
};

class Derived : public DBase1, public DBase2
{
public:
    double z;
};
```
- 典型布局：
```
total size of Derived: 56 bytes
vptr of DBase1  8bytes
xxx             8bytes
x               8bytes
vptr of DBase2  8bytes
xxx             8bytes
y               8bytes
z               8bytes
```
- 例子3：多重继承，菱形继承，公共基类虚继承。
```C++
class VBase
{
public:
   virtual ~VBase() {}
   double xv;
};

class VDBase1 : virtual public VBase
{
public:
    double x;
};

class VDBase2 : virtual public VBase
{
public:
    double y;
};

class VDerived : public VDBase1, public VDBase2
{
public:
    double z;
};
```
- 典型布局：
```
total size of VDerived: 56 bytes
vptr of VDBase1     8bytes
x                   8bytes
vptr of VDBase2     8bytes
y                   8bytes
z                   8bytes
vptr of VBase       8bytes
xv                  8bytes
```
- 由这三个例子已经可以基本窥一斑而知全豹了（一定注意这是当前环境特定不可移植的）：
    - 虚指针放在对象或者子对象最开头。
    - 单继承：基类子对象与派生类对象拥有共同的地址，共用同一个虚指针，基类子对象成员在前，派生类成员在后。
    - 多重非虚继承：派生类对象拥有多个虚指针（每个拥有虚函数的基类对应一个），第一个基类子对象与派生类共用同一个地址，多个基类虚指针和成员依次存放在布局中，最后是派生类成员。多个基类中的非虚公共顶层基类在每个基类子对象中都存在一份，布局逻辑同理。
    - 多重虚继承：同多重非虚继承，不过顶层的虚基类子对象只会存在一份，所以放哪一个里面都显得不合适，所以被单独剔出来了（这里是放在所有直接基类之后），拥有公共虚基类的多个基类也就没有这一部分了。最后部分依然是派生类成员。
- 那么在当前环境下，什么情况下多态指针转换地址会变也就一清二楚了。当然并不应该依赖这样的东西编程，很明显这不在标准里，不可移植。在MSVC里面可能情况又会有区别（谁前谁后，虚指针放哪都可能有差别），但都是虚表实现基本逻辑都大同小异。

最后的注意点：
- 将派生类对象赋给基类对象会引起切片（sliced），此过程中调用基类的拷贝构造函数。在拷贝过程中并不会拷贝派生类的虚指针，而是在任何情况下都直接将基类对象虚指针赋为该类的虚表地址。
- 所以说只有指针和引用具有多态。他们的操作（复制、绑定）并不会影响到原始的对象，仅仅只是改变了所指向内存的解释方式（指针赋值时其地址是可能变化的，见上面的讨论），虚指针的值并没有变，指向的虚表还是那个虚表，所以才能实现多态。
