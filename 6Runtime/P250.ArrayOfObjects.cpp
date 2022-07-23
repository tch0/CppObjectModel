#include <iostream>
#include <string>
using namespace std;

class Foo
{
public:
    Foo()
    {
        name = "count(" + to_string(++count) + ")";
        cout << "Foo() of " << name << endl;
    }
    Foo(const string& _name) : name(_name)
    {
        cout << "Foo() of " << name << endl;
        ++count;
    }
    ~Foo()
    {
        cout << "~Foo() of " << name << endl;
    }
    static int count;
private:
    string name;
};

int Foo::count = 0;

int main(int argc, char const *argv[])
{
    Foo fooArr[10] = {"hello"s, "yes"s, "no"s};
    return 0;
}
