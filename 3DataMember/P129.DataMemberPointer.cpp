#include <iostream>
#include <cstdio>
using namespace std;

class Point
{
public:
    ~Point() {}
public:
    double x, y, z;
};

int main(int argc, char const *argv[])
{
    printf("%d\n", &Point::x); // 8
    printf("%d\n", &Point::y); // 16
    printf("%d\n", &Point::z); // 24

    double Point::* p = nullptr;
    printf("%d\n", p); // -1
    Point x{1, 2, 3};
    cout << (&x)->*(&Point::z) << endl; // .* and ->* operator
    return 0;
}
