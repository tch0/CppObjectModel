#include "inline.h"

extern void sumTo100();

void sumTo1000()
{
    std::cout << sumToInputNum(100) << std::endl;
}

int main(int argc, char const *argv[])
{
    sumTo100(); // count: 1
    sumTo1000(); // count: 2
    // no side effect
    return 0;
}
