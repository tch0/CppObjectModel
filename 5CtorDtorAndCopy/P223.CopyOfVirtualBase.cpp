#include <iostream>
using namespace std;

class Foo
{
public:
    Foo() {}
    Foo(const Foo& foo)
    {
        cout << "Foo(const Foo& foo)" << endl;
    }
    Foo& operator=(const Foo& foo)
    {
        cout << "Foo& operator=(const Foo& foo)" << endl;
        return *this;
    }
    virtual ~Foo() {}
};

class DFoo1 : virtual public Foo
{
public:
    DFoo1() {}
    DFoo1(const DFoo1& foo) : Foo(foo)
    {
        cout << "DFoo1(const DFoo1& foo)" << endl;
    }
    DFoo1& operator=(const DFoo1& foo)
    {
        Foo::operator=(foo);
        cout << "DFoo1& operator=(const DFoo1& foo)" << endl;
        return *this;
    }
};

class DFoo2 : public DFoo1
{
public:
    DFoo2() {}
    DFoo2(const DFoo2& foo) : Foo(foo), DFoo1(foo)
    {
        cout << "DFoo2(const DFoo2& foo)" << endl;
    }
    DFoo2& operator=(const DFoo2& foo)
    {
        Foo::operator=(foo);
        DFoo1::operator=(foo);
        cout << "DFoo2& operator=(const DFoo2& foo)" << endl;
        return *this;
    }
};

int main(int argc, char const *argv[])
{
    DFoo2 foo1;
    DFoo2 foo2(foo1); // only construct virtual base Foo once, copy constructor of Foo in DFoo1 will be suppressed

    foo1 = foo2; // will call Foo::operator= twice, Foo::operator= in DFoo1 will not be suppressed
    return 0;
}
