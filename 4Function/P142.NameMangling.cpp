#include <iostream>
using namespace std;

class Foo
{
public:
    void bar(int x) // after name-mangling in GCC 8.1.0: _ZN3Foo3barEi
    {
        cout << "void Bar(int x)" << endl;
    }
};

int main(int argc, char const *argv[])
{
    Foo foo;
    foo.bar(1);
    return 0;
}
