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

void bar()
{
    static Foo foo("static foo in bar");
    cout << "bar()" << endl;
}

int main(int argc, char const *argv[])
{
    cout << "begin" << endl;
    bar();
    cout << "end" << endl;
    return 0;
}
