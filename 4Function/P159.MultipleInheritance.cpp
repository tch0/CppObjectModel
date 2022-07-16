#include <iostream>
#include <cassert>
using namespace std;

class Base1
{
public:
    virtual ~Base1() {}
    virtual Base1* clone()
    {
        return new Base1();
    }
};

class Base2
{
public:
    virtual ~Base2() {}
    virtual Base2* clone()
    {
        return new Base2();
    }
};

class Derived : public Base1, public Base2
{
public:
    virtual Derived* clone() override
    {
        return new Derived();
    }
};


int main(int argc, char const *argv[])
{
    Derived d;
    Base1* p = &d;
    Base1* pc = p->clone();
    assert(typeid(*pc) == typeid(Derived));
    return 0;
}
