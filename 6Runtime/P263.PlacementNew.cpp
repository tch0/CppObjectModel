#include <iostream>
#include <memory>
using namespace std;

class Foo
{
public:
    Foo()
    {
        cout << "Foo()" << endl;
    }
    virtual ~Foo()
    {
        cout << "~Foo()" << endl;
    }
private:
    double x;
};

class DFoo : public Foo
{
public:
    DFoo()
    {
        cout << "DFoo()" << endl;
    }
    ~DFoo()
    {
        cout << "~DFoo()" << endl;
    }
private:
    double y;
};

int main(int argc, char const *argv[])
{
    void *p = operator new(sizeof(DFoo)); // allocation
    Foo* pFoo = new (p) DFoo(); // construction
    pFoo->~Foo(); // destruction
    operator delete(p); // deallocation
    return 0;
}
