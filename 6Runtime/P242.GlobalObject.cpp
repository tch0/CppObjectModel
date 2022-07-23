#include <iostream>
using namespace std;

class Foo
{
public:
    Foo(const string& _name) : name(_name)
    {
        cout << "Foo() of " << name << endl;
    }
    ~Foo()
    {
        cout << "~Foo() of " << name << endl;
    }
private:
    string name;
};

Foo foo1("foo1");
Foo foo2("foo2");

int main(int argc, char const *argv[])
{
    cout << "begin" << endl;
    cout << "end" << endl;
    return 0;
}
