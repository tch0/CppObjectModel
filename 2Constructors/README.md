## 构造函数语义

当不期待单参数的转换构造函数带有隐式转换能力时，使用explicit。

### 默认构造

默认构造：
- 没有任何构造函数被显式定义时，编译器会生成合成的默认构造。
- 合成的默认构造只会对每个成员做默认初始化。即对自定义类型调用默认构造（non-trivial），对内建类型执行默认初始化（trivial）。
- 内建类型的默认初始化行为取决于变量的生命周期，这个行为是源自于C的。全局和静态成员会初始化为0，局部自动变量和动态分配的堆中的对象仅分配内存不做任何初始化（所以保持为该内存处的原有值，不一定为0）。
- 如果有内置类型成员且期望默认构造后内置类型被置0，那么就不应该依赖于编译器合成的默认构造。

一些容易被忽略的细节：
- 合成的默认构造只有在默认构造被调用时才会发生，就像模板实例化那样。
- 所以合成的默认构造和拷贝控制成员在每个编译单元内都会存在，为了避免链接时的符号重定义，他们都是默认inline的。而inline的函数是**静态链接**的，不会被编译单元外看到。

内联函数：
- 一个内联函数如果太复杂，为了保证静态链接，会被转换成一个**显式的静态非内联函数**。
- 但是不要会错意，这里的显式的静态非内联函数，并非一个真正意义上的静态函数，你可以将其和普通函数看做毫无差别，而非具有静态函数的副作用（比如函数中定义的静态变量不会每个编译单元都生成一份）。
- 更多细节后续章节解读。

编译器对构造函数做的事情：
- 对于自定义的构造，成员的构造顺序总是与其在类中声明的顺序相同，如果构造函数中没有在构造函数初始化列表中显式地调用成员构造，那么编译器会自动叫调用其默认构造的操作安插在用户代码之前，初始化列表之间。
- 基类也是同理，无论是合成的默认构造还是自定义的没有显式调用基类构造的构造，都会去调用基类的默认构造。
- 即是编译器会扩充现有的每一个构造，将所有必要的成员默认构造、基类默认构造添加到其中，其中所有基类构造在前，成员构造按照声明顺序在后。
- 带有虚函数的情况：
    - 虚表会被编译器生成出来，内放该类的所有虚函数的地址。
    - 此时虚指针作为类对象的一个隐式的成员，当然会被初始化为其虚表地址，也即是编译器会在构造函数中安插一些代码来做到这件事。
- 带有虚基类的情况：
    - 虚基类的实现因编译器而异，每种实现都必须保证虚基类子对象在每一个派生类对象中的位置在运行时已经准备妥当。
    - 虚基类成员在不同对象（普通对象，亦或派生类的基类子对象）中的偏移可能有区别，那么取派生类成员的操作可能需要做一个转化，改变为运行时根据具体对象而定。
    - 当然就上一章中的讨论来说，GCC可能是将其固定下来了的，以省去运行时开销，但不代表所有编译器都会这样做。如果不是，那么相关的隐式的成员的初始化也需要在构造中做。
    - 比如说可能的一个做法是在派生类中保存一个虚基类的指针，将对虚基类继承来的成员的访问转变为通过该指针的间接访问，那么这个指针就必须在构造时得到恰当的初始化。
    - 当然必须要说一说的是，现在的普遍做法是将虚基类子对象在派生类对象中的偏移存储在虚表中，而不是多存一个虚基类指针。

总结：
- 4种情况下：有自定义类类型成员、有定义了默认构造的基类、有虚函数、有虚基类的时候编译器会合成non-trivial默认构造，其中对类类型成员、基类执行默认构造、并且初始化一些虚函数或者虚基类机制必须的东西。
- 剩余情况称之为其隐式默认构造是trivial的，实际上不会被合成出来（也就是构造时不需要调用默认构造函数这个东西，内存申请好之后即可算是初始化好了）。

### 拷贝构造

用户定义的拷贝构造可以是多参数，后续参数给默认值就行。

合成拷贝构造的逐成员初始化：
- 编译器合成的默认拷贝构造会进行逐成员拷贝操作，内置类型、复合类型会直接赋值，自定义类型则调用其拷贝构造。
- 同合成的默认构造，其在需要时（被调用时）才会被定义出来。

