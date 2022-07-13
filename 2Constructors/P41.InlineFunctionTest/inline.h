#include <iostream>

// a simple inline
inline void hello()
{
    std::cout << "hello" << std::endl;
}

// a complicated inline
inline int sumToInputNum(int num)
{
    static int count = 0;
    std::cout << "count: " << ++count << std::endl;
    int sum = 0;
    for (int i = 0; i < num; i ++)
    {
        sum += i;
    }
    return sum;
}
