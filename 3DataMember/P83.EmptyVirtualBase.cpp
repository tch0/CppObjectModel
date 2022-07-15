#include <iostream>
using namespace std;

class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};

/*
memory layout of A:
vptr of Y
vptr of Z

the empty virtual base class X did not take any space.
*/

int main(int argc, char const *argv[])
{
    cout << "sizeof(X): " << sizeof(X) << endl; // 1
    cout << "sizeof(Y): " << sizeof(Y) << endl; // 8
    cout << "sizeof(Z): " << sizeof(Z) << endl; // 8
    cout << "sizeof(A): " << sizeof(A) << endl; // 16
    A a;
    X* px = &a;
    Y* py = &a;
    Z* pz = &a;
    cout << "&a: " << &a << endl;
    cout << "px: " << px << endl;
    cout << "py: " << py << endl;
    cout << "pz: " << pz << endl;
    return 0;
}


