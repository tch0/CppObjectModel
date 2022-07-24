#include <iostream>
#include <exception>
#include <string>
using namespace std;

class TestException : public exception
{
public:
    TestException(const string& _name) : name(_name)
    {
        cout << "construction of exception: " << name << endl;
    }
    TestException(const TestException& e) : name("copy of " + e.name)
    {
        cout << "copy construction of exception: " << name << endl;
    }
    ~TestException()
    {
        cout << "detruction of exception: " << name << endl;
    }
    // must declare as noexcept
    virtual const char* what() const noexcept override
    {
        return ("TestException " + name).c_str();
    }
private:
    string name;
};

int main(int argc, char const *argv[])
{
    // catch by reference
    cout << "test of catching by value: " << endl;
    try
    {
        throw TestException("test1");
    }
    catch(const std::exception& e)
    {
        std::cerr << "catch exception: " << e.what() << '\n';
    }
    cout << endl;

    // catch by value
    cout << "test of catching by reference: " << endl;
    try
    {
        throw TestException("test2");
    }
    catch(TestException e)
    {
        std::cerr << "catch exception: " << e.what() << '\n';
    }
    cout << endl;

    // test of exception_ptr
    exception_ptr eptr;
    try
    {
        throw TestException("test3");
    }
    catch(const TestException& e)
    {
        std::cerr << "catch exception: " << e.what() << '\n';
        eptr = std::current_exception();
    }
    cout << endl;

    cout << "end of main" << endl; // test3 will destruct when eptr destruct
    return 0;
}
