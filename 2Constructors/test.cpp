#include <iostream>
using namespace std;


class Foo
{
public:
    Foo() {}
    explicit Foo(const Foo& foo) {}
};

int main(int argc, char const *argv[])
{
    Foo foo;
    Foo foo2(foo);
    return 0;
}
