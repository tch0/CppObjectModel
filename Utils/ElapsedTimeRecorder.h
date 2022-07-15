#ifndef ELAPSED_TIME_RECORDER
#define ELAPSED_TIME_RECORDER

#include <string>
#include <iostream>
#include <chrono>

// utilities for every program

// record elapsed CPU time
class ElapsedTimeRecorder
{
public:
    ElapsedTimeRecorder(const std::string& str = "default") : name(str), begin(std::chrono::high_resolution_clock::now()), last(begin)
    {
    }
    ~ElapsedTimeRecorder()
    {
        // std::cout << "Destruction of recorder " << name << ", existing time: " << getTotalElapsedTimeInMs() << " ms" << std::endl;
    }
    void reset()
    {
        last = std::chrono::high_resolution_clock::now();
    }
    void showElapsedTime()
    {
        std::cout << "Elapsed time: " << getElapsedTimeInMs() << " ms" << std::endl;
    }
    double getElapsedTimeInMs()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last).count() / 1000000.0;
    }
    double getTotalElapsedTimeInMs()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() / 1000000.0;
    }
private:
    std::string name;
    std::chrono::high_resolution_clock::time_point begin;
    std::chrono::high_resolution_clock::time_point last;
};

#endif