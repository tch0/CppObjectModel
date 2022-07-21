#include <iostream>
using namespace std;

class Foo
{
public:
    Foo()
    {
        hello();
    }
    virtual void hello()
    {
        cout << "hello from Foo" << endl;
        bar();
    }
    virtual void bar()
    {
        cout << "bar from Foo" << endl;
    }
    virtual ~Foo()
    {
        hello();
    }
};

class DFoo : public Foo
{
public:
    DFoo() {}
    virtual void hello() override
    {
        cout << "hello from DFoo" << endl;
    }
    virtual void bar() override
    {
        cout << "bar from DFoo" << endl;
    }
    virtual ~DFoo()
    {
    }
};

int main(int argc, char const *argv[])
{
    DFoo foo;
    cout << endl;
    return 0;
}
