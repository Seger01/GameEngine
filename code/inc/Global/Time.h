#ifndef TIME_H
#define TIME_H

#include <chrono>

class Time {
public:
    // Call this once at the start of the application
    static void initialize() {
        startTime = std::chrono::high_resolution_clock::now();
        lastFrameTime = std::chrono::milliseconds(0); // Initialize as 0 ms from start
        ticks = 0;
        deltaTime = 0;
    }

    // Call this once per frame to update deltaTime and elapsed time since start
    static void update() {
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate time since start
        auto timeSinceStart = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

        // Calculate deltaTime as the difference from last frame
        deltaTime = (timeSinceStart - lastFrameTime).count();

        // Update lastFrameTime to the current time since start
        lastFrameTime = timeSinceStart;

        // Accumulate total elapsed time
        ticks += deltaTime;
    }

    static long deltaTime; // Time in milliseconds since last frame
    static long ticks;     // Total time since start in milliseconds

private:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::milliseconds lastFrameTime; // Duration since start
};

#endif // TIME_H
