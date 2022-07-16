#include <iostream>
using namespace std;

class Base1
{
public:
    virtual ~Base1() {}
    virtual Base1* clone()
    {
        return new Base1();
    }
    virtual void printThis()
    {
        cout << "this: " << this << endl;
    }
    double x;
};

class Base2
{
public:
    virtual ~Base2() {}
    virtual Base2* clone()
    {
        return new Base2();
    }
    virtual void printThis()
    {
        cout << "this: " << this << endl;
    }
    double y;
};

class Derived : public Base1, public Base2
{
public:
    virtual Derived* clone() override
    {
        cout << "this: " << this << endl;
        return new Derived();
    }
    double z;
};

int main(int argc, char const *argv[])
{
    Derived d;
    Base1* p1 = &d;
    Base2* p2 = &d;
    
    cout << "sizeof(Derived): " << sizeof(Derived) << endl; // 40
    cout << "&d: " << &d << endl;
    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;
    auto pc1 = p1->clone();
    auto pc2 = p2->clone();
    p1->printThis();
    p2->printThis();
    return 0;
}
