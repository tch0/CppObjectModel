#include <iostream>
#include <chrono>
#include <string>
#include "../Utils/ElapsedTimeRecorder.h"
using namespace std;

// test access overhead of of aggregation, encapsulation, inheritance
// test every expression for 10,000,000 times

struct Point3d1
{
    double x, y, z;
};

class Point3d2
{
public:
    Point3d2(double xx = 0, double yy = 0, double zz = 0) : _x(xx), _y(yy), _z(zz) {}
    double& x() {return _x;}
    double& y() {return _y;}
    double& z() {return _z;}
private:
    double _x, _y, _z;
};

class Point3d3
{
public:
    Point3d3(double xx = 0, double yy = 0, double zz = 0) : _x(xx), _y(yy), _z(zz) {}
    double x() {return _x;}
    double y() {return _y;}
    double z() {return _z;}
    void x(double val) {_x = val;}
    void y(double val) {_y = val;}
    void z(double val) {_z = val;}
private:
    double _x, _y, _z;
};

// multiple layer inheritance
class Point1d4
{
public:
    Point1d4(double xx = 0) : _x(xx) {}
    virtual double x() {return _x;}
    virtual double y() = 0;
    virtual double z() = 0;
    virtual void x(double val) {_x = val;}
    virtual void y(double val) = 0;
    virtual void z(double val) = 0;
private:
    double _x;
};

class Point2d4 : public Point1d4
{
public:
    Point2d4(double xx = 0, double yy = 0) : Point1d4(xx), _y(yy) {}
    double y() {return _y;}
    void y(double val) {_y = val;}
private:
    double _y;
};

class Point3d4 : public Point2d4
{
public:
    Point3d4(double xx = 0, double yy = 0, double zz = 0) : Point2d4(xx, yy), _z(zz) {}
    double z() {return _z;}
    void z(double val) {_z = val;}
private:
    double _z;
};

// virtual inheritance
class Point5
{
public:
    Point5(double xx = 0, double yy = 0, double zz = 0) : _x(xx), _y(yy), _z(zz) {}
    virtual double x() {return _x;}
    virtual double y() {return _y;}
    virtual double z() {return _z;}
    virtual void x(double val) {_x = val;}
    virtual void y(double val) {_y = val;}
    virtual void z(double val) {_z = val;}
public:
    double _x, _y, _z;
};

class Point3d5 : virtual public Point5
{
public:
    Point3d5(double xx = 0, double yy = 0, double zz = 0) : Point5(xx, yy, zz) {}
private:
    double xxx, yyy, zzz;
};

int main(int argc, char const *argv[])
{
    ElapsedTimeRecorder r;
    // test 1: local array
    {
        cout << "test1 : ";
        enum index {x, y, z};
        double pA[3] = {1.725, 0.875, 0.478};
        double pB[3] = {0.315, 0.317, 0.838};
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB[x] = pA[x] - pB[z];
            pB[y] = pA[y] + pB[x];
            pB[z] = pA[z] + pB[y];
        }
        r.showElapsedTime();
    }
    // test 2: C struct
    {
        cout << "test2 : ";
        Point3d1 pA = {1.725, 0.875, 0.478};
        Point3d1 pB = {0.315, 0.317, 0.838};
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB.x = pA.x - pB.z;
            pB.y = pA.y + pB.x;
            pB.z = pA.z + pB.y;
        }
        r.showElapsedTime();
    }
    // test 3: simple encapsulation with inline get
    {
        cout << "test3 : ";
        Point3d2 pA{1.725, 0.875, 0.478};
        Point3d2 pB{0.315, 0.317, 0.838};
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB.x() = pA.x() - pB.z();
            pB.y() = pA.y() + pB.x();
            pB.z() = pA.z() + pB.y();
        }
        r.showElapsedTime();
    }
    // test 4: encapsulation with inline get and set
    {
        cout << "test4 : ";
        Point3d3 pA{1.725, 0.875, 0.478};
        Point3d3 pB{0.315, 0.317, 0.838};
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB.x(pA.x() - pB.z());
            pB.y(pA.y() + pB.x());
            pB.z(pA.z() + pB.y());
        }
        r.showElapsedTime();
    }
    // test 5: simple inheritance, call virtual functions by object
    {
        cout << "test5 : ";
        Point3d4 pA{1.725, 0.875, 0.478};
        Point3d4 pB{0.315, 0.317, 0.838};
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB.x(pA.x() - pB.z());
            pB.y(pA.y() + pB.x());
            pB.z(pA.z() + pB.y());
        }
        r.showElapsedTime();
    }
    // test 6: simple inheritance, call virtual functions by pointer
    {
        cout << "test6 : ";
        Point3d4 A{1.725, 0.875, 0.478};
        Point3d4 B{0.315, 0.317, 0.838};
        Point1d4* pA = &A;
        Point1d4* pB = &B;
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB->x(pA->x() - pB->z());
            pB->y(pA->y() + pB->x());
            pB->z(pA->z() + pB->y());
        }
        r.showElapsedTime();
    }
    // test 7: virtual inheritance
    {
        cout << "test7 : ";
        Point3d5 A{1.725, 0.875, 0.478};
        Point3d5 B{0.315, 0.317, 0.838};
        Point5* pA = &A;
        Point5* pB = &B;
        r.reset();
        for (int i = 0; i < 10000000; ++i)
        {
            pB->x(pA->x() - pB->z());
            pB->y(pA->y() + pB->x());
            pB->z(pA->z() + pB->y());
        }
        r.showElapsedTime();
    }
    // result : test7/6 > test5/4/3 > test2/1
    return 0;
}
