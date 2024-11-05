#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H

#include <chrono>
#include <iostream>
#include <string>

class ScopedTimer {
public:
    ScopedTimer(const std::string& aName) : mName(aName), mStart(std::chrono::high_resolution_clock::now()) {}
    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(mStart).time_since_epoch().count();
        auto duration = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count() - start;
        double ms = duration * 0.001;
        std::cout << mName << ": " << ms << "ms" << std::endl;
    }

private:
    std::string mName;
    std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
};

#endif // SCOPEDTIMER_H
