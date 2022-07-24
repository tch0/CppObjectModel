#include <iostream>
#include <typeinfo>
using namespace std;

class Foo {};
class DFoo : public Foo {};


void printTypeInfo(const type_info& info)
{
    cout << "name: " << info.name() << endl;
}

int main(int argc, char const *argv[])
{
    printTypeInfo(typeid(Foo));
    printTypeInfo(typeid(DFoo));
    return 0;
}
