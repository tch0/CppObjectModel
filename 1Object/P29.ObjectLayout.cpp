#include <iostream>
using namespace std;

class Foo {};

class DFoo : public Foo {};

class Bar
{
public:
    virtual ~Bar() {}
};

class DBar : public Bar
{
};

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
/*
layout of DBuz:
vptr    8bytes
x       8bytes
y       8bytes
*/

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
/*
multiple non-virtual inheritance:
memory layout of Derived: 56 bytes
vptr of DBase1
xxx
x
vptr of DBase2
xxx
y
z
*/

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
/*
multiple virtual inheritance: 56 bytes
memory layout of VDerived:
vptr of VDBase1
x
vptr of VDBase2
y
z
vptr of VBase
xv
*/

class VDerived2 : virtual public VBase
{
public:
    double x;
};
/*
normal virtual inheritance: 32bytes
memory layout of VDerived2:
vptr of VDerived2
x
vptr of VBase
xv
*/

int main(int argc, char const *argv[])
{
    {
        // no polymorphism, same
        DFoo df;
        Foo* pf = &df;
        cout << "sizeof(Foo): " << sizeof(Foo) << endl; // 1
        cout << "sizeof(DFoo): " << sizeof(DFoo) << endl; // 1
        cout << "&df: " << &df << endl;
        cout << "pf: " << pf << endl;

        // polymorphism, but no member
        cout << endl;
        DBar db;
        Bar* pbar = &db;
        cout << "sizeof(Bar): " << sizeof(Bar) << endl; // 8, only vptr
        cout << "sizeof(DBar): " << sizeof(DBar) << endl; // 8
        cout << "&db: " << &db << endl;
        cout << "pbar: " << pbar << endl;

        cout << endl;
        DBuz dbz;
        Buz* pbz = &dbz;
        cout << "sizeof(Buz): " << sizeof(Buz) << endl; // 16
        cout << "sizeof(DBuz): " << sizeof(DBuz) << endl; // 24
        cout << "&dbz: " << &dbz << endl; // same as Buz subobject.
        cout << "pbz: " << pbz << endl;
        cout << "&dbz.x: " << &dbz.x << endl;
        cout << "puz->x: " << &pbz->x << endl;
        cout << "&dbz.y: " << &dbz.y << endl;
    }

    // multiple non-virtual inheritance.
    {
        cout << endl;
        Derived d;
        DBase1* pdb1 = &d;
        DBase2* pdb2 = &d;
        Base* pb1 = pdb1;
        Base* pb2 = pdb2;
        cout << "sizeof(Derived): " << sizeof(Derived) << endl; // 40, with double Base
        cout << "&d: " << &d << endl;
        cout << "pb1: " << pb1 << endl; // the Base subobject in DBase1 
        cout << "pb2: " << pb2 << endl; // the Base subobject in DBase2
        cout << "pdb1: " << pdb1 << endl;
        cout << "pdb2: " << pdb2 << endl;
        cout << "&d.DBase1::xxx: " << &d.DBase1::xxx << endl;
        cout << "&d.DBase2::xxx: " << &d.DBase2::xxx << endl;
        cout << "&d.x: " << &d.x << endl;
        cout << "&d.y: " << &d.y << endl;
        cout << "&d.z: " << &d.z << endl;
    }

    // multiple virtual inheritance
    {
        cout << endl;
        VDerived d;
        VDBase1* pdb1 = &d;
        VDBase2* pdb2 = &d;
        VBase* pvb1 = pdb1;
        VBase* pvb2 = pdb2;
        cout << "sizeof(VDerived): " << sizeof(VDerived) << endl; // 56
        cout << "&d: " << &d << endl;
        cout << "pdb1: " << pdb1 << endl;
        cout << "pdb2: " << pdb2 << endl;
        cout << "pvb1: " << pvb1 << endl;
        cout << "pvb2: " << pvb2 << endl;
        cout << "&d.x: " << &d.x << endl;
        cout << "&d.y: " << &d.y << endl;
        cout << "&d.z: " << &d.z << endl;
        cout << "&d.xv: " << &d.xv << endl;
    }

    {
        cout << endl;
        VDerived2 d;
        VBase* pb = &d;
        cout << "sizeof(VDerived2): " << sizeof(VDerived2) << endl; // 32
        cout << "&d: " << &d << endl;
        cout << "pb: " << pb << endl;
        cout << "&d.xv: " << &d.xv << endl;
        cout << "&d.x: " << &d.x << endl;
    }
    return 0;
}