逐位拷贝语义：
- 如果一个类没有类对象成员、存在拷贝构造的基类、虚函数、虚基类，而只有内置类型或复合类型的成员，那么隐式的拷贝构造的逻辑就是逐位拷贝，也就是trivial的。
- 这种情况下并不需要合成一个默认拷贝构造出来。也就是说这时候的拷贝构造并不需要调用这样一个函数，而是直接做一个按位拷贝即可（同C语义，等价于一个简单的`memcpy`调用）。

处理虚指针：
- 当一个类有了虚函数，就需要构建虚表，给类对象添加虚指针。此时的拷贝构造就是non-trivial的。
- 当以一个派生类对象拷贝构造一个基类对象时，派生类对象被切片/切割。很显然虚指针不应该直接拷贝过去，而应该将其设置为基类虚表地址。也就是说拷贝构造过程中虚指针不会被拷贝，它永远被设置为正在构造的那个对象的类型的虚表地址。

处理虚基类子对象：
- 当一个类有一个虚基类，拷贝该类对象时就需要考虑到其虚基类子对象的拷贝问题。
- 当然直接拷贝该类型时，逐位拷贝或许是够用的。
- 但是考虑用这个类的派生类对象拷贝构造该类对象时，虚基类子对象的处理问题。该类型和其派生类中，虚基类子对象的偏移可能是不同的，那么就不能直接按位拷贝了。此时虚基类指针、偏移的设定及必要的虚基类成员初始化、相关内存操作必须被执行。

总结：
- 合成拷贝构造中的4种non-trivial的不会简单执行按位拷贝的情况（和构造的情况相同）：有自定类类型成员、有定义了拷贝构造的基类、有虚函数、有虚基类。
- 这些情况都会需要编译器做一定的工作，而不能简单按位拷贝。

### 程序转化语义

这一节的主题是老生常谈的[复制消除](https://zh.cppreference.com/w/cpp/language/copy_elision)（Copy Elision）的问题：

- 复制消除的细节就不赘述了，看链接理解清楚即可，比书上信息更多，应该结合现在的标准来理解。
- NRVO的实现手段中，会有将返回值等效为引用传递的参数放在参数列表中等转换，各种代码转换与等效不一一赘述，大致理解即可。 

是否需要显式定义一个拷贝构造：
- 如果类是trivial的（没有类对象成员、没有基类、没有虚函数、没有虚基类），那么合成的拷贝构造会直接按位拷贝，这个逻辑是完全足够并且高效的（当然前提是这就是我们想要的逻辑）。此时我们最好不要自定义拷贝构造（这是书中观点）。
- 但是如果成员很多，很依赖编译器提供的NRVO来提供效率优化的，可能最好还是定义一个显式的拷贝构造。（只有显式定义了拷贝构造才能有NRVO吗？现在的观点来看应该不是，但书中好像表达了这个意思）。

总结：
- 了解了复制消除，就基本能够确定程序在拷贝时会怎么做，并明确了解相关逻辑的执行效率了。

### 成员初始化列表

必须在成员初始化列表中初始化的情况（即无法默认初始化的情况）：
- 含有引用成员。
- 含有const成员。
- 调用基类非默认构造。
- 调用成员非默认构造。

成员初始化列表做些什么：
- 按照成员声明顺序初始化各个成员，在成员初始化列表中的按照其中的逻辑初始化，未在的执行默认初始化（当然内置类型的默认初始化就是什么都不做，而自定义类型是调用默认构造）。
- 一定注意初始化顺序一定就等同于声明顺序，而与成员初始化列表的顺序无关，所以尽量避免在初始化列表中引入成员间的数据依赖，并且最好按照成员声明顺序书写成员初始化列表中的初始化顺序。
- 成员初始化列表中的初始化操作执行于构造函数函数体之前。
- 构造函数成员初始化列表中可以使用成员函数，this指针此时已经安排妥当，但是如果函数中不应该使用尚未初始化的成员。
- 另外基类子对象将先于成员进行初始化。
- 当然构造还会做一些虚指针和虚基类相关处理，也会被安插在其中，具体见前面的讨论。
