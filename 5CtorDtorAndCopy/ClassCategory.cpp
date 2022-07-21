#include <iostream>
#include <type_traits>
using namespace std;

class Foo
{
private:
    int i;
    double d;
    char str[10];
    std::nullptr_t p;
    bool b;
    double *pd;
};

class DFoo : public Foo
{
public:
    Foo* pFoo;
};

// multiple access level, so Bar doesn't has a standard layout
class Bar
{
private:
    int i;
    double d;
public:
    Foo* pFoo;
};

class Buz : public Bar
{
public:
    Buz() {} // user defined constructor, so Buz is not trivial, but it's trivially copyable, and trivially constructible
};

class Qux : public Buz
{
public:
    Qux(double _d) : d(_d) {} // not a trivial constructor, so it's not trivially constructible
    Qux(const Qux& qux) : d(qux.d) {} // user defined copy constructor, so Qux is not trivially copyable
    double d;
};

template<typename T>
void printTraitsOfClass(const string& str)
{
    cout << str << " is pod: " << is_pod<T>::value << endl;
    cout << str << " is trivial: " << is_trivial<T>::value << endl;
    cout << str << " is trivially constructible: " << is_trivially_constructible<T, T>::value << endl;
    cout << str << " is trivially copyable: " << is_trivially_copyable<T>::value << endl;
    cout << str << " is standard layout: " << is_standard_layout<T>::value << endl;
    // cout << str << " is aggregate class: " << is_aggregate<T>::value << endl; // since C++17
    cout << endl;
}

int main(int argc, char const *argv[])
{
    cout << boolalpha;
    printTraitsOfClass<Foo>("Foo");
    printTraitsOfClass<DFoo>("DFoo");   // not pod, not standard layout
    printTraitsOfClass<Bar>("Bar");     // not pod, not standard layout
    printTraitsOfClass<Buz>("Buz");     // not pod, not standard layout, not trivial
    printTraitsOfClass<Qux>("Qux");     // not pod, not standard layout, not trivial, not trivially copyable
    return 0;
}
