#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
public:
    // Call this once at the start of the application
    static void initialize() {
        startTime = std::chrono::high_resolution_clock::now();
        lastFrameTime = startTime;
        ticks = 0.0;
        deltaTime = 0.0;
    }

    // Call this once per frame to update deltaTime and elapsed time since start
    static void update() {
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate deltaTime in seconds
        deltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

        // Update lastFrameTime to the current time
        lastFrameTime = currentTime;

        // Accumulate total elapsed time in seconds
        ticks += deltaTime;
    }

    static double deltaTime; // Time in seconds since last frame
    static double ticks;     // Total time since start in seconds

private:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::high_resolution_clock::time_point lastFrameTime;
};

#endif // TIME_H
